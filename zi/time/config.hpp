/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_TIME_CONFIG_HPP
#define ZI_TIME_CONFIG_HPP 1

#include <zi/config/config.hpp>

#ifndef ZI_HAS_CLOCK_GETTIME
#
#  if defined( ZI_HAS_GETTIMEOFDAY )
#    include <sys/time.h>
#
#  elif defined( ZI_OS_WINDOWS )
#
#    ifndef WIN32_LEAN_AND_MEAN
#      define WIN32_LEAN_AND_MEAN
#      include <windows.h>
#      undef  WIN32_LEAN_AND_MEAN
#    else
#      include <windows.h>
#    endif
#
#    include <time.h>
#
#    ifndef _TIMEVAL_DEFINED
#      define _TIMEVAL_DEFINED
#      define ZI_NEEDS_TIMEVAL
#    endif
#
#  else
#    error "no high precision timer found"
#
#  endif
#
#endif

#ifdef ZI_NEEDS_TIMEVAL

struct timeval {
    long tv_sec;
    long tv_usec;
};

#undef ZI_NEEDS_TIMEVAL
#endif

#endif
