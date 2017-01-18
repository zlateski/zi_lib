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

#ifndef ZI_WATERSHED_XXL_CHUNK_DIMENSIONS_HPP
#define ZI_WATERSHED_XXL_CHUNK_DIMENSIONS_HPP 1

#include <zi/watershed/detail/types.hpp>

namespace zi {
namespace watershed {

class chunk_dimensions
{
private:
    int dims_[ 3 ];

public:
    explicit chunk_dimensions( int xdim = 0, int ydim = 0, int zdim = 0 )
        : dims_()
    {
        dims_[0] = xdim;
        dims_[1] = ydim;
        dims_[2] = zdim;
    }

    size_type dim( size_type d ) const
    {
        return dims_[ d ];
    }

    size_type x() const
    {
        return dims_[0];
    }

    size_type y() const
    {
        return dims_[1];
    }

    size_type z() const
    {
        return dims_[2];
    }

};

} // namespace watershed
} // namespace zi

#endif

