/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONFIG_CONFIG_HPP
#define ZI_CONFIG_CONFIG_HPP 1

#if   defined ( __GNUC__ )
#  define ZI_CXX_GCC
#  include <zi/config/compiler/gcc.hpp>
#
#elif defined ( _MSC_VER )
#  define ZI_CXX_MSVC
#  include <zi/config/compiler/msvc.hpp>
#
#else
#
#  warning "Compiler not supported"
#
#endif

#if defined( linux ) || defined( __linux ) || defined( __linux__ )
#  define ZI_OS_LINUX
#  include <zi/config/os/linux.hpp>
#
#elif defined( __GNU__ ) || defined ( __GLIBC__ )
#  define ZI_OS_LINUX
#  include <zi/config/os/linux.hpp>
#
#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ )
#  define ZI_OS_WINDOWS
#  include <zi/config/os/windows.hpp>
#
#elif defined( __APPLE__ ) || defined( __APPLE_CC__ ) || defined( macintosh )
#  define ZI_OS_MACOS
#  include <zi/config/os/macos.hpp>
#
#elif defined( __CYGWIN32__ ) || defined ( __CYGWIN__ )
#  define ZI_OS_CYGWIN
#  include <zi/config/os/cygwin.hpp>
#
#else
#
#  warning "OS not supported"
#
#endif

#endif
