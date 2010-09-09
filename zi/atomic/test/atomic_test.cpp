/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/atomic/atomic.hpp>

#include <zi/zunit/zunit.hpp>

ZiSUITE( ZiLib_Atomic_Tests );

typedef zi::atomic::atomic_word atomic_t;

ZiTEST( Test_Increment_Decrement )
{
    volatile atomic_t x = 0;
    EXPECT_EQ( x, 0 );
    EXPECT_EQ( zi::atomic::increment_swap( &x ), 0 );
    EXPECT_EQ( zi::atomic::increment_swap( &x ), 1 );
    EXPECT_EQ( zi::atomic::read( &x ), 2 );
    EXPECT_EQ( zi::atomic::decrement_swap( &x ), 2 );
    EXPECT_EQ( zi::atomic::decrement_swap( &x ), 1 );
    EXPECT_EQ( zi::atomic::read( &x ), 0 );

    zi::atomic::increment( &x );
    EXPECT_EQ( zi::atomic::read( &x ), 1 );
    zi::atomic::increment( &x );
    EXPECT_EQ( zi::atomic::read( &x ), 2 );


    zi::atomic::decrement( &x );
    EXPECT_EQ( zi::atomic::read( &x ), 1 );
    zi::atomic::decrement( &x );
    EXPECT_EQ( zi::atomic::read( &x ), 0 );

}


ZiTEST( Test_Compare_Swap )
{
    atomic_t x = 0;
    EXPECT_EQ( x, 0 );

    EXPECT_EQ( zi::atomic::compare_swap( &x, 1, 1 ), 0 );
    EXPECT_EQ( zi::atomic::compare_swap( &x, 1, 0 ), 0 );
    EXPECT_EQ( zi::atomic::read( &x ), 1 );

    EXPECT_EQ( zi::atomic::compare_swap( &x, 0, 0 ), 1 );
    EXPECT_EQ( zi::atomic::compare_swap( &x, 0, 1 ), 1 );
    EXPECT_EQ( zi::atomic::read( &x ), 0 );

}
