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

#ifndef ZI_WATERSHED_XXL_CHUNK_HPP
#define ZI_WATERSHED_XXL_CHUNK_HPP 1

#include <zi/watershed/xxl/traits.hpp>
#include <zi/watershed/detail/volume_ref.hpp>
#include <zi/watershed/detail/types.hpp>
#include <zi/watershed/detail/mmap_file.hpp>

#include <zi/utility/string_printf.hpp>

namespace zi {
namespace watershed {

namespace xxl {

template< class T, class Id, class Count >
class xxl_chunk: public watershed_traits, non_copyable
{
private:

    const size_type   pos_[ 3 ];
    const size_type   dim_[ 3 ];
    const int         flags_ ;
    const std::string prefix_;

    value_ptr            affinity_data_ ;
    affinity_volume_ref  affinity_      ;

    id_ptr               seg_data_      ;
    id_volume_ref        seg_           ;

    count_type   count_ ;
    id_type      offset_;

    mmap_vector< edge_type > dendr_           ;
    mmap_vector< edge_type > dendr_more_      ;
    mmap_vector< id_pair   > merged_pairs_    ;
    mmap_vector< edge_type > merged_plateaus_ ;

public:

    xxl_chunk( size_type xpos,
               size_type ypos,
               size_type zpos,
               size_type xdim,
               size_type ydim,
               size_type zdim,
               int       flags,
               const std::string& prefix )
        : pos_(),
          dim_(),
          flags_( flags ),
          prefix_( prefix + string_printf( ".%.3d.%.3d.%.3d", xpos, ypos, zpos ) ),
          affinity_data_( 0 ),
          affinity_( affinity_data_, extents[xdim][ydim][zdim][3] ),
          seg_data_( 0 ),
          seg_( seg_data_, extents[xdim][ydim][zdim] ),
          count_( 0 ),
          offset_( 0 ),
          dendr_( prefix_ + ".dendr.tmp" ),
          dendr_more( prefix_ + ".dendr_more.tmp" ),
          merged_pairs_( prefix_ + ".pairs.tmp" ),
          merged_plateaus_( prefix_ + ".plateaus.tmp" )
    {
        pos_[ 0 ] = xpos;
        pos_[ 1 ] = ypos;
        pos_[ 2 ] = zpos;
        dim_[ 0 ] = xdim;
        dim_[ 1 ] = ydim;
        dim_[ 2 ] = zdim;
    }

    const size_type xpos() const
    {
        return pos_[ 0 ];
    }

    const size_type ypos() const
    {
        return pos_[ 1 ];
    }

    const size_type zpos() const
    {
        return pos_[ 2 ];
    }

    const size_type xdim() const
    {
        return dim_[ 0 ];
    }

    const size_type ydim() const
    {
        return dim_[ 1 ];
    }

    const size_type zdim() const
    {
        return dim_[ 2 ];
    }

    const size_type pos( const size_type i ) const
    {
        return pos_[ i ];
    }

    const size_type dim( const size_type i ) const
    {
        return dim_[ i ];
    }

    const int flags() const
    {
        return flags_;
    }

    const id_type count() const
    {
        return count_;
    }

    const id_type offset() const
    {
        return offset_;
    }

    void offset( id_type o )
    {
        offset_ = o;
    }

    mmap_vector< edge_type >& dendr( bool reload = true )
    {
        if ( reload )
        {
            return dendr_.reload();
        }
        return dendr_;
    }

    mmap_vector< edge_type > dendr_more( bool reload = true )
    {
        if ( reload )
        {
            return dendr_more_.reload();
        }
        return dendr_more_;
    }

    mmap_vector< id_pair > merged_pairs( bool reload = true )
    {
        if ( reload )
        {
            return merged_pairs_.reload();
        }
        return merged_pairs_;
    }

    mmap_vector< edge_type > merged_plateaus( bool reload = true )
    {
        if ( reload )
        {
            return merged_plateaus_.reload();
        }
        return merged_plateaus_;
    }


    conn_ptr conn()
    {
        return conn_;
    }

    void save_border_affinities()
    {
        std::vector< affinity_type > v;

        conn_->xslice( 1, 0, v );
        mmap_file::write( prefix_ + ".affinity.0.tmp", v.begin(), v.end() );

        conn_->yslice( 1, 1, v );
        mmap_file::write( prefix_ + ".affinity.1.tmp", v.begin(), v.end() );

        conn_->zslice( 1, 2, v );
        mmap_file::write( prefix_ + ".affinity.2.tmp", v.begin(), v.end() );
    }

    void get_border_affinities( int orientation, std::vector< affinity_type >& v )
    {
        size_type  len = 0;
        std::string  filename = prefix_;

        switch ( orientation )
        {
        case 0:
            len       = chunk_->y() * chunk_->z();
            filename += ".affinity.0.tmp";
            break;
        case 1:
            len       = chunk_->x() * chunk_->z();
            filename += ".affinity.1.tmp";
            break;
        case 2:
            len       = chunk_->x() * chunk_->y();
            filename += ".affinity.2.tmp";
            break;
        default:
            return;
        }

        v.resize( len );
        mmap_file::read_n( filename, v.begin(), len );
    }

    void flush_chunk( bool save_borders = false )
    {
        mmap_file::write( prefix_ + ".seg",
                               chunk_->data(),
                               chunk_->size() * sizeof( id_type ) );

        if ( save_borders )
        {
            std::vector< id_type > v;

            chunk_->xslice( 0, v );
            mmap_file::write( prefix_ + ".fsegx0", v.begin(), v.end() );
            chunk_->xslice( 1, v );
            mmap_file::write( prefix_ + ".fsegx1", v.begin(), v.end() );

            chunk_->yslice( 0, v );
            mmap_file::write( prefix_ + ".fsegy0", v.begin(), v.end() );
            chunk_->yslice( 1, v );
            mmap_file::write( prefix_ + ".fsegy1", v.begin(), v.end() );

            chunk_->zslice( 0, v );
            mmap_file::write( prefix_ + ".fsegz0", v.begin(), v.end() );
            chunk_->zslice( 1, v );
            mmap_file::write( prefix_ + ".fsegz1", v.begin(), v.end() );

            chunk_->xslice( chunk_->x() - 2, v );
            mmap_file::write( prefix_ + ".bsegx0", v.begin(), v.end() );
            chunk_->xslice( chunk_->x() - 1, v );
            mmap_file::write( prefix_ + ".bsegx1", v.begin(), v.end() );

            chunk_->yslice( chunk_->y() - 2, v );
            mmap_file::write( prefix_ + ".bsegy0", v.begin(), v.end() );
            chunk_->yslice( chunk_->y() - 1, v );
            mmap_file::write( prefix_ + ".bsegy1", v.begin(), v.end() );

            chunk_->zslice( chunk_->z() - 2, v );
            mmap_file::write( prefix_ + ".bsegz0", v.begin(), v.end() );
            chunk_->zslice( chunk_->z() - 1, v );
            mmap_file::write( prefix_ + ".bsegz1", v.begin(), v.end() );
        }
    }

    void get_seg_faces( int orientation,
                        bool is_front,
                        std::vector< id_type >& v0,
                        std::vector< id_type >& v1 )
    {
        size_type  len = 0;
        std::string  filename = prefix_;

        if ( is_front )
        {
            filename += ".f";
        }
        else
        {
            filename += ".b";
        }

        switch ( orientation )
        {
        case 0:
            len       = chunk_->y() * chunk_->z();
            filename += "segx";
            break;
        case 1:
            len       = chunk_->x() * chunk_->z();
            filename += "segy";
            break;
        case 2:
            len       = chunk_->x() * chunk_->y();
            filename += "segz";
            break;
        default:
            return;
        }

        v0.resize( len );
        v1.resize( len );
        mmap_file::read_n( filename + "0", v0.begin(), len );
        mmap_file::read_n( filename + "1", v1.begin(), len );
    }

    void save_sizes( const std::vector< sizes_type >& v )
    {
        count_ = v.size();
        mmap_file::write( prefix_ + ".sizes", v.begin(), v.end() );
    }

    void save_dendr( const dendr_type& v )
    {
        own_dendr_size_ = v.size();
        mmap_file::write( prefix_ + ".dendr", v.begin(), v.end() );
    }

    void save_merge_graph( const dendr_type& v )
    {
        merge_graph_size_ = v.size();
        mmap_file::write( prefix_ + ".merge_graph", v.begin(), v.end() );
    }

    void save_merge_pairs( const unordered_set< id_pair >& v )
    {
        merge_pairs_size_ = v.size();
        mmap_file::write_n( prefix_ + ".merge_pairs", v.begin(), v.size() );
    }

    void get_sizes( std::vector< sizes_type >& v )
    {
        if ( count_ )
        {
            size_type cnt = count_;
            v.resize( cnt );
            mmap_file::read_n( prefix_ + ".sizes", v.begin(), cnt );
        }
    }

    void get_dendr( dendr_type& v )
    {
        if ( own_dendr_size_ )
        {
            v.resize( own_dendr_size_ );
            mmap_file::read_n( prefix_ + ".dendr", v.begin(), own_dendr_size_ );
        }
    }

    void get_merge_graph( dendr_type& v )
    {
        if ( merge_graph_size_ )
        {
            v.resize( merge_graph_size_ );
            mmap_file::read_n( prefix_ + ".merge_graph", v.begin(), merge_graph_size_ );
        }
    }

    void get_merge_pairs( std::vector< id_pair >& v )
    {
        if ( merge_graph_size_ )
        {
            v.resize( merge_pairs_size_ );
            mmap_file::read_n( prefix_ + ".merge_pairs", v.begin(), v.size() );
        }
    }

    const std::string& prefix() const
    {
        return prefix_;
    }

    chunk_ptr get()
    {
        return chunk_;
    }

    chunk_ptr load_chunk()
    {
        chunk_->allocate();
        mmap_file::read( prefix_ + ".seg",
                         chunk_->data(),
                         chunk_->size() * sizeof( id_type ) );
        return chunk_;
    }

    const size_type merge_graph_size() const
    {
        return merge_graph_size_;
    }

    const size_type merge_pairs_size() const
    {
        return merge_pairs_size_;
    }

    const size_type dendr_size() const
    {
        return own_dendr_size_;
    }

};

} // namespace detail

} // namespace watershed
} // namespace zi

#endif
