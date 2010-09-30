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

#include <zi/utility/assert.hpp>
#include <zi/utility/enable_if.hpp>
#include <zi/utility/static_assert.hpp>
#include <zi/utility/static_if.hpp>

#include <zi/bits/type_traits.hpp>

#include <cstddef>
#include <ostream>
#include <numeric>
#include <cmath>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

template< class T, std::size_t S, bool Lvalue = true >
class vector
{
private:
    ZI_STATIC_ASSERT( S > 0 , zero_length_vector );
    ZI_STATIC_ASSERT( is_scalar< T >::value , non_scalar_vector );

    typedef typename if_< Lvalue, T[ S ], T* >::type storage_type;

    typedef vector< T, S, Lvalue > vector_t;

protected:
    storage_type v_;

public:
    template< class Y, std::size_t Z, bool BB > friend class vector;

    // ctors
    //---------------------------------------------------------------------

    vector()
    {
        ZI_VERIFY( Lvalue );
    }

    explicit vector( const storage_type& v ): v_( v )
    {
        ZI_VERIFY( !Lvalue );
    }

    explicit vector( const T& v )
    {
        ZI_VERIFY( Lvalue );
        set( v );
    }

    vector( const T& x, const T& y )
    {
        ZI_VERIFY( Lvalue );
        set( x, y );
    }

    vector( const T& x, const T& y, const T& z )
    {
        set( x, y, z );
        ZI_VERIFY( Lvalue );
    }

    vector( const T& x, const T& y, const T& z, const T& t )
    {
        ZI_VERIFY( Lvalue );
        set( x, y, z, t );
    }

    template< class Y, bool B >
    vector( const vector< Y, S, B >& v )
    {
        ZI_VERIFY( Lvalue );
        this->template set< Y, B >( v );
    }

    template< class Y, std::size_t Z, bool B >
    vector( const vector< Y, Z, B >& v, const T& t, ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        ZI_VERIFY( Lvalue );
        this->template set< Y, Z, B >( v, t );
    }

    template< class Y, std::size_t Z, bool B >
    vector( const T& t, const vector< Y, Z, B >& v, ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        ZI_VERIFY( Lvalue );
        this->template set< Y, Z, B >( t, v );
    }

    // to homogenous
    template< class Y, std::size_t Z, bool B >
    vector( const vector< Y, Z, B >& v, ZI_VL_ENABLE_IF( S + 1 == Z, Y ) = 0 )
    {
        ZI_VERIFY( Lvalue );
        this->template operator=< Y, Z, B >( v );
    }

    // from homogenous
    template< class Y, std::size_t Z, bool B >
    vector( const vector< Y, Z, B >& v, ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        ZI_VERIFY( Lvalue );
        this->template set< T, Z, B >( v, 1 );
    }

    // access
    //---------------------------------------------------------------------

    inline const T& at( std::size_t i ) const
    {
        ZI_VERIFY( S > i );
        return v_[ i ];
    }
    inline const T& operator[]( std::size_t i ) const { return at( i ); }
    inline const T& operator()( std::size_t i ) const { return at( i ); }

    inline T& at( std::size_t i )
    {
        ZI_VERIFY( S > i );
        return v_[ i ];
    }
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

    template< class Y, bool B > inline bool operator==( const vector< Y, S, B >& v ) const
    {
        return std::equal( v_, v_ + S, v.v_ );
    }

    template< class Y, bool B > inline bool operator< ( const vector< Y, S, B >& v ) const
    {
        return std::lexicographical_compare( v_, v_ + S, v.v_, v.v_ + S );
    }

    template< class Y, bool B > inline bool
    operator!=( const vector< Y, S, B >& o ) const { return !( *this == o ); }
    template< class Y, bool B > inline bool
    operator<=( const vector< Y, S, B >& o ) const { return !( o < *this );  }
    template< class Y, bool B > inline bool
    operator>=( const vector< Y, S, B >& o ) const { return !( *this < o );  }
    template< class Y, bool B > inline bool
    operator> ( const vector< Y, S, B >& o ) const { return o < *this;       }

    // unary arithmetic operators
    //---------------------------------------------------------------------

    template< class Y, bool B > inline vector_t& operator+=( const vector< Y, S, B >& o )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] += o.v_[ i ];
        }
        return *this;
    }

    template< class Y, bool B > inline vector_t& operator-=( const vector< Y, S, B >& o )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] -= o.v_[ i ];
        }
        return *this;
    }

    template< class Y, bool B > inline vector_t& operator*=( const vector< Y, S, B >& o )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] *= o.v_[ i ];
        }
        return *this;
    }

    template< class Y, bool B > inline vector_t& operator/=( const vector< Y, S, B >& o )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] /= o.v_[ i ];
        }
        return *this;
    }

    inline vector_t& operator+=( const T& v )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] += v;
        }
        return *this;
    }

    inline vector_t& operator-=( const T& v )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] -= v;
        }
        return *this;
    }

    inline vector_t& operator*=( const T& v )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] *= v;
        }
        return *this;
    }

    inline vector_t& operator/=( const T& v )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] /= v;
        }
        return *this;
    }

    inline vector< T, S > operator-() const
    {
        vector< T, S > r( *this );
        return r.negate();
    }

    // unary arithmetic operators
    //---------------------------------------------------------------------

    template< class Y, bool B > inline vector< T, S > operator+( const vector< Y, S, B >& o ) const
    {
        vector< T, S > r( *this );
        r += o;
        return r;
    }

    template< class Y, bool B > inline vector< T, S > operator-( const vector< Y, S, B >& o ) const
    {
        vector< T, S > r( *this );
        r -= o;
        return r;
    }

    template< class Y, bool B > inline vector< T, S > operator*( const vector< Y, S, B >& o ) const
    {
        vector< T, S > r( *this );
        r *= o;
        return r;
    }

    template< class Y, bool B > inline vector< T, S > operator/( const vector< Y, S, B >& o ) const
    {
        vector< T, S > r( *this );
        r /= o;
        return r;
    }


    inline vector< T, S > operator+( const T& v ) const
    {
        vector< T, S > r( *this );
        r += v;
        return r;
    }

    inline vector< T, S > operator-( const T& v ) const
    {
        vector< T, S > r( *this );
        r -= v;
        return r;
    }

    inline vector< T, S > operator*( const T& v ) const
    {
        vector< T, S > r( *this );
        r *= v;
        return r;
    }

    inline vector< T, S > operator/( const T& v ) const
    {
        vector< T, S > r( *this );
        r /= v;
        return r;
    }

    friend inline vector< T, S > operator+( const T& t, const vector_t& v ) { return  v + t; }
    friend inline vector< T, S > operator-( const T& t, const vector_t& v ) { return -v + t; }
    friend inline vector< T, S > operator*( const T& t, const vector_t& v ) { return  v * t; }

    // assignment operators
    //---------------------------------------------------------------------

    template< class Y, bool B >
    vector_t& operator=( const vector< Y, S, B >& other )
    {
        this->template set< Y, B >( other );
        return *this;
    }

    vector_t& operator=( const T& v )
    {
        this->set( v );
        return *this;
    }

    // to homogenous
    template< class Y, std::size_t Z, bool B >
    typename enable_if< ( Z == S + 1 ), vector_t& >::type
    operator= ( const vector< Y, Z, B >& other )
    {
        const T scale = other.v_[ S ];
        std::copy( other.v_, other.v_ + S, v_ );
        *this /= scale;
        return *this;
    }

    // from homogenous
    template< class Y, std::size_t Z, bool B >
    typename enable_if< ( Z + 1 == S ), vector_t& >::type
    operator= ( const vector< Y, Z, B >& other )
    {
        this->template set< Y, Z, B >( other, 1 );
        return *this;
    }

    void fill( const T& v ) { set( v ); }

    template< class Iterator > inline void assign( Iterator beg, Iterator end )
    {
        std::copy( beg, end, v_ );
    }

    void set( const T& v )
    {
        std::fill_n( v_, S, v );
    }

    void set( const T& x, const T& y )
    {
        ZI_VERIFY( S > 1 );
        v_[ 0 ] = x;
        v_[ 1 ] = y;
    }

    void set( const T& x, const T& y, const T& z )
    {
        ZI_VERIFY( S > 2 );
        v_[ 0 ] = x;
        v_[ 1 ] = y;
        v_[ 2 ] = z;
    }

    void set( const T& x, const T& y, const T& z, const T& t )
    {
        ZI_VERIFY( S > 3 );
        v_[ 0 ] = x;
        v_[ 1 ] = y;
        v_[ 2 ] = z;
        v_[ 3 ] = t;
    }

    template< class Y, bool B > inline void set( const vector< Y, S, B >& v )
    {
        std::copy( v.v_, v.v_ + S, v_ );
    }

    template< class Y, std::size_t Z, bool B >
    inline void set( const vector< Y, Z, B >& v, const T& x = T( 0 ),
                     ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        std::copy( v.v_, v.v_ + Z, v_ );
        v_[ Z ] = x;
    }

    template< class Y, std::size_t Z, bool B >
    inline void set( const T& x, const vector< Y, Z, B >& v,
                     ZI_VL_ENABLE_IF( S == Z + 1, Y ) = 0 )
    {
        std::copy( v.v_, v.v_ + Z, v_ + 1 );
        v_[ 0 ] = x;
    }

    template< class Y, bool B > inline bool equal( const vector< T, S, B >& o ) { return *this == o; }

    vector_t& negate()
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] = - v_[ i ];
        }
        return *this;
    }

    template< class Y, bool B > inline vector_t& negate( const vector< Y, S, B >& v )
    {
        this->template set< Y, B >( v );
        return negate();
    }

    template< class Y, bool B > inline T dot( const vector< Y, S, B >& v ) const
    {
        return std::inner_product( v_, v_ + S, v.v_, T( 0 ) );
    }

    // cross products
    //---------------------------------------------------------------------

    vector_t& cross()
    {
        ZI_VERIFY( S == 2 );
        T v( v_[ 0 ] );
        v_[ 0 ] = v_[ 1 ];
        v_[ 1 ] = -v;
        return *this;
    }

    template< class Y, std::size_t Z, bool B >
    vector_t& cross( const vector< Y, Z, B >& v,
                     ZI_VL_ENABLE_IF( S == 2 && Z == S, Y ) = 0 )
    {
        T a( v.v_[ 0 ] );
        v_[ 0 ] =  v.v_[ 1 ];
        v_[ 1 ] = -a;
        return *this;
    }

    template< class Y, std::size_t Z, bool B >
    vector_t& cross( const vector< Y, Z, B >& v,
                     ZI_VL_ENABLE_IF( S == 3 && Z == S, Y ) = 0 )
    {
        if ( this == &v )
        {
            this->set( 0 );
        }
        else
        {
            T v0( v_[ 0 ] );
            T v1( v_[ 1 ] );
            v_[ 0 ] = v1 * v.v_[ 2 ] - v_[ 2 ] * v.v_[ 1 ];
            v_[ 1 ] = v_[ 2 ] * v.v_[ 0 ] - v0 * v.v_[ 2 ];
            v_[ 2 ] = v0 * v.v_[ 1 ] - v1 * v.v_[ 0 ];
        }
        return *this;
    }

    template< class Y, class W, std::size_t Z, bool B1, bool B2 >
    vector_t& cross( const vector< Y, Z, B1 >& v1,
                     const vector< W, Z, B2 >& v2,
                     ZI_VL_ENABLE_IF( S == 3 && Z == S, Y ) = 0 )
    {
        T a0( v1.v_[ 1 ] * v2.v_[ 2 ] - v1.v_[ 2 ] * v2.v_[ 1 ] );
        T a1( v1.v_[ 2 ] * v2.v_[ 0 ] - v1.v_[ 0 ] * v2.v_[ 2 ] );
        v_[ 2 ] = v1.v_[ 0 ] * v2.v_[ 1 ] - v1.v_[ 1 ] * v2.v_[ 0 ];

        v_[ 0 ] = a0;
        v_[ 1 ] = a1;

        return *this;
    }

    template< class Y, class W, std::size_t Z, bool B1, bool B2 >
    vector_t& cross( const vector< Y, Z, B1 >& v1,
                     const vector< W, Z, B2 >& v2,
                     ZI_VL_ENABLE_IF( S == 4 && Z == S, Y ) = 0 )
    {
        T a0( v_[ 1 ]    * ( v1.v_[ 2 ] * v2.v_[ 3 ] - v1.v_[ 3 ] * v2.v_[ 2 ] ) +
              v1.v_[ 1 ] * ( v2.v_[ 2 ] *    v_[ 3 ] - v2.v_[ 3 ] *    v_[ 2 ] ) +
              v2.v_[ 1 ] * (    v_[ 2 ] * v1.v_[ 3 ] -    v_[ 3 ] * v1.v_[ 2 ] ) );

        T a1( v_[ 0 ]    * ( v1.v_[ 2 ] * v2.v_[ 3 ] - v1.v_[ 3 ] * v2.v_[ 2 ] ) +
              v1.v_[ 0 ] * ( v2.v_[ 2 ] *    v_[ 3 ] - v2.v_[ 3 ] *    v_[ 2 ] ) +
              v2.v_[ 0 ] * (    v_[ 2 ] * v1.v_[ 3 ] -    v_[ 3 ] * v1.v_[ 2 ] ) );

        T a2( v_[ 0 ]    * ( v1.v_[ 1 ] * v2.v_[ 3 ] - v1.v_[ 3 ] * v2.v_[ 1 ] ) +
              v1.v_[ 0 ] * ( v2.v_[ 1 ] *    v_[ 3 ] - v2.v_[ 3 ] *    v_[ 1 ] ) +
              v2.v_[ 0 ] * (    v_[ 1 ] * v1.v_[ 3 ] -    v_[ 3 ] * v1.v_[ 1 ] ) );

        v_[ 3 ] =
            v_[ 0 ]    * ( v1.v_[ 1 ] * v2.v_[ 2 ] - v1.v_[ 2 ] * v2.v_[ 1 ] ) +
            v1.v_[ 0 ] * ( v2.v_[ 1 ] *    v_[ 2 ] - v2.v_[ 2 ] *    v_[ 1 ] ) +
            v2.v_[ 0 ] * (    v_[ 1 ] * v1.v_[ 2 ] -    v_[ 2 ] * v1.v_[ 1 ] );

        v_[ 0 ] = a0;
        v_[ 1 ] = a1;
        v_[ 2 ] = a2;

        return *this;
    }

    template< class Y, class W, class X, std::size_t Z, bool B1, bool B2, bool B3 >
    vector_t& cross( const vector< Y, Z, B1 >& v1,
                     const vector< W, Z, B2 >& v2,
                     const vector< X, Z, B3 >& v3,
                     ZI_VL_ENABLE_IF( S == 4 && S == Z, Y ) = 0 )
    {
        vector< T, S > r( v1 );
        r.template cross< W, X, Z, B2, B3 >( v2, v3 );
        this->template set< T, true >( r );
        return *this;
    }


    inline T length() const
    {
        return std::sqrt( squared_length() );
    }

    inline T squared_length() const
    {
        T r( 0 );
        for ( std::size_t i = 0; i < S; ++i )
        {
            r += v_[ i ] * v_[ i ];
        }
        return r;
    }

    inline T normalize()
    {
        T r = length();
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] /= r;
        }
        return r;
    }

    template< class Y, bool B > inline T normalize( const vector< Y, S, B >& o )
    {
        this->template set< Y, B >( o );
        return normalize();
    }

    template< class Y, bool B > inline T distance( const vector< Y, S, B >& o ) const
    {
        return std::sqrt( squared_distance( o ) );
    }

    template< class Y, bool B > inline T squared_distance( const vector< Y, S, B >& v ) const
    {
        T r( 0 );
        for ( std::size_t i = 0; i < S; ++i )
        {
            T d = v_[ i ] - v.v_[ i ];
            r += d * d;
        }
        return r;
    }

    template< class Y, bool B1, bool B2 > vector_t& normal( const vector< Y, S, B1 >& x,
                                                            const vector< Y, S, B2 >& y,
                                                            ZI_VL_ENABLE_IF( S == 3, Y ) = 0 )
    {
        vector< T, S > z( x );
        z -= y;
        *this -= x;
        cross( z );
        normalize();
        return *this;
    }

    template< class Y, bool B1, bool B2, bool B3 >
    vector_t& normal( const vector< Y, S, B1 >& x,
                      const vector< Y, S, B2 >& y,
                      const vector< Y, S, B3 >& z,
                      ZI_VL_ENABLE_IF( S == 3, Y ) = 0 )
    {
        *this = x;
        return this->template normal< Y, B2, B3 >( y, z );
    }


    std::size_t size() const { return S; }

    inline vector_t& clamp( const T& minv = T( 0 ), const T& maxv = T( 1 ) )
    {
        for ( std::size_t i = 0; i < S; ++i )
        {
            v_[ i ] = ( v_[ i ] < minv ? minv : ( v_[ i ] > maxv ? maxv : v_[ i ] ) );
        }
        return *this;
    }

    template< typename Y > void scale_to( const T& = T( -1 ), const T& = T( 1 ) );
    template< typename Y > void scale_to( vector< Y, S >&, const T& = T( -1 ), const T& = T( 1 ) );

    static const vector< T, S, true > one;
    static const vector< T, S, true > zero;

    friend inline std::ostream& operator<< ( std::ostream& os, const vector_t& v )
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

template< class T, class Y, std::size_t S, bool B1, bool B2 >
inline T dot( const vector< T, S, B1 >& x, const vector< Y, S, B2 >& y )
{
    return x.template dot< Y, B2 >( y );
}

template< class T, std::size_t S, bool B >
inline T length( const vector< T, S, B >& x )
{
    return x.length();
}

template< class T, std::size_t S, bool B >
inline T squared_length( const vector< T, S, B >& x )
{
    return x.squared_length();
}

template< class T, std::size_t S, bool B >
inline T normalize( const vector< T, S, B >& x )
{
    return x.normalize();
}

template< class T, class Y, std::size_t S, bool B1, bool B2 >
inline T distance( const vector< T, S, B1 >& x, const vector< Y, S, B2 >& y )
{
    return x.template distance< Y, B2 >( y );
}

template< class T, class Y, std::size_t S, bool B1, bool B2 >
inline T squared_distance( const vector< T, S, B1 >& x, const vector< Y, S, B2 >& y )
{
    return x.template squared_distance< Y, B2 >( y );
}

template< class T, bool B >
inline vector< T, 2 > cross( const vector< T, 2, B >& x )
{
    vector< T, 2 > r( x );
    r.cross();
    return r;
}

template< class T, class Y, bool B1, bool B2 >
inline vector< T, 3 > cross( const vector< T, 3, B1 >& x, const vector< Y, 3, B2 >& y )
{
    vector< T, 3 > r( x );
    r.template cross< Y, 3, B2 >( y );
    return r;
}

template< class T, class Y, class W, bool B1, bool B2, bool B3 >
inline vector< T, 4 > cross( const vector< T, 4, B1 >& x,
                             const vector< Y, 4, B2 >& y,
                             const vector< W, 4, B3 >& z )
{
    vector< T, 4 > r( x );
    r.template cross< Y, W, 4, B2, B3 >( y, z );
    return r;
}

template< class T, bool B1, bool B2, bool B3 >
inline vector< T, 3 > normal( const vector< T, 3, B1 >& x,
                              const vector< T, 3, B2 >& y,
                              const vector< T, 3, B3 >& z )
{
    vector< T, 3 > r( x );
    r.template normal< T, B2, B3 >( y, z );
    return r;
}

template< class T, std::size_t S, bool B >
const vector< T, S, true > vector< T, S, B >::one( 1 );

template< class T, std::size_t S, bool B >
const vector< T, S, true > vector< T, S, B >::zero( 0 );


} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

