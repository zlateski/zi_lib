/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/utility/is_printable.hpp>
#include <zi/zunit/zunit.hpp>

ZiSUITE( ZiLib_Utility_Tests );

namespace zi_test_is_printable {

struct not_printable {};

struct custom_printable {};

std::ostream& operator<<( std::ostream& os, const custom_printable& cp)
{
    return os;
}

}

ZiTEST( IsPrintable_Test )
{
    using namespace zi_test_is_printable;

    EXPECT_TRUE(  zi::is_printable< int >::value            );
    EXPECT_TRUE(  zi::is_printable< float >::value          );
    EXPECT_TRUE(  zi::is_printable< const int >::value      );
    EXPECT_TRUE(  zi::is_printable< const float >::value    );
    EXPECT_TRUE(  zi::is_printable< const volatile int >::value   );
    EXPECT_TRUE(  zi::is_printable< const volatile float >::value );

    EXPECT_TRUE(  zi::is_printable< float& >::value          );
    EXPECT_TRUE(  zi::is_printable< const float& >::value    );
    EXPECT_TRUE(  zi::is_printable< const volatile float& >::value );

    EXPECT_FALSE( zi::is_printable< void >::value            );
    EXPECT_FALSE( zi::is_printable< const void >::value      );
    EXPECT_FALSE( zi::is_printable< const volatile void >::value   );


    EXPECT_FALSE( zi::is_printable< not_printable >::value            );
    EXPECT_FALSE( zi::is_printable< const not_printable >::value      );
    EXPECT_FALSE( zi::is_printable< const volatile not_printable >::value   );

    EXPECT_FALSE( zi::is_printable< not_printable& >::value            );
    EXPECT_FALSE( zi::is_printable< const not_printable& >::value      );
    EXPECT_FALSE( zi::is_printable< const volatile not_printable& >::value   );

    EXPECT_TRUE(  zi::is_printable< not_printable* >::value            );
    EXPECT_TRUE(  zi::is_printable< const not_printable* >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile not_printable* >::value   );

    EXPECT_TRUE(  zi::is_printable< not_printable[10] >::value            );
    EXPECT_TRUE(  zi::is_printable< const not_printable[10] >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile not_printable[10] >::value   );


    EXPECT_TRUE(  zi::is_printable< custom_printable >::value            );
    EXPECT_TRUE(  zi::is_printable< const custom_printable >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile custom_printable >::value   );

    EXPECT_TRUE(  zi::is_printable< custom_printable& >::value            );
    EXPECT_TRUE(  zi::is_printable< const custom_printable& >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile custom_printable& >::value   );

    EXPECT_TRUE(  zi::is_printable< custom_printable[10] >::value            );
    EXPECT_TRUE(  zi::is_printable< const custom_printable[10] >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile custom_printable[10] >::value   );

    EXPECT_TRUE(  zi::is_printable< custom_printable[10][10] >::value            );
    EXPECT_TRUE(  zi::is_printable< const custom_printable[10][10] >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile custom_printable[10][10] >::value   );

    EXPECT_TRUE(  zi::is_printable< void* >::value            );
    EXPECT_TRUE(  zi::is_printable< const void* >::value      );
    EXPECT_TRUE(  zi::is_printable< const volatile void* >::value   );


}
