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

#ifndef ZI_CONCURRENCY_DETAIL_RWMUTEX_IMPL_HPP
#define ZI_CONCURRENCY_DETAIL_RWMUTEX_IMPL_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/condition_variable.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>

#include <zi/bits/cstdint.hpp>
#include <zi/time/interval.hpp>

namespace zi {
namespace concurrency_ {

class rwmutex_impl: non_copyable
{
private:

    mutable uint32_t reader_count_  ;
    mutable bool     has_writer_    ;
    mutable bool     writer_waiting_;

    mutex              mutex_    ;
    condition_variable reader_cv_;
    condition_variable writer_cv_;

public:

    rwmutex_impl()
        : reader_count_( 0 ),
          has_writer_( false ),
          writer_waiting_( false ),
          mutex_(),
          reader_cv_(),
          writer_cv_()
    {
    }

    inline bool try_acquire_read() const
    {
        mutex::guard g( mutex_ );

        if ( has_writer_ || writer_waiting_ )
        {
            return false;
        }

        ++reader_count_;
        return true;
    }

    inline void acquire_read() const
    {
        mutex::guard g( mutex_ );

        while ( has_writer_ || writer_waiting_ )
        {
            reader_cv_.wait( mutex_ );
        }

        ++reader_count_;
    }

    inline bool timed_acquire_read( int64_t ttl ) const
    {
        mutex::guard g( mutex_ );

        while ( has_writer_ || writer_waiting_ )
        {
            if ( !reader_cv_.timed_wait( mutex_, ttl ) )
            {
                return false;
            }
        }

        ++reader_count_;
        return true;
    }

    template< int64_t I >
    inline bool timed_acquire_read( const interval::detail::interval_tpl< I > &ttl ) const
    {
        mutex::guard g( mutex_ );

        while ( has_writer_ || writer_waiting_ )
        {
            if ( !reader_cv_.timed_wait( mutex_, ttl ) )
            {
                return false;
            }
        }

        ++reader_count_;
        return true;
    }

    inline void release_read() const
    {
        mutex::guard g( mutex_ );

        if ( !--reader_count_ )
        {
            writer_waiting_ = false;
            writer_cv_.notify_one();
            reader_cv_.notify_all();
        }
    }


    inline bool try_acquire_write() const
    {
        mutex::guard g( mutex_ );

        if ( reader_count_ || has_writer_ )
        {
            return false;
        }

        has_writer_ = true;
        return true;
    }

    inline void acquire_write() const
    {
        mutex::guard g( mutex_ );

        while ( reader_count_ || has_writer_ )
        {
            writer_waiting_ = true;
            writer_cv_.wait( mutex_ );
        }

        has_writer_ = true;
    }

    inline void release_write() const
    {
        mutex::guard g( mutex_ );
        has_writer_ = writer_waiting_ = false;
        writer_cv_.notify_one();
        reader_cv_.notify_all();
    }

    // todo: timed_acquire_write

    class read_guard
    {
    private:
        const rwmutex_impl &m_;

    public:
        explicit read_guard( const rwmutex_impl &m ): m_( m )
        {
            m_.acquire_read();
        }

        ~read_guard()
        {
            m_.release_read();
        }
    };

    class write_guard
    {
    private:
        const rwmutex_impl &m_;

    public:
        explicit write_guard( const rwmutex_impl &m ): m_( m )
        {
            m_.acquire_write();
        }

        ~write_guard()
        {
            m_.release_write();
        }
    };

    typedef write_guard guard;

};

} // namespace concurrency_
} // namespace zi

#endif
