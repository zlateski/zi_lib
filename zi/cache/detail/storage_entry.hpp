/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_DETAIL_STORAGE_ENTRY_HPP
#define ZI_CACHE_DETAIL_STORAGE_ENTRY_HPP 1

#include <zi/cache/config.hpp>
#include <zi/time/now.hpp>

#include <zi/bits/function.hpp>
#include <zi/bits/ref.hpp>

namespace zi {
namespace cache {
namespace detail {

struct storage_entry
{
    typedef function< void() > callback_t;

    int64_t     token_          ;
    int64_t     group_          ;
    int64_t     timestamp_      ;
    int64_t     size_           ;
    callback_t  flush_callback_ ;

    storage_entry( int64_t token, int64_t group,
                   const callback_t &callback, int64_t size = 0 ):
        token_( token ),
        group_( group ),
        timestamp_( now::usec() ),
        size_( size ),
        flush_callback_( callback )
    {
    }

    storage_entry( int64_t token, int64_t group,
                   const reference_wrapper< callback_t >& callback,
                   int64_t size = 0 ) :
        token_( token ),
        group_( group ),
        timestamp_( now::usec() ),
        size_( size ),
        flush_callback_( callback.get() )
    {
    }

    void flush() const
    {
        flush_callback_();
    }

    struct timestamp_updater
    {
        void operator() ( storage_entry &e )
        {
            e.timestamp_ = now::usec();
        }
    };

    struct size_updater
    {
    private:
        int64_t size_;

    public:
        size_updater( int64_t size ): size_( size )
        {
        }

        void operator() ( storage_entry &e )
        {
            e.size_ = size_;
        }
    };

};

} // namespace detail
} // namespace cache
} // namespace zi

#endif
