/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_MUTEX_GUARD_HPP
#define ZI_CONCURRENCY_DETAIL_MUTEX_GUARD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/utility/non_copyable.hpp>

namespace zi {
namespace concurrency_ {

// forward declaration
class condition_variable;

template< class Mutex > class mutex_guard: non_copyable
{
private:

    const Mutex &m_;
    friend class condition_variable;

public:

    typedef mutex_guard< Mutex > guard_type;

    explicit mutex_guard( const Mutex &m ): m_( m )
    {
        m_.lock();
    }

    ~mutex_guard()
    {
        m_.unlock();
    }

};


} // namespace concurrency_
} // namespace zi

#endif
