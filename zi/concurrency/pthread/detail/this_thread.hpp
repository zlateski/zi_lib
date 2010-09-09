/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_PTHREAD_DETAIL_THIS_THREAD_HPP
#define ZI_CONCURRENCY_PTHREAD_DETAIL_THIS_THREAD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/bits/cstdint.hpp>

#include <pthread.h>
#include <sched.h>

namespace zi {
namespace concurrency_ {

namespace this_thread {

inline void usleep_nt( int64_t usec )
{
    usleep( usec );
}

inline pthread_t id()
{
    return pthread_self();
}

inline void yield()
{
    sched_yield();
}


} // namespace this_thread

} // namespace concurrency_
} // namespace zi

#endif

