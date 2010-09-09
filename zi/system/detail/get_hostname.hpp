/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_SYSTEM_DETAIL_GET_HOSTNAME_HPP
#define ZI_SYSTEM_DETAIL_GET_HOSTNAME_HPP 1

#include <zi/system/config.hpp>

#include <zi/utility/assert.hpp>

namespace zi {
namespace system {
namespace detail {

inline std::string get_hostname()
{

    char buff[1024];

#if ( defined( ZI_OS_LINUX ) || defined( ZI_OS_MACOS ) )

    if ( !gethostname(buff, 1023) )
    {
        return std::string(buff);
    }
    else
    {
        return "hostname";
    }

#elif defined ( ZI_OS_WINDOWS )

    WSADATA wsaData = { 0 };
    if ( WSAStartup(MAKEWORD(2, 2), &wsaData) )
    {
        return "hostname";
    }
    else
    {
        DWORD maxLen = 1023;
        gethostname(buff, maxLen);
        return std::string(buff);
    }

#else
#warning "no get_hostname function available"
#endif

}

} // namespace detail
} // namespace system
} // namespace zi


#endif
