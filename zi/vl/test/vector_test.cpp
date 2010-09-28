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

#include <zi/vl/vector.hpp>
#include <zi/zunit/zunit.hpp>

ZiSUITE( ZiLib_VL_Tests );

ZiTEST( TestVectorCtors )
{
    namespace vl = zi::vl;

    {
        vl::vector< int, 1 > v1;
        vl::vector< int, 1 > v2( 0 );
        vl::vector< int, 1 > v3( 1 );

        EXPECT_EQ( v2.at( 0 ), 0 );
        EXPECT_EQ( v3.at( 0 ), 1 );
    }

    {
        vl::vector< int, 2 > v1;
        vl::vector< int, 2 > v2( 0 );
        vl::vector< int, 2 > v3( 1 );
        vl::vector< int, 2 > v4( 2, 3 );

        EXPECT_EQ( v2.at( 0 ), 0 );
        EXPECT_EQ( v2.at( 1 ), 0 );
        EXPECT_EQ( v3.at( 0 ), 1 );
        EXPECT_EQ( v3.at( 1 ), 1 );
        EXPECT_EQ( v4.at( 0 ), 2 );
        EXPECT_EQ( v4.at( 1 ), 3 );
    }


}

ZiTEST( TestVectorCross )
{

    namespace vl = zi::vl;

    {
        vl::vector< int, 2 > v1( 1, 2 );

        vl::vector< int, 2 > v = cross( v1 );

        EXPECT_EQ( v.at( 0 ), 2 );
        EXPECT_EQ( v.at( 1 ), -1 );

        v = cross( v );

        EXPECT_EQ( v.at( 0 ), -1 );
        EXPECT_EQ( v.at( 1 ), -2 );

        v1.cross();

        EXPECT_EQ( v1.at( 0 ), 2 );
        EXPECT_EQ( v1.at( 1 ), -1 );

        v1.cross( v1 );

        EXPECT_EQ( v1.at( 0 ), -1 );
        EXPECT_EQ( v1.at( 1 ), -2 );
    }


    {
        vl::vector< int, 2 > v1( 1, 2 );
        vl::vector< int, 2 > v2( 1, 2 );
        v2.cross( v1 );

        EXPECT_EQ( v2.at( 0 ), 2 );
        EXPECT_EQ( v2.at( 1 ), -1 );
        EXPECT_EQ( v1.at( 0 ), 1 );
        EXPECT_EQ( v1.at( 1 ), 2 );
    }

    {
        vl::vector< int, 3 > v1( 1, 2, 4 );
        vl::vector< int, 3 > v2( 1, 2, 4 );
        vl::vector< int, 3 > v3( 1, 1, 1 );
        vl::vector< int, 3 > v4( 1, 2, 4 );

        vl::vector< int, 3 > v = cross( v1, v1 );

        EXPECT_EQ( v.at( 0 ), 0 );
        EXPECT_EQ( v.at( 1 ), 0 );
        EXPECT_EQ( v.at( 2 ), 0 );

        v = cross( v1, v3 );

        EXPECT_EQ( v.at( 0 ), -2 );
        EXPECT_EQ( v.at( 1 ), 3 );
        EXPECT_EQ( v.at( 2 ), -1 );

        v = v3;
        v = cross( v, v1 );

        EXPECT_EQ( v.at( 0 ), 2 );
        EXPECT_EQ( v.at( 1 ), -3 );
        EXPECT_EQ( v.at( 2 ), 1 );

        v1.cross( v1, v1 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );

        v1.cross( v2, v2 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 4 );

        v1.cross( v2, v3 );

        EXPECT_EQ( v1.at( 0 ), -2 );
        EXPECT_EQ( v1.at( 1 ), 3 );
        EXPECT_EQ( v1.at( 2 ), -1 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 4 );

        EXPECT_EQ( v3.at( 0 ), 1 );
        EXPECT_EQ( v3.at( 1 ), 1 );
        EXPECT_EQ( v3.at( 2 ), 1 );

        v2.cross( v2, v3 );

        EXPECT_EQ( v2.at( 0 ), -2 );
        EXPECT_EQ( v2.at( 1 ), 3 );
        EXPECT_EQ( v2.at( 2 ), -1 );

        EXPECT_EQ( v3.at( 0 ), 1 );
        EXPECT_EQ( v3.at( 1 ), 1 );
        EXPECT_EQ( v3.at( 2 ), 1 );

        v4.cross( v3, v4 );

        EXPECT_EQ( v4.at( 0 ), 2 );
        EXPECT_EQ( v4.at( 1 ), -3 );
        EXPECT_EQ( v4.at( 2 ), 1 );

        EXPECT_EQ( v3.at( 0 ), 1 );
        EXPECT_EQ( v3.at( 1 ), 1 );
        EXPECT_EQ( v3.at( 2 ), 1 );
    }

    {
        vl::vector< int, 4 > v1( 1, 2, 4, 8 );
        v1.cross( v1, v1 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

    }

    {
        vl::vector< int, 4 > v1( 1, 2, 3, 4 );
        vl::vector< int, 4 > v2( 1, 2, 3, 4 );

        v1.cross( v1, v2, v2 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

        v1.set( 1, 2, 3, 4 );
        v1.cross( v2, v1, v2 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

        v1.set( 1, 2, 3, 4 );
        v1.cross( v2, v2, v1 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

        v1.set( 1, 2, 3, 4 );
        v1.cross( v2, v1, v1 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

        v1.set( 1, 2, 3, 4 );
        v1.cross( v1, v2, v1 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

        v1.set( 1, 2, 3, 4 );
        v1.cross( v1, v1, v2 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

        v1.cross( v1, v1, v1 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        v1.cross( v2, v2, v2 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );
        EXPECT_EQ( v1.at( 3 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 3 );
        EXPECT_EQ( v2.at( 3 ), 4 );

    }

}

ZiTEST( TestVectorFromToHomogenous )
{
    namespace vl = zi::vl;

    {
        vl::vector< int, 1 > v1( 2 );
        vl::vector< int, 2 > v2( v1 );
        vl::vector< int, 2 > v3 = v1;

        EXPECT_EQ( v2.at( 0 ), 2 );
        EXPECT_EQ( v2.at( 1 ), 1 );

        EXPECT_EQ( v3.at( 0 ), 2 );
        EXPECT_EQ( v3.at( 1 ), 1 );

        v3 = v1;

        EXPECT_EQ( v3.at( 0 ), 2 );
        EXPECT_EQ( v3.at( 1 ), 1 );
    }

    {
        vl::vector< int, 2 > v1( 1, 2 );
        vl::vector< int, 3 > v2( v1 );
        vl::vector< int, 3 > v3 = v1;
        vl::vector< int, 2 > v4( 2, 3 );

        EXPECT_EQ( v2.at( 0 ), 1 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 1 );

        EXPECT_EQ( v3.at( 0 ), 1 );
        EXPECT_EQ( v3.at( 1 ), 2 );
        EXPECT_EQ( v3.at( 2 ), 1 );

        v2 = v3 = v4;

        EXPECT_EQ( v2.at( 0 ), 2 );
        EXPECT_EQ( v2.at( 1 ), 3 );
        EXPECT_EQ( v2.at( 2 ), 1 );

        EXPECT_EQ( v3.at( 0 ), 2 );
        EXPECT_EQ( v3.at( 1 ), 3 );
        EXPECT_EQ( v3.at( 2 ), 1 );
    }

    {
        vl::vector< int, 2 > v1( 4, 2 );
        vl::vector< int, 1 > v2( v1 );
        vl::vector< int, 1 > v3 = v1;

        EXPECT_EQ( v2.at( 0 ), 2 );
        EXPECT_EQ( v3.at( 0 ), 2 );

        v3 = v1;

        EXPECT_EQ( v3.at( 0 ), 2 );
    }

    {
        vl::vector< int, 3 > v1( 8, 4, 2 );
        vl::vector< int, 2 > v2( v1 );
        vl::vector< int, 2 > v3 = v1;

        EXPECT_EQ( v2.at( 0 ), 4 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v3.at( 0 ), 4 );
        EXPECT_EQ( v3.at( 1 ), 2 );

        v3 = v1;

        EXPECT_EQ( v3.at( 0 ), 4 );
        EXPECT_EQ( v3.at( 1 ), 2 );
    }


    {
        vl::vector< double, 4 > v1( 8, 4, 2, 2 );
        vl::vector< int, 3 > v2( v1 );
        vl::vector< int, 3 > v3 = v1;

        EXPECT_EQ( v2.at( 0 ), 4 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 1 );
        EXPECT_EQ( v3.at( 0 ), 4 );
        EXPECT_EQ( v3.at( 1 ), 2 );
        EXPECT_EQ( v3.at( 2 ), 1 );

        v3 = v1;

        EXPECT_EQ( v3.at( 0 ), 4 );
        EXPECT_EQ( v3.at( 1 ), 2 );
        EXPECT_EQ( v3.at( 2 ), 1 );
    }


}

ZiTEST( TestVectorNormal )
{
    namespace vl = zi::vl;

    {
        vl::vector< int, 3 > v1( 2, 0, 0 );
        vl::vector< int, 3 > v2( 0, 2, 0 );
        vl::vector< int, 3 > v3( 0, 0, 0 );

        v1.normal( v2, v3 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 1 );

        v1.set( 2, 0, 0 );

        v1 = normal( v1, v2, v3 );

        EXPECT_EQ( v1.at( 0 ), 0 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 1 );

    }

    {
        vl::vector< int, 3 > v1( 0, 0, 0 );
        vl::vector< int, 3 > v2( 0, 2, 0 );
        vl::vector< int, 3 > v3( 0, 0, 2 );

        v1.normal( v2, v3 );

        EXPECT_EQ( v1.at( 0 ), 1 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 0 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 0 );

        EXPECT_EQ( v3.at( 0 ), 0 );
        EXPECT_EQ( v3.at( 1 ), 0 );
        EXPECT_EQ( v3.at( 2 ), 2 );

        v1.set( 0, 0, 0 );

        v1.normal( v3, v2 );

        EXPECT_EQ( v1.at( 0 ), -1 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 0 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 0 );

        EXPECT_EQ( v3.at( 0 ), 0 );
        EXPECT_EQ( v3.at( 1 ), 0 );
        EXPECT_EQ( v3.at( 2 ), 2 );

        v1.set( 0, 0, 0 );

        v1.normal( v1, v3, v2 );

        EXPECT_EQ( v1.at( 0 ), -1 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 0 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 0 );

        EXPECT_EQ( v3.at( 0 ), 0 );
        EXPECT_EQ( v3.at( 1 ), 0 );
        EXPECT_EQ( v3.at( 2 ), 2 );

        v1.set( 0, 0, 0 );

        vl::vector< int, 3 > off( 10, 20, 30 );

        v1.normal( v1 + off, v3 + off, v2 + off );

        EXPECT_EQ( v1.at( 0 ), -1 );
        EXPECT_EQ( v1.at( 1 ), 0 );
        EXPECT_EQ( v1.at( 2 ), 0 );

        EXPECT_EQ( v2.at( 0 ), 0 );
        EXPECT_EQ( v2.at( 1 ), 2 );
        EXPECT_EQ( v2.at( 2 ), 0 );

        EXPECT_EQ( v3.at( 0 ), 0 );
        EXPECT_EQ( v3.at( 1 ), 0 );
        EXPECT_EQ( v3.at( 2 ), 2 );
    }
}
