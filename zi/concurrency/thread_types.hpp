/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_THREAD_TYPES_HPP
#define ZI_CONCURRENCY_THREAD_TYPES_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/types.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/types.hpp>
#
#else
#  error "add other"
#endif

#include <zi/concurrency/detail/thread_info.hpp>
#include <zi/concurrency/detail/all_threads_data.hpp>
#include <zi/utility/singleton.hpp>

namespace zi {
namespace concurrency_ {


typedef detail::thread_info< thread_id_type, native_thread_handle >      thread_info      ;
typedef detail::all_threads_data< thread_id_type, native_thread_handle > all_threads_data ;


namespace {

static all_threads_data &all_threads_info = singleton< all_threads_data >::instance();

}


} // namespace concurrency_
} // namespace zi

#endif
