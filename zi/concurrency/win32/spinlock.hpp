/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_SPINLOCK_HPP
#define ZI_CONCURRENCY_WIN32_SPINLOCK_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/detail/mutex_guard.hpp>
#include <zi/concurrency/detail/mutex_pool.hpp>
#include <zi/concurrency/win32/detail/primitives.hpp>

#include <zi/utility/non_copyable.hpp>

// silly int->bool warning in msvc
#if defined( ZI_CXX_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4800 )
#endif

namespace zi {
namespace concurrency_ {


class spinlock: non_copyable
{
private:
    mutable win32::critical_section cs_;

public:

    spinlock()
    {
        win32::InitializeCriticalSection( &cs_ );
    }

    ~spinlock()
    {
        win32::DeleteCriticalSection( &cs_ );
    }

    inline bool try_lock() const
    {
        return win32::TryEnterCriticalSection( &cs_ );
    }

    inline void lock() const
    {
        win32::EnterCriticalSection( &cs_ );
    }

    inline void unlock() const
    {
        win32::LeaveCriticalSection( &cs_ );
    }

    typedef mutex_guard< spinlock > guard;

    template< class Tag >
    struct pool: mutex_pool< Tag, spinlock > { };

};


} // namespace concurrency_
} // namespace zi

#if defined( ZI_CXX_MSVC )
#  pragma warning( pop )
#endif

#endif
