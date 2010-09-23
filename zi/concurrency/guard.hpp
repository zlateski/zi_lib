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

#ifndef ZI_CONCURRENCY_GUARD_HPP
#define ZI_CONCURRENCY_GUARD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/spinlock.hpp>
#include <zi/concurrency/rwmutex.hpp>
#include <zi/concurrency/detail/is_mutex.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/enable_if.hpp>

namespace zi {
namespace concurrency_ {

// forward declarations
class condition_variable;
class event;

struct guard_container_wrapper
{
    guard_container_wrapper() {}
    virtual ~guard_container_wrapper() {}
};

template< class Mutex >
struct guard_container: guard_container_wrapper
{
private:
    const Mutex &m_;

public:
    explicit guard_container( const Mutex& m ): m_( m )
    {
        m_.lock();
    }

    virtual ~guard_container()
    {
        m_.unlock();
    }

    friend class condition_variable;
    friend class event;

};

class guard: non_copyable
{
private:
    guard_container_wrapper *gcw_;

public:
    template< class Guarded >
    explicit guard( const Guarded &m,
                    typename enable_if< detail::is_mutex< Guarded >::value >::type* = 0 ):
        gcw_( new guard_container< Guarded >( m ) )
    {
    }

    ~guard()
    {
        delete gcw_;
    }

    friend class condition_variable;
    friend class event;
};

} // namespace concurrency_

using concurrency_::guard;

} // namespace zi

#endif
