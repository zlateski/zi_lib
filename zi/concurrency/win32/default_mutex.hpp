/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_DEFAULT_MUTEX_HPP
#define ZI_CONCURRENCY_WIN32_DEFAULT_MUTEX_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/detail/mutex_guard.hpp>
#include <zi/concurrency/detail/mutex_pool.hpp>
#include <zi/concurrency/win32/detail/primitives.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>

namespace zi {
namespace concurrency_ {

// forward declaration
class condition_variable;

class default_mutex: non_copyable
{
private:
    win32::handle handle_;

public:

    default_mutex():
        handle_( win32::CreateMutex( NULL, false, NULL ) )
    {
        ZI_VERIFY( handle_ != NULL );
    }

    ~default_mutex()
    {
        ZI_VERIFY( win32::CloseHandle( handle_ ) );
    }

    inline bool try_lock() const
    {
        return win32::WaitForSingleObject( handle_, 0 ) == 0;
    }

    inline void lock() const
    {
        ZI_VERIFY_0( win32::WaitForSingleObject( handle_, win32::forever ));
    }

    inline void unlock() const
    {
        ZI_VERIFY( win32::ReleaseMutex( handle_ ) );
    }

    typedef mutex_guard< default_mutex > guard;

    template< class Tag >
    struct pool: mutex_pool< Tag, default_mutex > { };

    friend class condition_variable;

};


} // namespace concurrency_
} // namespace zi

#endif
