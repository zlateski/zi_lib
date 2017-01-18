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

#ifndef ZI_WATERSHED_XXL_QUEUE_HPP
#define ZI_WATERSHED_XXL_QUEUE_HPP 1

#include <zi/bits/cstdint.hpp>
#include <zi/bits/unordered_set.hpp>
#include <zi/bits/unordered_map.hpp>
#include <zi/bits/bind.hpp>
#include <zi/bits/function.hpp>
#include <zi/bits/shared_ptr.hpp>

#include <zi/disjoint_sets/disjoint_sets.hpp>
#include <zi/watershed/detail/mmap_file.hpp>
#include <zi/watershed/detail/mmap_ostream.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/rwmutex.hpp>
#include <zi/concurrency/task_manager.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/string_printf.hpp>
#include <zi/utility/singleton.hpp>

#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace zi {
namespace watershed {

namespace detail {

struct default_xxl_queue_manager_tag {};

template< std::size_t NoThreads, class Tag = default_xxl_queue_manager_tag >
class xxl_queue_manager
{
public:
    typedef xxl_queue_manager< NoThreads, Tag > type;

private:
    task_manager::simple manager_;

public:
    xxl_queue_manager()
        : manager_( NoThreads )
    {
        manager_.start();
    }

    ~xxl_queue_manager()
    {
        manager_.stop();
    }

    task_manager::simple& get_manager()
    {
        return manager_;
    }

    static task_manager::simple& manager()
    {
        return singleton< type >::instance().get_manager();
    }

};

template< class T >
class xxl_queue_source: non_copyable
{
public:

    typedef xxl_queue_source< T > this_type;

private:

    const std::string filename_    ;
    const std::size_t cache_size_  ;
    const std::size_t cache_len_   ;
    const std::size_t size_        ;

    const std::size_t type_        ;

    std::size_t remaining_         ;
    std::size_t file_pos_          ;
    std::size_t remaining_in_file_ ;

    std::size_t buffer_pos_     ;
    std::size_t buffer_len_     ;
    T*          buffer_         ;

    std::size_t pending_        ;
    T*          pending_buffer_ ;

    std::size_t fetching_       ;

    mutex               mutex_  ;
    condition_variable  cv_     ;

    std::size_t num_waiting_    ;

    void try_fetch()
    {
        if ( remaining_in_file_ && !fetching_ )
        {
            fetching_ = std::min( cache_len_, remaining_in_file_ );
            xxl_queue_manager< 10 >::manager().push_back
                ( zi::bind( &this_type::fetch_cb, this ) );
        }
    }

    bool can_read()
    {
        if ( buffer_pos_ == buffer_len_ )
        {
            try_fetch();

            if ( pending_ )
            {
                std::swap( pending_buffer_, buffer_ );
                buffer_pos_ = 0;
                buffer_len_ = pending_;
                pending_    = 0;

                return true;
            }

            return false;
        }

        return true;;
    }

    bool try_to_fetch_nl()
    {
        if ( fetching_ == 0 && remaining_in_file_ && pending_ == 0 )
        {
            std::size_t fetching_ = std::min( remaining_in_file_, cache_len_ );
            xxl_queue_manager< 10 >::manager().push_back
                ( zi::bind( &this_type::fetch_cb, this ) );
        }
        else
        {
            return false;
        }
    }

public:

    xxl_queue_source( const std::string& filename,
                      const std::size_t  size,
                      const std::size_t  cache_size,
                      const std::size_t  typ = 0 )
        : filename_( filename ),
          cache_size_( cache_size ),
          cache_len_( cache_size / 2 / sizeof( T ) ),
          size_( size ),
          type_( typ ),
          remaining_( size ),
          file_pos_( 0 ),
          remaining_in_file_( size ),
          buffer_pos_( 0 ),
          buffer_len_( 0 ),
          buffer_( reinterpret_cast< T* >( std::malloc( cache_len_ * sizeof( T )) )),
          pending_( 0 ),
          pending_buffer_( reinterpret_cast< T* >( std::malloc( cache_len_ * sizeof( T ) ) )),
          fetching_( 0 ),
          mutex_(),
          cv_(),
          num_waiting_( 0 )
    {
    }

    ~xxl_queue_source()
    {
        mutex::guard g( mutex_ );
        std::free( buffer_ );
        std::free( pending_buffer_ );
    }

    std::size_t get_type() const
    {
        mutex::guard g( mutex_ );
        return type_;
    }

    const std::string& filename() const
    {
        return filename_;
    }

    void fetch_cb()
    {

        mutex::guard g( mutex_ );

        if ( !fetching_ )
        {
            throw std::runtime_error( "!fetching_ wtf?" );
        }

        mmap_file::read_n( filename_, pending_buffer_,
                           fetching_,
                           file_pos_ * sizeof( T ) );



        pending_   = fetching_;
        file_pos_ += fetching_;
        remaining_in_file_ -= fetching_;
        fetching_ = 0;

        if ( num_waiting_ )
        {
            cv_.notify_all();
        }
    }

    std::size_t size() const
    {
        mutex::guard g( mutex_ );
        return remaining_;
    }

    bool empty() const
    {
        mutex::guard g( mutex_ );
        return remaining_ == 0;
    }

    T pop()
    {
        mutex::guard g( mutex_ );

        while ( remaining_ && !can_read() )
        {
            ++num_waiting_;
            cv_.wait( mutex_ );
            --num_waiting_;
        }

        if ( !remaining_ )
        {
            return T();
        }

        if ( buffer_pos_ >= buffer_len_ )
        {
            throw std::runtime_error( "wtf?" );
        }

        --remaining_  ;
        ++buffer_pos_ ;

        return buffer_[ buffer_pos_ - 1 ];
    }

};

template< class T, class Compare = std::less< T > >
class xxl_queue: non_copyable
{
private:
    typedef std::pair< T, xxl_queue_source< T >* > entry_type;

    struct xxl_queue_comparator
        : public std::binary_function< entry_type, entry_type, bool >
    {
    private:
        Compare compare_;

    public:
        explicit xxl_queue_comparator( const Compare& c = Compare() )
            : compare_( c )
        {
        }

        bool operator()( const entry_type& a, const entry_type& b ) const
        {
            return compare_( a.first, b.first );
        }
    };

private:
    xxl_queue_comparator      compare_   ;
    std::vector< entry_type > heap_      ;

    void update_source( xxl_queue_source< T >* src )
    {
        if ( src->empty() )
        {
            delete src;
            return;
        }

        heap_.push_back( std::make_pair( src->pop(), src ) );
        std::push_heap( heap_.begin(), heap_.end(), compare_ );
    }

public:
    explicit xxl_queue( const Compare& c = Compare() )
        : compare_( c ),
          heap_( 0 )
    {
    }

    ~xxl_queue()
    {
        FOR_EACH( it, heap_ )
        {
            delete it->second;
        }
    }

    void add_source( const std::string& filename,
                     std::size_t len,
                     std::size_t k = 0 )
    {
        if ( len > 0 )
        {
            xxl_queue_source< T >* src =
                new xxl_queue_source< T >( filename, len, 1 << 20, k );

            update_source( src );
        }
    }

    bool empty() const
    {
        return heap_.empty();
    }

    void clear()
    {
        FOR_EACH( it, heap_ )
        {
            delete it->second;
        }
        heap_.clear();
    }

    std::size_t top_key() const
    {
        return heap_[ 0 ].second->get_type();
    }

    const T& top() const
    {
        return heap_[ 0 ].first;
    }

    bool pop()
    {
        if ( heap_.empty() )
        {
            return false;
        }

        std::pop_heap( heap_.begin(), heap_.end(), compare_ );

        if ( heap_.back().second->empty() )
        {
            delete heap_.back().second;
            heap_.pop_back();
            return false;
        }
        else
        {
            heap_.back().first = heap_.back().second->pop();
            std::push_heap( heap_.begin(), heap_.end(), compare_ );
        }

        return true;
    }
};

} // namespace detail

} // namespace watershed
} // namespace zi

#endif
