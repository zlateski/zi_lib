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

#ifndef ZI_WATERSHED_DETAIL_MMAP_FILE_HPP
#define ZI_WATERSHED_DETAIL_MMAP_FILE_HPP 1

#include <zi/watershed/detail/file_base.hpp>
#include <zi/watershed/detail/exception.hpp>

#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/guard.hpp>
#include <zi/utility/singleton.hpp>
#include <zi/time/timer.hpp>
#include <zi/bits/bind.hpp>

#include <iterator>
#include <cerrno>
#include <cstring>
#include <sys/mman.h>

namespace zi {
namespace watershed {

class mmap_file: public file
{
public:
    static const std::size_t page_size = 4096;

private:

    typedef class_rwmutex< mmap_file > static_mutex;

    inline void* mmap_( std::size_t len, int prot, std::size_t offset )
    {
        void *r = mmap( NULL, len, prot, MAP_SHARED, fd_, offset );

        if ( r == MAP_FAILED || r == NULL )
        {
            ZI_THROWF( resource_error, "can't mmap file %s", filename_.c_str() );
        }

        return r;
    }

    inline void munmap_( void *m, std::size_t len )
    {
        if ( munmap( m, len ) < 0 )
        {
            ZI_THROWF( resource_error, "can't munmap file %s", filename_.c_str() );
        }
    }

public:

    inline mmap_file( const std::string& filename, int mode )
        : file( filename, mode )
    { }

    void report_read( int64_t size, int64_t time )
    {
        std::cout << "\rMMap Read: " << size << " in " << time << "\n";
    }

    void report_write( int64_t size, int64_t time )
    {
        std::cout << "\rMMap Wrote: " << size << " in " << time << "\n";
    }

    void read( void *buffer, std::size_t len, std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mutex::guard g( mutex_ );

        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        //timer::wall::scoped st( bind( &mmap_file::report_read, this, len-padding, placeholders::_1 ));

        void *r = mmap_( len, PROT_READ, offset );

        if ( padding )
        {
            void *vm = reinterpret_cast< void* >
                ( reinterpret_cast< char* >( r ) + padding );

            std::memcpy( buffer, vm, len - padding );
        }
        else
        {
            std::memcpy( buffer, r, len - padding );
        }

        munmap_( r, len );
    }

    void write( const void *buffer, std::size_t len, std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mutex::guard g( mutex_ );

        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        //timer::wall::scoped st( bind( &mmap_file::report_write, this, len-padding, placeholders::_1 ));

        void *r = mmap_( len, PROT_WRITE, offset );

        if ( padding )
        {
            void *vm = reinterpret_cast< void* >
                ( reinterpret_cast< char* >( r ) + padding );

            std::memcpy( vm, buffer, len - padding );
        }
        else
        {
            std::memcpy( r, buffer, len - padding );
        }

        munmap_( r, len );
    }

    template< class OutputIterator >
    void read_n( OutputIterator first, std::size_t count, std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mutex::guard g( mutex_ );

        typedef typename std::iterator_traits< OutputIterator >::value_type value_type;

        std::size_t len     = count * sizeof( value_type );
        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_READ, offset );

        value_type *vm = reinterpret_cast< value_type* >
            ( reinterpret_cast< char* >( r ) + padding );

        std::copy( vm, vm + count, first );

        munmap_( r, len );
    }

    template< class InputIterator >
    void write( InputIterator first, InputIterator last, std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mutex::guard g( mutex_ );

        typedef typename std::iterator_traits< InputIterator >::value_type value_type;

        std::size_t len = static_cast< std::size_t >
            ( last - first + 1 ) * sizeof( value_type );

        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_WRITE, offset );

        value_type *vm = reinterpret_cast< value_type* >
            ( reinterpret_cast< char* >( r ) + padding );

        std::copy( first, last, vm );

        munmap_( r, len );
    }

    template< class InputIterator >
    void write_n( InputIterator first, std::size_t count, std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mutex::guard g( mutex_ );

        typedef typename std::iterator_traits< InputIterator >::value_type value_type;

        std::size_t len     = count * sizeof( value_type );
        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_WRITE, offset );

        value_type *vm = reinterpret_cast< value_type* >
            ( reinterpret_cast< char* >( r ) + padding );

        for ( std::size_t i = 0; i < count; ++i )
        {
            vm[ i ] = *( first++ );
        }

        munmap_( r, len );
    }

    static void write( const std::string& filename,
                       const void *buffer,
                       std::size_t len,
                       std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mmap_file f( filename, file::CREAT | file::RDWR | file::TRUNC | file::DIRECT  );
        f.size( len );
        return f.write( buffer, len, offset );
    }

    template< class InputIterator >
    static void write_n( const std::string& filename,
                         InputIterator first,
                         std::size_t count,
                         std::size_t offset = 0 )
    {
        typedef typename std::iterator_traits< InputIterator >::value_type value_type;
        mmap_file f( filename, file::CREAT | file::RDWR | file::TRUNC | file::DIRECT  );
        f.size( count * sizeof( value_type ) );
        return f.template write_n< InputIterator >( first, count, offset );
    }

    template< class InputIterator >
    static void write( const std::string& filename,
                       InputIterator first,
                       InputIterator last,
                       std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        typedef typename std::iterator_traits< InputIterator >::value_type value_type;

        std::size_t len =
            static_cast< std::size_t >( last - first + 1 ) * sizeof( value_type );

        mmap_file f( filename, file::CREAT | file::RDWR | file::TRUNC | file::DIRECT );
        f.size( len );
        return f.template write< InputIterator >( first, last, offset );
    }

    static void read( const std::string& filename,
                      void *buffer,
                      std::size_t len,
                      std::size_t offset = 0 )
    {
        //szi::guard g( static_mutex::instance() );
        mmap_file f( filename, file::RDONLY );
        return f.read( buffer, len, offset );
    }

    template< class OutputIterator >
    static void read_n( const std::string& filename,
                        OutputIterator first,
                        std::size_t count,
                        std::size_t offset = 0 )
    {
        //zi::guard g( static_mutex::instance() );
        mmap_file f( filename, file::RDONLY );
        return f.template read_n< OutputIterator >( first, count, offset );
    }

    static void erase( const std::string& filename )
    {
        ::remove( filename.c_str() );
    }

};

} // namespace watershed
} // namespace zi

#endif
