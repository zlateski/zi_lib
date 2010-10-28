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

#ifndef ZI_VL_QUADRATICS_HPP
#define ZI_VL_QUADRATICS_HPP 1

#include <zi/vl/vector.hpp>
#include <zi/vl/matrix.hpp>

#include <zi/utility/assert.hpp>
#include <zi/utility/static_assert.hpp>

#include <zi/bits/type_traits.hpp>

#include <ostream>
#include <iomanip>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

template< class T >
class quadratics
{
private:
    ZI_STATIC_ASSERT( is_floating_point< T >::value , non_floating_point_quadratics );

private:
    T    a2,   ab,   ac,   ad;
    T /* ab,*/ b2,   bc,   bd;
    T /* ac,   bc,*/ c2,   cd;
    T /* ad,   bd,   cd,*/ d2;

    T A;

public:
    inline void area( const T& a )
    {
        A = a;
    }

    inline T area() const
    {
        return A;
    }

    inline T offset() const
    {
        return d2;
    }

    inline ::zi::vl::vector< T, 3 > get_vector() const
    {
        return ::zi::vl::vector< T, 3 >( ad, bd, cd );
    }

    inline void get_vector( ::zi::vl::vector< T, 3 >& v ) const
    {
        v.at( 0 ) = ad;
        v.at( 1 ) = bd;
        v.at( 2 ) = cd;
    }


private:
    inline void init( const T &a, const T &b, const T &c, const T &d, const T &area )
    {
        a2 = a * a; b2 = b * b; c2 = c * c; d2 = d * d;
        ab = a * b; ac = a * c; ad = a * d;
        bc = b * c; bd = b * d;
        cd = c * d;
        A = area;
    }

    template< class Y >
    inline void init( const matrix< Y, 4 > &m, const T &area )
    {
        a2 = m.at( 0, 0 );  ab = m.at( 0, 1 );  ac = m.at( 0, 2 );  ad = m.at( 0, 3 );
        b2 = m.at( 1, 1 );  bc = m.at( 1, 2 );  bd = m.at( 1, 3 );
        c2 = m.at( 2, 2 );  cd = m.at( 2, 3 );
        d2 = m.at( 3, 3 );
        A = area;
    }


public:

    inline quadratics()
    {
        clear();
    }

    inline quadratics( const T &a, const T &b, const T &c, const T &d, T area = 1.0 )
    {
        init( a, b, c, d, area );
    }

    template< class Y >
    inline quadratics( const quadratics< Y >& o )
    {
        *this = o;
    }

    template< class Y >
    inline quadratics& operator=( const quadratics< Y >& x )
    {
        a2 = x.a2; b2 = x.b2; c2 = x.c2; d2 = x.d2;
        ab = x.ab; ac = x.ac; ad = x.ad;
        bc = x.bc; bd = x.bd;
        cd = x.cd;
        A  = x.A;
        return *this;
    }

    template< class Y >
    inline quadratics& operator+=( const quadratics< Y >& x )
    {
        a2 += x.a2; b2 += x.b2; c2 += x.c2; d2 += x.d2;
        ab += x.ab; ac += x.ac; ad += x.ad;
        bc += x.bc; bd += x.bd;
        cd += x.cd;
        A  += x.A;
        return *this;
    }

    template< class Y >
    inline quadratics& operator-=( const quadratics< Y >& x )
    {
        a2 -= x.a2; b2 -= x.b2; c2 -= x.c2; d2 -= x.d2;
        ab -= x.ab; ac -= x.ac; ad -= x.ad;
        bc -= x.bc; bd -= x.bd;
        cd -= x.cd;
        A  -= x.A;
        return *this;
    }

    inline quadratics& operator*=( const T& c )
    {
        a2 *= c; b2 *= c; c2 *= c; d2 *= c;
        ab *= c; ac *= c; ad *= c;
        bc *= c; bd *= c;
        cd *= c;
        return *this;
    }

    template< class Y >
    inline quadratics operator+( const quadratics< Y >& y )
    {
        quadratics< T > q( *this );
        q += y;
        return q;
    }

    template< class Y >
    inline quadratics operator-( const quadratics< Y >& y )
    {
        quadratics< T > q( *this );
        q -= y;
        return q;
    }

    inline T evaluate( const T& x, const T& y, const T& z ) const
    {
        return ( x * ( x * a2 + ( y * ab + z * ac + ad ) * 2.0 ) +
                 y * ( y * b2 + ( z * bc + bd ) * 2.0 ) +
                 z * ( z * c2 + cd * 2.0 ) +
                 d2 );

    }

    template< class Y >
    inline T evaluate( const vector< Y, 3 >& v ) const
    {
        return evaluate( v.at( 0 ), v.at( 1 ), v.at( 2 ) );
    }

    inline T operator()( const T& x, const T& y, const T& z ) const
    {
        return evaluate( x, y, z );
    }

    template< class Y >
    inline T operator()( const vector< Y, 3 >& v ) const
    {
        return evaluate( v.at( 0 ), v.at( 1 ), v.at( 2 ) );
    }

    inline void get_tensor( matrix< T, 3 >& m ) const
    {
        m.at( 0, 0 ) = a2;
        m.at( 1, 1 ) = b2;
        m.at( 2, 2 ) = c2;
        m.at( 1, 0 ) = m.at( 0, 1 ) = ab;
        m.at( 2, 0 ) = m.at( 0, 2 ) = ac;
        m.at( 2, 1 ) = m.at( 1, 2 ) = bc;
    }

    inline matrix< T, 3 > get_tensor() const
    {
        matrix< T, 3 > r;
        get_tensor( r );
        return r;
    }

    inline void homogenous( matrix< T, 4 >& m ) const
    {
        m.at( 0, 0 ) = a2;
        m.at( 1, 1 ) = b2;
        m.at( 2, 2 ) = c2;
        m.at( 3, 3 ) = d2;
        m.at( 1, 0 ) = m.at( 0, 1 ) = ab;
        m.at( 2, 0 ) = m.at( 0, 2 ) = ac;
        m.at( 3, 0 ) = m.at( 0, 3 ) = ad;
        m.at( 2, 1 ) = m.at( 1, 2 ) = bc;
        m.at( 3, 1 ) = m.at( 1, 3 ) = bd;
        m.at( 3, 2 ) = m.at( 2, 3 ) = cd;
    }

    inline matrix< T, 4 > homogenous() const
    {
        matrix< T, 4 > r;
        homogenous( r );
        return r;
    }

    inline void clear( const T& x = 0 )
    {
        a2 = ab = ac = ad = b2 = bc = bd = c2 = cd = d2 = A = x;
    }

    inline quadratics& set_transform( const matrix< T, 4 >& t )
    {
        matrix< T, 4 > q, a;
        homogenous( q );
        q.get_adjugate( a );
        q = a * q;
        q *= a;
        init( q, A );
        return *this;
    }

    inline bool optimize( ::zi::vl::vector< T, 3 >& v ) const
    {
        matrix< T, 3 > ainv;
        get_tensor( ainv );
        if ( ainv.invert() )
        {
            ainv.multiply_to( this->get_vector(), v );
            v.negate();
            return true;
        }
        return false;
    }

    inline bool optimize( T &x, T& y, T& z )
    {
        ::zi::vl::vector< T, 3 > v;
        if ( optimize( v ) )
        {
            x = v.at( 0 );
            y = v.at( 1 );
            z = v.at( 2 );
            return true;
        }
        return false;
    }

    inline bool optimize( vector< T, 3 >& v,
                          const vector< T, 3 >& v1,
                          const vector< T, 3 >& v2 ) const
    {
        vector< T, 3 > d = v1 - v2;
        matrix< T, 3 > a;
        get_tensor( a );
        vector< T, 3 > av2 = a * v2;
        vector< T, 3 > ad  = a * d;

        const T denom = d.dot( ad ) * 2;

        if ( std::fabs( denom ) <= std::numeric_limits< T >::epsilon() )
        {
            return false;
        }

        vector< T, 3 > vc;
        get_vector( vc );

        T invdenom = static_cast< T >( 1 ) / denom;
        T q = -( vc.dot( d ) * 2 + d.dot( av2 ) + v2.dot( ad ) ) * invdenom;

        q = q < 0 ? 0 : ( q > 1 ? 1 : q );

        v = q * d + v2;

        return true;
    }


    inline bool optimize( vector< T, 3 >& v,
                          const vector< T, 3 >&v1,
                          const vector< T, 3 >&v2,
                          const vector< T, 3 >&v3 ) const
    {
        vector< T, 3 > d13( v1 ); d13 -= v3;
        vector< T, 3 > d23( v2 ); d23 -= v3;

        matrix< T, 3 > A; get_tensor( A );
        vector< T, 3 > b; get_vector( b );

        vector< T, 3 > Ad13; A.multiply_to( d13, Ad13 );
        vector< T, 3 > Ad23; A.multiply_to( d23, Ad23 );
        vector< T, 3 > Av3;  A.multiply_to( v3, Av3 );

        const T d13_d23 = d13 * Ad23 + d23 * Ad13;
        const T v3_d13  = d13 * Av3 + v3 * Ad13;
        const T v3_d23  = d23 * Av3 + v3 * Ad23;

        const T d23Ad23 = d23 * Ad23;
        const T d13Ad13 = d13 * Ad13;

        const T denom = d13Ad13 * d23Ad23 - d13_d23 * 2;

        if ( std::fabs( denom ) <= std::numeric_limits< T >::epsilon() )
        {
            return false;
        }

        T invdenom = static_cast< T >( 1 ) / denom;

        T a = ( d23Ad23 * 2 * b * d13 + v3_d13 -
                ( d13_d23 * 2 * b * d23 + v3_d23 ) ) * -invdenom;

        T q = ( d13Ad13 * 2 * b * d23 + v3_d23 -
                ( d13_d23 * 2 * b * d13 + v3_d13 ) ) * -invdenom;


        a = a < 0 ? 0 : ( a > 1 ? 1 : a );
        q = q < 0 ? 0 : ( q > 1 ? 1 : q );

        v = a * d13 + q * d23 + v3;
        return true;
    }

    friend inline std::ostream& operator<< ( std::ostream& os, const quadratics& q )
    {
        std::cout << "/ " << q.a2 << ' ' << q.ab << ' ' << q.ac << ' ' << q.ad << " \\\n"
                  << "| " << q.ab << ' ' << q.b2 << ' ' << q.bc << ' ' << q.bd << " |\n"
                  << "| " << q.ac << ' ' << q.bc << ' ' << q.c2 << ' ' << q.cd << " |\n"
                  << "\\ " << q.ad << ' ' << q.bd << ' ' << q.cd << ' ' << q.d2 << " /\n";
        return os;
    }



};

} // namespace vl
} // namespace zi

#include <zi/vl/vl_suffix.hpp>

#endif

