/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_SYSTEM_DETAIL_MEMORY_SIZE_HPP
#define ZI_SYSTEM_DETAIL_MEMORY_SIZE_HPP 1

#include <zi/system/config.hpp>

namespace zi {
namespace system {
namespace detail {

inline int64_t memory_size()
{

#if defined( ZI_OS_LINUX )

    int64_t page_size = static_cast< uint32_t >( sysconf( _SC_PAGE_SIZE ) );
    return page_size * sysconf( _SC_PHYS_PAGES );

#elif defined( ZI_OS_MACOS )

    int     sysconf_command[2];
    int64_t sysconf_result    ;

    size_t  length = sizeof(int64_t);
    sysconf_command[0] = CTL_HW;
    sysconf_command[1] = HW_MEMSIZE;

    sysctl( sysconf_command, 2, &sysconf_result, &length, NULL, 0);

    return sysconf_result;

#elif defined ( ZI_OS_WINDOWS )

#ifdef WIN64
    MEMORYSTATUSEX ms;
#else
    MEMORYSTATUS ms;
#endif

    ms.dwLength = sizeof(ms);

    GlobalMemoryStatus(&ms);

#ifdef WIN64
    return static_cast< int64_t >( ms.ullTotalPhys );
#else
    return static_cast< int64_t >( ms.dwTotalPhys );
#endif

#else
#warning "no memory_size function available"
#endif

}

inline int64_t memory_available()
{

#if defined( ZI_OS_LINUX )

    int64_t page_size = static_cast< uint32_t >( sysconf( _SC_PAGE_SIZE ) );
    return page_size * sysconf( _SC_AVPHYS_PAGES );

#elif defined( ZI_OS_MACOS )

    return memory_size();

#elif defined ( ZI_OS_WINDOWS )

#ifdef WIN64
    MEMORYSTATUSEX ms;
#else
    MEMORYSTATUS ms;
#endif

    ms.dwLength = sizeof(ms);
    GlobalMemoryStatus(&ms);

#ifdef WIN64
    return static_cast< int64_t >( ms.ullAvailPhys );
#else
    return static_cast< int64_t >( ms.dwAvailPhys );
#endif

#else
#warning "no memory_size function available"
#endif

}

} // namespace detail
} // namespace system
} // namespace zi


#endif
