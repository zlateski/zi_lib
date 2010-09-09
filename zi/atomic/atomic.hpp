/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ATOMIC_ATOMIC_HPP
#define ZI_ATOMIC_ATOMIC_HPP 1

#include <zi/atomic/config.hpp>

#define ZI_ATOMIC_ATOMIC_HPP_INCLUDING 1

namespace zi {
namespace atomic {

#if defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
#  include <zi/atomic/detail/atomic_gcc_i386.hpp>
#
#elif defined( __GNUC__ ) && ( defined( __PPC__ ) || defined( __ppc__ ) )
#  include <zi/atomic/detail/atomic_gcc_ppc.hpp>
#
#elif defined( ZI_HAS_GCC_SYNC )
#  include <zi/atomic/detail/atomic_gcc_sync.hpp>
#
#elif defined( ZI_OS_WINDOWS )
#  include <zi/atomic/detail/atomic_win32.hpp>
#
#else
#  error "no atomic functions available"
#
#endif


} // namespace atomic
} // namespace zi

#undef ZI_ATOMIC_ATOMIC_HPP_INCLUDING

#endif
