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

protected:
    T v_[ S ];

public:
    template< class Y, std::size_t Z > friend class vector;

    // ctors
    //---------------------------------------------------------------------

    vector() {}

    vector( const T& v ) { set( v ); }
    vector( const T& x, const T& y ) { set( x, y ); }
    vector( const T& x, const T& y, const T& z ) { set( x, y, z ); }
    vector( const T& x, const T& y, const T& z, const T& t ) { set( x, y, z, t ); }

    template< class Y > explicit vector( const vector< Y, S >& v )
    {
        set( v );
    }

    template< class Y, std::size_t Z > explicit vector( const vector< Y, Z >& v, const T& t,
                                                        ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        set( v, t );
    }

    template< class Y, std::size_t Z > explicit vector( const T& t, const vector< Y, Z >& v,
                                                        ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        set( t, v );
    }

    // to homogenous
    template< class Y, std::size_t Z > vector( const vector< Y, Z >& v,
                                               ZI_VL_ENABLE_IF( S + 1 == Z, Y ) = 0 )
    {
        *this = v;
    }

    // from homogenous
    template< class Y, std::size_t Z > vector( const vector< Y, Z >& v,
                                               ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        set( v, 1 );
    }

    // access
    //---------------------------------------------------------------------

    inline const T& at( std::size_t ) const;
    inline const T& operator[]( std::size_t i ) const { return at( i ); }
    inline const T& operator()( std::size_t i ) const { return at( i ); }

    inline T& at( std::size_t );
    inline T& operator[]( std::size_t i ) { return at( i ); }
    inline T& operator()( std::size_t i ) { return at( i ); }

    inline const T& x() const { return at( 0 ); }
    inline const T& y() const { return at( 1 ); }
    inline const T& z() const { return at( 2 ); }
    inline const T& w() const { return at( 3 ); }
    inline const T& t() const { return at( 3 ); }

    inline T& x() { return at( 0 ); }
    inline T& y() { return at( 1 ); }
    inline T& z() { return at( 2 ); }
    inline T& w() { return at( 3 ); }
    inline T& t() { return at( 4 ); }

    inline const T& r() const { return at( 0 ); }
    inline const T& g() const { return at( 1 ); }
    inline const T& b() const { return at( 2 ); }
    inline const T& a() const { return at( 3 ); }

    inline T& r() { return at( 0 ); }
    inline T& g() { return at( 1 ); }
    inline T& b() { return at( 2 ); }
    inline T& a() { return at( 3 ); }

    // data ptrs
    //---------------------------------------------------------------------

    inline operator const T*() const { return v_; }
    inline operator T*() { return v_; }

    inline const T* data() const { return v_; }
    inline T* data() { return v_; }

    // comperison operators
    //---------------------------------------------------------------------

    template< class Y > inline bool operator==( const vector< Y, S >& ) const ;
    template< class Y > inline bool operator< ( const vector< Y, S >& ) const ;

    template< class Y > inline bool operator!=( const vector< Y, S >& o ) const { return !( *this == o ); }
    template< class Y > inline bool operator<=( const vector< Y, S >& o ) const { return !( o < *this );  }
    template< class Y > inline bool operator>=( const vector< Y, S >& o ) const { return !( *this < o );  }
    template< class Y > inline bool operator> ( const vector< Y, S >& o ) const { return o < *this;       }

    // unary arithmetic operators
    //---------------------------------------------------------------------

    template< class Y > vector< T, S >& operator+=( const vector< Y, S >& );
    template< class Y > vector< T, S >& operator-=( const vector< Y, S >& );
    template< class Y > vector< T, S >& operator*=( const vector< Y, S >& );
    template< class Y > vector< T, S >& operator/=( const vector< Y, S >& );

    vector< T, S >& operator+=( const T& );
    vector< T, S >& operator-=( const T& );
    vector< T, S >& operator*=( const T& );
    vector< T, S >& operator/=( const T& );

    vector< T, S > operator-() const;

    // unary arithmetic operators
    //---------------------------------------------------------------------

    template< class Y > inline vector< T, S > operator+( const vector< Y, S >& ) const ;
    template< class Y > inline vector< T, S > operator-( const vector< Y, S >& ) const ;
    template< class Y > inline vector< T, S > operator*( const vector< Y, S >& ) const ;
    template< class Y > inline vector< T, S > operator/( const vector< Y, S >& ) const ;

    inline vector< T, S > operator+( const T& ) const ;
    inline vector< T, S > operator-( const T& ) const ;
    inline vector< T, S > operator*( const T& ) const ;
    inline vector< T, S > operator/( const T& ) const ;

    friend inline vector< T, S > operator+( const T& t, const vector< T, S >& v ) { return  v + t; }
    friend inline vector< T, S > operator-( const T& t, const vector< T, S >& v ) { return -v + t; }
    friend inline vector< T, S > operator*( const T& t, const vector< T, S >& v ) { return  v * t; }

    // assignment operators
    //---------------------------------------------------------------------

    template< class Y >
    vector< T, S >& operator=( const vector< Y, S >& other )
    {
        set( other );
        return *this;
    }

    vector< T, S >& operator=( const T& v )
    {
        set( v );
        return *this;
    }

    // to homogenous
    template< class Y, std::size_t Z >
    typename enable_if< ( Z == S + 1 ), vector< T, S >& >::type
    operator= ( const vector< Y, Z >& other )
    {
        const T scale = other.v_[ S ];
        std::copy( other.v_, other.v_ + S, v_ );
        *this /= scale;
        return *this;
    }

    // from homogenous
    template< class Y, std::size_t Z >
    typename enable_if< ( Z + 1 == S ), vector< T, S >& >::type
    operator= ( const vector< Y, Z >& other )
    {
        set( other, 1 );
        return *this;
    }

    void fill( const T& v ) { set( v ); }
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

    vector< T, S >& negate();
    template< class Y > vector< T, S >& negate( const vector< Y, S >& v )
    {
        set( v );
        return negate();
    }

    template< class Y > T dot( const vector< Y, S >& ) const;

    // cross products!

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

