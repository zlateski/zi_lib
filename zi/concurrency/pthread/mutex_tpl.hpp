/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_PTHREAD_MUTEX_TPL_HPP
#define ZI_CONCURRENCY_PTHREAD_MUTEX_TPL_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/detail/mutex_guard.hpp>
#include <zi/concurrency/detail/mutex_pool.hpp>
#include <zi/concurrency/pthread/mutex_tags.hpp>
#include <zi/concurrency/pthread/mutex_initializers.hpp>

#include <zi/utility/assert.hpp>
#include <zi/utility/non_copyable.hpp>

#include <pthread.h>

namespace zi {
namespace concurrency_ {

// forward declaration
class condition_variable;

template< class PtMutexTag > class mutex_tpl: non_copyable
{
private:

    mutable pthread_mutex_t mutex_;
    friend class condition_variable;

public:

    mutex_tpl()
    {
        mutex_initializer< PtMutexTag >::initialize(mutex_);
    }

    ~mutex_tpl()
    {
        ZI_VERIFY_0( pthread_mutex_destroy( &mutex_ ) );
    }

    inline bool try_lock() const
    {
        return pthread_mutex_trylock( &mutex_ ) == 0;
    }

    inline void lock() const
    {
        ZI_VERIFY_0( pthread_mutex_lock( &mutex_ ) );
    }

    inline void unlock() const
    {
        ZI_VERIFY_0( pthread_mutex_unlock( &mutex_ ) );
    }

    typedef mutex_guard< mutex_tpl< PtMutexTag > > guard;

    template< class Tag >
    struct pool: mutex_pool< Tag, mutex_tpl< PtMutexTag > > { };

};

} // namespace concurrency_

} // namespace zi

#endif
