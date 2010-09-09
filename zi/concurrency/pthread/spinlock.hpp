/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_PTHREAD_SPINLOCK_HPP
#define ZI_CONCURRENCY_PTHREAD_SPINLOCK_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/detail/mutex_guard.hpp>
#include <zi/concurrency/detail/mutex_pool.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>

#include <pthread.h>

namespace zi {
namespace concurrency_ {


class spinlock: non_copyable
{
private:

    mutable pthread_spinlock_t spin_;

public:

    spinlock(): spin_(0)
    {
        ZI_VERIFY_0( pthread_spin_init( &spin_, 0 ) );
    }

    ~spinlock()
    {
        ZI_VERIFY_0( pthread_spin_destroy( &spin_ ) );
    }

    inline bool try_lock() const
    {
        return pthread_spin_trylock( &spin_ ) == 0;
    }

    inline void lock() const
    {
        ZI_VERIFY_0( pthread_spin_lock( &spin_ ) );
    }

    inline void unlock() const
    {
        ZI_VERIFY_0( pthread_spin_unlock( &spin_ ) );
    }

    typedef mutex_guard< spinlock > guard;

    template< class Tag >
    struct pool: mutex_pool< Tag, spinlock > { };

};



} // namespace concurrency_
} // namespace zi

#endif
