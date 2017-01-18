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

#ifndef ZI_WATERSHED_XXL_WATERSHED_HPP
#define ZI_WATERSHED_XXL_WATERSHED_HPP 1

#include <zi/watershed/detail/config.hpp>
#include <zi/watershed/xxl/base.hpp>
#include <zi/watershed/xxl/chunk_dimensions.hpp>
#include <zi/watershed/quickie/quickie_impl.hpp>
#include <zi/watershed/detail/xxl_chunk.hpp>
#include <zi/watershed/detail/xxl_queue.hpp>

#include <zi/watershed/detail/mmap_file.hpp>
#include <zi/watershed/detail/mmap_vector.hpp>
#include <zi/watershed/detail/mmap_ostream.hpp>

#include <zi/bits/shared_ptr.hpp>

#include <zi/graph/biconnected_components.hpp>

#include <limits>
#include <list>
#include <algorithm>

namespace zi {
namespace watershed {

namespace xxl {

template< class T, class Id, class Count >
class watershed_impl:
        watershed_base< T, Id, Count, shared_ptr< detail::xxl_chunk< T, Id, Count > > >
{
public:
    typedef watershed_base< T, Id, Count, shared_ptr< detail::xxl_chunk< T, Id, Count > > > super_type;
    typedef watershed_impl< T, Id, Count > this_type;

    typedef detail::xxl_chunk< T, Id, Count > xxl_chunk_type;
    typedef shared_ptr< xxl_chunk_type >      chunk_type;

    typedef typename super_type::value_type value_type;
    typedef typename super_type::const_value_type const_value_type;
    typedef typename super_type::value_ptr value_ptr;
    typedef typename super_type::const_value_ptr const_value_ptr;

    typedef typename super_type::id_type id_type;
    typedef typename super_type::const_id_type const_id_type;
    typedef typename super_type::id_ptr id_ptr;
    typedef typename super_type::const_id_ptr const_id_ptr;

    typedef typename super_type::count_type count_type;
    typedef typename super_type::const_count_type const_count_type;

    typedef typename super_type::affinity_volume_ref affinity_volume_ref;
    typedef typename super_type::id_volume_ref id_volume_ref;

    typedef typename super_type::id_pair id_pair;
    typedef typename super_type::edge_type edge_type;
    typedef typename super_type::count_vector count_vector;

    typedef typename super_type::edge_vector edge_vector;
    typedef typename super_type::id_pair_vector id_pair_vector;
    typedef typename super_type::edge_map edge_map;

    typedef unordered_map< id_pair, value_type > edgemap_type;


protected:
    task_manager::simple task_manager_;
    task_manager::simple serial_task_manager_;

    std::vector< count_type > sizes_      ;
    disjoint_sets< id_type  > sets_       ;
    std::vector< value_type > merge_at_   ;
    std::vector< id_type    > reorder_    ;

    size_type tasks_done_                 ;
    id_type   total_num_domains_          ;

    bool verbose_;

private:
    void run_all( void (this_type::*fn_ptr)(chunk_type) )
    {
        tasks_done_ = 0;
        task_manager_.start();
        for ( size_type i = 0; i < super_type::size(); ++i )
        {
            task_manager_.push_back(zi::bind( fn_ptr, this, super_type::chunks_.at( i )));
        }
        task_manager_.join();
    }

public:
    watershed_impl( const std::string& filename,
                    const value_type&  high_threshold,
                    const value_type&  low_threshold,
                    const count_type&  dust_threshold,
                    const value_type&  dust_merge_threshold,
                    size_type xsize,
                    size_type ysize,
                    size_type zsize,
                    const chunk_dimensions* cdims,
                    size_type num_threads = 2,
                    bool verbose = true )
        : super_type( filename, high_threshold, low_threshold, dust_threshold,
                      dust_merge_threshold, xsize, ysize, zsize ),
          task_manager_( num_threads ),
          serial_task_manager_( 1 ),
          sizes_(),
          sets_(),
          merge_at_(),
          reorder_(),
          tasks_done_(0),
          total_num_domains_(0),
          verbose_( verbose || detail::ws_always_verbose() )
    {
        std::string aff_file_name = filename + ".affinity.data";
        shared_ptr< boost::interprocess::file_mapping > aff_file
            ( new boost::interprocess::file_mapping
              ( aff_file_name.c_str(),
                boost::interprocess::read_only ));

        size_type foff = 0;

        for ( size_type x = 0, idx = 0; x < super_type::xdim(); ++x )
            for ( size_type y = 0; y < super_type::ydim(); ++y )
                for ( size_type z = 0; z < super_type::zdim(); ++z, ++idx )
                {
                    int flags = 0;
                    flags |= x == 0 ? 0 : border::before_x;
                    flags |= y == 0 ? 0 : border::before_y;
                    flags |= z == 0 ? 0 : border::before_z;
                    flags |= x == super_type::xdim() - 1 ? 0 : border::after_x;
                    flags |= y == super_type::ydim() - 1 ? 0 : border::after_y;
                    flags |= z == super_type::zdim() - 1 ? 0 : border::after_z;

                    super_type::chunk( x, y, z ) = chunk_type
                        ( new xxl_chunk_type( x, y, z, flags,
                                              cdims[idx].x(),
                                              cdims[idx].y(),
                                              cdims[idx].z(),
                                              filename,
                                              aff_file,
                                              foff * sizeof( T ) * 3 ));

                    foff += size_type( cdims[idx].x() ) * cdims[idx].y() * cdims[idx].z();
                }

    }


    void initial_chunk_quickie( chunk_type c )
    {
        const_value_ptr affinities = c->load_affinities();
        id_ptr          ids        = c->allocate_chunk();

        mmap_vector< count_type >& counts     = c->counts();
        mmap_vector< edge_type  >& dendrogram = c->dendr();

        chunk_quickie_impl< T, Id, Count >
            ( c->xdim(), c->ydim(), c->zdim(),
              affinities,
              super_type::high_threshold(),
              super_type::low_threshold(),
              super_type::dust_threshold(),
              super_type::dust_merge_threshold(),
              c->flags(),
              ids,
              counts,
              dendrogram );

        c->flush_chunk( true );
        c->count( counts.size() );
        c->conn()->free_data();

        if ( verbose_ )
        {
            mutex::guard g( super_type::mutex_ );
            std::cout << "\rQuickieWS on chunk " << (*c)
                      << " domains: " << counts.size()
                      << " dendr: " << dendrogram.size() << '\n'
                      << "Initial per chunk Watershed " << (++tasks_done_) << '/'
                      << super_type::size() << std::flush;
        }
        counts.flush();
        dendrogram.flush();
    }

    void get_chunk_sizes( chunk_type c )
    {
        mmap_vector< count_type >& counts = c->counts( true );
        id_type offset = c->offset();

        {
            mutex::guard g( super_type::mutex_ );
            sizes_[ 0 ] += counts[ 0 ];
        }

        for ( size_type i = 1; i < counts.size(); ++i )
        {
            sizes_[ i + offset ] = counts[ i ];
        }

        counts.clear();
    }

    void process_chunk_borders( chunk_type c )
    {
        const index x = c->xpos();
        const index y = c->ypos();
        const index z = c->zpos();
        const index dx[6] = { 1, 0, 0, -1, 0, 0 };
        const index dy[6] = { 0, 1, 0, 0, -1, 0 };
        const index dz[6] = { 0, 0, 1, 0, 0, -1 };

        edgemap_type              edges;
        unordered_set< id_pair >  same;

        std::vector< value_type > conn;
        std::vector< id_type >    a0, b0, a1, b1;

        const value_type low_threshold  = super_type::low_threshold();
        const value_type min_threshold  = super_type::dust_merge_threshold();
        const value_type high_threshold = super_type::high_threshold();

        for ( int i = 0; i < 6; ++i )
        {
            if ( x+dx[i] >= 0 && y+dy[i] >= 0 && z+dz[i] >= 0 &&
                 x+dx[i] < static_cast<index>(super_type::xdim()) &&
                 y+dy[i] < static_cast<index>(super_type::ydim()) &&
                 z+dz[i] < static_cast<index>(super_type::zdim()) )
            {

                chunk_type o = super_type::chunks_[x+dx[i]][y+dy[i]][z+dz[i]];

                if ( i < 3 )
                {
                    o->get_border_affinities( i, conn );
                    c->get_seg_faces( i, 0, a0, a1 );
                    o->get_seg_faces( i, 1, b0, b1 );
                }
                else
                {
                    c->get_border_affinities( i-3, conn );
                    c->get_seg_faces( i-3, 1, a1, a0 );
                    o->get_seg_faces( i-3, 0, b1, b0 );
                }


                id_type aoff = c->offset();
                id_type boff = o->offset();

                ZI_ASSERT(a0.size()==b0.size()&&a1.size()==a0.size()&&b1.size()==a0.size());

                for ( size_type idx = 0; idx < a0.size(); ++idx )
                {
                    if ( a0[idx] && b1[idx] )
                    {
                        bool needs_an_edge = false;
                        id_pair xp( aoff+a0[idx], boff+b1[idx] );

                        if ( conn[idx] >= low_threshold )
                        {
                            needs_an_edge=true;
                            if ( a1[idx] )
                            {
                                ZI_ASSERT(a0[idx]==a1[idx]);
                                if ( conn[idx] >= high_threshold )
                                {
                                    ZI_ASSERT(b0[idx]==b1[idx]);
                                    needs_an_edge=false;
                                    if ( i < 3 )
                                    {
                                        same.insert(xp);
                                    }
                                }
                                else
                                {
                                    ZI_ASSERT(merge_at_[xp.first]==high_threshold ||
                                              merge_at_[xp.first]==conn[idx]);
                                    merge_at_[xp.first] = conn[idx];
                                }
                            }
                            else
                            {
                                ZI_ASSERT(b1[idx]||conn[idx]<high_threshold);
                            }
                        }
                        else if ( conn[idx] >= min_threshold )
                        {
                            needs_an_edge = true;
                        }

                        if ( needs_an_edge && ( i < 3 ) )
                        {
                            typename edgemap_type::iterator it = edges.find( xp );
                            if ( it == edges.end() )
                            {
                                ZI_ASSERT(conn[idx]<high_threshold);
                                edges.insert( std::make_pair( xp, conn[ idx ] ));
                            }
                            else if ( it->second < conn[ idx ] )
                            {
                                it->second = conn[ idx ];
                            }
                        }
                    }
                }
            }
        }

        mmap_vector< edge_type >& more_dendr = c->more_dendr();
        more_dendr.resize( edges.size() );

        size_type idx = 0;
        FOR_EACH( it, edges )
        {
            more_dendr[ idx++ ] = tie( it->first.first, it->first.second, it->second );
        }
        zi::sort( more_dendr.begin(), more_dendr.end(), edge_compare< T, id_type > );
        more_dendr.flush();

        mmap_vector< id_pair >& merge_pairs = c->merge_pairs();
        merge_pairs.resize( same.size() );
        idx = 0;
        FOR_EACH( it, same )
        {
            merge_pairs[ idx++ ] = (*it);
        }
        merge_pairs.flush();

        // we can add stuff to the serial manager
        serial_task_manager_.push_back( bind( &this_type::merge_chunk_borders, this, c ) );

        if ( verbose_ )
        {
            mutex::guard g( super_type::mutex_ );
            std::cout << "\rChunk " << (*c) << " border edges: "
                      << more_dendr.stored_size() << " merge pairs: "
                      << merge_pairs.stored_size() << "\n"
                      << "Processing chunk's lower borders " << (++tasks_done_) << '/'
                      << super_type::size() << std::flush;
        }

    }

    void merge_chunk_borders( chunk_type c )
    {
        mmap_vector< id_pair >& pairs = c->merge_pairs( true );
        const value_type high_threshold = super_type::high_threshold();

        FOR_EACH( it, pairs )
        {
            const id_type v1 = sets_.find_set( zi::get< 0 >( *it ) );
            const id_type v2 = sets_.find_set( zi::get< 1 >( *it ) );

            ZI_ASSERT(v1&&v2);

            if ( v1 != v2 )
            {
                const id_type vr = sets_.join( v1, v2 );
                sizes_[ v1 ] += sizes_[ v2 ];
                sizes_[ v2 ]  = 0;
                std::swap( sizes_[ vr ], sizes_[ v1 ] );
                ZI_ASSERT(merge_at_[v1]==high_threshold||merge_at_[v2]==high_threshold);
                merge_at_[vr] = high_threshold;
            }
        }
        pairs.clear();
    }

    void update_chunk_data( chunk_type c )
    {
        id_type off = c->offset();

        // rename the domains in the stored dendrogram(s)

        mmap_vector< edge_type >& dendr = c->dendr( true );
        size_type idx = 0;

        for ( size_type i = 0; i < dendr.size(); ++i )
        {
            id_type a1 = reorder_[ zi::get< 0 >( dendr[ i ] ) + off ];
            id_type a2 = reorder_[ zi::get< 1 >( dendr[ i ] ) + off ];

            if ( a1 && a2 && a1 != a2 )
            {
                dendr[ idx++ ] = tie( a1, a2, zi::get< 2 >( dendr[ i ] ));
            }
        }
        dendr.resize( idx );

        mmap_vector< edge_type >& more_dendr = c->more_dendr( true );

        FOR_EACH( it, more_dendr )
        {
            id_type a1 = reorder_[ zi::get< 0 >( *it ) ];
            id_type a2 = reorder_[ zi::get< 1 >( *it ) ];

            if ( a1 && a2 && a1 != a2 )
            {
                dendr.push_back( tie( a1, a2, zi::get< 2 >( *it ) ));
            }
        }

        zi::sort( dendr.begin(), dendr.end(), edge_compare< T, id_type > );
        dendr.flush();
        more_dendr.clear();

        c->load_chunk();
        id_ptr seg = c->get()->data();

        const size_type chunk_size = c->get()->size();

        for ( size_type i = 0; i < chunk_size; ++i )
        {
            if ( seg[ i ] )
            {
                seg[ i ] = reorder_[ seg[ i ] + off ];
            }
        }

        c->flush_chunk();

        if ( verbose_ )
        {
            mutex::guard g( super_type::mutex_ );
            std::cout << "\rFlushed chunk: " << (*c) << "\n"
                      << "Updating chunks' segmentation IDs "
                      << (++tasks_done_) << '/' << super_type::size()
                      << std::flush;
        }

    }


    void operator()( bool verbose = true )
    {
        verbose_ = verbose;

        // do quickie on all the chunks
        run_all( &this_type::initial_chunk_quickie );

        for ( size_type i = 0; i < super_type::size(); ++i )
        {
            super_type::chunks_.at( i )->offset( total_num_domains_ );
            total_num_domains_ += super_type::chunks_.at( i )->count() - 1;
        }

        ++total_num_domains_; // count zero (empty) as domain
        sizes_.resize( total_num_domains_ );

        // get the segment sizes
        run_all( &this_type::get_chunk_sizes );

        sets_.resize( total_num_domains_ );
        merge_at_.resize( total_num_domains_ );
        std::fill(merge_at_.begin(), merge_at_.end(), super_type::high_threshold());

        run_all( &this_type::process_chunk_borders );

        // the serial task manager has pending tasks already
        // we have to start and join it
        serial_task_manager_.start();
        serial_task_manager_.join();

        // now we have to apply the size threshold, and perform the
        // merges that we have to do because of the chunking
        // we do fix the missing mergers first

        detail::xxl_queue< edge_type, edge_less<value_type,id_type> > queue;

        for ( size_type i = 0; i < super_type::size(); ++i )
        {
            chunk_type c = super_type::chunks_.at( i );
            queue.add_source( c->dendr().filename(), c->dendr().stored_size(), c->offset() );
            queue.add_source( c->more_dendr().filename(), c->more_dendr().stored_size(), 0 );
        }

        const value_type min_threshold  = super_type::dust_merge_threshold();
        const value_type low_threshold  = super_type::low_threshold();
        const value_type high_threshold = super_type::high_threshold();
        const count_type dust_threshold = super_type::dust_threshold();

        while ( !queue.empty() )
        {
            const id_type off = queue.top_key();
            const id_type v1  = sets_.find_set( zi::get< 0 >( queue.top() ) + off );
            const id_type v2  = sets_.find_set( zi::get< 1 >( queue.top() ) + off );

            ZI_ASSERT( v1 && v2 );
            if ( v1 != v2 )
            {
                const value_type& val = zi::get<2>(queue.top());
                ZI_ASSERT(val<high_threshold);

                if ( val < low_threshold )
                {
                    break;
                }

                if ( merge_at_[v1] == val || merge_at_[v2] == val )
                {
                    const id_type vr = sets_.join( v1, v2 );
                    sizes_[ v1 ] += sizes_[ v2 ];
                    sizes_[ v2 ]  = 0;
                    std::swap( sizes_[ vr ], sizes_[ v1 ] );
                    merge_at_[vr] = std::max( merge_at_[v1], merge_at_[v2] );
                    if ( merge_at_[vr] != val )
                    {
                        merge_at_[vr] = high_threshold;
                    }
                }
            }

            queue.pop();
        }

        queue.clear();

        // second pass through the queue to apply the size threshold
        // we might have done this during the first pass, but
        // we would have to maintain a messy data structure

        for ( size_type i = 0; i < super_type::size(); ++i )
        {
            chunk_type c = super_type::chunks_.at( i );
            queue.add_source( c->dendr().filename(), c->dendr().stored_size(), c->offset() );
            queue.add_source( c->more_dendr().filename(), c->more_dendr().stored_size(), 0 );
        }

        while ( !queue.empty() )
        {
            const id_type off     = queue.top_key();
            const id_type v1      = sets_.find_set( zi::get< 0 >( queue.top() ) + off );
            const id_type v2      = sets_.find_set( zi::get< 1 >( queue.top() ) + off );
            const value_type& val = zi::get< 2 >( queue.top() );

            ZI_ASSERT( v1 && v2 );

            if ( val < min_threshold )
            {
                break;
            }

            if ( v1 != v2 )
            {

                if ( sizes_[v1] < dust_threshold || sizes_[v2] < dust_threshold )
                {
                    const id_type vr = sets_.join( v1, v2 );
                    sizes_[ v1 ] += sizes_[ v2 ];
                    sizes_[ v2 ]  = 0;
                    std::swap( sizes_[ vr ], sizes_[ v1 ] );
                }
            }

            queue.pop();
        }

        queue.clear();

        // renumber the domains, to save some space before the final processing
        // we don't need the merge_at_ vector

        merge_at_.clear();
        reorder_.resize( total_num_domains_ );

        id_type new_index = 1;
        for ( id_type i = 1; i < total_num_domains_; ++i )
        {
            if ( sizes_[ i ] )
            {
                ZI_ASSERT(sets_.find_set(i)==static_cast<id_type>(i));

                if ( sizes_[ i ] >= dust_threshold )
                {
                    sizes_[ new_index ] = sizes_[ i ];
                    reorder_[ i ] = new_index;
                    ++new_index;
                }
                else
                {
                    sizes_[ 0 ] += sizes_[ i ];
                    sizes_[ i ]  = reorder_[ i ] = 0;
                }
            }
        }

        for ( id_type i = 1; i < total_num_domains_; ++i )
        {
            if ( !reorder_[ i ] )
            {
                reorder_[ i ] = reorder_[ sets_.find_set( i ) ];
            }
        }

        sets_.resize( new_index );
        sizes_.resize( new_index );

        mmap_file::write_n( super_type::filename() + ".sizes", sizes_.begin(), new_index );

        // we know the total number of domains now!

        if ( verbose_ )
        {
            mutex::guard g( super_type::mutex_ );
            std::cout << "\nTotal number of domains: " << new_index << std::endl;
        }

        run_all( &this_type::update_chunk_data );

        for ( size_type i = 0; i < super_type::size(); ++i )
        {
            chunk_type c = super_type::chunks_.at( i );
            queue.add_source( c->dendr().filename(), c->dendr().stored_size() );
        }

        std::fill_n( sizes_.begin(), new_index, 0 );
        std::vector< std::list< id_type > > children( new_index );

        id_pair_vector dend_pairs;
        detail::mmap_ostream< value_type > dend_vals_out( super_type::filename() + ".dend_values" );

        while ( !queue.empty() )
        {
            const id_type a1  = zi::get< 0 >( queue.top() );
            const id_type a2  = zi::get< 1 >( queue.top() );

            ZI_ASSERT( a1 && a2 );

            const id_type v1  = sets_.find_set( a1 );
            const id_type v2  = sets_.find_set( a2 );

            ZI_ASSERT( v1 && v2 );

            if ( v1 != v2 )
            {
                dend_vals_out << zi::get< 2 >( queue.top() );
                dend_pairs.push_back( std::make_pair( a1, a2 ) );
                children[ a1 ].push_back( a2 );
                children[ a2 ].push_back( a1 );
                sets_.join( v1, v2 );
            }
            queue.pop();
        }

        queue.clear();
        sets_.resize( 0 );

        // todo: make singly linked lists
        id_type topologically_next = 0;

        for ( id_type i = 1; i < new_index; ++i )
        {
            if ( !sizes_[ i ] )
            {
                sizes_[ i ] = ++topologically_next;
            }

            FOR_EACH( it, children[ i ] )
            {
                if ( !sizes_[ *it ] )
                {
                    sizes_[ *it ] = ++topologically_next;
                    children[ i ].splice( children[ i ].end(), children[ *it ] );
                }
            }
        }

        detail::mmap_ostream< id_type > dend_pairs_out( super_type::filename() + ".dend_pairs" );


        FOR_EACH( it, dend_pairs )
        {
            if ( sizes_[ it->first ] > sizes_[ it->second ] )
            {
                dend_pairs_out << it->first;
                dend_pairs_out << it->second;
            }
            else
            {
                dend_pairs_out << it->second;
                dend_pairs_out << it->first;
            }
        }

        if ( verbose_ )
        {
            std::cout << "\nTotal dendrogram size: " << dend_pairs.size() << "\n";
        }


    }


};


} // namespace xxl

} // namespace watershed
} // namespace zi

#endif

