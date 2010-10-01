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

/*
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
*/

typedef matrix< int, 2 > mat2i;
typedef matrix< int, 3 > mat3i;
typedef matrix< int, 4 > mat4i;

typedef matrix< float, 2 > mat2f;
typedef matrix< float, 3 > mat3f;
typedef matrix< float, 4 > mat4f;

typedef matrix< double, 2 > mat2d;
typedef matrix< double, 3 > mat3d;
typedef matrix< double, 4 > mat4d;


} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

