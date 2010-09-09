/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#if defined( ZI_OS_LINUX )
#
#include <cstdlib>
#
#define  ZI_HAS_POSIX_SUPPORT
#include <zi/config/posix.hpp>
#
#if defined( __USE_POSIX199309 ) && !defined( ZI_HAS_NANOSLEEP )
#  define ZI_HAS_NANOSLEEP
#endif
#
#if defined( __GLIBC__ ) && ( __GLIBC__ >= 2 ) && !defined( ZI_HAS_GETTIMEOFDAY)
#  define ZI_HAS_GETTIMEOFDAY
#endif
#
#endif
