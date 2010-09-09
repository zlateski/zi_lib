/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_MUTEX_POOL_HPP
#define ZI_CONCURRENCY_DETAIL_MUTEX_POOL_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/utility/non_copyable.hpp>

#include <cstddef>

namespace zi {

namespace concurrency_ {


template< class Tag, class Mutex, std::size_t Size = 83 > // 83 is prime!
class mutex_pool: non_copyable
{
public:

    typedef Mutex mutex_type;
    static const std::size_t pool_size = Size;

private:

    static Mutex pool_[ Size ];

public:

    class guard: non_copyable
    {
    public:

        explicit guard( void const * ptr ):
            m_( pool_[ reinterpret_cast< std::size_t >( ptr ) % pool_size ] )
        {
            m_.lock();
        }

        ~guard()
        {
            m_.unlock();
        }

    private:

        Mutex &m_;
    };

private:

    friend class guard;

};

template< class Tag, class Mutex, std::size_t Size >
Mutex mutex_pool< Tag, Mutex, Size >::pool_[ Size ];

} // namespace concurrency_

} // namespace zi

#endif
