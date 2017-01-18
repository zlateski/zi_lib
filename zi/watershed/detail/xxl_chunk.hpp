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

#ifndef ZI_WATERSHED_DETAIL_XXL_CHUNK_HPP
#define ZI_WATERSHED_DETAIL_XXL_CHUNK_HPP 1

#include <zi/bits/cstdint.hpp>
#include <zi/bits/unordered_set.hpp>
#include <zi/bits/unordered_map.hpp>
#include <zi/bits/bind.hpp>
#include <zi/bits/shared_ptr.hpp>

#include <zi/watershed/detail/mmap_file.hpp>
#include <zi/watershed/detail/mmap_vector.hpp>
#include <zi/watershed/chunk.hpp>
#include <zi/watershed/affinity_matrix.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/rwmutex.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/string_printf.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <cstddef>
#include <iostream>
#include <vector>

namespace zi {
namespace watershed {

namespace detail {

template< class AffinityType, class IdType, class SizeType >
class xxl_chunk: non_copyable
{
public:
    typedef boost::interprocess::file_mapping  file_mapping;

    typedef SizeType                           count_type  ;
    typedef AffinityType                       value_type  ;
    typedef IdType                             id_type     ;
    typedef SizeType                           sizes_type  ;
    typedef chunk< IdType >                    chunk_type  ;
    typedef shared_ptr< chunk< IdType > >      chunk_ptr   ;
    typedef std::pair< IdType, IdType >        id_pair     ;
    typedef affinity_matrix< value_type >      conn_type   ;
    typedef shared_ptr< conn_type >            conn_ptr    ;

    typedef std::vector< std::pair< value_type, id_pair > > dendr_type;
    typedef unordered_map< id_pair, value_type >            edgemap_type;

private:

    const std::size_t xpos_  ;
    const std::size_t ypos_  ;
    const std::size_t zpos_  ;
    const int         flags_ ;
    const std::string prefix_;

    shared_ptr< file_mapping > connfile_;
    sizes_type connfoff_;

    id_type count_ ;
    id_type offset_;

    std::size_t own_dendr_size_   ;
    std::size_t merge_graph_size_ ;
    std::size_t merge_pairs_size_ ;

    chunk_ptr chunk_;
    conn_ptr  conn_ ;

    std::size_t pos_[ 3 ];

    mmap_vector< count_type > counts_;
    mmap_vector< tuple< id_type, id_type, value_type > > dendr_       ;
    mmap_vector< tuple< id_type, id_type, value_type > > more_dendr_  ;
    mmap_vector< tuple< id_type, id_type, value_type > > false_minima_;
    mmap_vector< id_pair >                               merge_pairs_ ;

public:

    xxl_chunk( std::size_t xpos,
               std::size_t ypos,
               std::size_t zpos,
               int         flags,
               std::size_t xw,
               std::size_t yw,
               std::size_t zw,
               const std::string& prefix,
               shared_ptr< file_mapping > connf,
               size_type connoff )
        : xpos_( xpos ),
          ypos_( ypos ),
          zpos_( zpos ),
          flags_( flags ),
          prefix_( prefix + string_printf( ".chunks/%d/%d/%d/", xpos, ypos, zpos ) ),
          connfile_( connf ),
          connfoff_( connoff ),
          count_( 0 ),
          offset_( 0 ),
          own_dendr_size_( 0 ),
          merge_graph_size_( 0 ),
          merge_pairs_size_( 0 ),

          chunk_( new chunk_type( xw, yw, zw, prefix_+".seg" ) ),
          conn_ ( new conn_type( xw, yw, zw, connf, connoff, false ) ),
          pos_(),
          counts_( prefix_ + "counts.tmp" ),
          dendr_( prefix_ + "dendr.tmp" ),
          more_dendr_( prefix_ + "more_dendr.tmp" ),
          false_minima_( prefix_ + "false_minima.tmp" ),
          merge_pairs_( prefix_ + "merge_pairs.tmp" )

    {
        pos_[ 0 ] = xpos_;
        pos_[ 1 ] = ypos_;
        pos_[ 2 ] = zpos_;
    }

    mmap_vector< tuple< id_type, id_type, value_type > >& dendr( bool reload = false )
    {
        if ( reload )
        {
            return dendr_.reload();
        }
        return dendr_;
    }

    mmap_vector< tuple< id_type, id_type, value_type > >& false_minima( bool reload = false )
    {
        if ( reload )
        {
            return false_minima_.reload();
        }
        return false_minima_;
    }

    mmap_vector< tuple< id_type, id_type, value_type > >& more_dendr( bool reload = false )
    {
        if ( reload )
        {
            return more_dendr_.reload();
        }
        return more_dendr_;
    }

    mmap_vector< id_pair >& merge_pairs( bool reload = false )
    {
        if ( reload )
        {
            return merge_pairs_.reload();
        }
        return merge_pairs_;
    }

    mmap_vector< count_type >& counts( bool reload = false )
    {
        if ( reload )
        {
            return counts_.reload();
        }
        return counts_;
    }

    std::size_t xpos() const
    {
        return xpos_;
    }

    std::size_t ypos() const
    {
        return ypos_;
    }

    std::size_t zpos() const
    {
        return zpos_;
    }

    std::size_t x() const
    {
        return chunk_->x();
    }

    std::size_t size() const
    {
        return x() * y() * z();
    }

    std::size_t y() const
    {
        return chunk_->y();
    }

    std::size_t z() const
    {
        return chunk_->z();
    }

    std::size_t xdim() const
    {
        return chunk_->x();
    }

    std::size_t ydim() const
    {
        return chunk_->y();
    }

    std::size_t zdim() const
    {
        return chunk_->z();
    }

    std::size_t pos( const std::size_t i ) const
    {
        return pos_[ i ];
    }

    int flags() const
    {
        return flags_;
    }

    const id_type count() const
    {
        return count_;
    }

    void count( id_type c )
    {
        count_ = c;
    }

    const id_type& offset() const
    {
        return offset_;
    }

    void offset( id_type o )
    {
        offset_ = o;
    }

    conn_ptr conn()
    {
        return conn_;
    }

    const value_type* load_affinities( bool save_borders = true )
    {
        conn()->load();
        if ( save_borders )
        {
            save_border_affinities();
        }
        return conn()->data();
    }

    void free_affinities()
    {
        //conn()->free_data();
    }

    void save_border_affinities()
    {
        std::vector< value_type > v;

        conn_->xslice( 1, 0, v );
        mmap_file::write( prefix_ + "0affinity.tmp", v.begin(), v.end() );

        conn_->yslice( 1, 1, v );
        mmap_file::write( prefix_ + "1affinity.tmp", v.begin(), v.end() );

        conn_->zslice( 1, 2, v );
        mmap_file::write( prefix_ + "2affinity.tmp", v.begin(), v.end() );
    }

    void get_border_affinities( int orientation, std::vector< value_type >& v )
    {
        std::size_t  len = 0;
        std::string  filename = prefix_;

        switch ( orientation )
        {
        case 0:
            len       = (chunk_->y()-2) * (chunk_->z()-2);
            filename += "0affinity.tmp";
            break;
        case 1:
            len       = (chunk_->x()-2) * (chunk_->z()-2);
            filename += "1affinity.tmp";
            break;
        case 2:
            len       = (chunk_->x()-2) * (chunk_->y()-2);
            filename += "2affinity.tmp";
            break;
        default:
            return;
        }

        v.resize( len );
        mmap_file::read_n( filename, v.begin(), len );
    }

    void flush_chunk( bool save_borders = false )
    {
        chunk_->flush();

        if ( save_borders )
        {
            std::vector< id_type > v;

            chunk_->xslice( 0, v );
            mmap_file::write( prefix_ + "fsegx0.tmp", v.begin(), v.end() );
            chunk_->xslice( 1, v );
            mmap_file::write( prefix_ + "fsegx1.tmp", v.begin(), v.end() );

            chunk_->yslice( 0, v );
            mmap_file::write( prefix_ + "fsegy0.tmp", v.begin(), v.end() );
            chunk_->yslice( 1, v );
            mmap_file::write( prefix_ + "fsegy1.tmp", v.begin(), v.end() );

            chunk_->zslice( 0, v );
            mmap_file::write( prefix_ + "fsegz0.tmp", v.begin(), v.end() );
            chunk_->zslice( 1, v );
            mmap_file::write( prefix_ + "fsegz1.tmp", v.begin(), v.end() );

            chunk_->xslice( chunk_->x() - 2, v );
            mmap_file::write( prefix_ + "bsegx0.tmp", v.begin(), v.end() );
            chunk_->xslice( chunk_->x() - 1, v );
            mmap_file::write( prefix_ + "bsegx1.tmp", v.begin(), v.end() );

            chunk_->yslice( chunk_->y() - 2, v );
            mmap_file::write( prefix_ + "bsegy0.tmp", v.begin(), v.end() );
            chunk_->yslice( chunk_->y() - 1, v );
            mmap_file::write( prefix_ + "bsegy1.tmp", v.begin(), v.end() );

            chunk_->zslice( chunk_->z() - 2, v );
            mmap_file::write( prefix_ + "bsegz0.tmp", v.begin(), v.end() );
            chunk_->zslice( chunk_->z() - 1, v );
            mmap_file::write( prefix_ + "bsegz1.tmp", v.begin(), v.end() );
        }

        chunk_->close();
    }

    void get_seg_faces( int orientation,
                        bool is_front,
                        std::vector< id_type >& v0,
                        std::vector< id_type >& v1 )
    {
        std::size_t  len = 0;
        std::string  filename = prefix_;

        if ( is_front )
        {
            filename += "f";
        }
        else
        {
            filename += "b";
        }

        switch ( orientation )
        {
        case 0:
            len       = (chunk_->y()-2) * (chunk_->z()-2);
            filename += "segx";
            break;
        case 1:
            len       = (chunk_->x()-2) * (chunk_->z()-2);
            filename += "segy";
            break;
        case 2:
            len       = (chunk_->x()-2) * (chunk_->y()-2);
            filename += "segz";
            break;
        default:
            return;
        }

        v0.resize( len );
        v1.resize( len );
        mmap_file::read_n( filename + "0.tmp", v0.begin(), len );
        mmap_file::read_n( filename + "1.tmp", v1.begin(), len );
    }

    void get_seg_face( int orientation,
                       bool is_front,
                       std::vector< id_type >& v0 )
    {
        std::size_t  len = 0;
        std::string  filename = prefix_;

        if ( is_front )
        {
            filename += "f";
        }
        else
        {
            filename += "b";
        }

        switch ( orientation )
        {
        case 0:
            len       = (chunk_->y()-2) * (chunk_->z()-2);
            filename += "segx";
            break;
        case 1:
            len       = (chunk_->x()-2) * (chunk_->z()-2);
            filename += "segy";
            break;
        case 2:
            len       = (chunk_->x()-2) * (chunk_->y()-2);
            filename += "segz";
            break;
        default:
            return;
        }

        v0.resize( len );
        if ( is_front )
        {
            mmap_file::read_n( filename + "1.tmp", v0.begin(), len );
        }
        else
        {
            mmap_file::read_n( filename + "0.tmp", v0.begin(), len );
        }
    }

    void save_sizes( const std::vector< sizes_type >& v )
    {
        count_ = v.size();
        mmap_file::write( prefix_ + "sizes.tmp", v.begin(), v.end() );
    }

    void save_dendr( const dendr_type& v )
    {
        own_dendr_size_ = v.size();
        mmap_file::write( prefix_ + "dendrx.tmp", v.begin(), v.end() );
    }

    void save_merge_graph( const dendr_type& v )
    {
        merge_graph_size_ = v.size();
        mmap_file::write( prefix_ + "merge_graph.tmp", v.begin(), v.end() );
    }

    void save_merge_pairs( const unordered_set< id_pair >& v )
    {
        merge_pairs_size_ = v.size();
        mmap_file::write_n( prefix_ + "merge_pairsx.tmp", v.begin(), v.size() );
    }

    void get_sizes( std::vector< sizes_type >& v )
    {
        if ( count_ )
        {
            std::size_t cnt = count_;
            v.resize( cnt );
            mmap_file::read_n( prefix_ + "sizes.tmp", v.begin(), cnt );
        }
    }

    void get_dendr( dendr_type& v )
    {
        if ( own_dendr_size_ )
        {
            v.resize( own_dendr_size_ );
            mmap_file::read_n( prefix_ + "dendrx.tmp", v.begin(), own_dendr_size_ );
        }
    }

    void get_merge_graph( dendr_type& v )
    {
        if ( merge_graph_size_ )
        {
            v.resize( merge_graph_size_ );
            mmap_file::read_n( prefix_ + "merge_graph.tmp", v.begin(), merge_graph_size_ );
        }
    }

    void get_merge_pairs( std::vector< id_pair >& v )
    {
        if ( merge_graph_size_ )
        {
            v.resize( merge_pairs_size_ );
            mmap_file::read_n( prefix_ + "merge_pairsx.tmp", v.begin(), v.size() );
        }
    }

    const std::string& prefix() const
    {
        return prefix_;
    }

    id_type* allocate_chunk()
    {
        return chunk_->get_data();
    }

    chunk_ptr get()
    {
        return chunk_;
    }

    chunk_ptr load_chunk()
    {
        chunk_->get_data();
        return chunk_;
    }

    std::size_t merge_graph_size() const
    {
        return merge_graph_size_;
    }

    std::size_t merge_pairs_size() const
    {
        return merge_pairs_size_;
    }

    std::size_t dendr_size() const
    {
        return own_dendr_size_;
    }

};

template< class T, class Id, class Count, class CharT, class Traits >
::std::basic_ostream< CharT, Traits >&
operator<<( ::std::basic_ostream< CharT, Traits >& os,
            const xxl_chunk< T, Id, Count >& c )
{
    os << '<' << c.xpos() << ','
       << c.ypos() << ',' << c.zpos() << '>'
       << " (" << c.xdim() << 'x' << c.ydim()
       << 'x' << c.zdim() << ")";

    return os;
}


} // namespace detail

} // namespace watershed
} // namespace zi

#endif
