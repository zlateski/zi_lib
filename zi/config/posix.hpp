/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#if defined( ZI_HAS_POSIX_SUPPORT )
#
#include <unistd.h>
#
#if defined( _POSIX_TIMERS ) && ( _POSIX_TIMERS+0 >= 0 )
#  define ZI_HAS_CLOCK_GETTIME
#  define ZI_HAS_NANOSLEEP
#elif defined( _XOPEN_REALTIME ) && ( _XOPEN_REALTIME+0 >= 0 )
#  define ZI_HAS_NANOSLEEP
#endif
#
#
#if defined( _POSIX_THREADS ) && ( _POSIX_THREADS+0 >= 0 )
#  define ZI_HAS_PTHREADS
#endif
#
#
#if defined( _XOPEN_VERSION ) && ( _XOPEN_VERSION+0 >= 500 )
#  define ZI_HAS_GETTIMEOFDAY
#endif
#
#
#endif
