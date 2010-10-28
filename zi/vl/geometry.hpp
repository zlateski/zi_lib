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

#ifndef ZI_VL_GEOMETRY_HPP
#define ZI_VL_GEOMETRY_HPP 1

#include <zi/vl/vector.hpp>
#include <zi/vl/matrix.hpp>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

namespace triangle {

template< class T >
inline vector< T, 3 > real_normal( const vector< T, 3 >& p1,
                                   const vector< T, 3 >& p2,
                                   const vector< T, 3 >& p3 )
{
    vector< T, 3 > a( p2 ); a -= p1;
    vector< T, 3 > b( p3 ); b -= p1;
    return cross( a, b );
}

template< class T >
inline vector< T, 3 > normal( const vector< T, 3 >& p1,
                              const vector< T, 3 >& p2,
                              const vector< T, 3 >& p3 )
{
    vector< T, 3 > a( p2 ); a -= p1;
    vector< T, 3 > b( p3 ); b -= p1;
    a.cross( b ).normalize();
    return a;
}

template< class T >
inline vector< T, 4 > real_plane( const vector< T, 3 >& p1,
                                  const vector< T, 3 >& p2,
                                  const vector< T, 3 >& p3 )
{
    vector< T, 3 > a( p2 ); a -= p1;
    vector< T, 3 > b( p3 ); b -= p1;
    a.cross( b );
    return vector< T, 4 >( a, -a.dot( p1 ) );
}

template< class T >
inline vector< T, 4 > plane( const vector< T, 3 >& p1,
                             const vector< T, 3 >& p2,
                             const vector< T, 3 >& p3 )
{
    vector< T, 3 > a( p2 ); a -= p1;
    vector< T, 3 > b( p3 ); b -= p1;
    a.cross( b ).normalize();
    return vector< T, 4 >( a, -a.dot( p1 ) );
}

template< class T >
inline T area( const vector< T, 3 >& p1,
               const vector< T, 3 >& p2,
               const vector< T, 3 >& p3 )
{
    vector< T, 3 > a( p2 ); a -= p1;
    vector< T, 3 > b( p3 ); b -= p1;
    return a.cross( b ).length() * 0.5;
}

template< class T >
inline T compactness( const vector< T, 3 >& p1,
                      const vector< T, 3 >& p2,
                      const vector< T, 3 >& p3 )
{
    return
        area( p1, p2, p3 ) * 6.928203230275509L
        / ( squared_length( p1 - p2 ) +
            squared_length( p2 - p3 ) +
            squared_length( p3 - p1 ) );
}


} // namespace triangle

} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

