/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#if defined( ZI_OS_WINDOWS )
#
#if defined( __MINGW32__ )
#
#  include <_mingw.h>
#
#  if ( __MINGW32_MAJOR_VERSION >= 2 )
#    define  ZI_HAS_POSIX_SUPPORT
#    include <zi/config/posix.hpp>
#  endif
#
#endif
#
#if !defined( WIN64 )
#  if ( defined( _WIN64 ) || defined( __WIN64__ ) || defined( _M_X64 ) )
#    define WIN64
#  endif
#endif
#
#else
#  warning "windows not detected"
#endif
