/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/zunit/zunit.hpp>

#include <zi/utility/enable_if.hpp>

ZiSUITE( ZiLib_Utility_Tests );

namespace zi_test {

struct enable_if_test
{

    template< bool B >
    static int simple( typename zi::enable_if< B >::type* = 0 )
    {
        return 1;
    }

    template< bool B >
    static int simple( typename zi::disable_if< B >::type* = 0 )
    {
        return 0;
    }


    template< class T >
    static int result( typename zi::enable_if< sizeof( T ) == 1 >::type* = 0 )
    {
        return 1;
    }

    template< class T >
    static int result( typename zi::disable_if< sizeof( T ) == 1 >::type* = 0 )
    {
        return 0;
    }
};

}

ZiTEST( EnableIf_Test )
{
    EXPECT_EQ( zi_test::enable_if_test::simple< true >(), 1 );
    EXPECT_EQ( zi_test::enable_if_test::simple< false >(), 0 );

    EXPECT_EQ( zi_test::enable_if_test::result< char  >(), 1 );
    EXPECT_EQ( zi_test::enable_if_test::result< int   >(), 0 );
    EXPECT_EQ( zi_test::enable_if_test::result< void* >(), 0 );
}
