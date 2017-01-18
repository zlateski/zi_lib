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

#ifndef ZI_WATERSHED_AFFINITY_MATRIX_HPP
#define ZI_WATERSHED_AFFINITY_MATRIX_HPP 1

#include <zi/watershed/detail/tictoc.hpp>
#include <zi/watershed/detail/buffer.hpp>
#include <zi/watershed/detail/io.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>
#include <zi/bits/shared_ptr.hpp>
#include <zi/time/timer.hpp>
#include <zi/concurrency/rwmutex.hpp>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>

#include <boost/interprocess/file_mapping.hpp>

namespace zi {
namespace watershed {

template< class T >
class affinity_matrix: non_copyable
{
private:
    typedef boost::interprocess::file_mapping  file_mapping ;

    T                             *data_             ;
    std::size_t                    x_, y_, z_, size_ ;
    shared_ptr< file_mapping >     file_;
    std::size_t                    foff_;

    std::size_t         *yoffs_;
    std::size_t         *zoffs_;
    std::size_t         *doffs_;

    void load_data()
    {
        if ( data_ == 0 )
        {
            data_ = detail::get_buffer<T>( size_ );
            io::file_read( *file_.get(), data_, size_*sizeof(T), foff_ );
        }
    }

public:
    affinity_matrix( std::size_t x,
                     std::size_t y,
                     std::size_t z,
                     shared_ptr< file_mapping > f,
                     std::size_t foff,
                     bool preload = false )
        : data_( 0 ),
          x_( x ),
          y_( y ),
          z_( z ),
          size_( x * y * z * 3 ),
          file_( f ),
          foff_( foff )
    {
        if ( preload )
        {
            load_data();
        }

        yoffs_ = reinterpret_cast< std::size_t* >( malloc( y * sizeof( std::size_t ) ));

        yoffs_[ 0 ] = 0;
        for ( std::size_t i = 1; i < y; ++i )
        {
            yoffs_[ i ] = yoffs_[ i - 1 ] + x;
        }

        std::size_t xy = x * y;
        zoffs_ = reinterpret_cast< std::size_t* >( malloc( z * sizeof( std::size_t ) ));

        zoffs_[ 0 ] = 0;
        for ( std::size_t i = 1; i < z; ++i )
        {
            zoffs_[ i ] = zoffs_[ i - 1 ] + xy;
        }

        doffs_ = reinterpret_cast< std::size_t* >( malloc( 3 * sizeof( std::size_t ) ));

        doffs_[ 0 ] = 0;
        doffs_[ 1 ] = x*y*z;
        doffs_[ 2 ] = x*y*z*2;

    }

    ~affinity_matrix()
    {
        if ( yoffs_ )
        {
            free( yoffs_ );
        }
        if ( zoffs_ )
        {
            free( zoffs_ );
        }
        if ( doffs_ )
        {
            free( doffs_ );
        }

        free_data();
    }

    void free_data()
    {
        if ( data_ )
        {
            detail::return_buffer(data_);
            data_ = 0;
        }
    }

    const T* data() const
    {
        load_data();
        return data_;
    }

    T* data()
    {
        load_data();
        return data_;
    }

    std::size_t x() const
    {
        return x_;
    }

    std::size_t y() const
    {
        return y_;
    }

    std::size_t z() const
    {
        return z_;
    }

    std::size_t size() const
    {
        return size_;
    }

    const T& at( std::size_t i, std::size_t j, std::size_t k, std::size_t l ) const
    {
        return data_[ i + yoffs_[ j ] + zoffs_[ k ] + doffs_[ l ] ];
    }

    T& at( std::size_t i, std::size_t j, std::size_t k, std::size_t l )
    {
        return data_[ i + yoffs_[ j ] + zoffs_[ k ] + doffs_[ l ] ];
    }

    const T& operator()( std::size_t i, std::size_t j, std::size_t k, std::size_t l ) const
    {
        return data_[ i + yoffs_[ j ] + zoffs_[ k ] + doffs_[ l ] ];
    }

    T& operator()( std::size_t i, std::size_t j, std::size_t k, std::size_t l )
    {
        return data_[ i + yoffs_[ j ] + zoffs_[ k ] + doffs_[ l ] ];
    }

    std::size_t xslice( std::size_t n, std::size_t d, std::vector< T >& out ) const
    {
        out.resize( (y_-2) * (z_-2) );
        for ( std::size_t i = 0, z = 1; z < z_-1; ++z )
        {
            for ( std::size_t y = 1; y < y_-1; ++y, ++i )
            {
                out[ i ] = at( n, y, z, d );
            }
        }
        return out.size();
    }

    std::size_t yslice( std::size_t n, std::size_t d, std::vector< T >& out ) const
    {
        out.resize( (x_-2) * (z_-2) );
        for ( std::size_t i = 0, z = 1; z < z_-1; ++z )
        {
            for ( std::size_t x = 1; x < x_-1; ++x, ++i )
            {
                out[ i ] = at( x, n, z, d );
            }
        }
        return out.size();
    }

    std::size_t zslice( std::size_t n, std::size_t d, std::vector< T >& out ) const
    {
        out.resize( (x_-2) * (y_-2) );
        for ( std::size_t i = 0, y = 1; y < y_-1; ++y )
        {
            for ( std::size_t x = 1; x < x_-1; ++x, ++i )
            {
                out[ i ] = at( x, y, n, d );
            }
        }
        return out.size();
    }

    void load()
    {
        load_data();
    }

};


} // namespace watershed
} // namespace zi

#endif

