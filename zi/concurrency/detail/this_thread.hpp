/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_THIS_THREAD_HPP
#define ZI_CONCURRENCY_DETAIL_THIS_THREAD_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/detail/this_thread.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/detail/this_thread.hpp>
#
#else
#  error "add other"
#
#endif

#include <zi/concurrency/thread_types.hpp>
#include <zi/utility/assert.hpp>
#include <zi/time/interval.hpp>
#include <zi/bits/cstdint.hpp>

namespace zi {
namespace concurrency_ {
namespace this_thread {


template< int64_t I >
void sleep( const interval::detail::interval_tpl< I > &i )
{
    shared_ptr< thread_info > ti = all_threads_info.get_thread( id() );

    if ( ti )
    {
        ti->sleep( i );
    }
    else
    {
        usleep_nt( i.usecs() );
    }
}

inline void sleep( int64_t msec )
{
    shared_ptr< thread_info > ti = all_threads_info.get_thread( id() );

    if ( ti )
    {
        ti->sleep( msec );
    }
    else
    {
        usleep_nt( msec * 1000 );
    }
}

inline void usleep( int64_t usec )
{
    shared_ptr< thread_info > ti = all_threads_info.get_thread( id() );

    if ( ti )
    {
        ti->usleep( usec );
    }
    else
    {
        usleep_nt( usec );
    }
}


} // namespace this_thread
} // namespace concurrency_
} // namespace zi

#endif
