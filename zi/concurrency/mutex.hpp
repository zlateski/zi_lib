/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_MUTEX_HPP
#define ZI_CONCURRENCY_MUTEX_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/mutex_types.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/mutex_types.hpp>
#
#else
#  error "add other"
#endif

namespace zi {


struct mutex: concurrency_::mutex_default
{
    typedef concurrency_::mutex_adaptive  adaptive;
    typedef concurrency_::mutex_recursive recursive;
};

// alternative:
// typedef concurrency_::mutex_default   mutex;

typedef concurrency_::mutex_adaptive  adaptive_mutex;
typedef concurrency_::mutex_recursive recursive_mutex;

} // namespace zi

#endif
