/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_SYSTEM_SYSTEM_HPP
#define ZI_SYSTEM_SYSTEM_HPP 1

#include <zi/system/detail/cpu_count.hpp>
#include <zi/system/detail/memory_size.hpp>
#include <zi/system/detail/memory_usage.hpp>
#include <zi/system/detail/get_username.hpp>
#include <zi/system/detail/get_hostname.hpp>

#include <zi/bits/cstdint.hpp>
#include <zi/bits/type_traits.hpp>

#include <zi/utility/enable_if.hpp>

#include <string>

namespace zi {
namespace system {

namespace {

static int32_t cpu_count  ( detail::cpu_count()   );
static int64_t memory_size( detail::memory_size() );
static int64_t memory_kb  ( ( memory_size >> 10 ) + ( memory_size % 1024 >= 512 ? 1 : 0 ));
static int64_t memory_mb  ( ( memory_kb   >> 10 ) + ( memory_kb   % 1024 >= 512 ? 1 : 0 ));
static int64_t memory_gb  ( ( memory_mb   >> 10 ) + ( memory_mb   % 1024 >= 512 ? 1 : 0 ));

} // namespace

struct memory {

    static inline int64_t available()
    {
        return detail::memory_available();
    }

    static inline int64_t available_kb()
    {
        int64_t m = available();
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    static inline int64_t available_mb()
    {
        int64_t m = available_kb();
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    static inline int64_t available_gb()
    {
        int64_t m = available_mb();
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    static inline int64_t total()
    {
        return memory_size;
    }

    static inline int64_t total_kb()
    {
        return memory_kb;
    }

    static inline int64_t total_mb()
    {
        return memory_mb;
    }

    static inline int64_t total_gb()
    {
        return memory_gb;
    }

    static inline int64_t usage( bool virt = false )
    {
        return detail::memory_usage( virt );
    }

    static inline int64_t usage_kb( bool virt = false )
    {
        int64_t m = usage( virt );
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    static inline int64_t usage_mb( bool virt = false )
    {
        int64_t m = usage_kb( virt );
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    static inline int64_t usage_gb( bool virt = false )
    {
        int64_t m = usage_mb( virt );
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T available
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        return available();
    }

    template< class T >
    static inline T available
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( available() );
    }

    template< class T >
    static inline T available_kb
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        int64_t m = available();
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T available_kb
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( available() ) / 1024.0;
    }

    template< class T >
    static inline T available_mb
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        int64_t m = available_kb();
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T available_mb
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( available() ) / 1024.0 / 1024.0;
    }

    template< class T >
    static inline T available_gb
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        int64_t m = available_mb();
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T available_gb
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( available() ) / 1024.0 / 1024.0 / 1024.0;
    }

    template< class T >
    static inline T total
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        return memory_size;
    }

    template< class T >
    static inline T total
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( memory_size );
    }

    template< class T >
    static inline T total_kb
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        return memory_kb;
    }

    template< class T >
    static inline T total_kb
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( memory_size ) / 1024.0;
    }

    template< class T >
    static inline T total_mb
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        return memory_mb;
    }

    template< class T >
    static inline T total_mb
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( memory_size ) / 1024.0 / 1024.0;
    }

    template< class T >
    static inline T total_gb
    ( typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        return memory_gb;
    }

    template< class T >
    static inline T total_gb
    ( typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( memory_size ) / 1024.0 / 1024.0 / 1024.0;
    }

    template< class T >
    static inline T usage
    ( bool virt = false, typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        return usage( virt );
    }

    template< class T >
    static inline T usage
    ( bool virt = false, typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( usage( virt ) );
    }

    template< class T >
    static inline T usage_kb
    ( bool virt = false, typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        int64_t m = usage( virt );
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T usage_kb
    ( bool virt = false, typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( usage( virt ) ) / 1024.0;
    }

    template< class T >
    static inline T usage_mb
    ( bool virt = false, typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        int64_t m = usage_kb( virt );
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T usage_mb
    ( bool virt = false, typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( usage( virt ) ) / 1024.0 / 1024.0;
    }

    template< class T >
    static inline T usage_gb
    ( bool virt = false, typename enable_if< is_integral< T >::value >::type* = 0 )
    {
        int64_t m = usage_mb( virt );
        return ( m >> 10 ) + ( m % 1024 > 512 ? 1 : 0 );
    }

    template< class T >
    static inline T usage_gb
    ( bool virt = false, typename enable_if< is_floating_point< T >::value >::type* = 0 )
    {
        return static_cast< T >( usage( virt ) ) / 1024.0 / 1024.0 / 1024.0;
    }

};

using detail::get_username;
using detail::get_hostname;

namespace {

static std::string username( get_username() );
static std::string hostname( get_hostname() );

} // namespace

} // namespace system
} // namespace zi


#endif
