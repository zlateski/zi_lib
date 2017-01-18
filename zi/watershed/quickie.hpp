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

#include <zi/watershed/detail/types.hpp>
#include <zi/watershed/detail/utility.hpp>
#include <zi/watershed/detail/volume_ref.hpp>
#include <zi/watershed/constants.hpp>
#include <zi/watershed/chunk.hpp>
#include <zi/watershed/affinity_matrix.hpp>
#include <zi/watershed/thresholds.hpp>
#include <zi/parallel/algorithm.hpp>
#include <zi/concurrency.hpp>
#include <zi/system.hpp>
#include <zi/time/timer.hpp>
#include <zi/bits/cstdint.hpp>
#include <zi/bits/tuple.hpp>
#include <zi/bits/unordered_map.hpp>
#include <zi/utility/for_each.hpp>
#include <zi/utility/string_printf.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>
#include <zi/disjoint_sets/disjoint_sets.hpp>

#include <algorithm>
#include <utility>
#include <cstddef>
#include <iostream>
#include <limits>
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

#define QW_FOR3( type, v1, f1, t1, v2, f2, t2, v3, f3, t3 )     \
    for ( type v1 = f1; v1 < t1; ++v1 )                         \
        for ( type v2 = f2; v2 < t2; ++v2 )                     \
            for ( type v3 = f3; v3 < t3; ++v3 )

namespace zi {
namespace watershed {

template< class AffinityType, class IdType, class SizeType >
class quickie_impl_c: non_copyable
{
private:

    typedef IdType id_type;
    typedef quickie_impl_c< AffinityType, IdType, SizeType > type;

    typedef std::vector< std::pair< AffinityType, std::pair< IdType, IdType > > > DendrType;
    typedef affinity_matrix< AffinityType > ConnType;
    typedef std::pair< IdType, IdType >     IdPair;

    affinity_matrix< AffinityType >&  conn_       ;
    const AffinityType                hi_         ;
    const AffinityType                low_        ;
    const AffinityType                mst_        ;
    const SizeType                    size_       ;

    chunk< IdType >&                  chunk_      ;

    int                               position_   ;
    zi::timer::wall                   timer_      ;
    bool                              verbose_    ;

    std::vector< SizeType >&          sizes_      ;
    DendrType&                        dendr_      ;
    DendrType                         graph_      ;
    std::vector< bool >               on_border_  ;
    std::vector< IdType >             remaps_     ;

public:
    quickie_impl_c( ConnType&                conn,
                  const AffinityType       hi_thold,
                  const AffinityType       low_thold,
                  const AffinityType       mst_thold,
                  const SizeType           size_thold,
                  chunk< IdType >&         chnk,
                  std::vector< SizeType >& sizes,
                  DendrType&               dendr,
                  int position  )
        : conn_( conn ),
          hi_( hi_thold ),
          low_( low_thold ),
          mst_( mst_thold ),
          size_( size_thold ),
          chunk_( chnk ),
          position_( position ),
          timer_(),
          verbose_( false ),
          sizes_( sizes ),
          dendr_( dendr ),
          graph_()
    {
    }

private:
    static const std::size_t visited_bit = 1ull << ( sizeof( IdType ) * 8 - 1 );
    static const std::size_t countme_bit = 1ull << 7 ;
    static const std::size_t clear_bits  = visited_bit - 1;

    //
    // Step 0.
    //

    void initialize()
    {
        timer_.reset();
        QW_CHECKPOINT( "starting..." );

        static_cast< void >( conn_.data() );
        QW_CHECKPOINT( "data obtained" );

        chunk_.allocate();
        chunk_.fill( 0 );
        QW_CHECKPOINT( "chunk initialized" );

        sizes_.clear();
        dendr_.clear();
        QW_CHECKPOINT( "sizes and dendr arrays prepared" );
    }

    //
    // Step 1.
    //

    void add_edges()
    {

        AffinityType *conn = conn_.data();
        IdType       *ids  = chunk_.data();

        const std::ptrdiff_t xy_dim      = chunk_.x() * chunk_.y();
        const std::ptrdiff_t xyz_dim     = xy_dim * chunk_.z();
        const std::ptrdiff_t xyz_dim2    = xy_dim * chunk_.z() * 2;
        const std::ptrdiff_t nhood[ 6 ]  =
            {
                -1,
                -chunk_.x(),
                -xy_dim,
                1,
                chunk_.x(),
                xy_dim
            };

        const std::ptrdiff_t around[ 7 ]  =
            {
                -1,
                0,
                xyz_dim,
                xyz_dim2,
                1,
                xyz_dim + chunk_.x(),
                xyz_dim2 + xy_dim
            };

        const index left_edge [ 3 ] = { 1,  2,  4 };
        const index right_edge[ 3 ] = { 8, 16, 32 };
        const index dx[ 3 ] = { -1,  0,  0 };
        const index dy[ 3 ] = {  0, -1,  0 };
        const index dz[ 3 ] = {  0,  0, -1 };

        volume_ref< AffinityType, 4 >
            vol( conn_.data(), extents[ chunk_.x() ][ chunk_.y() ][ chunk_.z() ][ 4 ] );

        for ( std::size_t i = 0, d = 0; d < 3; ++d )
        {
            for ( std::size_t j = 0, z = 0; z < chunk_.z(); ++z )
            {
                for ( std::size_t y = 0; y < chunk_.y(); ++y )
                {
                    for ( std::size_t x = 0; x < chunk_.x(); ++x, ++j, ++i )
                    {
                        if ( ( x == 0 ) || ( y == 0 ) || ( z == 0 ) ||
                             ( ( x == chunk_.x() - 1 ) && ( d != 0 ) )  ||
                             ( ( y == chunk_.y() - 1 ) && ( d != 1 ) )  ||
                             ( ( z == chunk_.z() - 1 ) && ( d != 2 ) ) )
                        {
                            conn[ i ] = std::numeric_limits< AffinityType >::min();
                        }
                        else
                        {
                            if ( conn[ i ] >= hi_ )
                            //if ( vol[ x ][ y ][ z ][ d ] >= hi_ )
                            {
                                ids[ j ]              |= ( 1 << d );
                                ids[ j + nhood[ d ] ] |= ( 8 << d );
                            }
                        }
                    }
                }
            }
        }


        for ( std::size_t j = 0, z = 0; z < chunk_.z(); ++z )
        {
            for ( std::size_t y = 0; y < chunk_.y(); ++y )
            {
                for ( std::size_t x = 0; x < chunk_.x(); ++x, ++j )
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

                    int direction = 0;
                    AffinityType top    = std::numeric_limits< AffinityType >::min();
                    AffinityType second = std::numeric_limits< AffinityType >::min();

                    for ( std::size_t i = 1; i < 7; ++i )
                    {
                        if ( conn[ j + around[ i ] ] > top )
                        {
                            direction = i;
                            second    = top;
                            top       = conn[ j + around[ i ] ];
                        }
                    }

                    if ( direction > 0 )
                    {
                        if ( top >= low_ )
                        {
                            if ( top > second )
                            {
                                --direction;
                                if ( direction < 3 )
                                {
                                    ids[ j ]                      |= ( 1 << direction );
                                    ids[ j + nhood[ direction ] ] |= ( 8 << direction );
                                }
                                else
                                {
                                    ids[ j ]                      |= ( 1 << direction );
                                    ids[ j + nhood[ direction ] ] |= ( 1 << ( direction - 3 ));
                                }
                            }
                            ids[ j ] |= countme_bit;
                        }
                        /* else
                        {
                            if ( top >= mst_ )
                            {
                                ids[ j ] |= countme_bit;
                            }
                            }*/
                    }
                }
            }
        }

        QW_CHECKPOINT( "initial edges added" );
    }

    //
    // Step 2.
    //

    void initial_domains()
    {
        IdType       *ids  = chunk_.data();
        IdType        next_index = 1;

        std::deque< std::size_t > queue;

        const std::size_t xy_dim      = chunk_.x() * chunk_.y();
        const std::size_t xyz_dim     = xy_dim * chunk_.z();
        const std::size_t nhood[ 6 ]  =
            {
                -1,
                -chunk_.x(),
                -xy_dim,
                1,
                chunk_.x(),
                xy_dim
            };

        for ( std::size_t i = 0; i < xyz_dim; ++i )
        {
            if ( !( visited_bit & ids[ i ] ) && ids[ i ] )
            {
                queue.push_back( i );

                while ( queue.size() )
                {
                    const std::size_t cur = queue.front();
                    queue.pop_front();

                    for ( std::size_t z = 0; z < 6; ++z )
                    {
                        if ( ids[ cur ] & ( 1 << z ) )
                        {
                            const std::size_t pot = cur + nhood[ z ];

                            if ( !( ids[ pot ] & visited_bit ) )
                            {
                                ids[ pot ] |= visited_bit;
                                queue.push_back( pot );
                            }
                        }
                    }

                    ids[ cur ] = visited_bit | next_index;
                }
                ++next_index;
            }
        }

        sizes_.resize( next_index );

        for ( std::size_t j = 0, z = 0; z < chunk_.z(); ++z )
        {
            for ( std::size_t y = 0; y < chunk_.y(); ++y )
            {
                for ( std::size_t x = 0; x < chunk_.x(); ++x, ++j )
                {

                    ids[ j ] = ids[ j ] & clear_bits;

                    if ( ( x == 0 ) ||
                         ( y == 0 ) ||
                         ( z == 0 ) ||
                         ( x == chunk_.x() - 1 ) ||
                         ( y == chunk_.y() - 1 ) ||
                         ( z == chunk_.z() - 1 ) )
                    {
                    }
                    else
                    {
                        ++sizes_[ ids[ j ] ];
                    }

                }
            }
        }

        QW_CHECKPOINT( "initial domains found ( %lld )", sizes_.size() - 1 );

    }

    //
    // Step 3.
    //

    void create_graph()
    {

        AffinityType *conn = conn_.data();
        IdType       *ids  = chunk_.data();

        const std::size_t xy_dim      = chunk_.x() * chunk_.y();
        const int64_t nhood[ 6 ]  =
            {
                -1,
                -chunk_.x(),
                -xy_dim,
                1,
                chunk_.x(),
                xy_dim
            };


        zi::unordered_map< IdPair, AffinityType > uniques;

        AffinityType dend_min_ = std::min( low_, mst_ );

        for ( std::size_t i = 0, d = 0; d < 3; ++d )
        {
            for ( std::size_t j = 0, z = 0; z < chunk_.z(); ++z )
            {
                for ( std::size_t y = 0; y < chunk_.y(); ++y )
                {
                    for ( std::size_t x = 0; x < chunk_.x(); ++x, ++j, ++i )
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

                        if ( conn[ i ] >= dend_min_ )
                        {
                            if ( ids[ j ] &&
                                 ids[ j + nhood[ d ] ] &&
                                 ids[ j ] != ids[ j + nhood[ d ] ] )
                            {
                                IdPair x = minmax( ids[ j ], ids[ j + nhood[ d ] ] );

                                typename zi::unordered_map< IdPair, AffinityType >::iterator it =
                                    uniques.find( x );

                                if ( it == uniques.end() )
                                {
                                    uniques.insert( std::make_pair( x, conn[ i ] ) );
                                }
                                else
                                {
                                    if ( it->second < conn[ i ] )
                                    {
                                        it->second = conn[ i ];
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

        graph_.resize( uniques.size() );

        std::size_t idx = 0;

        FOR_EACH( it, uniques )
        {
            graph_[ idx++ ] = make_pair( it->second, it->first );
        }

        uniques.clear();

        zi::sort( graph_.begin(), graph_.end() );

        QW_CHECKPOINT( "graph created and sorted by value" );
    }


    //
    // Step 4.
    //

    void process_borders()
    {

        on_border_.resize( sizes_.size() );
        std::vector< IdType > vals;

        if ( !( position_ & XFRONT ) )
        {
            chunk_.xslice( 1, vals );

            FOR_EACH( it, vals )
            {
                on_border_[ *it ] = true;
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
                on_border_[ *it ] = true;
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
                on_border_[ *it ] = true;
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
                on_border_[ *it ] = true;
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
                on_border_[ *it ] = true;
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
                on_border_[ *it ] = true;
            }
        }
        else
        {
            chunk_.fill_zslice( chunk_.z() - 1, 0 );
        }

        QW_CHECKPOINT( "border values found" );

    }

    //
    // Step 5.
    //

    void apply_size_threshold()
    {

        IdType *ids  = chunk_.data();

        disjoint_sets< IdType > trees( sizes_.size() );

        IdType next_index = sizes_.size();

        FOR_EACH_R( it, graph_ )
        {
            if ( it->first < mst_ )
            {
                break;
            }

            const IdType v1 = trees.find_set( it->second.first  );
            const IdType v2 = trees.find_set( it->second.second );

            if ( v1 != v2 )
            {
                if ( ( sizes_[ v1 ] < size_ && ( !on_border_[ v1 ] ) ) ||
                     ( sizes_[ v2 ] < size_ && ( !on_border_[ v2 ] ) ) )
                {
                    sizes_[ v1 ] += sizes_[ v2 ];
                    sizes_[ v2 ]  = 0;
                    const IdType vr = trees.join( v1, v2 );
                    std::swap( sizes_[ vr ], sizes_[ v1 ] );
                    on_border_[ vr ] = on_border_[ v1 ] || on_border_[ v2 ];
                    --next_index;
                }
            }
        }

        remaps_.resize( sizes_.size() );

        IdType new_index = 1;

        for ( std::size_t i = 1; i < sizes_.size(); ++i )
        {
            if ( sizes_[ i ] )
            {
                if ( sizes_[ i ] >= size_ || on_border_[ i ] )
                {
                    sizes_[ new_index ] = sizes_[ i ];
                    remaps_[ i ] = new_index;
                    ++new_index;
                }
                else
                {
                    sizes_[ 0 ] += sizes_[ i ];
                    sizes_[ i ] = remaps_[ i ] = 0;
                    --next_index;
                }
            }
        }

        ZI_ASSERT( next_index == new_index );

        on_border_.clear();
        sizes_.resize( new_index );

        QW_CHECKPOINT( "size threshold applied new domains ( %lld )", sizes_.size() - 1 );

        for ( std::size_t i = 0; i < chunk_.size(); ++i )
        {
            ids[ i ] = remaps_[ trees.find_set( ids[ i ] ) ];
        }

        QW_CHECKPOINT( "new domain ids applied" );
    }

    //
    // Step 6.
    //

    void generate_dendr()
    {

        disjoint_sets< IdType > trees( sizes_.size() );

        FOR_EACH_R( it, graph_ )
        {

            IdType a1 = remaps_[ it->second.first  ];
            IdType a2 = remaps_[ it->second.second ];

            if ( a1 && a2 )
            {
                IdType v1 = trees.find_set( a1 );
                IdType v2 = trees.find_set( a2 );

                if ( v1 != v2 )
                {
                    dendr_.push_back( std::make_pair( it->first, std::make_pair( a1, a2 ) ));
                    trees.join( v1, v2 );
                }
            }
        }

        QW_CHECKPOINT( "dendr created ( %lld )", dendr_.size() );

        graph_.clear();

        zi::sort( dendr_.begin(), dendr_.end(),
                  std::greater< std::pair< AffinityType, IdPair > >() );

        QW_CHECKPOINT( "dendr sorted .. all done" );



    }


public:

    void run( bool verbose = true )
    {
        verbose_ = verbose;

        initialize();
        add_edges();
        initial_domains();
        create_graph();
        process_borders();
        apply_size_threshold();
        generate_dendr();
    }

    void operator()( bool verbose = true )
    {
        run();
    }

};

template< class AffinityType, class T1, class T2, class T3, class T4, class IdType, class SizeType >
std::size_t quickie( affinity_matrix< AffinityType >& conn,
                     const T1& hi_thold,
                     const T2& low_thold,
                     const T3& mst_thold,
                     const T4& size_thold,
                     chunk< IdType >& chnk,
                     int position,
                     std::vector< std::pair< AffinityType, std::pair< IdType, IdType > > >& dendr,
                     std::vector< SizeType >& sizes,
                     bool verbose = true )
{
    typedef quickie_impl_c< AffinityType, IdType, SizeType > QuickieType;

    QuickieType ws( conn,
                    static_cast< AffinityType >( hi_thold ),
                    static_cast< AffinityType >( low_thold ),
                    static_cast< AffinityType >( mst_thold, low_thold ),
                    static_cast< SizeType >( size_thold ),
                    chnk,
                    sizes,
                    dendr,
                    position );

    ws.run( verbose );

    return 3; //sizes.size();
}

template< class AffinityType, class ThresholdsType, class IdType, class SizeType >
inline std::size_t quickie( affinity_matrix< AffinityType >& conn,
                            const ThresholdsType& tholds,
                            chunk< IdType >& chnk,
                            const int position,
                            std::vector< std::pair< AffinityType, std::pair< IdType, IdType > > >& dendr,
                            std::vector< SizeType >& sizes,
                            const bool verbose = true )
{
    return quickie< AffinityType, AffinityType, AffinityType, AffinityType, SizeType, IdType, SizeType >
        ( conn,
          static_cast< AffinityType >( tholds.hi() ),
          static_cast< AffinityType >( tholds.low() ),
          static_cast< AffinityType >( tholds.mst() ),
          static_cast< SizeType >( tholds.size() ),
          chnk,
          position,
          dendr,
          sizes,
          verbose );
}


#undef QW_CHECKPOINT

} // namespace watershed
} // namespace zi

#endif

