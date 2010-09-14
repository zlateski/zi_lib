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

#ifndef ZI_CONCURRENCY_DETAIL_IS_MUTEX_HPP
#define ZI_CONCURRENCY_DETAIL_IS_MUTEX_HPP 1

#include <zi/bits/type_traits.hpp>

namespace zi {
namespace concurrency_ {
namespace detail {


template< class MaybeMutex, bool = is_class< MaybeMutex >::value >
struct has_member_lock
{
    static const bool value = false;
};

template< class MaybeMutex >
struct has_member_lock< MaybeMutex, true >
{
    struct base
    {
        int lock;
    };

    struct derrived: MaybeMutex, base
    {
        derrived();
    };

    template< int base::* > struct test_struct;

    template< class T > static int  test( test_struct< &T::lock >* );
    template< class T > static char test( ... );

    static const bool value = sizeof( test< derrived >(0) ) == 1;
};

template< class MaybeMutex, bool = is_class< MaybeMutex >::value >
struct has_member_unlock
{
    static const bool value = false;
};

template< class MaybeMutex >
struct has_member_unlock< MaybeMutex, true >
{
    struct base
    {
        int unlock;
    };

    struct derrived: MaybeMutex, base
    {
        derrived();
    };

    template< int base::* > struct test_struct;

    template< class T > static int  test( test_struct< &T::unlock >* );
    template< class T > static char test( ... );

    static const bool value = sizeof( test< derrived >(0) ) == 1;
};

template< class Maybe, bool = has_member_lock< Maybe >::value >
struct is_lockable
{
    static const bool value = false;
};

template< class Maybe >
struct is_lockable< Maybe, true >
{
    template< class T >
    static char test( void ( T::* )() const ); // note: member function

    template< class T >
    static int  test( T );                     // note: member type

    static const bool value =
        sizeof( is_lockable< Maybe >::test( &Maybe::lock ) ) == 1;
};

template< class Maybe, bool = has_member_unlock< Maybe >::value >
struct is_unlockable
{
    static const bool value = false;
};

template< class Maybe >
struct is_unlockable< Maybe, true >
{
    template< class T >
    static char test( void ( T::* )() const ); // note: member function

    template< class T >
    static int  test( T );                     // note: member type

    static const bool value =
        sizeof( is_unlockable< Maybe >::test( &Maybe::unlock ) ) == 1;
};

template< class MaybeMutex >
struct is_mutex
{
    static const bool value =
        is_lockable  < MaybeMutex >::value &&
        is_unlockable< MaybeMutex >::value;
};

} // namespace detail
} // namespace concurrency_
} // namespace zi

#endif

