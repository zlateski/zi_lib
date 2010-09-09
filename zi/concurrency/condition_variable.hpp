/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_CONDITION_VARIABLE_HPP
#define ZI_CONCURRENCY_CONDITION_VARIABLE_HPP 1

#include <zi/concurrency/config.hpp>

#if defined( ZI_HAS_PTHREADS )
#  include <zi/concurrency/pthread/condition_variable.hpp>
#
#elif defined( ZI_HAS_WINTHREADS )
#  include <zi/concurrency/win32/condition_variable.hpp>
#
#else
#  error "add other"
#endif

namespace zi {

typedef concurrency_::condition_variable condition_variable;

} // namespace zi

#endif
