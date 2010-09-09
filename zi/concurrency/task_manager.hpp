/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_TASK_MANAGER_HPP
#define ZI_CONCURRENCY_TASK_MANAGER_HPP 1

#include <zi/concurrency/detail/task_manager_impl.hpp>
#include <zi/concurrency/detail/simple_task_container.hpp>
#include <zi/concurrency/detail/priority_task_container.hpp>

namespace zi {
namespace concurrency_ {

template< class TaskContainer >
class task_manager_tpl
{
private:
    typedef task_manager_impl< TaskContainer > task_manager_t;
    shared_ptr< task_manager_t > manager_;

public:
    task_manager_tpl( std::size_t worker_limit,
                      std::size_t max_size = std::numeric_limits< std::size_t >::max() ) :
        manager_( new task_manager_t( worker_limit, max_size ) )
    {
    }


    std::size_t empty()
    {
        return manager_->empty();
    }

    std::size_t idle()
    {
        return manager_->empty();
    }

    std::size_t size()
    {
        return manager_->size();
    }

    std::size_t worker_count()
    {
        return manager_->worker_count();
    }

    std::size_t worker_limit()
    {
        return manager_->worker_limit();
    }

    std::size_t idle_workers()
    {
        return manager_->idle_workers();
    }

    bool start()
    {
        return manager_->start();
    }

    void stop( bool and_join = false )
    {
        return manager_->stop( and_join );
    }

    void join()
    {
        manager_->join();
    }

    void push_front( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        for ( ; count > 0; -- count )
        {
            manager_->push_front( task );
        }
    }

    void push_back( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        for ( ; count > 0; -- count )
        {
            manager_->push_back( task );
        }
    }

    void add_task( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        push_back( task, count );
    }

    void insert( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        push_back( task, count );
    }

    template< class Tag >
    void push_front( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        for ( ; count > 0; -- count )
        {
            manager_->template push_front< Tag >( task );
        }
    }

    template< class Tag >
    void push_back( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        for ( ; count > 0; -- count )
        {
            manager_->template push_back< Tag >( task );
        }
    }

    template< class Tag >
    void add_task( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        push_back< Tag >( task, count );
    }

    template< class Tag >
    void insert( shared_ptr< runnable > task, std::size_t count = 1 )
    {
        push_back< Tag >( task, count );
    }

    void clear()
    {
        manager_->clear();
    }

    void add_workers( std::size_t count )
    {
        manager_->add_workers( count );
    }

    void remove_workers( std::size_t count )
    {
        manager_->remove_workers( count );
    }

};



} // namespace concurrency_

namespace task_manager {

typedef concurrency_::task_manager_tpl< concurrency_::detail::simple_task_container >   simple     ;
typedef concurrency_::task_manager_tpl< concurrency_::detail::simple_task_container >   deque      ;
typedef concurrency_::task_manager_tpl< concurrency_::detail::priority_task_container > prioritized;

} // namespace task_manager

} // namespace zi

#endif

