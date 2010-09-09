/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_MUTEX_TYPES_HPP
#define ZI_CONCURRENCY_WIN32_MUTEX_TYPES_HPP 1

#include <zi/concurrency/win32/mutex_tpl.hpp>

namespace zi {
namespace concurrency_ {

typedef mutex_tpl< mutex_default_tag >   mutex_default;
typedef mutex_tpl< mutex_adaptive_tag >  mutex_adaptive;
typedef mutex_tpl< mutex_recursive_tag > mutex_recursive;

} // namespace concurrency_
} // namespace zi

#endif
