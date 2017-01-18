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

#ifndef ZI_WATERSHED_DETAIL_UTILITY_HPP
#define ZI_WATERSHED_DETAIL_UTILITY_HPP 1

#include <zi/bits/tuple.hpp>

#include <utility>
#include <functional>

namespace zi {
namespace watershed {

template< class A, class B >
inline const std::pair< A, B > minmax( const A &a, const B &b )
{
    return a < b
        ? std::pair< A, B >( a, b )
        : std::pair< A, B >( b, a );
}

template< class InputIterator, class Size, class OutputIterator >
inline OutputIterator
copy_n( InputIterator first, Size n, OutputIterator result )
{
    for ( ; n > 0; --n, ++first, ++result )
    {
        *result = *first;
    }
    return result;
}

template< class T, class Id >
bool edge_compare( const tuple< Id, Id, T >& t1, const tuple< Id, Id, T >& t2 )
{
    return zi::get< 2 >( t1 ) > zi::get< 2 >( t2 );
}

template< class T, class Id >
struct edge_less:
        std::binary_function< tuple< Id, Id, T >, tuple< Id, Id, T >, bool >
{
    bool operator()( const tuple< Id, Id, T >& t1, const tuple< Id, Id, T >& t2 ) const
    {
        return zi::get< 2 >( t1 ) < zi::get< 2 >( t2 );
    }
};

template< class T, class Id >
struct edge_greater:
        std::binary_function< tuple< Id, Id, T >, tuple< Id, Id, T >, bool >
{
    bool operator()( const tuple< Id, Id, T >& t1, const tuple< Id, Id, T >& t2 ) const
    {
        return zi::get< 2 >( t1 ) > zi::get< 2 >( t2 );
    }
};


} // namespace watershed
} // namespace zi

#endif
