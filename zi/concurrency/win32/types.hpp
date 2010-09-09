/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_TYPES_HPP
#define ZI_CONCURRENCY_WIN32_TYPES_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/win32/detail/primitives.hpp>

namespace zi {
namespace concurrency_ {

typedef win32::dword  thread_id_type      ;
typedef win32::handle native_thread_handle;


} // namespace concurrency_
} // namespace zi

#endif
