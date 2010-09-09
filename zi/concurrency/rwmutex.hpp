/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_RWMUTEX_HPP
#define ZI_CONCURRENCY_RWMUTEX_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/rwmutex.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/rwmutex.hpp>
#
#else
#  error "add other"
#endif

namespace zi {

typedef concurrency_::rwmutex rwmutex;

} // namespace zi

#endif
