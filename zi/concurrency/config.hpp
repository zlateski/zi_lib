/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_CONFIG_HPP
#define ZI_CONCURRENCY_CONFIG_HPP 1

#include <zi/config/config.hpp>

#if defined( ZI_OS_WINDOWS )
#  // damn, windows.h defines min and mac macros
#  // which make numerical_limits< ... >::min() / max() unusable!
#  ifndef NOMINMAX
#    define NOMINMAX 1
#  endif
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    undef  WIN32_LEAN_AND_MEAN
#  else
#    include <windows.h>
#  endif
#  undef NOMINMAX
#
#  ifndef ZI_HAS_WINTHREADS
#    define ZI_HAS_WINTHREADS
#  endif
#
#endif

#if defined( ZI_HAS_PTHREADS )
#  if ( defined( __USE_XOPEN2K ) && !defined( ZI_HAS_PT_SPINLOCK ) )
#    define ZI_HAS_PT_SPINLOCK
#  endif
#endif

#include <zi/bits/cstdint.hpp>

#endif
