/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ATOMIC_DETAIL_ATOMIC_WIN32_HPP
#define ZI_ATOMIC_DETAIL_ATOMIC_WIN32_HPP 1

#ifndef ZI_ATOMIC_ATOMIC_HPP_INCLUDING
#  error "don't include this file directly, use atomic.hpp"
#endif

typedef long atomic_word;

inline atomic_word compare_swap( volatile atomic_word *ptr, atomic_word val, atomic_word cmp )
{
    return ::InterlockedCompareExchange( ptr, val, cmp );
}

inline atomic_word add_swap( volatile atomic_word *ptr, atomic_word val )
{
    return ::InterlockedExchangeAdd( ptr, val );
}


inline void increment( volatile atomic_word *ptr )
{
    (void) ::InterlockedIncrement( ptr );
}

inline void decrement( volatile atomic_word *ptr )
{
    (void) ::InterlockedDecrement( ptr );
}

inline atomic_word increment_swap( volatile atomic_word *ptr )
{
    return ::InterlockedExchangeAdd( ptr, 1 );
}

inline atomic_word decrement_swap( volatile atomic_word *ptr )
{
    return ::InterlockedExchangeAdd( ptr, -1 );
}

inline void write( volatile atomic_word *ptr, atomic_word val )
{
    (void) ::InterlockedExchange( ptr, val );
}

inline atomic_word read( volatile atomic_word *ptr )
{
    return InterlockedCompareExchange( ptr, 0, 0 );
}

inline atomic_word test_increment_swap( volatile atomic_word *ptr )
{
    atomic_word ret;

    do {
        ret = read( ptr );

        if (ret == 0 )
        {
            return ret;
        }

    } while ( ::InterlockedCompareExchange( ptr, ret, ret + 1 ) != ret );

    return ret;
}

#endif
