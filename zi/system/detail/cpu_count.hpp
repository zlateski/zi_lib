/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_SYSTEM_DETAIL_CPU_COUNT_HPP
#define ZI_SYSTEM_DETAIL_CPU_COUNT_HPP 1

#include <zi/system/config.hpp>

namespace zi {
namespace system {
namespace detail {

inline int32_t cpu_count()
{

#if ( defined( ZI_OS_LINUX ) || defined( ZI_OS_MACOS ) )

    return static_cast< int32_t >( sysconf( _SC_NPROCESSORS_CONF ) );

#elif defined ( ZI_OS_WINDOWS )

    SYSTEM_INFO info;
    GetSystemInfo( &info );
    return static_cast< int32_t >( info.dwNumberOfProcessors );

#else
#warning "no cpu_count function available"
#endif

}

} // namespace detail
} // namespace system
} // namespace zi


#endif
