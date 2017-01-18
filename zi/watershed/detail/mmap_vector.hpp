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

#ifndef ZI_WATERSHED_DETAIL_MMAP_VECTOR_HPP
#define ZI_WATERSHED_DETAIL_MMAP_VECTOR_HPP 1

#include <zi/watershed/detail/mmap_file.hpp>
#include <zi/utility/non_copyable.hpp>

#include <vector>

namespace zi {
namespace watershed {

template< class T, class Allocator = std::allocator< T > >
class mmap_vector: public std::vector< T, Allocator >, non_copyable
{
private:
    std::size_t size_           ;
    std::string file_name_      ;
    bool        delete_on_close_;

public:

    mmap_vector( const std::string& file_name,
                 std::size_t size = 0,
                 bool delete_on_close = true )
        : std::vector< T, Allocator >( size ),
          size_( size ),
          file_name_( file_name ),
          delete_on_close_( delete_on_close )
    { }

    ~mmap_vector()
    {
        if ( delete_on_close_ )
        {
            mmap_file::erase( file_name_ );
        }
    }

    operator std::vector< T, Allocator >&()
    {
        return *this;
    }

    std::string filename() const
    {
        return file_name_;
    }

    std::size_t stored_size() const
    {
        return size_;
    }

    void flush()
    {
        size_ = std::vector< T, Allocator >::size();
        if ( size_ )
        {
            mmap_file::write( file_name_,
                              std::vector< T, Allocator >::begin(),
                              std::vector< T, Allocator >::end() );
            std::vector< T, Allocator > other;
            std::vector< T, Allocator >::swap(other);
        }
    }

    mmap_vector< T, Allocator >& reload()
    {
        std::vector< T, Allocator >::resize( size_ );
        if ( size_ )
        {
            mmap_file::read_n( file_name_, std::vector< T, Allocator >::begin(), size_ );
        }
        return *this;
    }

    class scope_cache
    {
    private:
        mmap_vector< T, Allocator >& mv_;

    public:
        operator std::vector< T, Allocator >& ()
        {
            return mv_;
        }

        std::vector< T, Allocator >& get()
        {
            return mv_;
        }

        scope_cache( mmap_vector< T, Allocator >& mv )
            : mv_( mv )
        { }

        ~scope_cache()
        {
            mv_.flush();
        }
    };



};

} // namespace watershed
} // namespace zi

#endif
