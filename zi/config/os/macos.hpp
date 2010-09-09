/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#if defined( Zi_OS_MACOS )
#
#define  ZI_HAS_POSIX_SUPPORT
#include <zi/config/posix.hpp>
#
#define ZI_HAS_GETTIMEOFDAY
#
#if ( __GNUC__ >= 4 )
#  define ZI_HAS_NANOSLEEP
#endif
#
#endif
