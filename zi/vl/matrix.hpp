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

#ifndef ZI_VL_MATRIX_HPP
#define ZI_VL_MATRIX_HPP 1

#include <zi/vl/matrix_fwd.hpp>
#include <numeric>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

// ctors

template< class T, std::size_t S >
inline matrix< T, S >::matrix( const T& v )
{
    std::fill_n( v_, num_elements, v );
}

template< class T, std::size_t S >
inline matrix< T, S >::matrix( const tags::eye_init_tag_type& )
{
    std::fill_n( v_, num_elements, 0 );
    for ( std::size_t i = 0; i < num_elements; i += S + 1 )
    {
        v_[ i ] = 1;
    }
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >::matrix( const matrix< Y, S >& other )
{
    std::copy( other.v_, other.v_ + num_elements, v_ );
}

template< class T, std::size_t S >
inline vector< T, S > matrix< T, S >::operator[]( std::size_t r ) const
{
    ZI_VERIFY( r < S );
    vector< T, S > ret;
    std::copy( v_ + r * S, v_ + r * S + S, ret.v_ );
    return ret;
}

template< class T, std::size_t S >
inline vector< T, S > matrix< T, S >::row( std::size_t r ) const
{
    ZI_VERIFY( r < S );
    vector< T, S > ret;
    std::copy( v_ + r * S, v_ + r * S + S, ret.v_ );
    return ret;
}

template< class T, std::size_t S >
inline vector< T, S > matrix< T, S >::col( std::size_t r ) const
{
    ZI_VERIFY( r < S );
    vector< T, S > ret;
    std::size_t d = 0;
    for ( std::size_t it = r; it < num_elements; it += S )
    {
        ret.v_[ d++ ] = v_[ it ];
    }
    return ret;
}

template< class T, std::size_t S >
inline const T& matrix< T, S >::operator()( std::size_t r, std::size_t c ) const
{
    ZI_VERIFY( r < S && c < S );
    return v_[ S * r + c ];
}

template< class T, std::size_t S >
inline T& matrix< T, S >::operator()( std::size_t r, std::size_t c )
{
    ZI_VERIFY( r < S && c < S );
    return v_[ S * r + c ];
}

template< class T, std::size_t S >
inline const T& matrix< T, S >::at( std::size_t r, std::size_t c ) const
{
    ZI_VERIFY( r < S && c < S );
    return v_[ S * r + c ];
}

template< class T, std::size_t S >
inline T& matrix< T, S >::at( std::size_t r, std::size_t c )
{
    ZI_VERIFY( r < S && c < S );
    return v_[ S * r + c ];
}

template< class T, std::size_t S >
inline matrix< T, S >::operator const T* () const
{
    return v_;
}

template< class T, std::size_t S >
inline matrix< T, S >::operator T* ()
{
    return v_;
}

template< class T, std::size_t S >
inline const T* matrix< T, S >::data() const
{
    return v_;
}

template< class T, std::size_t S >
inline T* matrix< T, S >::data()
{
    return v_;
}


template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator +=( const matrix< Y, S >& other )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] += other.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator -=( const matrix< Y, S >& other )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] -= other.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator *=( const matrix< Y, S >& other )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] *= other.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator /=( const matrix< Y, S >& other )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] /= other.v_[ i ];
    }
    return *this;
}


template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator +=( const vector< Y, S >& other )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        ( *reinterpret_cast< vector< T, S >* >( v_ + i * S ) ) += other;
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator -=( const vector< Y, S >& other )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        ( *reinterpret_cast< vector< T, S >* >( v_ + i * S ) ) -= other;
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator *=( const vector< Y, S >& other )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        ( *reinterpret_cast< vector< T, S >* >( v_ + i * S ) ) *= other;
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::operator /=( const vector< Y, S >& other )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        ( *reinterpret_cast< vector< T, S >* >( v_ + i * S ) ) /= other;
    }
    return *this;
}


template< class T, std::size_t S >
inline matrix< T, S >& matrix< T, S >::operator +=( const T& v )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] += v;
    }
    return *this;
}

template< class T, std::size_t S >
inline matrix< T, S >& matrix< T, S >::operator -=( const T& v )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] -= v;
    }
    return *this;
}

template< class T, std::size_t S >
inline matrix< T, S >& matrix< T, S >::operator *=( const T& v )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] *= v;
    }
    return *this;
}

template< class T, std::size_t S >
inline matrix< T, S >& matrix< T, S >::operator /=( const T& v )
{
    for ( std::size_t i = 0; i < num_elements; ++i )
    {
        v_[ i ] /= v;
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator +( const matrix< Y, S >& other )
{
    matrix< T, S > r( *this );
    r += other;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator -( const matrix< Y, S >& other )
{
    matrix< T, S > r( *this );
    r -= other;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator *( const matrix< Y, S >& other )
{
    matrix< T, S > r( *this );
    r *= other;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator /( const matrix< Y, S >& other )
{
    matrix< T, S > r( *this );
    r /= other;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator +( const vector< Y, S >& other )
{
    matrix< T, S > r( *this );
    r += other;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator -( const vector< Y, S >& other )
{
    matrix< T, S > r( *this );
    r -= other;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S > matrix< T, S >::operator *( const vector< Y, S >& other )
{
    vector< T, S > r( 0 );
    for ( std::size_t row = 0; row < num_elements; row += S )
    {
        for ( std::size_t col = 0; col < S; ++col )
        {
            r.v_[ col ] += other.v_[ col ] * v_[ row + col ];
        }
    }
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S > matrix< T, S >::operator /( const vector< Y, S >& other )
{
    matrix< T, S > r( *this );
    r /= other;
    return r;
}

template< class T, std::size_t S >
inline matrix< T, S > matrix< T, S >::operator +( const T& v )
{
    matrix< T, S > r( *this );
    r += v;
    return r;
}

template< class T, std::size_t S >
inline matrix< T, S > matrix< T, S >::operator -( const T& v )
{
    matrix< T, S > r( *this );
    r -= v;
    return r;
}

template< class T, std::size_t S >
inline matrix< T, S > matrix< T, S >::operator *( const T& v )
{
    matrix< T, S > r( *this );
    r *= v;
    return r;
}

template< class T, std::size_t S >
inline matrix< T, S > matrix< T, S >::operator /( const T& v )
{
    matrix< T, S > r( *this );
    r /= v;
    return r;
}

template< class T, std::size_t S >
inline matrix< T, S >& matrix< T, S >::transpose()
{
    for ( std::size_t row = 0; row < S; ++row )
    {
        for ( std::size_t col = 0; col < row; ++col )
        {
            std::swap( at( row, col ), at( col, row ) );
        }
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline matrix< T, S >& matrix< T, S >::transpose( const matrix< Y, S >& other )
{
    if ( reinterpret_cast< const char* >( &other ) ==
         const_cast< const char* >( reinterpret_cast< char* >( this ) ) )
    {
        return transpose();
    }

    for ( std::size_t row = 0; row < S; ++row )
    {
        for ( std::size_t col = 0; col < S; ++col )
        {
            at( row, col ) = other.at( col, row );
        }
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline void matrix< T, S >::transpose_to( matrix< Y, S >& other ) const
{
    if ( const_cast< const char* >( reinterpret_cast< char* >( &other ) ) ==
         reinterpret_cast< const char* >( this ) )
    {
        other.transpose();
        return;
    }

    for ( std::size_t row = 0; row < S; ++row )
    {
        for ( std::size_t col = 0; col < S; ++col )
        {
            other.at( row, col ) = at( col, row );
        }
    }
}

template< class T, std::size_t S >
const matrix< T, S > matrix< T, S >::zero( 0 );

template< class T, std::size_t S >
const matrix< T, S > matrix< T, S >::one( 1 );

template< class T, std::size_t S >
const matrix< T, S > matrix< T, S >::eye( tags::eye_init_tag );

template< class T, std::size_t S >
const matrix< T, S > matrix< T, S >::identity( tags::eye_init_tag );


} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

