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

#ifndef ZI_WATERSHED_DETAIL_IO_SEMAPHORE_HPP
#define ZI_WATERSHED_DETAIL_IO_SEMAPHORE_HPP 1

#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/condition_variable.hpp>
#include <zi/utility/singleton.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

namespace zi {
namespace watershed {
namespace detail {

template< int N > io_semaphore_lock
{
private:
    ZI_STATIC_ASSERT(N>0);

    mutable int credit_ ;
    mutable int waiters_;

    condition_variable cv_;
    mutex              m_ ;

public:

    semaphore():
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
        void* operator()( void* dest, const void* src, std::size_t n )
        {
            io::guard<DefaultN> g;
            return std::memcpy( dest, src, n );
        }
    };
};

} // namespace io

} // namespace watershed
} // namespace zi

#endif
