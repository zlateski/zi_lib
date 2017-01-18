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

#ifndef ZI_GRAPH_HUNGARIAN_MATCHING_HPP
#define ZI_GRAPH_HUNGARIAN_MATCHING_HPP 1

#include <cstddef>
#include <vector>
#include <utility>
#include <list>

#include <zi/utility/assert.hpp>
#include <zi/utility/for_each.hpp>

#include <zi/bits/function.hpp>
#include <zi/bits/bind.hpp>

namespace zi {
namespace graph {

namespace detail {

template< class T >
class munkres_impl: non_copyable
{
public:
    typedef std::size_t       size_type ;
    typedef T                 value_type;
    typedef munkres_impl< T > this_type ;

private:

    static const int NORMAL = 0;
    static const int STAR   = 1;
    static const int PRIME  = 2;

    const std::vector< std::vector< T > >& original_matrix_;
    std::vector< std::vector< T > >        matrix_         ;
    std::vector< std::vector< int > >      mask_matrix_    ;
    size_type                    nrows_, ncols_  ;
    std::vector< bool >          row_mask_       ;
    std::vector< bool >          col_mask_       ;
    size_type                    srow_, scol_    ;

    bool find_zero()
    {
        for ( size_type r = 0; r < nrows_; ++r )
        {
            if ( !row_mask_[r] )
            {
                for ( size_type c = 0; c < ncols_; ++c )
                {
                    if ( !col_mask_[c] && matrix_[r][c] == 0 )
                    {
                        srow_ = r;
                        scol_ = c;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    size_type step1()
    {
        std::vector< bool > starred_row( nrows_ );
        std::vector< bool > starred_col( ncols_ );

        for ( size_type r = 0; r < nrows_; ++r )
        {
            for ( size_type c = 0; c < ncols_; ++c )
            {
                if ( mask_matrix_[r][c] == STAR )
                {
                    starred_row[r] = true;
                    starred_col[c] = true;
                }
            }
        }

        for ( size_type r = 0; r < nrows_; ++r )
        {
            for ( size_type c = 0; c < ncols_; ++c )
            {
                if ( matrix_[r][c] == 0 )
                {
                    if ( !starred_row[r] && !starred_col[c] )
                    {
                        mask_matrix_[r][c] = STAR;
                        starred_row[r] = true;
                        starred_col[c] = true;
                    }
                }
            }
        }

        return 2;
    }

    size_type step2()
    {
        size_type cnt = 0;

        for ( size_type r = 0; r < nrows_; ++r )
        {
            for ( size_type c = 0; c < ncols_; ++c )
            {
                if ( mask_matrix_[r][c] == STAR )
                {
                    col_mask_[c] = true;
                    ++cnt;
                }
            }
        }

        return cnt >= std::min( ncols_, nrows_ ) ? 0 : 3;
    }

    size_type step3()
    {
    }

    size_type step4()
    {
    }

    size_type step5()
    {
    }



public:
    munkres_impl( const std::vector< std::vector< T > >& m )
        : original_matrix_( m ),
          matrix_( m.size() ),
          mask_matrix_( m.size() ),
          nrows_( m.size() ),
          ncols_(),
          row_mask_( m.size() ),
          col_mask_(),
          srow_(),
          scol_()
    {
        ZI_ASSERT( m.size() > 0 );
        ncols_ = m[0].size();

        col_mask_.resize( ncols_ );

        for ( size_type i = 0; i < m.size(); ++i )
        {
            ZI_ASSERT( ncols_ == m[ i ].size() );
            matrix_[ i ] = m[ i ];
            mask_matrix_[ i ].resize( ncols_ );
        }
    }

    T operator()()
    {
        zi::function< size_type() > fns[ 6 ];
        fns[ 1 ] = zi::bind( &this_type::step1, this );
        fns[ 2 ] = zi::bind( &this_type::step2, this );
        fns[ 3 ] = zi::bind( &this_type::step3, this );
        fns[ 4 ] = zi::bind( &this_type::step4, this );
        fns[ 5 ] = zi::bind( &this_type::step5, this );

        size_type step = 1;
        while ( step )
        {
            step = fns[step]();
        }

        return 0;
    }
};

} // namespace detail

template< class T >
T munkres( const std::vector< std::vector< T > >& m )
{
    detail::munkres_impl< T > mkr( m );
    return mkr();
}

} // namespace graph
} // namespace zi

#endif

