/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_PRIORITY_TASK_CONTAINER_HPP
#define ZI_CONCURRENCY_DETAIL_PRIORITY_TASK_CONTAINER_HPP 1

#include <zi/concurrency/runnable.hpp>
#include <zi/bits/shared_ptr.hpp>
#include <zi/bits/cstdint.hpp>

#include <cstddef>
#include <utility>
#include <functional>
#include <map>

namespace zi {


namespace priority {

struct highest
{
    static const int64_t value = 20;
};

struct high
{
    static const int64_t value = 10;
};

struct normal
{
    static const int64_t value = 0;
};

struct low
{
    static const int64_t value = -10;
};

struct lowest
{
    static const int64_t value = -20;
};

template< int64_t Value >
struct custom
{
    static const int64_t value = Value;
};

} // namespace priority


namespace concurrency_ {
namespace detail {

struct priority_task_container
{
private:
    std::map< int64_t, shared_ptr< runnable > > queue_  ;
    int64_t                                     counter_;

public:
    explicit priority_task_container(): queue_(), counter_( 0 )
    {
    }

    std::size_t size() const
    {
        return queue_.size();
    }

    std::size_t empty() const
    {
        return queue_.empty();
    }

    void clear()
    {
        queue_.clear();
    }

    shared_ptr< runnable > front()
    {
        return queue_.begin()->second;
    }

    void pop_front()
    {
        queue_.erase( queue_.begin() );
    }

    template< class Priority >
    void push_front( shared_ptr< runnable > task )
    {
        queue_.insert( std::make_pair( - ( Priority::value << 48 ) + ( ++counter_ ), task ) );
    }

    template< class Priority >
    void push_back( shared_ptr< runnable > task )
    {
        this->push_front< Priority >( task );
    }

    void push_front( shared_ptr< runnable > task )
    {
        this->push_front< priority::normal >( task );
    }

    void push_back( shared_ptr< runnable > task )
    {
        this->push_front< priority::normal >( task );
    }

};


} // namespace detail
} // namespace concurrency_
} // namespace zi

#endif

