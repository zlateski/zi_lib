//
// Copyright (C) 2010  Aleksandar Zlateski <zlateski@mit.edu>
// ----------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef ZI_WATERSHED_DETAIL_FILE_BASE_HPP
#define ZI_WATERSHED_DETAIL_FILE_BASE_HPP 1

#include <zi/utility/non_copyable.hpp>
#include <zi/watershed/detail/exception.hpp>
#include <zi/concurrency/mutex.hpp>

#include <zi/time/timer.hpp>
#include <zi/bits/bind.hpp>

#include <cstddef>
#include <cstdio>
#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace zi {
namespace watershed {

class file: non_copyable
{
public:
    enum file_mode
    {
        RDONLY = 0x01,
        WRONLY = 0x02,
        RDWR   = 0x04,
        CREAT  = 0x08,
        DIRECT = 0x10,
        TRUNC  = 0x20
    };

protected:
    mutex mutex_;
    int   fd_   ;
    int   mode_ ;
    const std::string filename_;

    std::size_t size_nl()
    {
        struct stat st;
        if ( ::fstat( fd_, &st ) < 0 )
        {
            throw std::runtime_error( std::string( "can't get size of file: " ) + filename_ );
        }
        return st.st_size;
    }


public:
    file( const std::string& filename, int mode )
        : mutex_(),
          fd_( -1 ),
          mode_( mode ),
          filename_( filename )
    {

        int flags = 0;

#define ZI_WATERSHED_DETAIL_ADD_FILE_FLAG( f )     \
        if ( mode & f )                      \
        {                                    \
            flags |= O_##f;                  \
        } static_cast< void >( 0 )           \

        ZI_WATERSHED_DETAIL_ADD_FILE_FLAG( RDONLY );
        ZI_WATERSHED_DETAIL_ADD_FILE_FLAG( WRONLY );
        ZI_WATERSHED_DETAIL_ADD_FILE_FLAG( RDWR   );
        ZI_WATERSHED_DETAIL_ADD_FILE_FLAG( CREAT  );
        ZI_WATERSHED_DETAIL_ADD_FILE_FLAG( TRUNC  );

#undef ZI_WATERSHED_DETAIL_ADD_FILE_FLAG

        if ( mode & DIRECT )
        {
            flags |= O_SYNC | O_RSYNC | O_DSYNC | O_DIRECT;
        }

        const int perms = S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP;

        fd_ = ::open( filename_.c_str(), flags, perms );

        if ( fd_ < 0 )
        {
            ZI_THROWF( std::runtime_error, "can't open file: %s",
                       filename_.c_str() );
        }
    }

    virtual ~file()
    {
        close();
    }

    void close()
    {
        if ( fd_ >= 0 )
        {
            mutex::guard g( mutex_ );

            if ( ::close( fd_ ) < 0 )
            {
                // error
            }
            else
            {
                fd_ = -1;
            }
        }
    }

    void lock()
    {
        mutex::guard g( mutex_ );

        struct flock f;

        f.l_type   = F_RDLCK | F_WRLCK;
        f.l_whence = SEEK_SET;
        f.l_start  = 0;

        f.l_len = 0;

        if ( ::fcntl( fd_, F_SETLK, &f ) < 0 )
        {
            ZI_THROWF( std::runtime_error, "can't lock file: %s",
                       filename_.c_str() );
        }
    }

    std::size_t size()
    {
        mutex::guard g( mutex_ );
        return size_nl();
    }

    void size( std::size_t s )
    {
        mutex::guard g( mutex_ );

        if ( !( mode_ & RDONLY ) )
        {

            if ( ::ftruncate( fd_, s ) < 0 )
            {
                ZI_THROWF( std::runtime_error, "can't set file size of %s",
                           filename_.c_str() );
            }
        }
    }

    void report_read( int64_t from, int64_t size, int64_t time )
    {
        std::cout << "File Read: " << from << " " << size << " in " << time << "\n";
    }

    void report_write( int64_t from, int64_t size, int64_t time )
    {
        std::cout << "File Wrote: " << from << " " << size << " in " << time << "\n";
    }

    virtual void read( void *buffer, std::size_t len, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        timer::wall::scoped st( bind( &file::report_read, this, offset, len, placeholders::_1 ));

        if ( ::lseek( fd_, offset, SEEK_SET ) < 0 )
        {
            ZI_THROWF( std::runtime_error, "can't seek to %lld in %s",
                       offset, filename_.c_str() );
        }

        if ( ::read( fd_, buffer, len ) != std::ptrdiff_t( len ) )
        {
            ZI_THROWF( std::runtime_error, "can't partial read in %s",
                       filename_.c_str() );
        }
    }

     virtual void write( const void *buffer, std::size_t len, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        timer::wall::scoped st( bind( &file::report_write, this, offset, len, placeholders::_1 ));

        if ( ::lseek( fd_, offset, SEEK_SET ) < 0 )
        {
            ZI_THROWF( std::runtime_error, "can't seek to %lld in %s",
                       offset, filename_.c_str() );
        }

        if ( ::write( fd_, buffer, len ) != std::ptrdiff_t( len ) )
        {
            ZI_THROWF( std::runtime_error, "can't partial write in %s",
                       filename_.c_str() );
        }
    }


    void remove()
    {
        close();
        ::remove( filename_.c_str() );
    }

};

} // namespace watershed
} // namespace zi

#endif
