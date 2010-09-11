/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/concurrency/concurrency.hpp>
#include <zi/zlog/sink.hpp>
#include <zi/zlog/logs.hpp>
#include <zi/zunit/zunit.hpp>

#include <sstream>
#include <string>

ZiSUITE( ZiLib_ZiLog_Tests );

ZiTEST( Test_Sink )
{

    std::stringstream ss;
    std::string       s ;

    zi::zlog::sink    sink_( ss );

    zi::zlog::token_wrapper( sink_ ).get() << "A" << 1 << "C";
    ss >> s;
    EXPECT_EQ( s, std::string( "A1C" ) );

    std::stringstream ss1;
    std::stringstream ss2;
    zi::zlog::sink    sink1_( ss1 );

    for ( int i = 0; i < 1000; ++i )
    {
        zi::zlog::token_wrapper( sink1_ ).get() << i << i + 1 << "Z";
        ss2 << i << i + 1 << "Z";

        EXPECT_EQ( ss1.str(), ss2.str() );
    }

    std::stringstream ss3;
    zi::zlog::sink    sink3_( ss3 );

    zi::zlog::token_wrapper( sink3_ ).get() << sink3_;
    ss3 >> s;
    EXPECT_EQ( s[0], '<' );
    ss3 >> s;
    EXPECT_EQ( s, std::string( "zi::zlog::sink" ) );

}
