/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_THREAD_HPP
#define ZI_CONCURRENCY_THREAD_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/thread.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/thread.hpp>
#
#else
#  error "add other"
#endif

#include <zi/concurrency/detail/this_thread.hpp>
#include <zi/concurrency/detail/all_threads.hpp>

namespace zi {

using concurrency_::thread;



namespace this_thread {

using namespace concurrency_::this_thread;

} // namespace this_thread



namespace all_threads {

using namespace concurrency_::all_threads;

} // namespace all_threads



} // namespace zi

#endif
