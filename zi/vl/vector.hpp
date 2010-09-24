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

#ifndef ZI_VL_VECTOR_HPP
#define ZI_VL_VECTOR_HPP 1

#include <zi/vl/vector_fwd.hpp>
#include <numeric>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

// ctors

template< class T, std::size_t S >
inline vector< T, S >::vector( const T& v )
{
    std::fill_n( v_, S, v );
}

template< class T, std::size_t S >
inline vector< T, S >::vector( const T& x, const T& y )
{
    ZI_VERIFY( S > 1 );
    v_[ 0 ] = x;
    v_[ 1 ] = y;
}

template< class T, std::size_t S >
inline vector< T, S >::vector( const T& x, const T& y, const T& z )
{
    ZI_VERIFY( S > 2 );
    v_[ 0 ] = x;
    v_[ 1 ] = y;
    v_[ 2 ] = z;
}

template< class T, std::size_t S >
inline vector< T, S >::vector( const T& x, const T& y, const T& z, const T& t )
{
    ZI_VERIFY( S > 3 );
    v_[ 0 ] = x;
    v_[ 1 ] = y;
    v_[ 2 ] = z;
    v_[ 3 ] = t;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S >::vector( const vector< Y, S >& other )
{
    this->set( other );
}

template< class T, std::size_t S >
template< class Y, std::size_t Z >
inline vector< T, S >::vector( const vector< Y, Z >& other, const T& x,
                               ZI_VL_ENABLE_IF( S == Z + 1, Y ) )
{
    this->set( other, x );
}

template< class T, std::size_t S >
template< class Y, std::size_t Z >
inline vector< T, S >::vector( const T& x, const vector< Y, Z >& other,
                        ZI_VL_ENABLE_IF( S == Z + 1, Y ) )
{
    this->set( x, other );
}

template< class T, std::size_t S >
inline const T& vector< T, S >::operator[]( std::size_t i ) const
{
    ZI_VERIFY( S > i );
    return v_[ i ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::operator()( std::size_t i ) const
{
    ZI_VERIFY( S > i );
    return v_[ i ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::operator[]( std::size_t i )
{
    ZI_VERIFY( S > i );
    return v_[ i ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::operator()( std::size_t i )
{
    ZI_VERIFY( S > i );
    return v_[ i ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::x() const
{
    return v_[ 0 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::y() const
{
    ZI_VERIFY( S > 1 );
    return v_[ 1 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::z() const
{
    ZI_VERIFY( S > 2 );
    return v_[ 2 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::t() const
{
    ZI_VERIFY( S > 3 );
    return v_[ 3 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::w() const
{
    ZI_VERIFY( S > 3 );
    return v_[ 3 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::x()
{
    return v_[ 0 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::y()
{
    ZI_VERIFY( S > 1 );
    return v_[ 1 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::z()
{
    ZI_VERIFY( S > 2 );
    return v_[ 2 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::t()
{
    ZI_VERIFY( S > 3 );
    return v_[ 3 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::w()
{
    ZI_VERIFY( S > 3 );
    return v_[ 3 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::r() const
{
    return v_[ 0 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::g() const
{
    ZI_VERIFY( S > 1 );
    return v_[ 1 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::b() const
{
    ZI_VERIFY( S > 2 );
    return v_[ 2 ];
}

template< class T, std::size_t S >
inline const T& vector< T, S >::a() const
{
    ZI_VERIFY( S > 3 );
    return v_[ 3 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::r()
{
    return v_[ 0 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::g()
{
    ZI_VERIFY( S > 1 );
    return v_[ 1 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::b()
{
    ZI_VERIFY( S > 2 );
    return v_[ 2 ];
}

template< class T, std::size_t S >
inline T& vector< T, S >::a()
{
    ZI_VERIFY( S > 3 );
    return v_[ 3 ];
}

template< class T, std::size_t S >
inline vector< T, S >::operator const T*() const
{
    return v_;
}

template< class T, std::size_t S >
inline vector< T, S >::operator T*()
{
    return v_;
}

template< class T, std::size_t S >
inline const T* vector< T, S >::data() const
{
    return v_;
}

template< class T, std::size_t S >
inline T* vector< T, S >::data()
{
    return v_;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S >& vector< T, S >::operator+=( const vector< Y, S >& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] += o.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S >& vector< T, S >::operator-=( const vector< Y, S >& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] -= o.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S >& vector< T, S >::operator*=( const vector< Y, S >& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] *= o.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S >& vector< T, S >::operator/=( const vector< Y, S >& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] /= o.v_[ i ];
    }
    return *this;
}


template< class T, std::size_t S >
inline vector< T, S >& vector< T, S >::operator+=( const T& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] += o;
    }
    return *this;
}

template< class T, std::size_t S >
inline vector< T, S >& vector< T, S >::operator-=( const T& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] -= o;
    }
    return *this;
}

template< class T, std::size_t S >
inline vector< T, S >& vector< T, S >::operator*=( const T& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] *= o;
    }
    return *this;
}

template< class T, std::size_t S >
inline vector< T, S >& vector< T, S >::operator/=( const T& o )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] /= o;
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S > vector< T, S >::operator+( const vector< Y, S >& o )
{
    vector< T, S > r( *this );
    r += o;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S > vector< T, S >::operator-( const vector< Y, S >& o )
{
    vector< T, S > r( *this );
    r -= o;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S > vector< T, S >::operator*( const vector< Y, S >& o )
{
    vector< T, S > r( *this );
    r *= o;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S > vector< T, S >::operator/( const vector< Y, S >& o )
{
    vector< T, S > r( *this );
    r /= o;
    return r;
}

template< class T, std::size_t S >
inline vector< T, S > vector< T, S >::operator+( const T& o )
{
    vector< T, S > r( *this );
    r += o;
    return r;
}

template< class T, std::size_t S >
inline vector< T, S > vector< T, S >::operator-( const T& o )
{
    vector< T, S > r( *this );
    r -= o;
    return r;
}

template< class T, std::size_t S >
inline vector< T, S > vector< T, S >::operator*( const T& o )
{
    vector< T, S > r( *this );
    r *= o;
    return r;
}

template< class T, std::size_t S >
inline vector< T, S > vector< T, S >::operator/( const T& o )
{
    vector< T, S > r( *this );
    r /= o;
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline const vector< T, S >& vector< T, S >::operator=( const vector< Y, S >& o )
{
    this->set( o );
    return *this;
}

template< class T, std::size_t S >
inline void vector< T, S >::operator=( const T& o )
{
    this->fill( o );
}

template< class T, std::size_t S >
inline void vector< T, S >::fill( const T& v )
{
    std::fill_n( v_, S, v );
}

template< class T, std::size_t S >
template< class Iterator >
inline void vector< T, S >::assign( Iterator beg, Iterator end )
{
    std::copy( beg, end, v_ );
}

template< class T, std::size_t S >
inline void vector< T, S >::set( const T& v )
{
    std::fill_n( v_, S, v );
}

template< class T, std::size_t S >
inline void vector< T, S >::set( const T& x, const T& y )
{
    ZI_VERIFY( S > 1 );
    v_[ 0 ] = x;
    v_[ 1 ] = y;
}

template< class T, std::size_t S >
inline void vector< T, S >::set( const T& x, const T& y, const T& z )
{
    ZI_VERIFY( S > 2 );
    v_[ 0 ] = x;
    v_[ 1 ] = y;
    v_[ 2 ] = z;
}

template< class T, std::size_t S >
inline void vector< T, S >::set( const T& x, const T& y, const T& z, const T& t )
{
    ZI_VERIFY( S > 3 );
    v_[ 0 ] = x;
    v_[ 1 ] = y;
    v_[ 2 ] = z;
    v_[ 3 ] = t;
}


template< class T, std::size_t S >
template< class Y >
inline void vector< T, S >::set( const vector< Y, S >& other )
{
    std::copy( other.v_, other.v_ + S, v_ );
}

template< class T, std::size_t S >
template< class Y, std::size_t Z >
inline void vector< T, S >::set( const vector< Y, Z >& other, const T& x,
                                 ZI_VL_ENABLE_IF( S == Z + 1, Y ) )
{
    std::copy( other.v_, other.v_ + Z, v_ );
    v_[ Z ] = x;
}

template< class T, std::size_t S >
template< class Y, std::size_t Z >
inline void vector< T, S >::set( const T& x, const vector< Y, Z >& other,
                                 ZI_VL_ENABLE_IF( S == Z + 1, Y ) )
{
    std::copy( other.v_, other.v_ + Z, v_ + 1 );
    v_[ 0 ] = x;
}

template< class T, std::size_t S >
inline bool vector< T, S >::equal( const vector< T, S >& other )
{
    return std::equal( v_, v_ + S, other.v_ );
}

template< class T, std::size_t S >
inline vector< T, S > vector< T, S >::operator-() const
{
    vector< T, S > r( *this );
    return r.negate();
}

template< class T, std::size_t S >
inline const vector< T, S >& vector< T, S >::negate()
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] = - v_[ i ];
    }
    return *this;

}

template< class T, std::size_t S >
template< class Y >
inline void vector< T, S >::negate( const vector< Y, S >& other )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] = - other.v_[ i ];
    }
    return *this;
}

template< class T, std::size_t S >
template< class Y >
inline T vector< T, S >::dot( const vector< Y, S >& other ) const
{
    return std::inner_product( v_, v_ + S, other.v_, T( 0 ) );
}

template< class T, std::size_t S >
template< class Y, class W >
inline void vector< T, S >::cross( const vector< Y, S >& x, const vector< W, S >& y,
                                   ZI_VL_ENABLE_IF( S == 3 , Y ) )
{
    v_[ 0 ] = x.v_[1] * y.v_[2] - x.v_[2] * y.v_[1];
    v_[ 1 ] = x.v_[2] * y.v_[0] - x.v_[0] * y.v_[2];
    v_[ 2 ] = x.v_[0] * y.v_[1] - x.v_[2] * y.v_[0];
}

template< class T, std::size_t S >
template< class Y >
inline vector< T, S > vector< T, S >::cross( const vector< Y, S >& other,
                                             ZI_VL_ENABLE_IF( S == 3 , Y ) )
{
    vector< T, S > r;
    r.cross( *this, other );
    return r;
}

template< class T, std::size_t S >
inline T vector< T, S >::length() const
{
    return std::sqrt( squared_length() );
}

template< class T, std::size_t S >
inline T vector< T, S >::squared_length() const
{
    T r( 0 );
    for ( std::size_t i = 0; i < S; ++i )
    {
        r += v_[ i ] * v_[ i ];
    }
    return r;
}

template< class T, std::size_t S >
inline T vector< T, S >::normalize()
{
    T r = length();
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] /= r;
    }
    return r;
}

template< class T, std::size_t S >
template< class Y >
inline T vector< T, S >::normalize( const vector< Y, S >& other )
{
    this->set( other );
    return normalize();
}

template< class T, std::size_t S >
template< class Y >
inline T vector< T, S >::distance( const vector< Y, S >& other ) const
{
    return std::sqrt( squared_distance( other ) );
}

template< class T, std::size_t S >
template< class Y >
inline T vector< T, S >::squared_distance( const vector< Y, S >& other ) const
{
    T r( 0 );
    for ( std::size_t i = 0; i < S; ++i )
    {
        T d = v_[ i ] - other.v_[ i ];
        r += d * d;
    }
    return r;
}

template< class T, std::size_t S >
inline std::size_t vector< T, S >::size() const
{
    return S;
}

template< class T, std::size_t S >
inline void vector< T, S >::clamp( const T& min, const T& max )
{
    for ( std::size_t i = 0; i < S; ++i )
    {
        v_[ i ] = ( v_[ i ] < min ? min : ( v_[ i ] > max ? max : v_[ i ] ) );
    }
}

template< class T, std::size_t S >
template< class Y >
inline bool vector< T, S >::operator==( const vector< Y, S >& other )
{
    return std::equal( v_, v_ + S, other.v_ );
}

template< class T, std::size_t S >
template< class Y >
inline bool vector< T, S >::operator!=( const vector< Y, S >& other )
{
    return !( *this == other );
}

template< class T, std::size_t S >
template< class Y >
inline bool vector< T, S >::operator<=( const vector< Y, S >& other )
{
    return !( other < *this );
}

template< class T, std::size_t S >
template< class Y >
inline bool vector< T, S >::operator>=( const vector< Y, S >& other )
{
    return !( *this < other );
}

template< class T, std::size_t S >
template< class Y >
inline bool vector< T, S >::operator<( const vector< Y, S >& other )
{
    return std::lexicographical_compare( v_, v_ + S, other.v_, other.v_ + S );
}

template< class T, std::size_t S >
template< class Y >
inline bool vector< T, S >::operator>( const vector< Y, S >& other )
{
    return !( *this >= other );
}

template< class T, std::size_t S >
const vector< T, S > vector< T, S >::one( 1 );

template< class T, std::size_t S >
const vector< T, S > vector< T, S >::zero( 0 );


} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

