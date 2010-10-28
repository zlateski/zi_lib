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

#ifndef ZI_VL_QUATERNION_HPP
#define ZI_VL_QUATERNION_HPP 1

#include <zi/vl/matrix.hpp>

#include <zi/vl/vl_prefix.hpp>

namespace zi {
namespace vl {

template< class T >
class quaternion
    : private vector< T, 4, true >
{
private:
    ZI_STATIC_ASSERT( is_floating_point< T >::value , non_floating_point_quaternion );

    typedef T                                        value_type   ;
    typedef vector< T, 4, true >                     base         ;
    typedef quaternion< T >                          quaternion_t ;

    using base::v_;

public:

    template< class Y > friend class quaternion;

    using base::operator[];
    using base::operator();
    using base::operator= ;
    using base::at;
    using base::x;
    using base::y;
    using base::z;
    using base::w;
    using base::t;
    using base::hash_value;

    struct hash:
        public std::unary_function< quaternion< T >, std::size_t >
    {
        std::size_t operator()( const quaternion< T >& q ) const
        {
            return q.hash_value();
        }
    };

    inline quaternion()
    {
    }

    inline quaternion( const T& x, const T& y, const T& z, const T& t )
    {
        base::set( x, y, z, t );
    }

    template< class Y, bool B >
    inline quaternion( const vector< Y, 3, B >& v )
    {
        base::template set< Y, 3, B >( v, static_cast< T >( 0 ) );
    }

    template< class Y, bool B >
    inline quaternion( const vector< Y, 3, B >& v, const T& t )
    {
        base::template set< Y, 3, B >( v, t );
    }

    template< class Y, std::size_t S >
    inline quaternion( const matrix< Y, S >& m, ZI_VL_ENABLE_IF( S >= 3, Y ) = 0 )
    {
        this->template set< Y, S >( m );
    }

    inline void set( const T& x, const T& y, const T& z, const T& t )
    {
        base::set( x, y, z, t );
    }

    template< class Y, bool B >
    inline void set( const vector< Y, 3, B >& v, const T& t )
    {
        base::template set< Y, 3, B>( v, t );
    }

    template< class Y, bool B >
    inline void set( const vector< Y, 3, B >& v )
    {
        base::template set< Y, 3, B>( v, static_cast< T >( 0 ) );
    }

    template< class Y, std::size_t S >
    inline void set( const matrix< Y, S >& m, ZI_VL_ENABLE_IF( S >= 3, Y ) = 0 )
    {
        static const T epsilon = std::sqrt( std::numeric_limits< T >::epsilon() );

        const T trace = m.at( 0, 0 ) + m.at( 1, 1 ) + m.at( 2, 2 ) + 1;

        if ( trace > epsilon )
        {
            const T r = std::sqrt( trace );
            const T s = static_cast< T >( 0.5 ) / r;

            v_[ 0 ] = m.at( 2, 1 ) - m.at( 1, 2 );
            v_[ 0 ] *= s;

            v_[ 1 ] = m.at( 0, 2 ) - m.at( 2, 0 );
            v_[ 1 ] *= s;

            v_[ 2 ] = m.at( 1, 0 ) - m.at( 0, 1 );
            v_[ 2 ] *= s;

            v_[ 3 ] = 0.5;
            v_[ 3 ] *= r;
        }
        else
        {
            std::size_t idx = std::max_element( v_, v_ + 4 ) - v_;

            if ( idx == 0 )
            {
                const T r = std::sqrt( m.at( 0, 0 ) + 1.0 - m.at( 1, 1 ) - m.at( 2, 2 ));
                const T s = static_cast< T >( 0.5 ) / r;

                v_[ 0 ] = 0.5;
                v_[ 0 ] *= r;

                v_[ 1 ] = m.at( 0, 1 ) + m.at( 1, 0 );
                v_[ 1 ] *= s;

                v_[ 2 ] = m.at( 0, 2 ) + m.at( 2, 0 );
                v_[ 2 ] *= s;

                v_[ 3 ] = m.at( 1, 2 ) - m.at( 2, 1 );
                v_[ 3 ] *= s;

            }
            else if ( idx == 1 )
            {
                const T r = std::sqrt( m.at( 1, 1 ) + 1.0 - m.at( 0, 0 ) - m.at( 2, 2 ));
                const T s = static_cast< T >( 0.5 ) / r;

                v_[ 0 ] = m.at( 0, 1 ) + m.at( 1, 0 );
                v_[ 0 ] *= s;

                v_[ 1 ] = 0.5;
                v_[ 1 ] *= r;

                v_[ 2 ] = m.at( 1, 2 ) + m.at( 2, 1 );
                v_[ 2 ] *= s;

                v_[ 3 ] = m.at( 0, 2 ) - m.at( 2, 0 );
                v_[ 3 ] *= s;
            }
            else if ( idx == 2 )
            {
                const T r = std::sqrt( m.at( 2, 2 ) + 1.0 - m.at( 0, 0 ) - m.at( 1, 1 ));
                const T s = static_cast< T >( 0.5 ) / r;

                v_[ 0 ] = m.at( 0, 2 ) + m.at( 2, 0 );
                v_[ 0 ] *= s;

                v_[ 1 ] = m.at( 1, 2 ) + m.at( 2, 1 );
                v_[ 1 ] *= s;

                v_[ 2 ] = 0.5;
                v_[ 2 ] *= r;

                v_[ 3 ] = m.at( 0, 1 ) - m.at( 1, 0 );
                v_[ 3 ] *= s;
            }
            else
            {
                ZI_THROW( "no max on the matrix diagonal" );
            }

        }

    }

    inline void to_identity()
    {
        base::set( 0, 0, 0, 1 );
    }

    inline bool operator==( const T& t ) const
    {
        return v_[ 3 ] == t && v_[ 0 ] == 0 && v_[ 1 ] == 0 && v_[ 2 ] == 0;
    }

    inline bool operator!=( const T& t ) const
    {
        return v_[ 3 ] != t || v_[ 0 ] != 0 || v_[ 1 ] != 0 || v_[ 2 ] != 0;
    }


    template< class Y >
    inline bool operator==( const quaternion< Y >& o ) const
    {
        return std::equal( v_, v_ + 4, o.v_ );
    }

    template< class Y >
    inline bool operator!=( const quaternion< Y >& o ) const
    {
        return !std::equal( v_, v_ + 4, o.v_ );
    }

    inline quaternion_t& conjugate()
    {
        v_[ 0 ] = -v_[ 0 ];
        v_[ 1 ] = -v_[ 1 ];
        v_[ 2 ] = -v_[ 2 ];
        return *this;
    }

    template< class Y >
    inline quaternion_t& conjugate( const quaternion< Y >& o )
    {
        v_[ 0 ] = -o.v_[ 0 ];
        v_[ 1 ] = -o.v_[ 1 ];
        v_[ 2 ] = -o.v_[ 2 ];
        v_[ 3 ] =  o.v_[ 3 ];
        return *this;
    }

    inline quaternion_t get_conjugate() const
    {
        return quaternion_t( -v_[ 0 ], -v_[ 1 ], -v_[ 2 ], v_[ 3 ] );
    }

    inline quaternion_t& negate()
    {
        v_[ 0 ] = -v_[ 0 ];
        v_[ 1 ] = -v_[ 1 ];
        v_[ 2 ] = -v_[ 2 ];
        v_[ 3 ] = -v_[ 3 ];
        return *this;
    }

    template< class Y >
    inline quaternion_t& negate( const quaternion< Y >& o )
    {
        v_[ 0 ] = -o.v_[ 0 ];
        v_[ 1 ] = -o.v_[ 1 ];
        v_[ 2 ] = -o.v_[ 2 ];
        v_[ 3 ] = -o.v_[ 3 ];
        return *this;
    }

    inline quaternion_t get_negate() const
    {
        return quaternion_t( -v_[ 0 ], -v_[ 1 ], -v_[ 2 ], -v_[ 3 ] );
    }

    inline quaternion_t operator-() const
    {
        return quaternion_t( -v_[ 0 ], -v_[ 1 ], -v_[ 2 ], -v_[ 3 ] );
    }

    inline T length() const
    {
        return std::sqrt( squared_length() );
    }

    inline T squared_length() const
    {
        return v_[ 0 ] * v_[ 0 ] + v_[ 1 ] * v_[ 1 ] + v_[ 2 ] * v_[ 2 ] + v_[ 3 ] * v_[ 3 ];
    }

    inline T abs() const
    {
        return std::sqrt( squared_length() );
    }

    inline T squared_abs() const
    {
        return v_[ 0 ] * v_[ 0 ] + v_[ 1 ] * v_[ 1 ] + v_[ 2 ] * v_[ 2 ] + v_[ 3 ] * v_[ 3 ];
    }

    template< class Y >
    inline quaternion_t& operator+=( const quaternion< Y >& o )
    {
        v_[ 0 ] += o.v_[ 0 ];
        v_[ 1 ] += o.v_[ 1 ];
        v_[ 2 ] += o.v_[ 2 ];
        v_[ 3 ] += o.v_[ 3 ];
        return *this;
    }

    template< class Y >
    inline quaternion_t& operator-=( const quaternion< Y >& o )
    {
        v_[ 0 ] -= o.v_[ 0 ];
        v_[ 1 ] -= o.v_[ 1 ];
        v_[ 2 ] -= o.v_[ 2 ];
        v_[ 3 ] -= o.v_[ 3 ];
        return *this;
    }

    template< class Y >
    inline quaternion_t& operator*=( const quaternion< Y >& o )
    {
        // Henrik Engstrom, from a gamedev.net article.

        const T& x0 = v_[ 3 ];
        const T& x1 = v_[ 0 ];
        const T& x2 = v_[ 1 ];
        const T& x3 = v_[ 2 ];
        const T& y0 = o.v_[ 3 ];
        const T& y1 = o.v_[ 0 ];
        const T& y2 = o.v_[ 1 ];
        const T& y3 = o.v_[ 2 ];

        const T tmp_00 = ( x3 - x2 ) * (y2 - y3);
        const T tmp_01 = ( x0 + x1 ) * (y0 + y1);
        const T tmp_02 = ( x0 - x1 ) * (y2 + y3);
        const T tmp_03 = ( x2 + x3 ) * (y0 - y1);
        const T tmp_04 = ( x3 - x1 ) * (y1 - y2);
        const T tmp_05 = ( x3 + x1 ) * (y1 + y2);
        const T tmp_06 = ( x0 + x2 ) * (y0 - y3);
        const T tmp_07 = ( x0 - x2 ) * (y0 + y3);
        const T tmp_08 = tmp_05 + tmp_06 + tmp_07;
        const T tmp_09 = 0.5 * (tmp_04 + tmp_08);

        v_[ 3 ] = tmp_00 + tmp_09 - tmp_05;
        v_[ 0 ] = tmp_01 + tmp_09 - tmp_08;
        v_[ 1 ] = tmp_02 + tmp_09 - tmp_07;
        v_[ 2 ] = tmp_03 + tmp_09 - tmp_06;

        return *this;
    }

    template< class Y >
    inline quaternion_t& operator/=( const quaternion< Y >& o )
    {
        quaternion_t q( o );
        q.invert();
        return this->operator*=( q );
    }

    inline quaternion_t& operator+=( const T& c )
    {
        v_[ 0 ] += c;
        v_[ 1 ] += c;
        v_[ 2 ] += c;
        v_[ 3 ] += c;
        return *this;
    }

    inline quaternion_t& operator-=( const T& c )
    {
        v_[ 0 ] -= c;
        v_[ 1 ] -= c;
        v_[ 2 ] -= c;
        v_[ 3 ] -= c;
        return *this;
    }

    inline quaternion_t& operator*=( const T& c )
    {
        v_[ 0 ] *= c;
        v_[ 1 ] *= c;
        v_[ 2 ] *= c;
        v_[ 3 ] *= c;
        return *this;
    }

    inline quaternion_t& operator/=( const T& c )
    {
        T invc = static_cast< T >( 1 ) / c;
        v_[ 0 ] *= invc;
        v_[ 1 ] *= invc;
        v_[ 2 ] *= invc;
        v_[ 3 ] *= invc;
        return *this;
    }

    template< class Y, bool B >
    inline quaternion_t& operator+=( const vector< Y, 3, B >& o )
    {
        v_[ 0 ] += o.v_[ 0 ];
        v_[ 1 ] += o.v_[ 1 ];
        v_[ 2 ] += o.v_[ 2 ];
        return *this;
    }

    template< class Y, bool B >
    inline quaternion_t& operator-=( const vector< Y, 3, B >& o )
    {
        v_[ 0 ] -= o.v_[ 0 ];
        v_[ 1 ] -= o.v_[ 1 ];
        v_[ 2 ] -= o.v_[ 2 ];
        return *this;
    }

    template< class Y, bool B >
    inline quaternion_t& operator*=( const vector< Y, 3, B >& o )
    {
        const T v0 = v_[ 0 ];
        const T v1 = v_[ 1 ];
        const T v2 = v_[ 2 ];

        v_[ 0 ] = v_[ 3 ] * o.v_[ 0 ] + v1 * o.v_[ 2 ] - v2 * o.v_[ 1 ];
        v_[ 1 ] = v_[ 3 ] * o.v_[ 1 ] + v2 * o.v_[ 0 ] - v0 * o.v_[ 2 ];
        v_[ 2 ] = v_[ 3 ] * o.v_[ 2 ] + v0 * o.v_[ 1 ] - v1 * o.v_[ 0 ];
        v_[ 3 ] = -v0 * o.v_[ 0 ] - v1 * o.v_[ 1 ] - v2 * o.v_[ 2 ];

        return *this;
    }

    template< class Y >
    inline vector< T, 3 > cross( const quaternion< Y >& o ) const
    {
        return vector< T, 3 >( v_[ 1 ] * o.v_[ 2 ] - v_[ 2 ] * o.v_[ 1 ],
                               v_[ 2 ] * o.v_[ 0 ] - v_[ 0 ] * o.v_[ 2 ],
                               v_[ 0 ] * o.v_[ 1 ] - v_[ 1 ] * o.v_[ 0 ] );
    }

    template< class Y >
    friend inline vector< T, 3 > cross( const quaternion_t& a, const quaternion< Y >& o )
    {
        return vector< T, 3 >( a.v_[ 1 ] * o.v_[ 2 ] - a.v_[ 2 ] * o.v_[ 1 ],
                               a.v_[ 2 ] * o.v_[ 0 ] - a.v_[ 0 ] * o.v_[ 2 ],
                               a.v_[ 0 ] * o.v_[ 1 ] - a.v_[ 1 ] * o.v_[ 0 ] );
    }

    template< class Y >
    inline T dot( const quaternion< Y >& o ) const
    {
        return std::inner_product( v_, v_ + 4, o.v_, static_cast< T >( 0 ) );
    }

    template< class Y >
    friend inline T dot( const quaternion_t& a, const quaternion< Y >& b )
    {
        return std::inner_product( a.v_, a.v_ + 4, b.v_, static_cast< T >( 0 ) );
    }

    inline T normalize()
    {
        T r = length();
        if ( std::fabs( r ) < std::numeric_limits< T >::epsilon() )
        {
            return 0;
        }

        T s = static_cast< T >( 1 ) / r;
        this->operator*=( s );

        return r;
    }

    inline quaternion_t get_normalized() const
    {
        quaternion_t q( *this );
        q.normalize();
        return q;
    }

    inline quaternion_t normalized() const
    {
        quaternion_t q( *this );
        q.normalize();
        return q;
    }

    inline quaternion_t& invert()
    {
        T r = static_cast< T >( 1 ) / squared_abs();
        this->conjugate();
        return this->operator*=( r );
    }

    inline quaternion_t inverse() const
    {
        quaternion_t q( *this );
        return q.invert();
    }

    inline quaternion_t operator+( const T& c ) const
    {
        quaternion_t q( *this );
        q += c;
        return q;
    }

    inline quaternion_t operator-( const T& c ) const
    {
        quaternion_t q( *this );
        q -= c;
        return q;
    }

    inline quaternion_t operator*( const T& c ) const
    {
        quaternion_t q( *this );
        q *= c;
        return q;
    }

    inline quaternion_t operator/( const T& c ) const
    {
        quaternion_t q( *this );
        q /= c;
        return q;
    }

    friend inline quaternion_t operator+( const T& c, const quaternion_t& o )
    {
        quaternion_t q( o );
        q += c;
        return q;
    }

    friend inline quaternion_t operator-( const T& c, const quaternion_t& o )
    {
        quaternion_t q( o );
        q -= c;
        return q;
    }

    friend inline quaternion_t operator*( const T& c, const quaternion_t& o )
    {
        quaternion_t q( o );
        q *= c;
        return q;
    }

    friend inline quaternion_t operator/( const T& c, const quaternion_t& o )
    {
        quaternion_t q( o );
        q.invert();
        q *= c;
        return q;
    }

    template< class Y >
    inline quaternion_t operator+( const quaternion< Y >& o ) const
    {
        quaternion_t q( *this );
        q += o;
        return q;
    }

    template< class Y >
    inline quaternion_t operator-( const quaternion< Y >& o ) const
    {
        quaternion_t q( *this );
        q -= o;
        return q;
    }

    template< class Y >
    inline quaternion_t operator*( const quaternion< Y >& o ) const
    {
        quaternion_t q( *this );
        q *= o;
        return q;
    }

    template< class Y >
    inline quaternion_t operator/( const quaternion< Y >& o ) const
    {
        quaternion_t q( *this );
        q /= o;
        return q;
    }

    template< class Y, bool B >
    inline quaternion_t operator+( const vector< Y, 3, B >& o ) const
    {
        quaternion_t q( *this );
        q += o;
        return q;
    }

    template< class Y, bool B >
    inline quaternion_t operator-( const vector< Y, 3, B >& o ) const
    {
        quaternion_t q( *this );
        q -= o;
        return q;
    }

    template< class Y, bool B >
    inline quaternion_t operator*( const vector< Y, 3, B >& o ) const
    {
        quaternion_t q( *this );
        q *= o;
        return q;
    }

    template< class X, class Y, class Z, class W >
    inline void as_normal( const quaternion< X >& qx,
                           const quaternion< Y >& qy,
                           const quaternion< Z >& qz,
                           const quaternion< W >& qw )
    {
        quaternion_t qyx( qy ); qyx -= qx;
        quaternion_t qzx( qz ); qzx -= qx;
        quaternion_t qwx( qw ); qwx -= qx;
        cross( qyx, qzx, qwx );
        normalize();
    }

    template< class Y, class Z, class W >
    inline quaternion_t& as_normal( const quaternion< Y >& qy,
                                    const quaternion< Z >& qz,
                                    const quaternion< W >& qw )
    {
        this->template as_normal< T, Y, Z, W >( *this, qy, qz, qw );
        return *this;
    }

    template< class Y, class Z, class W >
    friend inline quaternion_t normal( const quaternion_t&    qx,
                                       const quaternion< Y >& qy,
                                       const quaternion< Z >& qz,
                                       const quaternion< W >& qw )
    {
        quaternion_t q( qx );
        q.template as_normal< Y, Z, W >( qy, qz, qw );
        return q;
    }

    template< class Y, class Z, bool B1, bool B2 >
    inline quaternion_t& rotate( const T& theta, const vector< Y, 3, B1 >& axis,
                                 const vector< Z, 3, B2 >& o )
    {
        const T half_theta = theta / 2;
        quaternion_t q = std::cos( half_theta ) + ( std::sin( half_theta ) + axis.normalized() );
        *this = q;
        this->operator*=( o );
        q.normalize();
        this->operator*=( q );
        return *this;
    }

    template< class Y, bool B >
    inline quaternion_t& rotate_x( const T& theta, const vector< Y, 3, B >& o )
    {
        const T half_theta = theta / 2;
        quaternion_t q = std::cos( half_theta ) + ( std::sin( half_theta ) + quater_i );
        *this = q;
        this->operator*=( o );
        q.normalize();
        this->operator*=( q );
        return *this;
    }

    template< class Y, bool B >
    inline quaternion_t& rotate_y( const T& theta, const vector< Y, 3, B >& o )
    {
        const T half_theta = theta / 2;
        quaternion_t q = std::cos( half_theta ) + ( std::sin( half_theta ) + quater_j );
        *this = q;
        this->operator*=( o );
        q.normalize();
        this->operator*=( q );
        return *this;
    }

    template< class Y, bool B >
    inline quaternion_t& rotate_z( const T& theta, const vector< Y, 3, B >& o )
    {
        const T half_theta = theta / 2;
        quaternion_t q = std::cos( half_theta ) + ( std::sin( half_theta ) + quater_k );
        *this = q;
        this->operator*=( o );
        q.normalize();
        this->operator*=( q );
        return *this;
    }


    static const quaternion_t zero;
    static const quaternion_t quater_i;
    static const quaternion_t quater_j;
    static const quaternion_t quater_k;
    static const quaternion_t identity;

    friend inline std::ostream& operator<< ( std::ostream& os, const quaternion_t& q )
    {
        os << "( ";
        for ( std::size_t i = 0; i < 4; ++i )
        {
            os << q.at( i ) << ' ';
        }
        os << ')';

        return os;
    }

    template< class Y >
    inline void rotation_matrix( matrix< Y, 3 >& m ) const
    {
        const T w2 = v_[ 3 ] * v_[ 3 ];
        const T x2 = v_[ 0 ] * v_[ 0 ];
        const T y2 = v_[ 1 ] * v_[ 1 ];
        const T z2 = v_[ 2 ] * v_[ 2 ];
        const T wx = v_[ 3 ] * v_[ 0 ];
        const T wy = v_[ 3 ] * v_[ 1 ];
        const T wz = v_[ 3 ] * v_[ 2 ];
        const T xy = v_[ 0 ] * v_[ 1 ];
        const T xz = v_[ 0 ] * v_[ 2 ];
        const T yz = v_[ 1 ] * v_[ 2 ];

        m.at( 0, 0 ) = w2 + x2 - y2 - z2;
        m.at( 0, 1 ) = 2. * ( xy - wz );
        m.at( 0, 2 ) = 2. * ( xz + wy );
        m.at( 1, 0 ) = 2. * ( xy + wz );
        m.at( 1, 1 ) = w2 - x2 + y2 - z2;
        m.at( 1, 2 ) = 2. * ( yz - wx );
        m.at( 2, 0 ) = 2. * ( xz - wy );
        m.at( 2, 1 ) = 2. * ( yz + wx );
        m.at( 2, 2 ) = w2 - x2 - y2 + z2;
    }

    inline matrix< T, 3 > rotation_matrix() const
    {
        matrix< T, 3 > m;
        this->template rotation_matrix< T >( m );
        return m;
    }

    template< class X >
    inline quaternion_t slerp( const T r, const quaternion< X >& b ) const
    {
        T c = this->normalized().dot( b.normalized() );
        quaternion_t q( b );

        if ( c < 0 )
        {
            c = -c;
            q.negate();
        }

        if ( c + std::numeric_limits< T >::epsilon() < 1 )
        {
            T s     = std::sqrt( static_cast< T >( 1 ) - c * c );
            T angle = std::atan2( s, c );
            T invs  = static_cast< T >( 1 ) / s;
            T c1    = invs * angle * std::sin( static_cast< T >( 1 ) - r );
            T c2    = invs * std::sin( r * angle );
            q *= c2;
            return c1 * ( *this ) + q;
        }
        else
        {
            q *= r;
            q += ( *this ) * ( static_cast< T >( 1 ) - r );
            return q;
        }

    }


    template< class X, class Y >
    friend quaternion_t slerp( const T r, const quaternion< X >& a, const quaternion< Y >& b )
    {
        return a.template slerp< Y >( r, b );
    }

};

template< class T > const quaternion< T > quaternion< T >::zero    ( 0, 0, 0, 0 );
template< class T > const quaternion< T > quaternion< T >::quater_i( 1, 0, 0, 0 );
template< class T > const quaternion< T > quaternion< T >::quater_j( 0, 1, 0, 0 );
template< class T > const quaternion< T > quaternion< T >::quater_k( 0, 0, 1, 0 );
template< class T > const quaternion< T > quaternion< T >::identity( 0, 0, 0, 1 );

typedef quaternion< double > quaterniond;
typedef quaternion< float  > quaternionf;

} // namespace vl
} // namespace zi

ZI_HASH_EXPORT_NAMESPACE_BEGIN

template< class T >
struct hash< ::zi::vl::quaternion< T > >: ::zi::vl::quaternion< T >::hash
{
};

ZI_HASH_EXPORT_NAMESPACE_END

#include <zi/vl/vl_suffix.hpp>

#endif

