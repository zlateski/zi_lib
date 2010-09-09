/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_FOR_EACH_HPP
#define ZI_UTILITY_FOR_EACH_HPP 1

#include <zi/bits/typeof.hpp>

#if defined ( __typeof__ )

#  define FOR_EACH( it, cnt )                                   \
    FOR_EACH_RANGE( it, ( cnt ).begin(), ( cnt ).end() )

#  define FOR_EACH_R( it, cnt )                                 \
    FOR_EACH_RANGE( it, ( cnt ).rbegin(), ( cnt ).rend() )

#  define FOR_EACH_RANGE( it, begin, end )                              \
    for (__typeof__( begin ) it = ( begin ); it != ( end ); ++it)

#ifndef foreach
#  define foreach( it, cnt ) FOR_EACH( it, cnt )
#endif

#ifndef foreach_r
#  define foreach_r( it, cnt ) FOR_EACH_R( it, cnt )
#endif

#ifndef foreach_range
#  define foreach_range( it, begin, end ) FOR_EACH_RANGE( it, begin, end )
#endif

#else
#  error "can't define FOR_EACH macros with no __typeof__ defined"

#endif

#endif
