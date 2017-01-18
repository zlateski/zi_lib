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

#ifndef ZI_WATERSHED_QUICKIE_PREFIX_HPP
#define ZI_WATERSHED_QUICKIE_PREFIX_HPP 1

#define QW_LOG( x, ... ) static_cast< void >( 0 )

#define QW_FOR_2( type, v1, f1, t1, v2, f2, t2 )                \
    for ( type v1 = f1; v1 < t1; ++v1 )                         \
        for ( type v2 = f2; v2 < t2; ++v2 )


#define QW_FOR_3( type, v1, f1, t1, v2, f2, t2, v3, f3, t3 )    \
    QW_FOR_2( type, v1, f1, t1, v2, f2, t2 )                    \
    for ( type v3 = f3; v3 < t3; ++v3 )

#define QW_FOR_4( type, v1, f1, t1, v2, f2, t2, v3, f3, t3, v4, f4, t4 ) \
    QW_FOR_3( type, v1, f1, t1, v2, f2, t2, v3, f3, t3 )                \
    for ( type v4 = f4; v4 < t4; ++v4 )

#define QW_MAX4( v1, v2, v3, v4 )                       \
    std::max( std::max( v1, v2 ), std::max( v3, v4 ) )

#define QW_MAX6( v1, v2, v3, v4, v5, v6 )                       \
    std::max( QW_MAX4( v1, v2, v3, v4 ), std::max( v5, v6 ) )

#define QW_UPDATE_IF_BORDER(n,x,b) n = (x&b)?(n|0x80):n

#define QW_UPDATE_BORDER(n,b) n = (n&assigned_bit::value)?n:((n&0x80)?b:0)

#define QW_UPDATE_CORNER(a,b)                           \
    if (((a)&assigned_bit::value)&&((!((b)&assigned_bit::value)))&&((b)&0x80)) b=a; \
    if (((b)&assigned_bit::value)&&((!((a)&assigned_bit::value)))&&((a)&0x80)) a=b

#endif

