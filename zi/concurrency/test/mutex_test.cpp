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

template< class Mutex >
struct mutex_tester: zi::runnable
{
    mutex_tester( int &v ): v_( v ), m_()
    {
    }

    void run()
    {
        for ( int i = 0; i < 10000; ++i )
        {
            m_.lock();
            ++v_;
            for ( int j = 0; j < 100; ++j )
            {
                v_ *= 2;
                v_ /= 2;
            }
            m_.unlock();
        }
    }

    int   &v_;
    Mutex m_;
};

template< class Mutex >
struct recursive_mutex_tester: zi::runnable
{
    recursive_mutex_tester( int &v ): v_( v ), m_()
    {
    }

    void recursion( int n )
    {
        m_.lock();
        ++v_;
        for ( int j = 0; j < 100; ++j )
        {
            v_ *= 2;
            v_ /= 2;
        }

        if ( n > 1 )
        {
            recursion( n - 1 );
        }

        m_.unlock();
    }

    void run()
    {
        recursion( 10000 );
    }

    int   &v_;
    Mutex m_;
};

template< class Mutex >
struct guard_tester: zi::runnable
{
    guard_tester( int &v ): v_( v ), m_()
    {
    }

    void run()
    {
        for ( int i = 0; i < 10000; ++i )
        {
            zi::guard g( m_ );
            ++v_;
            for ( int j = 0; j < 100; ++j )
            {
                v_ *= 2;
                v_ /= 2;
            }
        }
    }

    int   &v_;
    Mutex m_;
};


} // namespace concurrency_tests

ZiTEST( Test_Spinlock )
{
    using concurrency_tests::mutex_tester;

    int v = 0;

    zi::shared_ptr< mutex_tester< zi::spinlock > >
        sl( new mutex_tester< zi::spinlock >( v ) );


    {
        v = 0;
        zi::thread t1( sl );
        zi::thread t2( sl );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

}

ZiTEST( Test_RecursiveMutex )
{
    using concurrency_tests::recursive_mutex_tester;

    int v = 0;

    zi::shared_ptr< recursive_mutex_tester< zi::mutex::recursive > >
        rm1( new recursive_mutex_tester< zi::mutex::recursive >( v ) );

    zi::shared_ptr< recursive_mutex_tester< zi::recursive_mutex > >
        rm2( new recursive_mutex_tester< zi::recursive_mutex >( v ) );


    {
        v = 0;
        zi::thread t1( rm1 );
        zi::thread t2( rm1 );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

    {
        v = 0;
        zi::thread t1( rm2 );
        zi::thread t2( rm2 );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

}


ZiTEST( Test_MutexAll )
{
    using concurrency_tests::mutex_tester;

    int v = 0;

    zi::shared_ptr< mutex_tester< zi::mutex > >
        mt( new mutex_tester< zi::mutex >( v ) );

    zi::shared_ptr< mutex_tester< zi::recursive_mutex > >
        rmt( new mutex_tester< zi::recursive_mutex >( v ) );

    zi::shared_ptr< mutex_tester< zi::adaptive_mutex > >
        amt( new mutex_tester< zi::adaptive_mutex >( v ) );


    {
        v = 0;
        zi::thread t1( mt );
        zi::thread t2( mt );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

    {
        v = 0;
        zi::thread t1( rmt );
        zi::thread t2( rmt );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }


    {
        v = 0;
        zi::thread t1( amt );
        zi::thread t2( amt );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

}


ZiTEST( Test_Guard )
{
    using concurrency_tests::guard_tester;

    int v = 0;

    zi::shared_ptr< guard_tester< zi::spinlock > >
        sl( new guard_tester< zi::spinlock >( v ) );

    zi::shared_ptr< guard_tester< zi::mutex > >
        mt( new guard_tester< zi::mutex >( v ) );

    zi::shared_ptr< guard_tester< zi::recursive_mutex > >
        rmt( new guard_tester< zi::recursive_mutex >( v ) );

    zi::shared_ptr< guard_tester< zi::adaptive_mutex > >
        amt( new guard_tester< zi::adaptive_mutex >( v ) );


    {
        v = 0;
        zi::thread t1( mt );
        zi::thread t2( mt );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

    {
        v = 0;
        zi::thread t1( rmt );
        zi::thread t2( rmt );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }


    {
        v = 0;
        zi::thread t1( amt );
        zi::thread t2( amt );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

    {
        v = 0;
        zi::thread t1( sl );
        zi::thread t2( sl );
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        EXPECT_EQ( v, 20000 );
    }

}
