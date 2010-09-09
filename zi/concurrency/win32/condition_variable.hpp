/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_CONDITION_VARIABLE_HPP
#define ZI_CONCURRENCY_WIN32_CONDITION_VARIABLE_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/win32/mutex_types.hpp>
#include <zi/concurrency/win32/spinlock.hpp>
#include <zi/concurrency/win32/detail/primitives.hpp>
#include <zi/concurrency/win32/detail/interlocked.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>

#include <zi/time/now.hpp>
#include <zi/time/interval.hpp>

namespace zi {
namespace concurrency_ {


class condition_variable: non_copyable
{
private:
    spinlock              spinlock_     ;
    win32::handle         semaphore_    ;
    win32::handle         last_event_   ;
    mutable bool          broadcasting_ ;
    mutable win32::dword  waiters_      ;

    template< class Mutex >
    bool wait_( const Mutex &mutex, win32::dword ttl = win32::forever ) const
    {
        {
            spinlock::guard g( spinlock_ );
            ++waiters_;
        }

        bool got_it = win32::SignalObjectAndWait( mutex.handle_, semaphore_, ttl, false ) == 0;

        bool last;

        {
            spinlock::guard g( spinlock_ );
            --waiters_;
            last = broadcasting_ && waiters_ == 0;
        }


        if ( last )
        {
            ZI_VERIFY_0( win32::SignalObjectAndWait( last_event_, mutex.handle_,
                                                     win32::forever, false ) );
        }
        else
        {
            ZI_VERIFY_0( win32::WaitForSingleObject( mutex.handle_, win32::forever ) );
        }

        return got_it;
    }


public:

    condition_variable():
        spinlock_(),
        semaphore_( win32::CreateSemaphore( NULL, 0, 0x7FFFFFFF, NULL ) ),
        last_event_( win32::CreateSemaphore( NULL, 0, 0x7FFFFFFF, NULL ) ),
        broadcasting_( false ),
        waiters_( 0 )
    {
        ZI_VERIFY( semaphore_  );
        ZI_VERIFY( last_event_ );
    }

    ~condition_variable()
    {
        ZI_VERIFY( win32::CloseHandle( semaphore_  ) );
        ZI_VERIFY( win32::CloseHandle( last_event_ ) );
    }

    template< class MutexTag >
    void wait( const mutex_tpl< MutexTag > &mutex ) const
    {
        (void) wait_( mutex );
    }

    template< class Mutex >
    void wait( const mutex_guard< Mutex > &g ) const
    {
        (void) wait_( g.m_ );
    }

    template< class MutexTag >
    bool timed_wait( const mutex_tpl< MutexTag > &mutex, int64_t ttl ) const
    {
        return wait_( mutex, static_cast< win32::dword >( ttl ) );
    }

    template< class Mutex >
    bool timed_wait( const mutex_guard< Mutex > &g, int64_t ttl ) const
    {
        return wait_( g.m_, static_cast< win32::dword >( ttl ) );
    }

    template< class MutexTag, int64_t I >
    bool timed_wait( const mutex_tpl< MutexTag > &mutex,
                     const interval::detail::interval_tpl< I > &ttl ) const
    {
        return wait_( mutex, ttl.msecs() );
    }

    template< class Mutex, int64_t I >
    bool timed_wait( const mutex_guard< Mutex > &g,
                     const interval::detail::interval_tpl< I > &ttl ) const
    {
        return wait_( g.m_, static_cast< win32::dword >( ttl.msecs() ) );
    }

    template< class MutexTag >
    bool timed_wait_usec( const mutex_tpl< MutexTag > &mutex, int64_t ttl ) const
    {
        return wait_( mutex, static_cast< win32::dword >
                      ( ttl / 1000 + ( ( ttl % 1000 > 500 ) ? 1 : 0 ) ) );
    }

    template< class Mutex >
    bool timed_wait_usec( const mutex_guard< Mutex > &g, int64_t ttl ) const
    {
        return wait_( g.m_, static_cast< win32::dword >
                      ( ttl / 1000 + ( ( ttl % 1000 > 500 ) ? 1 : 0 ) ) );
    }

    void notify_one() const
    {
        if ( waiters_ > 0 )
        {
            win32::ReleaseSemaphore( semaphore_, 1, 0 );
        }
    }

    void notify_all() const
    {
        spinlock_.lock();

        broadcasting_ = waiters_ > 0;

        if ( broadcasting_ )
        {
            ZI_VERIFY( win32::ReleaseSemaphore( semaphore_, waiters_, 0 ) );
            spinlock_.unlock();
            ZI_VERIFY_0( win32::WaitForSingleObject( last_event_, win32::forever ) );
            broadcasting_ = 0;
        }
        else
        {
            spinlock_.unlock();
        }
    }
};


} // namespace concurrency_
} // namespace zi


#endif
