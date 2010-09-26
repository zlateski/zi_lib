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
