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

#ifndef ZI_WATERSHED_XXL_WATERSHED_IMPL_HPP
#define ZI_WATERSHED_XXL_WATERSHED_IMPL_HPP 1

#include <zi/watershed/xxl/watershed.hpp>
#include <zi/watershed/detail/traits.hpp>
#include <zi/watershed/detail/file_path.hpp>
#include <zi/watershed/quickie/quickie_impl.hpp>
#include <zi/watershed/detail/xxl_chunk.hpp>
#include <zi/watershed/detail/mmap_vector.hpp>
#include <zi/watershed/detail/xxl_queue.hpp>
#include <zi/bits/cstdint.hpp>
#include <zi/bits/bind.hpp>
#include <zi/bits/shared_ptr.hpp>
#include <zi/bits/unordered_set.hpp>
#include <zi/memory/detail/mmap_file.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/rwmutex.hpp>
#include <zi/concurrency/task_manager.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/string_printf.hpp>

#include <cstddef>
#include <iostream>
#include <limits>
#include <vector>

namespace zi {
namespace watershed {

namespace detail {

struct watershed_task
{
    int32_t affinity_bitsize;
    int32_t id_bitsize      ;
    int32_t x;
    int32_t y;
    int32_t z;
};

} // namespace detail

template< class T, class Id, class Count >
class xxl_watershed_impl: non_copyable
{
public:
    typedef T           value_type      ;
    typedef const T*    const_value_ptr ;
    typedef Id          id_type         ;
    typedef Id*         id_ptr          ;
    typedef Count       count_type      ;

    typedef std::pair< id_type, id_type >         id_pair;
    typedef tuple< id_type, id_type, value_type > edge_type;
    typedef std::vector< edge_type >              edge_vector;

    typedef xxl_watershed_impl< T, Id, Count >    type;

    typedef std::vector< std::pair< T, std::pair< Id, Id > > > dendr_type;

    typedef unordered_map< id_pair, T > edgemap_type;

    typedef chunk< Id >                           chunk_type;
    typedef shared_ptr< chunk< Id > >             chunk_ptr ;


    typedef detail::xxl_chunk< T, Id, Count >     xxl_chunk     ;
    typedef shared_ptr< xxl_chunk >               xxl__chunk_ptr;

private:
    const std::string        file_prefix_;
    const size_type          size_;
    mutex                    mutex_;
    const T                  hi_;
    const T                  low_;
    const T                  mst_;
    const Count              size_thold_;

    xxl_chunk_ptr*                 chunk_data_;
    volume_ref< xxl_chunk_ptr, 3 > chunks_;

    task_manager::simple serial_tm_;

    std::vector< value_type > merge_at_;
    std::vector< Count > sizes_;
    std::vector< Id   > order_;
    std::vector< bool > needs_merge_;
    disjoint_sets< Id > trees_;

    size_type num_ready_  ;
    Id num_domains_;

    detail::xxl_queue< edge_type, edge_less< T, id_type > > queue_;

public:
    explicit xxl_watershed_impl( const std::string& file_prefix,
                                 const size_type  xdim,
                                 const size_type  ydim,
                                 const size_type  zdim,
                                 const T& hi,
                                 const T& low,
                                 const T& mst,
                                 const Count& size_thold )
        : file_prefix_( file_prefix ),
          size_( xdim * ydim * zdim ),
          mutex_(),
          hi_( hi ),
          low_( low ),
          mst_( mst ),
          size_thold_( size_thold ),
          chunk_data_( new xxl_chunk_ptr[ size_ ] ),
          chunks_( chunk_data_, extents[xdim][ydim][zdim] ),
          serial_tm_( 1 ),
          merge_at_(),
          sizes_(),
          order_(),
          trees_( 1 ),
          num_ready_(),
          num_domains_( 0 ),
          queue_()
    {

        int32_t chunk_sizes[ size_ * 3 ];
        mem::mmap_file::read( file_prefix_ + ".chunksizes", chunk_sizes, size_ * 12 );

        for ( size_type x = 0, idx = 0; x < xdim; ++x )
        {
            for ( size_type y = 0; y < ydim; ++y )
            {
                for ( size_type z = 0; z < zdim; ++z, ++idx )
                {
                    std::cout << "chunk [ " << x << ' '
                              << y << ' ' << z << " ] size: [ "
                              << chunk_sizes[ idx*3   ] << ' ';
                    std::cout << chunk_sizes[ idx*3+1 ] << ' ';
                    std::cout << chunk_sizes[ idx*3+2 ] << " ]\n";

                    int flags = 0;
                    flags |= x == 0 ? 0 : border::before_x;
                    flags |= y == 0 ? 0 : border::before_y;
                    flags |= z == 0 ? 0 : border::before_z;
                    flags |= x == xdim - 1 ? 0 : border::after_x;
                    flags |= y == ydim - 1 ? 0 : border::after_y;
                    flags |= z == zdim - 1 ? 0 : border::after_z;
                    //flags = 0;

                    chunks_[x][y][z] = xxl_chunk_ptr( new xxl_chunk( x, y, z, flags,
                                                                     chunk_sizes[ idx*3   ],
                                                                     chunk_sizes[ idx*3+1 ],
                                                                     chunk_sizes[ idx*3+2 ],
                                                                     file_prefix ) );
                }
            }
        }
    }

    ~xxl_watershed_impl()
    {
        delete [] chunk_data_;
    }

    void process_chunk( xxl_chunk_ptr c )
    {
        const_value_ptr affinities = c->load_affinities();
        id_ptr          ids        = c->allocate_chunk();

        typename mmap_vector< count_type >::scope_cache cache_counts( c->counts( false ) );
        typename mmap_vector< edge_type  >::scope_cache cache_dendr ( c->dendr( false ) );

        chunk_quickie_impl< T, Id, Count >
            ( c->xdim(),
              c->ydim(),
              c->zdim(),
              affinities,
              hi_, low_, size_thold_, mst_,
              c->flags(),
              ids,
              cache_counts.get(),
              cache_dendr.get(),
              bind( &xxl_chunk::free_affinities, c.get() ));

        c->flush_chunk( true );
        c->count( cache_counts.get().size() );

        {
            mutex::guard g( mutex_ );
            std::cout << "Done chunk [ " << c->xpos() << ' '
                      << c->ypos() << ' ' << c->zpos() << " ] :: "
                      << "domains = "
                      << cache_counts.get().size() << " , "
                      << cache_dendr.get().size() << "\n"
                      << std::flush;
        }
    }

    void fetch_chunk_sizes( xxl_chunk_ptr c )
    {
        mmap_vector< count_type >& counts = c->counts( true );
        id_type offset = c->offset();

        {
            mutex::guard g( mutex_ );
            sizes_[ 0 ] += counts[ 0 ];
        }

        size_type tot_this = counts[ 0 ];

        for ( size_type i = 1; i < counts.size(); ++i )
        {
            sizes_[ i + offset ] = counts[ i ];
            tot_this += counts[ i ];
        }

        counts.clear();

        {
            mutex::guard g( mutex_ );
            ++num_ready_;

            if ( num_ready_ == size_ )
            {
                merge_at_.resize( num_domains_ );
                trees_.resize( num_domains_ );
                serial_tm_.start();
            }

            std::cout << "sizes of " << num_ready_ << " / "
                      << size_ << " saved (" << tot_this << ")\n";
        }
    }

    void merge_false_minima( xxl_chunk_ptr c )
    {
        mmap_vector< edge_type >& fmin = c->false_minima();

        FOR_EACH( it, fmin )
        {
            const id_type v1 = trees_.find_set( zi::get< 0 >( *it ) );
            const id_type v2 = trees_.find_set( zi::get< 1 >( *it ) );

            ZI_ASSERT( v1 && v2 );

            if ( v1 != v2 )
            {
                const id_type vr = trees_.join( v1, v2 );
                sizes_[ v1 ] += sizes_[ v2 ];
                sizes_[ v2 ]  = 0;

                merge_at_[ v1 ] = std::max( zi::get< 2 >( *it ),
                                            std::max( merge_at_[ v1 ], merge_at_[ v2 ] ));
                merge_at_[ v2 ] = 0;

                std::swap( sizes_[ vr ], sizes_[ v1 ] );
                std::swap( merge_at_[ vr ], merge_at_[ v1 ] );
            }
            else
            {
                merge_at_[ v1 ] = std::max( merge_at_[ v1 ], zi::get< 2 >( *it ) );

            }
        }

        fmin.clear();

        mmap_vector< id_pair >& pairs = c->merge_pairs();
        size_type cnt = 0;

        FOR_EACH( it, pairs )
        {
            const id_type v1 = trees_.find_set( zi::get< 0 >( *it ) );
            const id_type v2 = trees_.find_set( zi::get< 1 >( *it ) );

            ZI_ASSERT( v1 && v2 );

            if ( v1 != v2 )
            {
                const id_type vr = trees_.join( v1, v2 );
                sizes_[ v1 ] += sizes_[ v2 ];
                sizes_[ v2 ]  = 0;
                std::swap( sizes_[ vr ], sizes_[ v1 ] );

                merge_at_[vr] = std::max( merge_at_[v1], merge_at_[v2] );
            }
        }

        pairs.clear();

        std::cout << "Merged for prefix: " << c->prefix()
                  << ": " << cnt << " out of " << pairs.stored_size() << "\n";
    }

    void process_chunk_borders( xxl_chunk_ptr c )
    {
        value_type abs_min = std::min( low_, mst_ );

        const index deltax[3] = { 1, 0, 0 };
        const index deltay[3] = { 0, 1, 0 };
        const index deltaz[3] = { 0, 0, 1 };

        mmap_vector< edge_type >& false_minima = c->false_minima( false );
        edgemap_type              edges;
        unordered_set< id_pair >  all_same;

        std::vector< value_type > conn;
        std::vector< id_type >    a0, a1, b0, b1;

        for ( int i = 0; i < 3; ++i )
        {
            if ( c->pos( i ) > 0 )
            {
                xxl_chunk_ptr o = chunks_
                    [c->xpos()-deltax[i]]
                    [c->ypos()-deltay[i]]
                    [c->zpos()-deltaz[i]];

                c->get_border_affinities( i, conn );
                c->get_seg_faces( i, 1, b0, b1 );
                o->get_seg_faces( i, 0, a0, a1 );

                id_type aoff = o->offset();
                id_type boff = c->offset();

                ZI_ASSERT( a0.size() == a1.size() &&
                           b0.size() == b1.size() &&
                           a1.size() == b0.size() );

                for ( size_type idx = 0; idx < a0.size(); ++idx )
                {
                    if ( a0[idx] && b1[idx] )
                    {
                        if ( a1[idx] && b0[idx] )
                        {
                            false_minima.push_back( edge_type( aoff+a1[idx],
                                                               boff+b0[idx],
                                                               conn[idx] ));
                        }
                        else if ( a1[idx] )
                        {
                            all_same.insert( std::make_pair( aoff+a1[idx], boff+b1[idx] ));
                        }
                        else if ( b0[idx] )
                        {
                            all_same.insert( std::make_pair( aoff+a0[idx], boff+b0[idx] ));
                        }
                        else
                        {
                            id_pair x( aoff+a0[idx], boff+b1[idx] );
                            typename edgemap_type::iterator it = edges.find( x );
                            if ( it == edges.end() )
                            {
                                edges.insert( std::make_pair( x, conn[ idx ] ));
                            }
                            else
                            {
                                if ( it->second < conn[ idx ] )
                                {
                                    it->second = conn[ idx ];
                                }
                            }
                        }
                    }

                    ZI_ASSERT( !a1[idx] || a0[idx] );
                    ZI_ASSERT( !b0[idx] || b1[idx] );

                }
            }
        }

        mmap_vector< edge_type >& more_dendr  = c->more_dendr( false );
        more_dendr.resize( edges.size() );

        size_type idx = 0;
        FOR_EACH( it, edges )
        {
            more_dendr[ idx++ ] = tie( it->first.first, it->first.second, it->second );
        }

        zi::sort( more_dendr.begin(), more_dendr.end(), edge_compare< T, id_type > );
        more_dendr.flush();

        mmap_vector< id_pair >& merge_pairs = c->merge_pairs( false );
        merge_pairs.resize( all_same.size() );
        std::copy( all_same.begin(), all_same.end(), merge_pairs.begin() );
        merge_pairs.flush();

        false_minima.flush();

        {
            mutex::guard g( mutex_ );
            std::cout << "process_chunk_borders [ " << c->xpos() << " "
                      << c->ypos() << " " << c->zpos() << " ] : "
                      << merge_pairs.stored_size() << " : "
                      << more_dendr.stored_size() << "\n";
        }

        serial_tm_.push_back( zi::run_fn( bind( &type::merge_false_minima, this, c ) ) );
    }

    void update_chunk_seg( xxl_chunk_ptr c )
    {
        chunk_ptr cp = c->load_chunk();
        id_type *seg = cp->data();

        for ( size_type i = 0; i < cp->size(); ++i )
        {
            if ( seg[ i ] )
            {
                seg[ i ] = trees_.find_set( seg[ i ] + c->offset() );
            }
        }

        c->flush_chunk( false );
    }


    void update_chunk_data( xxl_chunk_ptr c )
    {
        id_type  off = c->offset();

        //dendr_type dendr, new_dendr;
        //c->get_dendr( dendr );
        mmap_vector< edge_type >& dendr = c->dendr();

        size_type idx = 0;
        for ( size_type i = 0; i < dendr.size(); ++i )
        {
            id_type a1 = order_[ zi::get< 0 >( dendr[ i ] ) + off ];
            id_type a2 = order_[ zi::get< 1 >( dendr[ i ] ) + off ];

            if ( a1 != a2 && a1 && a2 )
            {
                dendr[ idx++ ] = tie( a1, a2, zi::get< 2 >( dendr[ i ] ) );
            }
        }

        dendr.resize( idx );

        mmap_vector< edge_type >& more_dendr = c->more_dendr();

        FOR_EACH( it, more_dendr )
        {
            id_type a1 = order_[ zi::get< 0 >( *it ) ];
            id_type a2 = order_[ zi::get< 1 >( *it ) ];

            if ( a1 != a2 && a1 && a2 )
            {
                dendr.push_back( tie( a1, a2, zi::get< 2 >( *it ) ));
            }
        }

        dendr.flush();

        more_dendr.clear();

        chunk_ptr cp = c->load_chunk();
        id_type *seg = cp->data();

        for ( size_type i = 0; i < cp->size(); ++i )
        {
            if ( seg[ i ] )
            {
                seg[ i ] = order_[ seg[ i ] + off ];
            }
        }

        c->flush_chunk( false );
    }

    void run( size_type num_threads = 1 )
    {
        task_manager::simple tm( num_threads );
        tm.start();

        for ( size_type i = 0; i < size_; ++i )
        {
            tm.push_back( bind( &type::process_chunk, this, chunks_.at( i ) ));
        }

        tm.join();

        num_domains_ = 0;
        for ( size_type i = 0; i < size_; ++i )
        {
            chunks_.at( i )->offset( num_domains_ );
            num_domains_ += chunks_.at( i )->count() - 1;
        }

        ++num_domains_;

        std::cout << "Total domains: " << num_domains_ << "\n";
        sizes_.resize( num_domains_ );

        tm.start();
        for ( size_type i = 0; i < size_; ++i )
        {
            tm.push_back( bind( &type::fetch_chunk_sizes,
                                    this,
                                    chunks_.at( i ) ) );
        }



        for ( size_type i = 0; i < size_; ++i )
        {
            tm.push_back( bind( &type::process_chunk_borders,
                                    this,
                                    chunks_.at( i ) ) );
        }

        tm.join();

        for ( size_type i = 0; i < size_; ++i )
        {
            //serial_tm_.push_back( zi::run_fn( bind( &type::merge_pairs, this, chunks_.at( i ) )));
        }

        serial_tm_.join();


        for( size_type i = 0; i < merge_at_.size(); ++i )
        {
            if ( merge_at_[ i ] == 0 )
            {
                merge_at_[ i ] = std::numeric_limits< value_type >::max();
            }
        }

        size_type expecxt_no = 0;

        for ( size_type i = 0; i < size_; ++i )
        {
            xxl_chunk_ptr c = chunks_.at( i );

            if ( c->dendr( false ).stored_size() )
            {
                queue_.add_source( c->prefix() + ".dendr.tmp",
                                   c->dendr( false ).stored_size(),
                                   c->offset() );
                expecxt_no += c->dendr( false ).stored_size();
            }

            if ( c->more_dendr( false ).stored_size() )
            {
                queue_.add_source( c->prefix() + ".more_dendr.tmp",
                                   c->more_dendr( false ).stored_size(), 0 );
                expecxt_no += c->more_dendr( false ).stored_size();
            }

            std::cout << c->more_dendr( false ).stored_size() << "\n";
        }

        this_thread::sleep( 213 );

        std::cout << "QUEUE SIZE: " << queue_.empty() << "\n";

        size_type actual_no = 0;
        //
        // apply size threshold
        //

        while ( !queue_.empty() )
        {
            ++actual_no;

            if ( zi::get< 2 >( queue_.top() ) < mst_ )
            {
                break;
            }

            const id_type off = queue_.top_key();
            const id_type v1  = trees_.find_set( zi::get< 0 >( queue_.top() ) + off );
            const id_type v2  = trees_.find_set( zi::get< 1 >( queue_.top() ) + off );

            ZI_ASSERT( v1 && v2 );

            if ( v1 != v2 )
            {
                if ( merge_at_[ v1 ] <= zi::get< 2 >( queue_.top() ) ||
                     merge_at_[ v2 ] <= zi::get< 2 >( queue_.top() ) ||
                     ( zi::get< 2 >( queue_.top()) >= mst_ &&
                       ( sizes_[ v1 ] < size_thold_ || sizes_[ v2 ] < size_thold_ ) ))
                {
                    const id_type vr = trees_.join( v1, v2 );
                    sizes_[ v1 ] += sizes_[ v2 ];
                    sizes_[ v2 ]  = 0;
                    std::swap( sizes_[ vr ], sizes_[ v1 ] );

                    merge_at_[ vr ] = std::max( merge_at_[ v1 ], merge_at_[ v2 ] );
                }
            }
            queue_.pop();
        }

        /*tm.start();

        for ( size_type i = 0; i < size_; ++i )
        {
            tm.push_back( zi::run_fn( bind( &type::update_chunk_seg,
                                                this,
                                                chunks_.at( i ) ) ) );
        }


        tm.join();*/

        order_.resize( num_domains_ );

        id_type new_index = 1;
        for ( id_type i = 1; i < num_domains_; ++i )
        {
            if ( sizes_[ i ] )
            {
                if ( sizes_[ i ] >= size_thold_ )
                {
                    sizes_[ new_index ] = sizes_[ i ];
                    order_[ i ] = new_index;
                    ++new_index;
                }
                else
                {
                    sizes_[ 0 ] += sizes_[ i ];
                    sizes_[ i ]  = order_[ i ] = 0;
                }
            }
        }

        for ( id_type i = 1; i < num_domains_; ++i )
        {
            if ( !order_[ i ] )
            {
                order_[ i ] = order_[ trees_.find_set( i ) ];
            }
        }

        std::cout << "zero order: " << order_[0] << "\n";

        trees_.resize( new_index );
        sizes_.resize( new_index );

        size_type tot = 0;
        FOR_EACH( it, sizes_ )
        {
            tot += *it;
        }

        mem::mmap_file::write_n( file_prefix_ + ".all_sizes",
                                 sizes_.begin(),
                                 new_index );
        sizes_.clear();

        std::cout << "Total size: " << new_index << " / " << tot << "\n";

        tm.start();

        for ( size_type i = 0; i < size_; ++i )
        {
            tm.push_back( bind( &type::update_chunk_data, this, chunks_.at( i ) ));
        }

        tm.join();

        std::fill( sizes_.begin(), sizes_.end(), 0 );

        queue_.clear();

        for ( size_type i = 0; i < size_; ++i )
        {
            xxl_chunk_ptr c = chunks_.at( i );
            if ( c->dendr( false ).stored_size() )
            {
                queue_.add_source( c->prefix() + ".dendr.tmp",
                                   c->dendr( false ).stored_size(), 0 );
            }
        }

        id_type round = 0;

        detail::mmap_ostream< edge_type > dend_out( file_prefix_ + ".dend" );


        while ( !queue_.empty() )
        {
            if ( zi::get< 2 >( queue_.top() ) < low_ )
            {
                break;
            }

            const id_type v1  = trees_.find_set( zi::get< 0 >( queue_.top() ) );
            const id_type v2  = trees_.find_set( zi::get< 1 >( queue_.top() ) );

            ZI_ASSERT( v1 && v2 );

            if ( v1 != v2 )
            {
                dend_out << queue_.top();
                round += sizeof( edge_type );
                sizes_[ v1 ] = sizes_[ v2 ] = round;
                trees_.join( v1, v2 );
            }

            queue_.pop();
        }

        std::cout << "FINAL DEND SIZE: " << round << "\n";
        std::cout << "AA? " << sizeof( edge_type ) << "\n";


    }

};

void test( const std::string& filename )
{

    std::cout << "ASDASD: " << sizeof( tuple< int, int, int > ) << "\n";

    detail::watershed_task ws;
    std::string f = filename + ".metadata";
    mem::mmap_file::get( f, ws );
    std::cout << ws.affinity_bitsize << "\n";
    std::cout << ws.id_bitsize << "\n";
    std::cout << ws.x << "\n";
    std::cout << ws.y << "\n";
    std::cout << ws.z << "\n";

    size_type total = ws.x * ws.y * ws.z;

    chunk_dimensions chunk_sizes[ total ];

    f = filename + ".chunksizes";
    mem::mmap_file::read( f, chunk_sizes, total * 12 );

    //xxl_watershed_impl< float, int32_t, int32_t > xxlws
    //( filename, ws.x, ws.y, ws.z, 0.98, 0.3, 0.3, 40 );

    //xxlws.run( 3 );
    xxl::watershed_impl< float, int32_t, int32_t >
        newws( filename, 0.98, 0.3, 40, 0.3, ws.x, ws.y, ws.z,
               chunk_sizes, 4 );

    newws();




    //int32_t chunk_sizes[ size_ * 3 ];
    //mem::mmap_file::read( file_prefix_ + ".chunksizes", chunk_sizes, total * 12 );


}

} // namespace watershed
} // namespace zi

#endif
