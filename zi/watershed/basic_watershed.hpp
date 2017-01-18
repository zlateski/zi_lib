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

#ifndef ZI_WATERSHED_BASIC_WATERSHED_HPP
#define ZI_WATERSHED_BASIC_WATERSHED_HPP 1

#include <zi/watershed/chunk.hpp>
#include <zi/watershed/affinity_matrix.hpp>
#include <zi/parallel/algorithm.hpp>
#include <zi/concurrency.hpp>
#include <zi/system.hpp>
#include <zi/bits/cstdint.hpp>
#include <zi/bits/unordered_map.hpp>
#include <zi/utility/for_each.hpp>
#include <zi/disjoint_sets/disjoint_sets.hpp>


#include <algorithm>
#include <utility>
#include <cstddef>
#include <iostream>
#include <deque>
#include <list>
#include <iostream>
#include <vector>

namespace zi {
namespace watershed {

template< class T, class R >
class basic_watershed_impl
{
private:
    affinity_matrix< T >& conn_       ;
    const T               low_thold_  ;
    const T               hi_thold_   ;
    const T               min_thold_  ;
    std::size_t           size_thold_ ;
    chunk< R >&           chunk_      ;
    zi::mutex             mutex_      ;

    static const std::size_t visited_bit = 1ull << ( sizeof( R ) * 8 - 1 );
    static const std::size_t countme_bit = 1ull << 7 ;
    static const std::size_t clear_bits  = visited_bit - 1;

    std::pair< R, R > min_max_pair( const R &a, const R &b )
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
                        //if ( ( (x == 0) && (d == 0) ) ||
                        //( (y == 0) && (d == 1) ) ||
                        //( (z == 0) && (d == 2) ) )
                        //{
                        //}
                        if ( ( x == 0 ) || ( y == 0 ) || ( z == 0 ) ||
                             ( ( x == chunk_.x() - 1 ) && ( d != 0 ) )  ||
                             ( ( y == chunk_.y() - 1 ) && ( d != 1 ) )  ||
                             ( ( z == chunk_.z() - 1 ) && ( d != 2 ) ) )
                        {
                        }
                        else
                        {
                            if ( connections_[ i ] >= hi_thold_ )
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

/*

#define ZI_WATERSHED_EDGE_CHECK( direction, condition_exp, index_exp )  \
                    if ( condition_exp )                                \
                    {                                                   \
                        if ( connections_[ index_exp ] >  top )         \
                        {                                               \
                            d = direction;                              \
                            second = top;                               \
                            top = connections_[ index_exp ];            \
                        }                                               \
                    }                                                   \
                    static_cast< void >( 0 )

                    ZI_WATERSHED_EDGE_CHECK( 1, x > 0, j );
                    ZI_WATERSHED_EDGE_CHECK( 2, y > 0, j + xyz_dim );
                    ZI_WATERSHED_EDGE_CHECK( 3, z > 0, j + xyz_dim2 );
                    ZI_WATERSHED_EDGE_CHECK( 4, x < chunk_.x() - 1, j + 1 );
                    ZI_WATERSHED_EDGE_CHECK( 5, y < chunk_.y() - 1, j + xyz_dim + chunk_.x() );
                    ZI_WATERSHED_EDGE_CHECK( 6, z < z_dim_ - 1, j + xyz_dim2 + xy_dim);

#undef ZI_WATERSHED_EDGE_CHECK

*/

#define ZI_WATERSHED_EDGE_CHECK( direction, index_exp )                 \
                    if ( connections_[ index_exp ] >  top )             \
                    {                                                   \
                        d = direction;                                  \
                        second = top;                                   \
                        top = connections_[ index_exp ];                \
                    }                                                   \
                    static_cast< void >( 0 )

                    ZI_WATERSHED_EDGE_CHECK( 1, j );
                    ZI_WATERSHED_EDGE_CHECK( 2, j + xyz_dim );
                    ZI_WATERSHED_EDGE_CHECK( 3, j + xyz_dim2 );
                    ZI_WATERSHED_EDGE_CHECK( 4, j + 1 );
                    ZI_WATERSHED_EDGE_CHECK( 5, j + xyz_dim + chunk_.x() );
                    ZI_WATERSHED_EDGE_CHECK( 6, j + xyz_dim2 + xy_dim);

#undef ZI_WATERSHED_EDGE_CHECK


                    if ( d > 0 )
                    {
                        if ( top >= low_thold_ )
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
                            if ( top >= min_thold_ )
                            {
                                result_[ j ] |= countme_bit;
                            }
                        }
                    }
                    //else
                    //{
                    //++sizes[ 0 ];
                    //}
                }
            }
        }

    }

public:
    basic_watershed_impl( affinity_matrix< T >& conn,
                          const T low_thold,
                          const T hi_thold,
                          const T min_thold,
                          std::size_t size_thold,
                          chunk< R >& chnk )
        : conn_( conn ),
          low_thold_( low_thold ),
          hi_thold_( hi_thold ),
          min_thold_( std::min( low_thold, min_thold ) ),
          size_thold_( size_thold ),
          chunk_( chnk ),
          mutex_()
    {
        chunk_.fill( 0 );
    }

    void doit( std::vector< std::pair< T, std::pair< R, R > > >& dendr,
               std::vector< R >& sizes )
    {
        R* result_ = chunk_.data();

        add_edges( sizes );

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

                        if ( connections_[ i ] >= min_thold_ )
                        {
                            if ( result_[ j ] &&
                                 result_[ j + nhood[ d ] ] &&
                                 result_[ j ] != result_[ j + nhood[ d ] ] )
                            {
                                ++total_valid_edges;

                                std::pair< R, R > x =
                                    min_max_pair( result_[ j ], result_[ j + nhood[ d ] ] );

                                if ( uniques.count( x ) == 0 )
                                {
                                    uniques.insert( std::make_pair( x, connections_[ i ] ) );
                                }
                                else
                                {
                                    if ( uniques[ x ] < connections_[ i ] )
                                    {
                                        uniques[ x ] = connections_[ i ];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        conn_.free_data();

        std::cout << "total_domains           : " << ( next_index - 1 ) << "\n";


        std::vector< std::pair< T, std::pair< R, R > > > graph( uniques.size() );

        std::size_t idx = 0;

        std::cout << "total_uniques: " << uniques.size() << "\n" << std::flush;

        FOR_EACH( it, uniques )
        {
            graph[ idx++ ] = make_pair( it->second, it->first );
        }

        uniques.clear();

        zi::sort( graph.begin(), graph.end() );


        //
        // mark segs touching the border

        std::vector< bool > on_border( next_index );
        std::vector< R > vals;

        chunk_.xslice( 1, vals );

        FOR_EACH( it, vals )
        {
            on_border[ *it ] = true;
        }

        chunk_.xslice( chunk_.x() - 2, vals );

        FOR_EACH( it, vals )
        {
            on_border[ *it ] = true;
        }
        chunk_.yslice( 1, vals );

        FOR_EACH( it, vals )
        {
            on_border[ *it ] = true;
        }
        chunk_.yslice( chunk_.y() - 2, vals );

        FOR_EACH( it, vals )
        {
            on_border[ *it ] = true;
        }
        chunk_.zslice( 1, vals );

        FOR_EACH( it, vals )
        {
            on_border[ *it ] = true;
        }

        chunk_.zslice( chunk_.z() - 2, vals );

        FOR_EACH( it, vals )
        {
            on_border[ *it ] = true;
        }

        vals.clear();

        zi::disjoint_sets< R > trees( next_index );

        std::cout << "min_thold: " << min_thold_ << "\n";

        FOR_EACH_R( it, graph )
        {
            if ( it->first < min_thold_ )
            {
                break;
            }

            const R v1 = trees.find_set( it->second.first  );
            const R v2 = trees.find_set( it->second.second );

            if ( v1 != v2 )
            {
                if ( ( sizes[ v1 ] < size_thold_ && ( !on_border[ v1 ] ) ) ||
                     ( sizes[ v2 ] < size_thold_ && ( !on_border[ v2 ] ) ) )
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
                if ( sizes[ i ] >= size_thold_ || on_border[ i ] )
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

        std::cout << new_index << " vx. " << next_index << "\n";
        std::cout << trees.find_set( 0 ) << "\n\n";

        for ( std::size_t i = 0; i < chunk_.size(); ++i )
        {
            result_[ i ] = remaps[ trees.find_set( result_[ i ] ) ];
        }

        trees.clear();
        dendr.clear();

        //std::vector< std::list< std::pair< T, R > > > edges( next_index );

        FOR_EACH_R( it, graph )
        {
            if ( it->first < low_thold_ )
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

    }

};

template< class T, class R, class ConvirtableToT1, class ConvirtableToT2, class ConvirtableToT3 >
std::size_t simple_watershed( affinity_matrix< T >& conn,
                              const ConvirtableToT1 low_thold,
                              const ConvirtableToT2 hi_thold,
                              const ConvirtableToT3 min_thold,
                              const std::size_t size_thold,
                              chunk< R >& chnk,
                              std::vector< std::pair< T, std::pair< R, R > > >& dendr,
                              std::vector< R >& sizes )
{
    typedef typename zi::watershed::basic_watershed_impl< T, R > ws_type;
    ws_type ws( conn, low_thold, hi_thold, min_thold, size_thold, chnk );
    ws.doit( dendr, sizes ) ;
    return sizes.size();
}

template< class T, class R, class ConvirtableToT >
std::size_t apply_dendrogram( std::vector< std::pair< T, std::pair< R, R > > >& dendr,
                              const ConvirtableToT threshold,
                              std::vector< R >& sizes,
                              R* result = NULL,
                              const std::size_t result_size = 0 )
{
    zi::disjoint_sets< R > trees( sizes.size() );

    int64_t dend_i = 0;

    for ( dend_i = 0; dend_i < dendr.size(); ++dend_i )
    {
        if ( dendr[ dend_i ].first < threshold )
        {
            break;
        }

        R v1 = trees.find_set( dendr[ dend_i ].second.first  );
        R v2 = trees.find_set( dendr[ dend_i ].second.second );

        if ( v1 != v2 )
        {
            sizes[ v1 ] += sizes[ v2 ];
            sizes[ v2 ]  = 0;
            std::swap( sizes[ trees.join( v1, v2 ) ], sizes[ v1 ] );
        }
    }

    std::vector< R > new_ids( sizes.size() );
    R new_ids_idx = 1;

    for ( int64_t i = 1; i < sizes.size(); ++i )
    {
        if ( sizes[ i ] )
        {
            sizes[ new_ids_idx ] = sizes[ i ];
            new_ids[ i ] = new_ids_idx;
            ++new_ids_idx;
        }
    }

    std::cout << "dend_i: " << dend_i << "\n";

    sizes.resize( new_ids_idx );

    for ( int64_t i = 1; i < new_ids_idx; ++i )
    {
        new_ids[ i ] = new_ids[ trees.find_set( i ) ];
    }

    if ( result && result_size )
    {
        for ( int64_t i = 0; i < result_size; ++i )
        {
            result[ i ] = new_ids[ result[ i ] ];
        }
    }

    int64_t dend_len = 0;

    for ( ; dend_i < dendr.size(); ++dend_i )
    {
        R v1 = trees.find_set( dendr[ dend_i ].second.first  );
        R v2 = trees.find_set( dendr[ dend_i ].second.second );

        if ( v1 != v2 )
        {
            dendr[ dend_len ] = std::make_pair( dendr[ dend_i ].first,
                                                std::make_pair( new_ids[ v1 ],
                                                                new_ids[ v2 ] ));
            ++dend_len;
        }
        else
        {
            std::cout << "ERROR!!!\n";
            exit( 0 );
        }
    }

    dendr.resize( dend_len );

    return sizes.size();

}


template< class T, class R, class ConvirtableToT >
std::size_t run_dendrogram( std::vector< std::pair< T, std::pair< R, R > > >& dendr,
                            std::vector< std::pair< T, std::pair< R, R > > >& new_dendr,
                            std::size_t size,
                            const ConvirtableToT threshold,
                            std::vector< R >& new_ids )
{
    zi::disjoint_sets< R > trees( size );

    int64_t dend_i = 0;

    for ( dend_i = 0; dend_i < dendr.size(); ++dend_i )
    {
        if ( dendr[ dend_i ].first < threshold )
        {
            break;
        }

        R v1 = trees.find_set( dendr[ dend_i ].second.first  );
        R v2 = trees.find_set( dendr[ dend_i ].second.second );

        if ( v1 != v2 )
        {
            trees.join( v1, v2 );
        }
    }

    new_ids.resize( size );
    R new_ids_idx = 1;

    for ( int64_t i = 1; i < size; ++i )
    {
        if ( trees.find_set( i ) == i )
        {
            new_ids[ i ] = new_ids_idx;
            ++new_ids_idx;
        }
    }

    for ( int64_t i = 1; i < new_ids_idx; ++i )
    {
        new_ids[ i ] = new_ids[ trees.find_set( i ) ];
    }

    new_dendr.resize( dendr.size() - dend_i );

    for ( std::size_t i = 0, j = dend_i; i < new_dendr.size(); ++i, ++j )
    {
        R v1 = trees.find_set( dendr[ j ].second.first  );
        R v2 = trees.find_set( dendr[ j ].second.second );

        if ( v1 != v2 )
        {
            new_dendr[ i ] = std::make_pair( dendr[ j ].first,
                                             std::make_pair( new_ids[ v1 ],
                                                             new_ids[ v2 ] ));
        }
        else
        {
            std::cout << "ERROR!!!\n";
            exit( 0 );
        }
    }

    dendr.resize( dend_i );

    return new_ids_idx;

}



} // namespace watershed
} // namespace zi

#endif

