/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_MUTEX_TPL_HPP
#define ZI_CONCURRENCY_WIN32_MUTEX_TPL_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/win32/mutex_tags.hpp>
#include <zi/concurrency/win32/default_mutex.hpp>
#include <zi/concurrency/win32/recursive_mutex.hpp>

namespace zi {
namespace concurrency_ {

template< class PtMutexTag > class mutex_tpl;

template<> struct mutex_tpl< mutex_default_tag >  : default_mutex  {};
template<> struct mutex_tpl< mutex_adaptive_tag > : default_mutex  {};
template<> struct mutex_tpl< mutex_recursive_tag >: recursive_mutex{};

// alternative:
// template<> class mutex_tpl< mutex_recursive_tag >: recursive_spinlock;


} // namespace concurrency_
} // namespace zi

#endif
