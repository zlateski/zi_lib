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

struct barrier_tester: zi::runnable
{
    barrier_tester( int &v, int count = 1 ):
        v_( v ), b_( count ), m_()
    {
    }

    void run()
    {
        if ( b_.wait() )
        {
            zi::guard g( m_ );
            ++v_;
        }
    }

    int         &v_;
    zi::barrier  b_;
    zi::mutex    m_;
};


} // namespace concurrency_tests

ZiTEST( Test_Barrier )
{
    using concurrency_tests::barrier_tester;

    for ( int i = 1; i < 10; ++i )
    {
        int v = 0;
        zi::shared_ptr< barrier_tester > bt( new barrier_tester( v, i ) );

        for ( int j = 0; j < i * ( i + 1 ); ++j )
        {
            zi::thread th( bt );
            th.start();
        }

        zi::all_threads::join();

        EXPECT_EQ( v, i + 1 );
    }

}
