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

#ifndef ZI_DISJOINT_SETS_DISJOINT_SETS_HPP
#define ZI_DISJOINT_SETS_DISJOINT_SETS_HPP 1

#include <zi/config/config.hpp>
#include <zi/bits/type_traits.hpp>
#include <zi/utility/enable_if.hpp>
#include <zi/utility/detail/empty_type.hpp>

#include <cstddef>
#include <cassert>

namespace zi {

template< class T >
class disjoint_sets:
        enable_if< is_integral< T >::value, empty_type >::type
{

private:
    struct node
    {
        T r, p;

        node(): r(0), p(0)
        {
        }
    };

    node  *x_;
    std::size_t size_ ;
    std::size_t sets_ ;

public:

    disjoint_sets( std::size_t s ): size_( s ), sets_( s )
    {
        assert( s >= 0 );
        x_ = new node[ s ];
        for ( std::size_t i = 0; i < s; ++i )
        {
            x_[ i ].p = i;
        }
    }

    ~disjoint_sets()
    {
        delete [] x_;
    }

    inline T find_set( T id ) const
    {
        assert( id < size_ );
        T n( id ), x;

        while ( n != x_[ n ].p )
        {
            n = x_[ n ].p;
        }

        while ( n != id )
        {
            x = x_[ id ].p;
            x_[ id ].p = n;
            id = x;
        }

        return n;
    }

    inline T join( T x, T y )
    {
        assert( x < size_ && x >= 0 );
        assert( y < size_ && y >= 0 );

        if ( x == y )
        {
            return x;
        }

        --sets_;

        if ( x_[ x ].r >= x_[ y ].r )
        {
            x_[ y ].p = x;
            if ( x_[ x ].r == x_[ y ].r )
            {
                ++x_[x].r;
            }
            return x;
        }

        x_[ x ].p = y;
        return y;
    }

    inline void clear()
    {
        for ( std::size_t i = 0; i < size_; ++i )
        {
            x_[ i ].p = i;
            x_[ i ].r = 0;
        }
        sets_ = size_;
    }

    std::size_t size() const
    {
        return size_;
    }

    std::size_t set_size() const
    {
        return sets_;
    }

};

} // namespace zi

#endif
