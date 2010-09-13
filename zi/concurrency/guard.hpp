/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_GUARD_HPP
#define ZI_CONCURRENCY_GUARD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/spinlock.hpp>
#include <zi/concurrency/rwmutex.hpp>
#include <zi/concurrency/detail/is_mutex.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/enable_if.hpp>

namespace zi {
namespace concurrency_ {

// forward declaration
class condition_variable;

struct guard_container_wrapper
{
    guard_container_wrapper() {}
    virtual ~guard_container_wrapper() {}
};

template< class Mutex >
struct guard_container: guard_container_wrapper
{
private:
    const Mutex &m_;

public:
    explicit guard_container( const Mutex& m ): m_( m )
    {
        m_.lock();
    }

    virtual ~guard_container()
    {
        m_.unlock();
    }

    friend class condition_variable;

};

class guard: non_copyable
{
private:
    guard_container_wrapper *gcw_;

public:
    template< class Guarded >
    explicit guard( const Guarded &m,
                    typename enable_if< detail::is_mutex< Guarded >::value >::type* = 0 ):
        gcw_( new guard_container< Guarded >( m ) )
    {
    }

    ~guard()
    {
        delete gcw_;
    }

    friend class condition_variable;
};

} // namespace concurrency_

using concurrency_::guard;

} // namespace zi

#endif
