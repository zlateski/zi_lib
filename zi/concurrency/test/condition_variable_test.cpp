/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/concurrency/concurrency.hpp>
#include <zi/zunit/zunit.hpp>

ZiSUITE( ZiLib_Concurrency_Tests );

namespace concurrency_tests {

struct cv_tester: zi::runnable
{
    cv_tester( int &v ):
        ok_( true ), v_( v ), vold_( 0 ), m_(), cv_()
    {
    }

    void run()
    {
        zi::mutex::guard g( m_ );
        vold_ = 1 + v_++;

        while ( v_ < 1000 )
        {
            cv_.notify_all();

            cv_.wait( g );

            if ( v_ != vold_ )
            {
                ok_ = false;
            }
            vold_ = 1 + v_++;
        }
        cv_.notify_all();
    }

    void notify()
    {
        cv_.notify_all();
    }

    bool       ok_  ;
    int       &v_   ;
    int        vold_;
    zi::mutex  m_   ;
    zi::condition_variable cv_;
};


struct cv_tester2: zi::runnable
{
    cv_tester2( int &v ):
        ok_( true ), v_( v ), vold_( 0 ), m_(), cv_()
    {
    }

    void run()
    {
        zi::mutex::guard g( m_ );
        vold_ = 1 + v_++;

        while ( v_ < 1000 )
        {
            cv_.notify_one();

            cv_.wait( g );

            if ( v_ != vold_ )
            {
                ok_ = false;
            }
            vold_ = 1 + v_++;
        }
        cv_.notify_one();
    }

    void notify()
    {
        cv_.notify_one();
    }

    bool       ok_  ;
    int       &v_   ;
    int        vold_;
    zi::mutex  m_   ;
    zi::condition_variable cv_;
};



}

ZiTEST( Test_ConditionVariableNotifyAll )
{
    using concurrency_tests::cv_tester;

    int v = 0;

    zi::shared_ptr< cv_tester > cvt( new cv_tester( v ) );

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t1.start();
        t2.start();

        t2.join();
        t1.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t1.start();
        zi::this_thread::sleep( 1 );
        t2.start();

        t1.join();
        t2.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t2.start();
        zi::this_thread::sleep( 1 );
        t1.start();

        t1.join();
        t2.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

}


ZiTEST( Test_ConditionVariableNotifyOne )
{
    using concurrency_tests::cv_tester2;

    int v = 0;

    zi::shared_ptr< cv_tester2 > cvt( new cv_tester2( v ) );

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t1.start();
        t2.start();

        t2.join();
        t1.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t1.start();
        zi::this_thread::sleep( 1 );
        t2.start();

        t1.join();
        t2.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

    for ( int i = 0; i < 10; ++i )
    {
        v = 0;
        zi::thread t1( cvt );
        zi::thread t2( cvt );

        t2.start();
        zi::this_thread::sleep( 1 );
        t1.start();

        t1.join();
        t2.join();

        EXPECT_EQ  ( v, 1001 );
        EXPECT_TRUE( cvt->ok_ );
    }

}
