/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_SPINLOCK_HPP
#define ZI_CONCURRENCY_SPINLOCK_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/spinlock.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/spinlock.hpp>
#
#else
#  error "add other"
#endif

namespace zi {

using concurrency_::spinlock;

} // namespace zi

#endif
