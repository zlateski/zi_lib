/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_SYSTEM_CONFIG_HPP
#define ZI_SYSTEM_CONFIG_HPP 1

#include <zi/config/config.hpp>

#if defined( ZI_OS_WINDOWS )
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    undef  WIN32_LEAN_AND_MEAN
#  else
#    include <windows.h>
#  endif
#  include <winsock.h>
#  pragma comment(lib, "Ws2_32.lib")
#
#elif defined( ZI_OS_MACOS )
#  include <sys/types.h>
#  include <sys/sysctl.h>
#  include <mach/task.h>
#  include <mach/mach_init.h>
#  include <mach/shared_memory_server.h>
#
#elif defined( ZI_OS_LINUX )
#
#else
#  error "detected os is not supported"
#
#endif

#include <zi/bits/cstdint.hpp>

#endif
