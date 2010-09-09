/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/zunit/zunit.hpp>

#include <zi/utility/address_of.hpp>
#include <string>

ZiSUITE( ZiLib_Utility_Tests );

ZiTEST( AddressOf_Test )
{
    int int_var;
    const int cint_var = 0;
    static int sint_var = 0;
    static const int scint_var = 0;

    EXPECT_EQ( zi::address_of( int_var ), &int_var );
    EXPECT_EQ( zi::address_of( cint_var ), &cint_var );
    EXPECT_EQ( zi::address_of( sint_var ), &sint_var );
    EXPECT_EQ( zi::address_of( scint_var ), &scint_var );
}


ZiTEST( AddressOf_Test2 )
{
    const char* str1 = "X";
    const char* str2 = "Y";

    EXPECT_NEQ( str1, str2 );

    char** x = const_cast< char** > ( zi::address_of( str1 ) );
    *x = const_cast< char* >( str2 );

    EXPECT_EQ( str1, str2 );
}

