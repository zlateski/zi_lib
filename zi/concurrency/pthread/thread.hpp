//
// Copyright (C) 2010  Aleksandar Zlateski <zlateski@mit.edu>
// ----------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef ZI_CONCURRENCY_PTHREAD_THREAD_HPP
#define ZI_CONCURRENCY_PTHREAD_THREAD_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/runnable.hpp>
#include <zi/concurrency/thread_types.hpp>
#include <zi/concurrency/detail/this_thread.hpp>
#include <zi/concurrency/pthread/thread_attributes.hpp>

#include <pthread.h>

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

        pthread_t me = pthread_self();

        t_->on_before_start( me, me );

        all_threads_info.register_started( t_ );

        t->run();

        all_threads_info.register_finished( t_ );

        t->on_before_join();

        pthread_exit( NULL );

        return static_cast< void* >( 0 );
    }

public:
    thread():
        t_()
    {
    }

    thread( shared_ptr< runnable > run ):
        t_( new thread_info( run ) )
    {
    }

    void start( bool detached = false )
    {

        all_threads_info.register_pending( );

        t_->initialize( pthread_self(), detached );

        pthread_t pt;

        ZI_VERIFY_0( pthread_create(&pt,
                                    thread_attributes< tag::detached >::get(),
                                    ENTRY,
                                    static_cast< void * >( t_.get() )));

        ZI_VERIFY( pt );

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
