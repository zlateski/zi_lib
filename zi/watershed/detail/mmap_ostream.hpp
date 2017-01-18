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

#ifndef ZI_WATERSHED_DETAIL_MMAP_OSTREAM_HPP
#define ZI_WATERSHED_DETAIL_MMAP_OSTREAM_HPP 1

#include <zi/watershed/detail/types.hpp>
#include <zi/watershed/detail/mmap_file.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/condition_variable.hpp>

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

template< class T >
class mmap_ostream: non_copyable
{
private:
    mmap_file file_          ;
    size_type buffer_size_   ;
    size_type pos_           ;
    size_type file_pos_      ;
    T*        buffer_        ;
    bool      good_          ;
    mutex     mutex_         ;

    void flush_nl()
    {
        if ( pos_ > 0 )
        {
            file_.size( file_pos_ + pos_ * sizeof( T ) );
            file_.write( buffer_, pos_ * sizeof( T ), file_pos_ );
            file_pos_ += pos_ * sizeof( T );
            pos_ = 0;
        }
    }

public:
    explicit mmap_ostream( const std::string& filename,
                           size_type buffer_size = 2048 )
        : file_( filename, file::CREAT | file::RDWR | file::TRUNC ),
          buffer_size_( 0 ),
          pos_( 0 ),
          file_pos_( 0 ),
          buffer_(),
          good_( true ),
          mutex_()
    {
        buffer_size_  = buffer_size * 1024 / sizeof( T );
        buffer_size_ -= buffer_size % mmap_file::page_size;

        buffer_ = reinterpret_cast< T* >( std::malloc( buffer_size_ * sizeof( T ) ));
    }

    mmap_ostream< T >& operator<<( const T& v )
    {
        mutex::guard g( mutex_ );
        buffer_[ pos_++ ] = v;
        if ( pos_ == buffer_size_ )
        {
            flush_nl();
        }
        return *this;
    }

    void flush()
    {
        mutex::guard g( mutex_ );
        flush_nl();
    }

    ~mmap_ostream()
    {
        flush();
        std::free( buffer_ );
    }

};


} // namespace detail

} // namespace watershed
} // namespace zi

#endif
