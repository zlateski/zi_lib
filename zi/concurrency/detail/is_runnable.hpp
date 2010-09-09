/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_IS_RUNNABLE_HPP
#define ZI_CONCURRENCY_DETAIL_IS_RUNNABLE_HPP 1

#include <zi/bits/type_traits.hpp>

namespace zi {
namespace concurrency_ {
namespace detail {


template< class Maybe, bool = is_class< Maybe >::value >
struct has_member_run
{
    static const bool value = false;
};

template< class Maybe >
struct has_member_run< Maybe, true >
{
    struct base
    {
        int run;
    };

    struct derrived: Maybe, base
    {
        derrived();
    };

    template< int base::* > struct test_struct;

    template< class T > static int  test( test_struct< &T::run >* );
    template< class T > static char test( ... );

    static const bool value = sizeof( test< derrived >(0) ) == 1;
};

template< class Maybe, bool = has_member_run< Maybe >::value >
struct has_runnable_run_method
{
    static const bool value = false;
};

template< class Maybe >
struct has_runnable_run_method< Maybe, true >
{
    template< class T >
    static char test( void ( T::* )() const ); // note: member function

    template< class T >
    static char test( void ( T::* )() );       // note: member function

    template< class T >
    static int  test( T );                     // note: member type

    static const bool value =
        sizeof( is_lockable< Maybe >::test( &Maybe::lock ) ) == 1;
};


} // namespace detail
} // namespace concurrency_
} // namespace zi

#endif

