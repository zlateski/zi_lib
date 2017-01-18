//
// Copyright (C) 2010-2011  Aleksandar Zlateski <zlateski@mit.edu>
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

#ifndef ZI_WATERSHED_DETAIL_IO_HPP
#define ZI_WATERSHED_DETAIL_IO_HPP 1

#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/condition_variable.hpp>
#include <zi/utility/singleton.hpp>
#include <zi/utility/static_assert.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <algorithm>
#include <fstream>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iterator>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace zi {
namespace watershed {
namespace detail {

template< int N >
class io_semaphore_lock
{
private:
    ZI_STATIC_ASSERT(N>0);

    mutable int credit_ ;
    mutable int waiters_;

    condition_variable cv_;
    mutex              m_ ;

public:

    io_semaphore_lock():
        credit_(N), waiters_(0), cv_(), m_()
    { };

    void acquire() const
    {
        mutex::guard g( m_ );
        while ( credit_ <= 0 )
        {
            ++waiters_;
            cv_.wait(m_);
            --waiters_;
        }
        --credit_;
    }

    void release( int n = 1 ) const
    {
        mutex::guard g( m_ );
        credit_ += n;
        n = std::min( n, waiters_ );

        while (n>0)
        {
            cv_.notify_one();
            --n;
        }
    }

};

} // namespace detail

namespace io {

template< int N >
struct guard
{
    guard()
    {
        singleton< detail::io_semaphore_lock<N> >::instance().acquire();
    };

    ~guard()
    {
        singleton< detail::io_semaphore_lock<N> >::instance().release();
    };
};


struct io_functions
{
    // modify this for good drives
    static const int DefaultN = 2;

    struct memcpy
    {
        void* operator()( void* dest, const void* src, std::size_t n ) const
        {
            io::guard<DefaultN> g;
            return std::memcpy( dest, src, n );
        }
    };

    struct file_read
    {
    private:
        typedef boost::interprocess::file_mapping  file_mapping ;
        typedef boost::interprocess::mapped_region mapped_region;

    public:
        void* operator()( const std::string& fname, void* dest,
                          std::size_t n, std::size_t off = 0) const
        {
            file_mapping fm( fname.c_str(), boost::interprocess::read_only );
            mapped_region mr( fm, boost::interprocess::read_only, off, n );
            ZI_ASSERT( mr.get_size() == n );
            {
                io::guard<DefaultN> g;
                return std::memcpy( dest, mr.get_address(), n );
            }
        }

        void* operator()( file_mapping& fm, void* dest,
                          std::size_t n, std::size_t off = 0 ) const
        {
            mapped_region mr( fm, boost::interprocess::read_only, off, n );
            ZI_ASSERT( mr.get_size() == n );
            {
                io::guard<DefaultN> g;
                return std::memcpy( dest, mr.get_address(), n );
            }
        }

        template< class Inserter >
        void operator()( const std::string& filename, Inserter i ) const
        {
            typedef typename Inserter::container_type      container_type;
            typedef typename container_type::value_type    value_type    ;
            typedef typename container_type::const_pointer const_pointer ;

            struct stat st;
            if ( ::fstat( filename.c_str(), &st ) < 0 )
            {
                throw std::runtime_error("can't get file's size");
            }

            if ( st.st_size % sizeof(value_type) != 0 )
            {
                throw std::logic_error("file size is not multipe of the type");
            }

            std::size_t n = static_cast<std::size_t>(st.st_size);
            file_mapping fm( filename.c_str(), boost::interprocess::read_only );
            mapped_region mr( fm, boost::interprocess::read_only, 0, n );
            ZI_ASSERT( mr.get_size() == n );

            const_pointer p = reinterpret_cast<const_pointer>(mr.get_address());

            {
                io::guard<DefaultN> g;
                std::copy( p, p + n/sizeof(value_type), i);
            }
        }

        template< class InIter >
        void operator()( const std::string& fname, InIter b, InIter e ) const
        {
            typedef typename std::iterator_traits<InIter>::value_type value_type;

            std::ptrdiff_t dist = std::distance(b,e);
            if ( dist < 0 )
            {
                return;
            }

            std::size_t n = static_cast<std::size_t>(dist) * sizeof(value_type);

            std::filebuf fbuf;
            fbuf.open( fname.c_str(),
                       std::ios_base::in    | std::ios_base::out |
                       std::ios_base::trunc | std::ios_base::binary);

            fbuf.pubseekoff( n-1, std::ios_base::beg );
            fbuf.sputc(0);
            fbuf.close();

            file_mapping fm( fname.c_str(), boost::interprocess::read_write );
            mapped_region mr( fm, boost::interprocess::read_write, 0, n );
            ZI_ASSERT( mr.get_size() == n );

            std::uninitialized_copy( b, e, mr.get_address() );
        }
    };

    struct file_write
    {
    private:
        typedef boost::interprocess::file_mapping  file_mapping ;
        typedef boost::interprocess::mapped_region mapped_region;

    public:
        void operator()( const std::string& fname, void* dest, std::size_t n ) const
        {
            std::filebuf fbuf;
            fbuf.open( fname.c_str(),
                       std::ios_base::in    | std::ios_base::out |
                       std::ios_base::trunc | std::ios_base::binary);

            fbuf.pubseekoff( n-1, std::ios_base::beg );
            fbuf.sputc(0);
            fbuf.close();

            file_mapping fm( fname.c_str(), boost::interprocess::read_write );
            mapped_region mr( fm, boost::interprocess::read_write, 0, n );
            ZI_ASSERT( mr.get_size() == n );
            ZI_VERIFY( mr.get_address() == std::memcpy( mr.get_address(), dest, n ) );
        }

        template< class InIter >
        void operator()( const std::string& fname, InIter b, InIter e ) const
        {
            typedef typename std::iterator_traits<InIter>::value_type value_type;

            std::ptrdiff_t dist = std::distance(b,e);
            if ( dist < 0 )
            {
                return;
            }

            std::size_t n = static_cast<std::size_t>(dist) * sizeof(value_type);

            std::filebuf fbuf;
            fbuf.open( fname.c_str(),
                       std::ios_base::in    | std::ios_base::out |
                       std::ios_base::trunc | std::ios_base::binary);

            fbuf.pubseekoff( n-1, std::ios_base::beg );
            fbuf.sputc(0);
            fbuf.close();

            file_mapping fm( fname.c_str(), boost::interprocess::read_write );
            mapped_region mr( fm, boost::interprocess::read_write, 0, n );
            ZI_ASSERT( mr.get_size() == n );

            std::uninitialized_copy( b, e, mr.get_address() );
        }
    };
};

namespace {

io_functions::memcpy& memcpy = singleton<io_functions::memcpy>::instance();
io_functions::file_read& file_read = singleton<io_functions::file_read>::instance();
io_functions::file_write& file_write = singleton<io_functions::file_write>::instance();

} // namespace




} // namespace io
} // namespace watershed
} // namespace zi

#endif
