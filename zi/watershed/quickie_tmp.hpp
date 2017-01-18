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

#ifndef ZI_WATERSHED_QUICKIE_HPP
#define ZI_WATERSHED_QUICKIE_HPP 1

#include <zi/parallel/algorithm.hpp>
#include <zi/concurrency.hpp>
#include <zi/system.hpp>
#include <zi/bits/cstdint.hpp>

#include <algorithm>
#include <utility>
#include <cstddef>
#include <iostream>
#include <deque>

namespace zi {
namespace watershed {

template< class T, class R >
class quickie_impl
{
private:
    const T*      connections_                          ;
    const int64_t x_dim_, y_dim_, z_dim_                ;
    const int64_t size_thold_                           ;
    const T       low_thold_, hi_thold_, abs_low_thold_ ;
    R*            result_                               ;
    zi::mutex     mutex_                                ;


    void add_hi_threshold_edges_thread( int64_t d, int64_t z )
    {

        const int64_t xy_dim      = x_dim_  * y_dim_;
        const int64_t xyz_dim     = xy_dim  * z_dim_;
        const int64_t xyz_dim2    = xy_dim  * z_dim_ * 2;
        const int64_t nhood[6]    = { -1, -x_dim_, -xy_dim, 1, x_dim_, xy_dim };

        int64_t j = z * xy_dim;
        int64_t i = j + d * xyz_dim;
        for ( int64_t y = 0; y < y_dim_; ++y )
        {
            for ( int64_t x = 0; x < x_dim_; ++x, ++i, ++j )
            {
                if ((x == 0) && (d == 0)) continue;
                if ((y == 0) && (d == 1)) continue;
                if ((z == 0) && (d == 2)) continue;

                if ( connections_[ i ] >= hi_thold_ )
                {
                    result_[ j ]              |= (1 << d);
                    result_[ j + nhood[ d ] ] |= (8 << d);
                }

            }
        }

    }

    void add_highest_descent_edges_thread( int64_t z )
    {
        const int64_t xy_dim      = x_dim_  * y_dim_;
        const int64_t xyz_dim     = xy_dim  * z_dim_;
        const int64_t xyz_dim2    = xy_dim  * z_dim_ * 2;
        const int64_t nhood[6]    = { -1, -x_dim_, -xy_dim, 1, x_dim_, xy_dim };

        int64_t j = z * xy_dim;

        for ( int64_t y = 0; y < y_dim_; ++y )
        {
            for ( int64_t x = 0; x < x_dim_; ++x, ++j )
            {

                R d      = static_cast< R >( 0 );
                T top    = static_cast< T >( 0 );
                T second = static_cast< T >( 0 );

#define ZI_WATERSHED_EDGE_CHECK( direction, condition_exp, index_exp )  \
                if ( condition_exp )                                    \
                {                                                       \
                    if ( connections_[ index_exp ] >= top )             \
                    {                                                   \
                        d = direction;                                  \
                        second = top;                                   \
                        top = connections_[ index_exp ];                \
                    }                                                   \
                }                                                       \

                ZI_WATERSHED_EDGE_CHECK( 1, x > 0, j )
                ZI_WATERSHED_EDGE_CHECK( 2, y > 0, j + xyz_dim )
                ZI_WATERSHED_EDGE_CHECK( 3, z > 0, j + xyz_dim2 )
                ZI_WATERSHED_EDGE_CHECK( 4, x < x_dim_ - 1, j + 1 )
                ZI_WATERSHED_EDGE_CHECK( 5, y < y_dim_ - 1, j + xyz_dim + x_dim_ )
                ZI_WATERSHED_EDGE_CHECK( 6, z < z_dim_ - 1, j + xyz_dim2 + xy_dim)

#undef ZI_WATERSHED_EDGE_CHECK


                if ( d > 0 )
                {
                    if ( top > abs_low_thold_ )
                    {
                        if ( top > second )
                        {
                            --d;
                            if ( d < 3 )
                            {
                                result_[ j ]              |= ( 1 << d );
                                result_[ j + nhood[ d ] ] |= ( 8 << d );
                            }
                            else
                            {
                                result_[ j ]              |= ( 1 << d );
                                result_[ j + nhood[ d ] ] |= ( 1 << ( d - 3 ));
                            }
                        }
                        else
                        {
                            result_[ j ] |= 0x10000000;
                        }
                    }
                }

            }
        }
    }

    void finalize_colors_thread( int64_t from, int64_t to )
    {
        for ( int64_t i = from; i < to; ++i )
        {
            result_[ i ] = ( result_[ i ] & 0x80000000 ) ? 0x7FFFFFFF & result_[ i ] : 0;
        }
    }

public:
    quickie_impl( const T* conn,
                  const int64_t x_dim,
                  const int64_t y_dim,
                  const int64_t z_dim,
                  const int64_t size_thold,
                  const T low_thold,
                  const T hi_thold,
                  const T abs_low_thold,
                  R* result )
        : connections_( conn ),
          x_dim_( x_dim ),
          y_dim_( y_dim ),
          z_dim_( z_dim ),
          size_thold_( size_thold ),
          low_thold_( low_thold ),
          hi_thold_( hi_thold ),
          abs_low_thold_( abs_low_thold ),
          result_( result ),
          mutex_()
    {
        std::fill_n( result, x_dim * y_dim * z_dim, 0 );
    }

    void doit( std::vector< R >& sizes )
    {
        sizes.clear();

        const int64_t no_threads = zi::system::cpu_count * 2;
        zi::task_manager::simple tm( no_threads );

        for ( int64_t d = 0; d < 3; ++d )
        {
            tm.start();

            for ( int64_t z = 0; z < z_dim_; z += 2 )
            {
                tm.push_back( zi::run_fn
                              ( zi::bind
                                ( &quickie_impl::add_hi_threshold_edges_thread,
                                  this,
                                  d, z ) ));
            }

            tm.join();
            tm.start();

            for ( int64_t z = 1; z < z_dim_; z += 2 )
            {
                tm.push_back( zi::run_fn
                              ( zi::bind
                                ( &quickie_impl::add_hi_threshold_edges_thread,
                                  this,
                                  d, z ) ));
            }
            tm.join();
        }

        tm.start();
        for ( int64_t z = 0; z < z_dim_; z += 2 )
        {
            tm.push_back( zi::run_fn
                          ( zi::bind
                            ( &quickie_impl::add_highest_descent_edges_thread,
                              this, z ) ));
        }
        tm.join();

        tm.start();
        for ( int64_t z = 1; z < z_dim_; z += 2 )
        {
            tm.push_back( zi::run_fn
                          ( zi::bind
                            ( &quickie_impl::add_highest_descent_edges_thread,
                              this, z ) ));
        }
        tm.join();

        //return;

        R next_index = 1;
        std::deque< int64_t > queue;

        const int64_t xy_dim    = x_dim_  * y_dim_;
        const int64_t xyz_dim   = xy_dim  * z_dim_;
//        const int64_t xyz_dim2  = xy_dim  * z_dim_ * 2;
        const int64_t nhood[6]  = { -1, -x_dim_, -xy_dim, 1, x_dim_, xy_dim };

        for ( int64_t i = 0; i < xyz_dim; ++i )
        {
            if ( !( result_[ i ] & 0x80000000 ) && result_[ i ] )
            {
                queue.push_back( i );
                sizes.push_back( 0 );

                while ( queue.size() )
                {

                    const int64_t cur = queue.front();
                    queue.pop_front();

                    for ( int64_t z = 0; z < 6; ++z )
                    {
                        if ( result_[ cur ] & ( 1 << z ) )
                        {
                            const int64_t pot = cur + nhood[z];

                            if ( !( result_[ pot ] & 0x80000000 ))
                            {
                                result_[ pot ] |= 0x80000000;
                                queue.push_back( pot );
                            }
                        }
                    }

                    result_[ cur ] = 0x80000000 | next_index;
                    ++sizes.back();
                }

                ++next_index;
            }

        }

        tm.start();
        for ( int64_t z = 0; z < z_dim_; ++z )
        {
            tm.push_back( zi::run_fn
                          ( zi::bind
                            ( &quickie_impl::finalize_colors_thread,
                              this, z * x_dim_ * y_dim_, ( z + 1 ) * x_dim_ * y_dim_ ) ));
        }
        tm.join();


    }

};


} // namespace watershed
} // namespace zi

#endif

