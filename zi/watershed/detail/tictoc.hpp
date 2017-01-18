//
// Copyright (C) 2010-2011  Aleksandar Zlateski <zlateski@mit.edu>
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

#ifndef ZI_WATERSHED_DETAIL_TICTOC_HPP
#define ZI_WATERSHED_DETAIL_TICTOC_HPP 1

#include <zi/time/timer.hpp>
#include <zi/concurrency/mutex.hpp>
#include <iostream>
#include <string>
#include <cstddef>
#include <vector>


namespace zi {
namespace watershed {
namespace detail {

template <typename T>
struct early_singleton
{
private:

    struct singleton_constructor
    {
        singleton_constructor()
        {
            early_singleton<T>::instance();
        }
        inline void no_op() const {}
    };

    static singleton_constructor constructor_call;
    early_singleton();

public:

    static T& instance()
    {
        static T single_instance;
        constructor_call.no_op();
        return single_instance;
    }

    static T* pointer()
    {
        return &early_singleton<T>::instance();
    }
};

template <typename T>
typename early_singleton<T>::singleton_constructor early_singleton<T>::constructor_call;

template <typename Tag>
struct thread_local_timer
{
private:
    std::vector< zi::wall_timer* >  all_timers;
    static __thread zi::wall_timer* wt;
    zi::mutex                       m_;

    zi::wall_timer* get_timer()
    {
        if ( thread_local_timer<Tag>::wt ==0 )
        {
            thread_local_timer<Tag>::wt = new zi::wall_timer;
            zi::mutex::guard g(m_);
            all_timers.push_back(thread_local_timer<Tag>::wt);
        }
        return thread_local_timer<Tag>::wt;
    }

public:
    thread_local_timer()
    {
    };

    ~thread_local_timer()
    {
        zi::mutex::guard g(m_);
        for ( std::size_t i = 0; i < all_timers.size(); ++i )
        {
            delete all_timers[i];
        }
    };

    void tic()
    {
        get_timer()->restart();
        std::cout << "TIC\n" << std::flush;
    }

    void tic( const std::string& message )
    {
        get_timer()->restart();
        std::cout << "TIC " << message << "\n" << std::flush;
    }

    void toc()
    {
        std::cout << "TOC (" << get_timer()->template elapsed<double>()
                  << ")\n" << std::flush;
    }

    void toc( const std::string& message )
    {
        std::cout << "TOC (" << get_timer()->template elapsed<double>()
                  << ") " << message << "\n" << std::flush;
    }


};

template <typename Tag>
__thread zi::wall_timer* thread_local_timer<Tag>::wt;

struct thread_local_timer_tag {};

namespace {
thread_local_timer<thread_local_timer_tag>& ticker =
early_singleton<thread_local_timer<thread_local_timer_tag> >::instance();
} // namespace


} // namespace detail


#if 1 //NDEBUG
inline void tic()
{
    detail::ticker.tic();
}

inline void tic( const std::string& message )
{
    detail::ticker.tic( message );
}

inline void toc()
{
    detail::ticker.toc();
}

inline void toc( const std::string& message )
{
    detail::ticker.toc( message );
}
#else
inline void tic(){}
inline void tic( const std::string& ) {}
inline void toc(){}
inline void toc( const std::string& ) {}
#endif
} // namespace watershed
} // namespace zi

#endif
