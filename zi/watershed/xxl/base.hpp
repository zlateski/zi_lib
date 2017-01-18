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

#ifndef ZI_WATERSHED_XXL_BASE_HPP
#define ZI_WATERSHED_XXL_BASE_HPP 1

#include <zi/watershed/detail/types.hpp>
#include <zi/watershed/detail/utility.hpp>
#include <zi/watershed/detail/volume_ref.hpp>
#include <zi/watershed/detail/xxl_chunk.hpp>

#include <zi/concurrency/mutex.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/bits/shared_ptr.hpp>
#include <zi/bits/tuple.hpp>
#include <zi/bits/unordered_map.hpp>

#include <vector>

namespace zi {
namespace watershed {
namespace xxl {

template< class T, class Id, class Count, class Chunk >
class watershed_base: non_copyable
{
public:
    typedef T         value_type       ;
    typedef const T   const_value_type ;
    typedef T*        value_ptr        ;
    typedef const T*  const_value_ptr  ;

    typedef Id        id_type          ;
    typedef const Id  const_id_type    ;
    typedef Id*       id_ptr           ;
    typedef const Id* const_id_ptr     ;

    typedef Count        count_type      ;
    typedef const Count  const_count_type      ;

    typedef const_volume_ref< value_type, 4 > affinity_volume_ref;
    typedef volume_ref< id_type, 3 >          id_volume_ref;

    typedef std::pair< id_type, id_type > id_pair;
    typedef tuple< id_type, id_type, T >  edge_type;
    typedef std::vector< count_type >     count_vector;

    typedef std::vector< edge_type >      edge_vector;
    typedef std::vector< id_pair >        id_pair_vector;
    typedef unordered_map< id_pair, T >   edge_map;

    typedef Chunk                         chunk_type;
    typedef volume_ref< chunk_type, 3 >   chunk_cube;

protected:
    const std::string filename_             ;
    const T           high_threshold_       ;
    const T           low_threshold_        ;
    const Count       dust_threshold_       ;
    const T           dust_merge_threshold_ ;
    chunk_type*       raw_chunk_data_       ;
    chunk_cube        chunks_               ;
    size_type         size_                 ;
    mutex             mutex_                ;

public:
    watershed_base( const std::string& filename,
                    const value_type&  high_threshold,
                    const value_type&  low_threshold,
                    const count_type&  dust_threshold,
                    const value_type&  dust_merge_threshold,
                    size_type xsize,
                    size_type ysize,
                    size_type zsize )
        : filename_( filename ),
          high_threshold_( high_threshold ),
          low_threshold_( low_threshold ),
          dust_threshold_( dust_threshold ),
          dust_merge_threshold_( dust_merge_threshold ),
          raw_chunk_data_( new chunk_type[ xsize*ysize*zsize ] ),
          chunks_( raw_chunk_data_, extents[xsize][ysize][zsize] ),
          size_( xsize*ysize*zsize ),
          mutex_()
    { }

    watershed_base( const std::string& filename,
                    const value_type&  high_threshold,
                    const value_type&  low_threshold,
                    const count_type&  dust_threshold,
                    const value_type&  dust_merge_threshold,
                    const chunk_cube&  chunks )
        : filename_( filename ),
          high_threshold_( high_threshold ),
          low_threshold_( low_threshold ),
          dust_threshold_( dust_threshold ),
          dust_merge_threshold_( dust_merge_threshold ),
          chunks_( chunks ),
          size_( chunks.num_elements() ),
          mutex_()
    { }

    ~watershed_base()
    {
        if ( raw_chunk_data_ )
        {
            delete [] raw_chunk_data_;
        }
    }

    size_type size() const
    {
        return size_;
    }

    const std::string& filename() const
    {
        return filename_;
    }

    const value_type& high_threshold() const
    {
        return high_threshold_;
    }

    const value_type& low_threshold() const
    {
        return low_threshold_;
    }

    const count_type& dust_threshold() const
    {
        return dust_threshold_;
    }

    const value_type& dust_merge_threshold() const
    {
        return dust_merge_threshold_;
    }

    size_type dim( size_type x ) const
    {
        ZI_ASSERT( x < 3 );
        return chunks_.shape()[x];
    }

    size_type xdim() const
    {
        return dim( 0 );
    }

    size_type ydim() const
    {
        return dim( 1 );
    }

    size_type zdim() const
    {
        return dim( 2 );
    }

    chunk_type& chunk( size_type x, size_type y, size_type z )
    {
        return chunks_[x][y][z];
    }

};

} // namespace xxl

} // namespace watershed
} // namespace zi

#endif

