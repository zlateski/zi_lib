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

#ifndef ZI_VL_MATRIX_FWD_HPP
#define ZI_VL_MATRIX_FWD_HPP 1

#include <zi/utility/assert.hpp>
#include <zi/utility/enable_if.hpp>
#include <zi/utility/static_assert.hpp>

#include <zi/bits/type_traits.hpp>

#include <cstddef>
#include <ostream>

#include <zi/vl/vector.hpp>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

namespace tags {

struct eye_init_tag_type
{
    eye_init_tag_type() {}
};

const eye_init_tag_type eye_init_tag;

}

template< class T, std::size_t S >
class matrix
{
private:
    ZI_STATIC_ASSERT( S > 0 , zero_length_matrix );
    ZI_STATIC_ASSERT( is_scalar< T >::value , non_scalar_matrix );

    static const std::size_t num_elements = S * S;

    typedef matrix< T, S > matrix_t;


public:
    T d_[ S * S ];

public:
    template< class Y, std::size_t Z > friend class matrix;

    // ctors
    //---------------------------------------------------------------------

    matrix()
    {
    }

    matrix( const T& t )
    {
        set( t );
    }

    matrix( const T& a00, const T& a01,
            const T& a10, const T& a11 )
    {
        set( a00, a01, a10, a11 );
    }

    matrix( const T& a00, const T& a01, const T& a02,
            const T& a10, const T& a11, const T& a12,
            const T& a20, const T& a21, const T& a22 )
    {
        set( a00, a01, a02, a10, a11, a12, a20, a21, a22 );
    }

    matrix( const T& a00, const T& a01, const T& a02, const T& a03,
            const T& a10, const T& a11, const T& a12, const T& a13,
            const T& a20, const T& a21, const T& a22, const T& a23,
            const T& a30, const T& a31, const T& a32, const T& a33 )
    {
        set( a00, a01, a02, a03,
             a10, a11, a12, a13,
             a20, a21, a22, a23,
             a30, a31, a32, a33 );
             }

    matrix( const T& v, const tags::eye_init_tag_type& )
    {
        this->set( 0 );
        for ( std::size_t i = 0; i < num_elements; i += S + 1 )
        {
            d_[ i ] = v;
        }
    }

    template< class Y >
    matrix( const matrix< Y, S >& m )
    {
        this->template set< Y >( m );
    }

    inline const vector< T, S, false > operator[]( std::size_t i ) const
    {
        return vector< T, S, false >( const_cast< T* >( d_ + S * i ) );
    }

    inline vector< T, S, false > operator[]( std::size_t i )
    {
        return vector< T, S, false >( d_ + S * i );
    }

    inline vector< T, S, false >& row( std::size_t i ) { return this->operator[]( i ); }
    inline const vector< T, S, false >& row( std::size_t i ) const { return this->operator[]( i ); }

    // todo: add cols
    //
    //vector< T, S > col( std::size_t ) const;

    T& operator()( std::size_t i, std::size_t j ) { return d_[ i * S + j ]; }
    const T& operator()( std::size_t i, std::size_t j ) const { return d_[ i * S + j ]; }

    T& at( std::size_t i, std::size_t j ) { return d_[ i * S + j ]; }
    const T& at( std::size_t i, std::size_t j ) const { return d_[ i * S + j ]; }

    operator const T*() const { return d_; }
    operator T*() { return d_; }

    const T* data() const { return d_; }
    T* data() { return d_; }

    // comperison operators
    //---------------------------------------------------------------------

    template< class Y > inline bool operator==( const matrix< Y, S >& o ) const
    {
        return std::equal( d_, d_ + num_elements, o.d_ );
    }

    template< class Y > inline bool operator< ( const matrix< Y, S >& o ) const
    {
        return std::lexicographical_compare( d_, d_ + num_elements, o.d_, o.d_ + num_elements );
    }

    template< class Y > inline bool
    operator!=( const matrix< Y, S >& o ) const { return !( *this == o ); }
    template< class Y > inline bool
    operator<=( const matrix< Y, S >& o ) const { return !( o < *this ); }
    template< class Y > inline bool
    operator>=( const matrix< Y, S >& o ) const { return !( *this < o ); }
    template< class Y > inline bool
    operator> ( const matrix< Y, S >& o ) const { return o < *this; }


    template< class Y > matrix_t& operator+=( const matrix< Y, S >& o )
    {
        for ( std::size_t i = 0; i < num_elements; ++i )
        {
            d_[ i ] += o.d_[ i ];
        }
        return *this;
    }

    template< class Y > matrix_t& operator-=( const matrix< Y, S >& o )
    {
        for ( std::size_t i = 0; i < num_elements; ++i )
        {
            d_[ i ] -= o.d_[ i ];
        }
        return *this;
    }

    matrix_t& square()
    {
        matrix_t m( *this );
        *this *= m;
        return *this;
    }

    template< class Y > matrix_t& operator*=( const matrix< Y, S >& o )
    {
        if ( this == &o )
        {
            return square();
        }

        matrix_t m( *this );

        for ( std::size_t row = 0, i = 0; row < S; ++row )
        {
            for ( std::size_t col = 0; col < S; ++col, ++i )
            {
                d_[ i ] = 0;
                for ( std::size_t j = 0; j < S; ++j )
                {
                    d_[ i ] += m.at( row, j ) * o.at( j, col );
                }
            }
        }

        return *this;

    }

    template< class Y > matrix_t& operator/=( const matrix< Y, S >& o );

    template< class Y, bool B > matrix_t& operator+=( const vector< Y, S, B >& );
    template< class Y, bool B > matrix_t& operator-=( const vector< Y, S, B >& );
    template< class Y, bool B > matrix_t& operator*=( const vector< Y, S, B >& );
    template< class Y, bool B > matrix_t& operator/=( const vector< Y, S, B >& );

    matrix_t& operator+=( const T& );
    matrix_t& operator-=( const T& );
    matrix_t& operator*=( const T& );
    matrix_t& operator/=( const T& );

    template< class Y > matrix_t operator+( const matrix< Y, S >& );
    template< class Y > matrix_t operator-( const matrix< Y, S >& );
    template< class Y > matrix_t operator*( const matrix< Y, S >& );
    template< class Y > matrix_t operator/( const matrix< Y, S >& );

    template< class Y > matrix_t operator+( const vector< Y, S >& );
    template< class Y > matrix_t operator-( const vector< Y, S >& );
    template< class Y > vector< T, S > operator*( const vector< Y, S >& );
    template< class Y > matrix_t operator/( const vector< Y, S >& );

    matrix_t operator+( const T& v )
    {
        matrix_t r( *this );
        r += v;
        return r;
    }

    matrix_t operator-( const T& v )
    {
        matrix_t r( *this );
        r -= v;
        return r;
    }

    matrix_t operator*( const T& v )
    {
        matrix_t r( *this );
        r *= v;
        return r;
    }

    matrix_t operator/( const T& v )
    {
        matrix_t r( *this );
        r /= v;
        return r;
    }

    matrix_t& transpose();
    template< class Y > matrix_t& transpose( const matrix< Y, S >& );
    template< class Y > void transpose_to( matrix< Y, S >& ) const;

    void set( const T& t )
    {
        std::fill_n( d_, num_elements, t );
    }

    void set( const T& a00, const T& a01,
              const T& a10, const T& a11 )
    {
        ZI_VERIFY( S == 2 );
        d_[ 0 ] = a00; d_[ 1 ] = a01;
        d_[ 2 ] = a10; d_[ 3 ] = a11;
    }

    void set( const T& a00, const T& a01, const T& a02,
              const T& a10, const T& a11, const T& a12,
              const T& a20, const T& a21, const T& a22 )
    {
        ZI_VERIFY( S == 3 );
        d_[ 0 ] = a00; d_[ 1 ] = a01; d_[ 2 ] = a02;
        d_[ 3 ] = a10; d_[ 4 ] = a11; d_[ 5 ] = a12;
        d_[ 6 ] = a20; d_[ 7 ] = a21; d_[ 8 ] = a22;
    }

    void set( const T& a00, const T& a01, const T& a02, const T& a03,
              const T& a10, const T& a11, const T& a12, const T& a13,
              const T& a20, const T& a21, const T& a22, const T& a23,
              const T& a30, const T& a31, const T& a32, const T& a33 )
    {
        ZI_VERIFY( S == 4 );
        d_[ 0  ] = a00; d_[ 1  ] = a01; d_[ 2  ] = a02; d_[ 3  ] = a03;
        d_[ 4  ] = a10; d_[ 5  ] = a11; d_[ 6  ] = a12; d_[ 7  ] = a13;
        d_[ 8  ] = a20; d_[ 9  ] = a21; d_[ 10 ] = a22; d_[ 11 ] = a23;
        d_[ 12 ] = a30; d_[ 13 ] = a31; d_[ 14 ] = a32; d_[ 15 ] = a33;
    }

    template< class Y >
    void set( const matrix< Y, S >& m )
    {
        std::copy( m.d_, m.d_ + num_elements, d_ );
    }



/*
    template< class Y > const matrix_t& operator=( const matrix< Y, S >& );
    void operator=( const T& );

    void fill( const T& );
    template< class Iterator > void assign( Iterator, Iterator );

    void set( const T& );
    void set( const T&, const T& );
    void set( const T&, const T&, const T& );
    void set( const T&, const T&, const T&, const T& );

    template< class Y > void set( const matrix< Y, S >& );

    template< class Y, std::size_t Z >
    void set( const matrix< Y, Z >&, const T& = T( 0 ),
              ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 );

    template< class Y, std::size_t Z >
    void set( const T&, const matrix< Y, Z >&,
              ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 );

    bool equal( const matrix_t& );

    matrix_t operator-() const;

    const matrix_t& negate();
    template< class Y > void negate( const matrix< Y, S >& );

    template< class Y > T dot( const matrix< Y, S >& ) const;

    template< class Y, class W >
    void cross( const matrix< Y, S >&, const matrix< W, S >&,
                ZI_VL_ENABLE_IF( S == 3 , Y ) = 0 );

    template< class Y >
    matrix_t cross( const matrix< Y, S >&,
                          ZI_VL_ENABLE_IF( S == 3 , Y ) = 0 );

    T length() const;
    T squared_length() const;

    T normalize();
    template< class Y > T normalize( const matrix< Y, S >& );

    template< class Y > T distance( const matrix< Y, S >& ) const;
    template< class Y > T squared_distance( const matrix< Y, S >& ) const;

    std::size_t size() const;

    void clamp( const T& = T( 0 ), const T& = T( 1 ) );

    template< typename Y > void scale_to( const T& = T( -1 ), const T& = T( 1 ) );
    template< typename Y > void scale_to( matrix< Y, S >&, const T& = T( -1 ), const T& = T( 1 ) );

    static const matrix one;
    static const matrix zero;


};

template< class T, class Y, std::size_t S >
inline T dot( const matrix_t& x, const matrix< Y, S >& y )
{
    return x.dot( y );
}

template< class T, std::size_t S >
inline T length( const matrix_t& x )
{
    return x.length();
}

template< class T, std::size_t S >
inline T squared_length( const matrix_t& x )
{
    return x.squared_length();
}

template< class T, class Y >
inline matrix< T, 3 > cross( const matrix< T, 3 >& x, const matrix< Y, 3 >& y )
{
    return x;
}
*/


    friend inline std::ostream& operator<< ( std::ostream& os, const matrix_t& v )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            os << v[ i ] << '\n';
        }
        return os;
    }


    static const matrix_t zero;
    static const matrix_t one;
    static const matrix_t eye;
    static const matrix_t identity;

};

} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

