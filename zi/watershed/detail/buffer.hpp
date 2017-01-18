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

#include <memory>
#include <utility>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <stdexcept>

#ifndef ZI_WATERSHED_DETAIL_BUFFER_HPP
#define ZI_WATERSHED_DETAIL_BUFFER_HPP 1

namespace zi {
namespace watershed {
namespace detail {

template <typename T>
T* get_buffer( std::size_t len )
{
    void* r = std::malloc( len * sizeof(T) );
    if (!r)
    {
        throw std::bad_alloc();
    }
    return reinterpret_cast<T*>(r);
}

template <typename T>
void return_buffer( T*& p )
{
    std::free( p );
    p = 0;
}

} // namespace detail
} // namespace watershed
} // namespace zi

#endif
