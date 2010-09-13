/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_PTHREAD_CONDITION_VARIABLE_HPP
#define ZI_CONCURRENCY_PTHREAD_CONDITION_VARIABLE_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/pthread/mutex_types.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>

#include <zi/time/now.hpp>
#include <zi/time/interval.hpp>

#include <pthread.h>


namespace zi {
namespace concurrency_ {


class condition_variable: non_copyable
{
private:
    mutable pthread_cond_t cv_;

public:
    condition_variable()
    {
        ZI_VERIFY_0( pthread_cond_init( &cv_, NULL ) );
    }

    ~condition_variable()
    {
        ZI_VERIFY_0( pthread_cond_destroy( &cv_ ) );
    }

    template< class MutexTag >
    void wait( const mutex_tpl< MutexTag > &mutex ) const
    {
        ZI_VERIFY_0( pthread_cond_wait( &cv_, &mutex.mutex_ ) );
    }

    template< class Mutex >
    void wait( const mutex_guard< Mutex > &g ) const
    {
        ZI_VERIFY_0( pthread_cond_wait( &cv_, &g.m_.mutex_ ) );
    }

    template< class MutexTag >
    bool timed_wait( const mutex_tpl< MutexTag > &mutex, int64_t ttl ) const
    {
        timespec ts;
        time_utils::msec_to_ts( ts, now::msec() + ttl );
        return pthread_cond_timedwait( &cv_, &mutex.mutex_, &ts ) == 0;
    }

    template< class Mutex >
    bool timed_wait( const mutex_guard< Mutex > &g, int64_t ttl ) const
    {
        timespec ts;
        time_utils::msec_to_ts( ts, now::msec() + ttl );
        return pthread_cond_timedwait( &cv_, &g.m_.mutex_, &ts ) == 0;
    }

    template< class MutexTag, int64_t I >
    bool timed_wait( const mutex_tpl< MutexTag > &mutex,
                     const interval::detail::interval_tpl< I > &ttl ) const
    {
        timespec ts;
        time_utils::nsec_to_ts( ts, now::nsec() + ttl.nsecs() );
        return pthread_cond_timedwait( &cv_, &mutex.mutex_, &ts ) == 0;
    }

    template< class Mutex, int64_t I >
    bool timed_wait( const mutex_guard< Mutex > &g,
                     const interval::detail::interval_tpl< I > &ttl ) const
    {
        timespec ts;
        time_utils::nsec_to_ts( ts, now::nsec() + ttl.nsecs() );
        return pthread_cond_timedwait( &cv_, &g.m_.mutex_, &ts ) == 0;
    }

    template< class MutexTag >
    bool timed_wait_usec( const mutex_tpl< MutexTag > &mutex, int64_t ttl ) const
    {
        timespec ts;
        time_utils::usec_to_ts( ts, now::usec() + ttl );
        return pthread_cond_timedwait( &cv_, &mutex.mutex_, &ts ) == 0;
    }

    template< class Mutex >
    bool timed_wait_usec( const mutex_guard< Mutex > &g, int64_t ttl ) const
    {
        timespec ts;
        time_utils::usec_to_ts( ts, now::usec() + ttl );
        return pthread_cond_timedwait( &cv_, &g.m_.mutex_, &ts ) == 0;
    }

    void notify_one() const
    {
        ZI_VERIFY_0( pthread_cond_signal( &cv_ ) );
    }

    void notify_all() const
    {
        ZI_VERIFY_0( pthread_cond_broadcast( &cv_ ) );
    }
};


} // namespace concurrency_
} // namespace zi


#endif
