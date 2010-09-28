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

#include <zi/zunit/zunit.hpp>
#include <zi/vl/matrix.hpp>

ZiSUITE( ZiLib_VL_Tests );

ZiTEST( TestMatrixCtors )
{
    namespace vl = zi::vl;

    {
        vl::matrix< int, 1 > m1;
        vl::matrix< int, 1 > m2( 0 );
        vl::matrix< int, 1 > m3( 1 );

        EXPECT_EQ( m2.at( 0, 0 ), 0 );
        EXPECT_EQ( m3.at( 0, 0 ), 1 );
    }

    {
        vl::matrix< int, 2 > m1;
        vl::matrix< int, 2 > m2( 0 );
        vl::matrix< int, 2 > m3( 1 );
        vl::matrix< int, 2 > m4( 1, 2, 3, 4 );

        EXPECT_EQ( m2.at( 0, 0 ), 0 );
        EXPECT_EQ( m2.at( 0, 1 ), 0 );
        EXPECT_EQ( m2.at( 1, 0 ), 0 );
        EXPECT_EQ( m2.at( 1, 1 ), 0 );
        EXPECT_EQ( m3.at( 0, 0 ), 1 );
        EXPECT_EQ( m3.at( 0, 1 ), 1 );
        EXPECT_EQ( m3.at( 1, 0 ), 1 );
        EXPECT_EQ( m3.at( 1, 1 ), 1 );
        EXPECT_EQ( m4.at( 0, 0 ), 1 );
        EXPECT_EQ( m4.at( 0, 1 ), 2 );
        EXPECT_EQ( m4.at( 1, 0 ), 3 );
        EXPECT_EQ( m4.at( 1, 1 ), 4 );

        EXPECT_EQ( m4[ 0 ][ 0 ], 1 );
        EXPECT_EQ( m4[ 0 ][ 1 ], 2 );
        EXPECT_EQ( m4[ 1 ][ 0 ], 3 );
        EXPECT_EQ( m4[ 1 ][ 1 ], 4 );

        int x = m4[ 0 ][ 0 ];
        EXPECT_EQ( x, 1 );
        x = m4[ 0 ][ 1 ];
        EXPECT_EQ( x, 2 );
        x = m4[ 1 ][ 0 ];
        EXPECT_EQ( x, 3 );
        x = m4[ 1 ][ 1 ];
        EXPECT_EQ( x, 4 );
    }

}


ZiTEST( TestMatrixRowAccess )
{
    namespace vl = zi::vl;

    {
        vl::matrix< int, 2 > m( 1 );

        m[ 1 ][ 0 ] = 2;
        m[ 0 ][ 1 ] = 3;
        m[ 1 ][ 1 ] = 4;

        EXPECT_EQ( m.at( 0, 0 ), 1 );
        EXPECT_EQ( m.at( 1, 0 ), 2 );
        EXPECT_EQ( m.at( 0, 1 ), 3 );
        EXPECT_EQ( m.at( 1, 1 ), 4 );
    }

    {
        vl::matrix< int, 3 > m( 0 );
        vl::vector< int, 3 > v1( 1, 2, 3 );
        vl::vector< int, 3 > v2( 4, 5, 6 );
        vl::vector< int, 3 > v3( 7, 8, 9 );

        m[ 0 ] = v1;
        m[ 1 ] = v2;
        m[ 2 ] = v3;

        EXPECT_EQ( m.at( 0, 0 ), 1 );
        EXPECT_EQ( m.at( 0, 1 ), 2 );
        EXPECT_EQ( m.at( 0, 2 ), 3 );
        EXPECT_EQ( m.at( 1, 0 ), 4 );
        EXPECT_EQ( m.at( 1, 1 ), 5 );
        EXPECT_EQ( m.at( 1, 2 ), 6 );
        EXPECT_EQ( m.at( 2, 0 ), 7 );
        EXPECT_EQ( m.at( 2, 1 ), 8 );
        EXPECT_EQ( m.at( 2, 2 ), 9 );

    }

}

ZiTEST( TestMatrixMultiply )
{

    namespace vl = zi::vl;

    {
        vl::matrix< int, 2 > m1( 2, 0, 0, 1 );
        vl::matrix< int, 2 > m1sq( 4, 0, 0, 1 );
        vl::matrix< int, 2 > m2( 1, 1, 1, 1 );
        vl::matrix< int, 2 > m3( 2, 2, 1, 1 );

        m1 *= m1;

        EXPECT_EQ( m1, m1sq );

    }

}

