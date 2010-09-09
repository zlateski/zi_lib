/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_DETAIL_THIS_THREAD_HPP
#define ZI_CONCURRENCY_WIN32_DETAIL_THIS_THREAD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/win32/detail/primitives.hpp>

namespace zi {
namespace concurrency_ {

namespace this_thread {

inline void usleep_nt( int64_t usec )
{
    Sleep( usec / 1000 + ( ( usec % 1000 > 500 ) ? 1 : 0 ) );
}

inline win32::dword id()
{
    return win32::GetCurrentThreadId();
}

inline void yield()
{
    Yield();
}

} // namespace this_thread

} // namespace concurrency_
} // namespace zi

#endif

