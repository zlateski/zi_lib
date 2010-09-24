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

#include <zi/vl/vector_fwd.hpp>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

template< class T, std::size_t S >
class matrix
{
private:
    ZI_STATIC_ASSERT( S > 0 , zero_length_matrix );
    ZI_STATIC_ASSERT( is_scalar< T >::value , non_scalar_matrix );

    static const std::size_t num_elements = S * S;

    struct eye_init_tag {};

public:
    T v_[ S * S ];

public:
    template< class Y, std::size_t Z > friend class matrix;

    matrix() {}

    matrix( const T& );
    matrix( const eye_init_tag& );

    template< class Y >
    explicit matrix( const matrix< Y, S >& );

    const vector< T, S >& operator[]( std::size_t ) const;
    vector< T, S >& operator[]( std::size_t );

    const T& operator()( std::size_t, std::size_t ) const;
    T& operator()( std::size_t, std::size_t );

    operator const T*() const;
    operator T*();

    const T* data() const;
    T* data();

    template< class Y > matrix< T, S >& operator+=( const matrix< Y, S >& );
    template< class Y > matrix< T, S >& operator-=( const matrix< Y, S >& );
    template< class Y > matrix< T, S >& operator*=( const matrix< Y, S >& );
    template< class Y > matrix< T, S >& operator/=( const matrix< Y, S >& );

    matrix< T, S >& operator+=( const T& );
    matrix< T, S >& operator-=( const T& );
    matrix< T, S >& operator*=( const T& );
    matrix< T, S >& operator/=( const T& );

    template< class Y > matrix< T, S > operator+( const matrix< Y, S >& );
    template< class Y > matrix< T, S > operator-( const matrix< Y, S >& );
    template< class Y > matrix< T, S > operator*( const matrix< Y, S >& );
    template< class Y > matrix< T, S > operator/( const matrix< Y, S >& );

    matrix< T, S > operator+( const T& );
    matrix< T, S > operator-( const T& );
    matrix< T, S > operator*( const T& );
    matrix< T, S > operator/( const T& );

    template< class Y > const matrix< T, S >& operator=( const matrix< Y, S >& );
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

    bool equal( const matrix< T, S >& );

    matrix< T, S > operator-() const;

    const matrix< T, S >& negate();
    template< class Y > void negate( const matrix< Y, S >& );

    template< class Y > T dot( const matrix< Y, S >& ) const;

    template< class Y, class W >
    void cross( const matrix< Y, S >&, const matrix< W, S >&,
                ZI_VL_ENABLE_IF( S == 3 , Y ) = 0 );

    template< class Y >
    matrix< T, S > cross( const matrix< Y, S >&,
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

    template< class Y > bool operator==( const matrix< Y, S >& );
    template< class Y > bool operator!=( const matrix< Y, S >& );
    template< class Y > bool operator<=( const matrix< Y, S >& );
    template< class Y > bool operator>=( const matrix< Y, S >& );
    template< class Y > bool operator< ( const matrix< Y, S >& );
    template< class Y > bool operator> ( const matrix< Y, S >& );

    static const matrix one;
    static const matrix zero;

    friend std::ostream& operator<< ( std::ostream& os, const matrix< T, S >& v )
    {
        os << "[ ";
        for ( std::size_t i = 0; i < S; ++i )
        {
            os << v.v_[ i ] << ' ';
        }
        os << ']';

        return os;
    }

};

template< class T, class Y, std::size_t S >
inline T dot( const matrix< T, S >& x, const matrix< Y, S >& y )
{
    return x.dot( y );
}

template< class T, std::size_t S >
inline T length( const matrix< T, S >& x )
{
    return x.length();
}

template< class T, std::size_t S >
inline T squared_length( const matrix< T, S >& x )
{
    return x.squared_length();
}

template< class T, class Y >
inline matrix< T, 3 > cross( const matrix< T, 3 >& x, const matrix< Y, 3 >& y )
{
    return x;
}


} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

