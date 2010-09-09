/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/concurrency/concurrency.hpp>
#include <zi/zunit/zunit.hpp>

#include <vector>

ZiSUITE( ZiLib_Concurrency_Tests );

namespace concurrency_tests {

zi::mutex mutex_;
int cnt = 0;

std::vector< int > vec_;

void inc_by( int x )
{
    zi::guard g( mutex_ );
    cnt *= 2;
    zi::this_thread::sleep( 1 );
    cnt /= 2;
    cnt += x;
}

void inc_by_and_insert( int x )
{
    zi::guard g( mutex_ );
    cnt *= 2;
    zi::this_thread::sleep( 1 );
    cnt /= 2;
    cnt += x;
    vec_.push_back( x );
}

};


ZiTEST( Test_SimpleTaskManager )
{
    using concurrency_tests::cnt;
    using concurrency_tests::inc_by;

    cnt = 0;

    zi::task_manager::deque tm( 10 );
    tm.start();

    for ( int i = 1; i < 1001; ++i )
    {
        tm.insert( zi::run_fn( zi::bind( &inc_by , i ) ) );
    }

    tm.join();

    EXPECT_EQ( cnt, 1000 * 1001 / 2 );
}


ZiTEST( Test_PrioritizedTaskManager )
{
    using concurrency_tests::cnt;
    using concurrency_tests::inc_by_and_insert;
    using concurrency_tests::vec_;

    cnt = 0;
    vec_.clear();

    zi::task_manager::prioritized tm( 1 );

    for ( int i = 1; i < 101; ++i )
    {
        tm.insert< zi::priority::low     >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
        tm.insert< zi::priority::high    >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
        tm.insert< zi::priority::normal  >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
        tm.insert< zi::priority::lowest  >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
        tm.insert< zi::priority::highest >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
        tm.insert< zi::priority::custom<  5 > >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
        tm.insert< zi::priority::custom< -5 > >( zi::run_fn( zi::bind( &inc_by_and_insert , i ) ) );
    }

    tm.start();
    tm.join();

    EXPECT_EQ( cnt, 7 * 100 * 101 / 2 );

    for ( std::size_t i = 0; i < 700; ++i )
    {
        EXPECT_EQ( vec_[ i ], i % 100 + 1 );
    }

}



ZiTEST( Test_PrioritizedTaskManager2 )
{
    using concurrency_tests::cnt;
    using concurrency_tests::inc_by_and_insert;
    using concurrency_tests::vec_;

    cnt = 0;
    vec_.clear();

    zi::task_manager::prioritized tm( 1 );

    for ( int i = 1; i < 101; ++i )
    {
        tm.insert< zi::priority::lowest       >( zi::run_fn( zi::bind( &inc_by_and_insert , 6 ) ) );
        tm.insert< zi::priority::low          >( zi::run_fn( zi::bind( &inc_by_and_insert , 5 ) ) );
        tm.insert< zi::priority::custom< -5 > >( zi::run_fn( zi::bind( &inc_by_and_insert , 4 ) ) );
        tm.insert< zi::priority::normal       >( zi::run_fn( zi::bind( &inc_by_and_insert , 3 ) ) );
        tm.insert< zi::priority::custom<  5 > >( zi::run_fn( zi::bind( &inc_by_and_insert , 2 ) ) );
        tm.insert< zi::priority::high         >( zi::run_fn( zi::bind( &inc_by_and_insert , 1 ) ) );
        tm.insert< zi::priority::highest      >( zi::run_fn( zi::bind( &inc_by_and_insert , 0 ) ) );
    }

    tm.start();
    tm.join();

    EXPECT_EQ( cnt, 21 * 100 );

    for ( std::size_t i = 0; i < 700; ++i )
    {
        EXPECT_EQ( vec_[ i ], i / 100 );
    }

}
