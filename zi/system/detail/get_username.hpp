/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_SYSTEM_DETAIL_GET_USERNAME_HPP
#define ZI_SYSTEM_DETAIL_GET_USERNAME_HPP 1

#include <zi/system/config.hpp>

#include <zi/utility/assert.hpp>

namespace zi {
namespace system {
namespace detail {

inline std::string get_username()
{

    char buff[1024];

#if ( defined( ZI_OS_LINUX ) || defined( ZI_OS_MACOS ) )

    if ( !getlogin_r(buff, 1023) )
    {
        return std::string(buff);
    }
    else
    {
        return "";
    }

#elif defined ( ZI_OS_WINDOWS )

    DWORD maxLen = 1023;
    ZI_VERIFY( GetUserName(buff, &maxLen) );
    return std::string(buff, maxLen);

#else
#warning "no get_username function available"
#endif

}

} // namespace detail
} // namespace system
} // namespace zi


#endif
