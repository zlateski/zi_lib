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

#ifndef ZI_WATERSHED_QUICKIE_WATERSHED_HPP
#define ZI_WATERSHED_QUICKIE_WATERSHED_HPP 1

#include <zi/watershed/constants.hpp>
#include <zi/watershed/chunk.hpp>
#include <zi/watershed/affinity_matrix.hpp>
#include <zi/watershed/thresholds.hpp>
#include <zi/parallel/algorithm.hpp>
#include <zi/concurrency.hpp>
#include <zi/system.hpp>
#include <zi/time/timer.hpp>
#include <zi/bits/cstdint.hpp>
#include <zi/bits/unordered_map.hpp>
#include <zi/utility/for_each.hpp>
#include <zi/utility/string_printf.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/disjoint_sets/disjoint_sets.hpp>

#include <algorithm>
#include <utility>
#include <cstddef>
#include <iostream>
#include <deque>
#include <list>
#include <iostream>
#include <vector>

#define QW_CHECKPOINT( x, ... )                                         \
    if ( verbose_ )                                                     \
    {                                                                   \
        std::cout << string_printf( "QW [ %8d ] ",                      \
                                    timer_.elapsed< zi::in_msecs >() ); \
        std::cout << string_printf( x, ##__VA_ARGS__ );                 \
        std::cout << "\n" << std::flush;                                \
    } else static_cast< void >( 0 )

namespace zi {
namespace watershed {

template< class T, class R >
class quickie_watershed_impl: non_copyable
{
private:

    affinity_matrix< T >&     conn_       ;
    const thresholds< T >&    thresholds_ ;
    chunk< R >&               chunk_      ;
    int                       position_   ;
    zi::mutex                 mutex_      ;
    zi::timer::wall           timer_      ;
    bool                      verbose_    ;

    static const std::size_t visited_bit = 1ull << ( sizeof( R ) * 8 - 1 );
    static const std::size_t countme_bit = 1ull << 7 ;
    static const std::size_t clear_bits  = visited_bit - 1;

    inline std::pair< R, R > min_max_pair( const R &a, const R &b ) const
    {
        if ( a < b )
        {
            return std::make_pair( a, b );
        }
        else
        {
            return std::make_pair( b, a );
        }
    }

    void add_edges( std::vector< R >& sizes )
    {

        T* connections_ = conn_.data();
        R* result_ = chunk_.data();

        const T hi_thold  = thresholds_.hi();
        const T low_thold = thresholds_.low();
        const T min_thold = thresholds_.min();

        const int64_t xy_dim      = chunk_.x()  * chunk_.y();
        const int64_t xyz_dim     = xy_dim  * chunk_.z();
        const int64_t xyz_dim2    = xy_dim  * chunk_.z() * 2;
        const int64_t nhood[6]    = { -1, -chunk_.x(), -xy_dim, 1, chunk_.x(), xy_dim };

        for ( int64_t i = 0, d = 0; d < 3; ++d )
        {
            for ( int64_t j = 0, z = 0; z < chunk_.z(); ++z )
            {
                for ( int64_t y = 0; y < chunk_.y(); ++y )
                {
                    for ( int64_t x = 0; x < chunk_.x(); ++x, ++j, ++i )
                    {
                        if ( ( x == 0 ) || ( y == 0 ) || ( z == 0 ) ||
                             ( ( x == chunk_.x() - 1 ) && ( d != 0 ) )  ||
                             ( ( y == chunk_.y() - 1 ) && ( d != 1 ) )  ||
                             ( ( z == chunk_.z() - 1 ) && ( d != 2 ) ) )
                        {
                        }
                        else
                        {
                            if ( connections_[ i ] >= hi_thold )
                            {
                                result_[ j ]              |= ( 1 << d );
                                result_[ j + nhood[ d ] ] |= ( 8 << d );
                            }
                        }
                    }
                }
            }
        }

        for ( int64_t j = 0, z = 0; z < chunk_.z(); ++z )
        {
            for ( int64_t y = 0; y < chunk_.y(); ++y )
            {
                for ( int64_t x = 0; x < chunk_.x(); ++x, ++j )
                {

                    if ( ( x == 0 ) ||
                         ( y == 0 ) ||
                         ( z == 0 ) ||
                         ( x == chunk_.x() - 1 ) ||
                         ( y == chunk_.y() - 1 ) ||
                         ( z == chunk_.z() - 1 ) )
                    {
                        continue;
                    }


                    R d      = static_cast< R >( 0  );
                    T top    = static_cast< T >( -1 );
                    T second = static_cast< T >( -1 );


#define ZI_WATERSHED_EDGE_CHECK( direction, index_exp )                 \
                    if ( connections_[ index_exp ] >  top )             \
                    {                                                   \
                        d = direction;                                  \
                        second = top;                                   \
                        top = connections_[ index_exp ];                \
                    }                                                   \
                    else static_cast< void >( 0 )

                    ZI_WATERSHED_EDGE_CHECK( 1, j );
                    ZI_WATERSHED_EDGE_CHECK( 2, j + xyz_dim );
                    ZI_WATERSHED_EDGE_CHECK( 3, j + xyz_dim2 );
                    ZI_WATERSHED_EDGE_CHECK( 4, j + 1 );
                    ZI_WATERSHED_EDGE_CHECK( 5, j + xyz_dim + chunk_.x() );
                    ZI_WATERSHED_EDGE_CHECK( 6, j + xyz_dim2 + xy_dim);

#undef ZI_WATERSHED_EDGE_CHECK


                    if ( d > 0 )
                    {
                        if ( top >= low_thold )
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
                            result_[ j ] |= countme_bit;
                        }
                        else
                        {
                            if ( top >= min_thold )
                            {
                                result_[ j ] |= countme_bit;
                            }
                        }
                    }
                }
            }
        }
    }

public:

    quickie_watershed_impl( affinity_matrix< T >& conn,
                            const thresholds< T >& tholds,
                            chunk< R >& chnk,
                            int position = 0 )
        : conn_( conn ),
          thresholds_( tholds ),
          chunk_( chnk ),
          position_( position ),
          mutex_(),
          timer_(),
          verbose_( false )
    {
        chunk_.fill( 0 );
    }

    void doit( std::vector< std::pair< T, std::pair< R, R > > >& dendr,
               std::vector< R >& sizes, bool verbose )
    {
        verbose_ = verbose;

        const T           hi_thold   = thresholds_.hi();
        const T           low_thold  = thresholds_.low();
        const T           min_thold  = thresholds_.min();
        const std::size_t size_thold = thresholds_.size();

        timer_.reset();
        QW_CHECKPOINT( "starting..." );

        R* result_ = chunk_.data();
        QW_CHECKPOINT( "data obtained" );

        add_edges( sizes );
        QW_CHECKPOINT( "initial edges added" );

        T* connections_ = conn_.data();

        R next_index = 1;
        std::deque< int64_t > queue;

        const int64_t xy_dim      = chunk_.x()  * chunk_.y();
        const int64_t xyz_dim     = xy_dim  * chunk_.z();
        const int64_t nhood[ 6 ]  = { -1, -chunk_.x(), -xy_dim, 1, chunk_.x(), xy_dim };

        for ( int64_t i = 0; i < xyz_dim; ++i )
        {
            if ( !( visited_bit & result_[ i ] ) && result_[ i ] )
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
                            const int64_t pot = cur + nhood[ z ];

                            if ( !( result_[ pot ] & visited_bit ) )
                            {
                                result_[ pot ] |= visited_bit;
                                queue.push_back( pot );
                            }
                        }
                    }

                    result_[ cur ] = visited_bit | next_index;
                }
                ++next_index;
            }
        }

        sizes.resize( next_index );

        for ( std::size_t i = 0; i < chunk_.size(); ++i )
        {
            result_[ i ] = result_[ i ] & clear_bits;
            ++sizes[ result_[ i ] ];
        }

        QW_CHECKPOINT( "initial domains found ( %lld )", sizes.size() - 1 );

        zi::unordered_map< std::pair< R, R >, T > uniques;

        int64_t total_valid_edges = 0;

        for ( int64_t i = 0, d = 0; d < 3; ++d )
        {
            for ( int64_t j = 0, z = 0; z < chunk_.z(); ++z )
            {
                for ( int64_t y = 0; y < chunk_.y(); ++y )
                {
                    for ( int64_t x = 0; x < chunk_.x(); ++x, ++j, ++i )
                    {
                        if ( (x == 0) && (d == 0) ) continue;
                        if ( (y == 0) && (d == 1) ) continue;
                        if ( (z == 0) && (d == 2) ) continue;

                        if ( connections_[ i ] >= min_thold )
                        {
                            if ( result_[ j ] &&
                                 result_[ j + nhood[ d ] ] &&
                                 result_[ j ] != result_[ j + nhood[ d ] ] )
                            {
                                ++total_valid_edges;

                                std::pair< R, R > x =
                                    min_max_pair( result_[ j ], result_[ j + nhood[ d ] ] );

                                typename zi::unordered_map< std::pair< R, R >, T >::iterator it =
                                    uniques.find( x );

                                if ( it == uniques.end() )
                                {
                                    uniques.insert( std::make_pair( x, connections_[ i ] ) );
                                }
                                else
                                {
                                    if ( it->second <  connections_[ i ] )
                                    {
                                        it->second = connections_[ i ];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        QW_CHECKPOINT( "unique graph edges found ( %lld )", uniques.size() );

        conn_.free_data();

        std::vector< std::pair< T, std::pair< R, R > > > graph( uniques.size() );

        std::size_t idx = 0;

        FOR_EACH( it, uniques )
        {
            graph[ idx++ ] = make_pair( it->second, it->first );
        }

        uniques.clear();

        zi::sort( graph.begin(), graph.end() );

        QW_CHECKPOINT( "graph created and sorted by value" );


        //
        // mark domains touching the border

        std::vector< bool > on_border( next_index );
        std::vector< R > vals;

        if ( !( position_ & XFRONT ) )
        {
            chunk_.xslice( 1, vals );

            FOR_EACH( it, vals )
            {
                on_border[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_xslice( 0, 0 );
        }


        if ( !( position_ & XBACK ) )
        {
            chunk_.xslice( chunk_.x() - 2, vals );

            FOR_EACH( it, vals )
            {
                on_border[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_xslice( chunk_.x() - 1, 0 );
        }

        if ( !( position_ & YFRONT ) )
        {
            chunk_.yslice( 1, vals );

            FOR_EACH( it, vals )
            {
                on_border[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_yslice( 0, 0 );
        }


        if ( !( position_ & YBACK ) )
        {
            chunk_.yslice( chunk_.y() - 2, vals );

            FOR_EACH( it, vals )
            {
                on_border[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_yslice( chunk_.y() - 1, 0 );
        }


        if ( !( position_ & ZFRONT ) )
        {
            chunk_.zslice( 1, vals );

            FOR_EACH( it, vals )
            {
                on_border[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_zslice( 0, 0 );
        }



        if ( !( position_ & ZBACK ) )
        {
            chunk_.zslice( chunk_.z() - 2, vals );

            FOR_EACH( it, vals )
            {
                on_border[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_zslice( chunk_.z() - 1, 0 );
        }

        vals.clear();

        QW_CHECKPOINT( "border values found" );

        zi::disjoint_sets< R > trees( next_index );

        FOR_EACH_R( it, graph )
        {
            if ( it->first < min_thold )
            {
                break;
            }

            const R v1 = trees.find_set( it->second.first  );
            const R v2 = trees.find_set( it->second.second );

            if ( v1 != v2 )
            {
                if ( ( sizes[ v1 ] < size_thold && ( !on_border[ v1 ] ) ) ||
                     ( sizes[ v2 ] < size_thold && ( !on_border[ v2 ] ) ) )
                {
                    sizes[ v1 ] += sizes[ v2 ];
                    sizes[ v2 ]  = 0;
                    const R vr = trees.join( v1, v2 );
                    std::swap( sizes[ vr ], sizes[ v1 ] );
                    on_border[ vr ] = on_border[ v1 ] || on_border[ v2 ];
                    --next_index;
                }
            }
        }

        std::vector< R > remaps( sizes.size() );

        R new_index = 1;
        for ( int64_t i = 1; i < sizes.size(); ++i )
        {
            if ( sizes[ i ] )
            {
                if ( sizes[ i ] >= size_thold || on_border[ i ] )
                {
                    sizes[ new_index ] = sizes[ i ];
                    remaps[ i ] = new_index;
                    ++new_index;
                }
                else
                {
                    sizes[ 0 ] += sizes[ i ];
                    sizes[ i ] = remaps[ i ] = 0;
                    --next_index;
                }
            }
        }

        on_border.clear();
        sizes.resize( new_index );

        QW_CHECKPOINT( "size threshold applied new domains ( %lld )", sizes.size() - 1 );

        for ( std::size_t i = 0; i < chunk_.size(); ++i )
        {
            result_[ i ] = remaps[ trees.find_set( result_[ i ] ) ];
        }

        trees.clear();
        dendr.clear();

        FOR_EACH_R( it, graph )
        {
            if ( it->first < low_thold )
            {
                break;
            }

            R a1 = remaps[ it->second.first  ];
            R a2 = remaps[ it->second.second ];

            if ( a1 && a2 )
            {
                R v1 = trees.find_set( a1 );
                R v2 = trees.find_set( a2 );

                if ( v1 != v2 )
                {
                    //edges[ a1 ].push_back( std::make_pair( it->first, a2 ) );
                    //edges[ a2 ].push_back( std::make_pair( it->first, a1 ) );
                    dendr.push_back( std::make_pair( it->first,
                                                     std::make_pair( a1, a2 ) ));
                    trees.join( v1, v2 );
                }
            }
        }


        QW_CHECKPOINT( "dendr created ( %lld )", dendr.size() );

        graph.clear();

        /*
        std::cout << "graph_cleared\n" << std::flush;

        std::deque< R > bfs;
        std::vector< bool > visited( next_index );

        for ( R i = 1; i < next_index; ++i )
        {
            if ( !visited[ i ] )
            {
                bfs.push_back( i );
                visited[ i ] = true;

                while ( !bfs.empty() )
                {
                    FOR_EACH( it, edges[ bfs.front() ] )
                    {
                        if ( !visited[ it->second ] )
                        {
                            visited[ it->second ] = true;
                            dendr.push_back( std::make_pair
                                             ( it->first,
                                               std::make_pair( it->second, bfs.front() )));
                            bfs.push_back( it->second );
                        }
                    }

                    edges[ bfs.front() ].clear();
                    bfs.pop_front();
                }
            }
            }*/

        zi::sort( dendr.begin(), dendr.end(),
                  std::greater< std::pair< T, std::pair< R, R > > >() );

        QW_CHECKPOINT( "dendr sorted .. all done" );


    }

};

template< class T, class R >
std::size_t simple_watershed( affinity_matrix< T >& conn,
                              const thresholds< T >& tholds,
                              chunk< R >& chnk,
                              int position,
                              std::vector< std::pair< T, std::pair< R, R > > >& dendr,
                              std::vector< R >& sizes,
                              bool verbose = true )
{
    typedef typename zi::watershed::quickie_watershed_impl< T, R > ws_type;
    ws_type ws( conn, tholds, chnk, position);
    ws.doit( dendr, sizes, verbose ) ;
    return sizes.size();
}


#undef QW_CHECKPOINT

} // namespace watershed
} // namespace zi

#endif

