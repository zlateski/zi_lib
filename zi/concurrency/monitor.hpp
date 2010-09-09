/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_MONITOR_HPP
#define ZI_CONCURRENCY_MONITOR_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/condition_variable.hpp>

#include <zi/bits/function.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>
#include <zi/time/now.hpp>

namespace zi {
namespace concurrency_ {


class monitor: non_copyable
{
private:

    condition_variable cv_;
    mutex              m_ ;

public:

    explicit monitor( int32_t count = 1 ): cv_(), m_()
    {
        ZI_VERIFY( count > 0 );
    }

private:

    void lock() const
    {
        m_.lock();
    }

    void unlock() const
    {
        m_.unlock();
    }

    void wait() const
    {
        cv_.wait( m_ );
    }

    bool timed_wait(int64_t ttl) const
    {
        return cv_.timed_wait( m_, ttl );
    }

    void notify_one() const
    {
        cv_.notify_one();
    }

    void notify_all() const
    {
        cv_.notify_all();
    }

public:

    class synchronized: non_copyable
    {
    private:

        const monitor &m_;

    public:

        synchronized( const monitor &s ): m_( s )
        {
            m_.lock();
        }

        ~synchronized()
        {
            m_.unlock();
        }

        void wait() const
        {
            m_.wait();
        }

        bool timed_wait( int64_t ttl ) const
        {
            return m_.timed_wait( ttl );
        }

        void notify_one() const
        {
            m_.notify_one();
        }

        void notify_all() const
        {
            m_.notify_all();
        }

        void wait_for( function< bool (void) > f ) const
        {
            while ( !f() )
            {
                m_.wait();
            }
        }

        template< class F, class A1 >
        void wait_for( F f, const A1& a1 ) const
        {
            while ( !f( a1 ) )
            {
                m_.wait();
            }
        }

        template< class F, class A1, class A2 >
        void wait_for( F f, const A1& a1, const A2& a2 ) const
        {
            while ( !f( a1, a2 ) )
            {
                m_.wait();
            }
        }

        template< class F, class A1, class A2, class A3 >
        void wait_for( F f, const A1& a1, const A2& a2, const A3& a3 ) const
        {
            while ( !f( a1, a2, a3 ) )
            {
                m_.wait();
            }
        }

        template< class F, class A1, class A2, class A3, class A4 >
        void wait_for( F f, const A1& a1, const A2& a2, const A3& a3, const A4& a4 ) const
        {
            while ( !f( a1, a2, a3, a4 ) )
            {
                m_.wait();
            }
        }

        template< class V >
        void wait_true( const V& v ) const
        {
            const volatile V& cvv = reinterpret_cast< const volatile V& >( v );

            while ( !(cvv) )
            {
                m_.wait();
            }
        }

        template< class V >
        void wait_false( const V& v ) const
        {
            const volatile V& cvv = reinterpret_cast< const volatile V& >( v );

            while ( cvv )
            {
                m_.wait();
            }
        }

        template< class L, class R >
        void wait_equals( const L& l, const R& r ) const
        {
            const volatile L& cvl = reinterpret_cast< const volatile L& >( l );
            const volatile R& cvr = reinterpret_cast< const volatile R& >( r );
            while ( cvl != cvr )
            {
                m_.wait();
            }
        }

        template< class L, class R >
        void wait_gt( const L& l, const R& r ) const
        {
            const volatile L& cvl = reinterpret_cast< const volatile L& >( l );
            const volatile R& cvr = reinterpret_cast< const volatile R& >( r );
            while ( cvl <= cvr )
            {
                m_.wait();
            }
        }

        template< class L, class R >
        void wait_gte( const L& l, const R& r ) const
        {
            const volatile L& cvl = reinterpret_cast< const volatile L& >( l );
            const volatile R& cvr = reinterpret_cast< const volatile R& >( r );
            while ( cvl < cvr )
            {
                m_.wait();
            }
        }

        template< class L, class R >
        void wait_lt( const L& l, const R& r ) const
        {
            const volatile L& cvl = reinterpret_cast< const volatile L& >( l );
            const volatile R& cvr = reinterpret_cast< const volatile R& >( r );
            while ( cvl >= cvr )
            {
                m_.wait();
            }
        }

        template< class L, class R >
        void wait_lte( const L& l, const R& r ) const
        {
            const volatile L& cvl = reinterpret_cast< const volatile L& >( l );
            const volatile R& cvr = reinterpret_cast< const volatile R& >( r );
            while ( cvl > cvr )
            {
                m_.wait();
            }
        }

        template< class L, class R >
        void wait_neq( const L& l, const R& r ) const
        {
            const volatile L& cvl = reinterpret_cast< const volatile L& >( l );
            const volatile R& cvr = reinterpret_cast< const volatile R& >( r );
            while ( cvl == cvr )
            {
                m_.wait();
            }
        }

    };

    typedef synchronized guard;


};



} // namespace concurrency_

using concurrency_::monitor;

} // namespace zi


#endif
