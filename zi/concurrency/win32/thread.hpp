/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_THREAD_HPP
#define ZI_CONCURRENCY_WIN32_THREAD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/runnable.hpp>
#include <zi/concurrency/thread_types.hpp>
#include <zi/concurrency/detail/this_thread.hpp>
#include <zi/concurrency/win32/detail/primitives.hpp>

namespace zi {
namespace concurrency_ {


struct thread
{
private:
    shared_ptr< thread_info > t_;

    static void* ENTRY( void* info )
    {
        thread_info *t = reinterpret_cast< thread_info* >( info );
        shared_ptr< thread_info > t_( t->get_ptr() );

        t_->on_before_start( win32::GetCurrentThreadId(), win32::GetCurrentThread() );

        all_threads_info.register_started( t_ );

        t->run();

        all_threads_info.register_finished( t_ );

        t->on_before_join();

        return static_cast< void* >( 0 );
    }

public:
    thread( shared_ptr< runnable > run ):
        t_( new thread_info( run ) )
    {
    }

    thread( const reference_wrapper< function< void() > >& f ):
        t_( new thread_info
            ( shared_ptr< runnable_function_wrapper >
              ( new runnable_function_wrapper( f ))))
    {
    }

    void start( bool detached = false )
    {
        all_threads_info.register_pending( );
        t_->initialize( win32::GetCurrentThreadId(), detached );

        win32::dword id;
        win32::handle h = win32::CreateThread( NULL, 0,
                                               (LPTHREAD_START_ROUTINE)ENTRY,
                                               (LPVOID)(t_.get()),
                                               0, &id );

        ZI_VERIFY( h  );
        ZI_VERIFY( id );
    }

    operator bool() const
    {
        return true;
    }

    bool join()
    {
        return t_->join();
    }

    void wake()
    {
        t_->wake();
    }

    void detach()
    {
        t_->detach();
    }

    thread_id_type get_id()
    {
        return t_->get_id();
    }

};


} // namespace concurrency_
} // namespace zi

#endif
