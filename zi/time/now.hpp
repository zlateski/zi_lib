/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_TIME_NOW_HPP
#define ZI_TIME_NOW_HPP 1

#include <zi/time/config.hpp>
#include <zi/time/time_utils.hpp>
#include <zi/bits/cstdint.hpp>
#include <zi/utility/assert.hpp>

#include <ctime>

namespace zi {
namespace now {

inline int64_t seconds()
{
    return static_cast< int64_t >( std::time( NULL ) );
}

inline int64_t sec()
{
    return static_cast< int64_t >( std::time( NULL ) );
}

#if defined( ZI_HAS_CLOCK_GETTIME )

#define NOW_IN_XXX_CONSTRUCT( what, fn )                        \
    inline int64_t fn ()                                        \
    {                                                           \
        timespec ts;                                            \
        ZI_VERIFY_0( clock_gettime( CLOCK_REALTIME, &ts ) );    \
        return time_utils::ts_to_##what ( ts );                 \
    }

#elif defined( ZI_HAS_GETTIMEOFDAY )

#define NOW_IN_XXX_CONSTRUCT( what, fn )                        \
    inline int64_t fn ()                                        \
    {                                                           \
        timeval tv;                                             \
        ZI_VERIFY_0( gettimeofday( &tv, NULL ) );               \
        return time_utils::tv_to_##what ( tv );                 \
    }

#else

#define NOW_IN_XXX_CONSTRUCT( what, fn )                        \
    inline int64_t fn ()                                        \
    {                                                           \
        FILETIME ft;                                            \
        GetSystemTimeAsFileTime( &ft );                         \
        return time_utils::ft_to_##what ( ft );                 \
    }

#endif

NOW_IN_XXX_CONSTRUCT( msec, msec )
NOW_IN_XXX_CONSTRUCT( msec, milliseconds )
NOW_IN_XXX_CONSTRUCT( usec, usec )
NOW_IN_XXX_CONSTRUCT( usec, microseconds )
NOW_IN_XXX_CONSTRUCT( nsec, nsec )
NOW_IN_XXX_CONSTRUCT( nsec, nanoseconds  )

#undef NOW_IN_XXX_CONSTRUCT

} // namespace now
} // namespace zi

#endif
