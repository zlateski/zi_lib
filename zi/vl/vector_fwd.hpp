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

#ifndef ZI_VL_VECTOR_FWD_HPP
#define ZI_VL_VECTOR_FWD_HPP 1

#include <zi/utility/assert.hpp>
#include <zi/utility/enable_if.hpp>
#include <zi/utility/static_assert.hpp>

#include <zi/bits/type_traits.hpp>

#include <cstddef>
#include <ostream>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

template< class T, std::size_t S >
class vector
{
private:
    ZI_STATIC_ASSERT( S > 0 , zero_length_vector );
    ZI_STATIC_ASSERT( is_scalar< T >::value , non_scalar_vector );

public:
    T v_[ S ];

public:
    template< class Y, std::size_t Z > friend class vector;

    vector() {}

    vector( const T& );
    vector( const T&, const T& );
    vector( const T&, const T&, const T& );
    vector( const T&, const T&, const T&, const T& );

    template< class Y >
    explicit vector( const vector< Y, S >& );

    template< class Y, std::size_t Z >
    explicit vector( const vector< Y, Z >&, const T& = T( 0 ),
                     ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 );

    template< class Y, std::size_t Z >
    explicit vector( const T&, const vector< Y, Z >&,
                     ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 );

    const T& operator[]( std::size_t ) const;
    const T& operator()( std::size_t ) const;

    T& operator[]( std::size_t );
    T& operator()( std::size_t );

    const T& x() const;
    const T& y() const;
    const T& z() const;
    const T& w() const;
    const T& t() const;

    T& x();
    T& y();
    T& z();
    T& w();
    T& t();

    const T& r() const;
    const T& g() const;
    const T& b() const;
    const T& a() const;

    T& r();
    T& g();
    T& b();
    T& a();

    operator const T*() const;
    operator T*();

    const T* data() const;
    T* data();

    template< class Y > vector< T, S >& operator+=( const vector< Y, S >& );
    template< class Y > vector< T, S >& operator-=( const vector< Y, S >& );
    template< class Y > vector< T, S >& operator*=( const vector< Y, S >& );
    template< class Y > vector< T, S >& operator/=( const vector< Y, S >& );

    vector< T, S >& operator+=( const T& );
    vector< T, S >& operator-=( const T& );
    vector< T, S >& operator*=( const T& );
    vector< T, S >& operator/=( const T& );

    template< class Y > vector< T, S > operator+( const vector< Y, S >& );
    template< class Y > vector< T, S > operator-( const vector< Y, S >& );
    template< class Y > vector< T, S > operator*( const vector< Y, S >& );
    template< class Y > vector< T, S > operator/( const vector< Y, S >& );

    vector< T, S > operator+( const T& );
    vector< T, S > operator-( const T& );
    vector< T, S > operator*( const T& );
    vector< T, S > operator/( const T& );

    template< class Y > const vector< T, S >& operator=( const vector< Y, S >& );
    void operator=( const T& );

    void fill( const T& );
    template< class Iterator > void assign( Iterator, Iterator );

    void set( const T& );
    void set( const T&, const T& );
    void set( const T&, const T&, const T& );
    void set( const T&, const T&, const T&, const T& );

    template< class Y > void set( const vector< Y, S >& );

    template< class Y, std::size_t Z >
    void set( const vector< Y, Z >&, const T& = T( 0 ),
              ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 );

    template< class Y, std::size_t Z >
    void set( const T&, const vector< Y, Z >&,
              ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 );

    bool equal( const vector< T, S >& );

    vector< T, S > operator-() const;

    const vector< T, S >& negate();
    template< class Y > void negate( const vector< Y, S >& );

    template< class Y > T dot( const vector< Y, S >& ) const;

    template< class Y, class W >
    void cross( const vector< Y, S >&, const vector< W, S >&,
                ZI_VL_ENABLE_IF( S == 3 , Y ) = 0 );

    template< class Y >
    vector< T, S > cross( const vector< Y, S >&,
                          ZI_VL_ENABLE_IF( S == 3 , Y ) = 0 );

    T length() const;
    T squared_length() const;

    T normalize();
    template< class Y > T normalize( const vector< Y, S >& );

    template< class Y > T distance( const vector< Y, S >& ) const;
    template< class Y > T squared_distance( const vector< Y, S >& ) const;

    std::size_t size() const;

    void clamp( const T& = T( 0 ), const T& = T( 1 ) );

    template< typename Y > void scale_to( const T& = T( -1 ), const T& = T( 1 ) );
    template< typename Y > void scale_to( vector< Y, S >&, const T& = T( -1 ), const T& = T( 1 ) );

    template< class Y > bool operator==( const vector< Y, S >& );
    template< class Y > bool operator!=( const vector< Y, S >& );
    template< class Y > bool operator<=( const vector< Y, S >& );
    template< class Y > bool operator>=( const vector< Y, S >& );
    template< class Y > bool operator< ( const vector< Y, S >& );
    template< class Y > bool operator> ( const vector< Y, S >& );

    static const vector one;
    static const vector zero;

    friend std::ostream& operator<< ( std::ostream& os, const vector< T, S >& v )
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
inline T dot( const vector< T, S >& x, const vector< Y, S >& y )
{
    return x.dot( y );
}

template< class T, std::size_t S >
inline T length( const vector< T, S >& x )
{
    return x.length();
}

template< class T, std::size_t S >
inline T squared_length( const vector< T, S >& x )
{
    return x.squared_length();
}

template< class T, class Y >
inline vector< T, 3 > cross( const vector< T, 3 >& x, const vector< Y, 3 >& y )
{
    return x;
}


} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

