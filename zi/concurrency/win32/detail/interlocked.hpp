/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_DETAIL_INTERLOCKED_HPP
#define ZI_CONCURRENCY_WIN32_DETAIL_INTERLOCKED_HPP 1

#include <zi/concurrency/config.hpp>

namespace zi {
namespace concurrency_ {
namespace win32 {

#if 0

#ifdef ZI_CXX_MSVC

extern "C" void _ReadWriteBarrier( void );
#pragma intrinsic( _ReadWriteBarrier )

inline long interlocked_read_acquire( volatile long* x )
{
    const long value = *x;
    _ReadWriteBarrier( );
    return value;
}

inline void* interlocked_read_acquire( volatile void** x )
{
    const void* value = *x;
    _ReadWriteBarrier( );
    return value;
}

inline void interlocked_write_release( volatile long* x, long value )
{
    _ReadWriteBarrier( );
    *x = value;
}

inline void interlocked_write_release( volatile void** x, void* value )
{
    _ReadWriteBarrier( );
    *x = value;
}

#else

inline long interlocked_read_acquire( long* x )
{
    return InterlockedCompareExchange( x, 0, 0 );
}

inline void* interlocked_read_acquire( void** x )
{
    return InterlockedCompareExchangePointer( x, 0, 0 );
}

inline void interlocked_write_release( long* x, long value )
{
    (void) InterlockedExchange( x, value );
}

inline void interlocked_write_release( void** x, void* value )
{
    (void) InterlockedExchangePointer( x, value );
}

#endif

#endif // 0

using ::InterlockedIncrement;
using ::InterlockedDecrement;
using ::InterlockedCompareExchange;
using ::InterlockedExchange;
using ::InterlockedExchangeAdd;


} // namespace win32
} // namespace concurrency_
} // namespace zi

#endif
