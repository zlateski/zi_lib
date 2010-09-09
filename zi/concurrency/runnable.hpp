/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_RUNNABLE_HPP
#define ZI_CONCURRENCY_RUNNABLE_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/mutex.hpp>

#include <zi/bits/shared_ptr.hpp>
#include <zi/bits/function.hpp>
#include <zi/bits/bind.hpp>
#include <zi/bits/ref.hpp>
#include <zi/utility/assert.hpp>

namespace zi {
namespace concurrency_ {

// forward decls
struct thread;

struct runnable
{
private:
    struct mutex_pool_tag;

    int   running_count_ ;
    int   finished_count_;

    mutex m_;

    void start()
    {
        mutex::guard g( m_ );
        ++running_count_;
    }

    void finish()
    {
        mutex::guard g( m_ );
        --running_count_;
        ++finished_count_;
    }

public:

    runnable():
        running_count_( 0 ),
        finished_count_( 0 )
    {
    }

    virtual ~runnable()
    {
        ZI_VERIFY( running_count_ == 0 );
    }

    virtual void run() = 0;

    void execute()
    {
        start();
        run();
        finish();
    }

protected:

    int finished_count() const
    {
        mutex::guard g( m_ );
        return finished_count_;
    }

    int started_count() const
    {
        mutex::guard g( m_ );
        return finished_count_ + running_count_;
    }

    int active_count() const
    {
        mutex::guard g( m_ );
        return running_count_;
    }

};

struct runnable_function_wrapper: runnable
{
private:
    function< void() > f_;

public:
    runnable_function_wrapper( function< void() > f ): f_( f )
    {
    };

    runnable_function_wrapper( const reference_wrapper< function< void() > >& f ):
        f_( f.get() )
    {
    };

    void run()
    {
        f_();
    }
};

inline shared_ptr< runnable > run_fn( function< void() > f )
{
    return shared_ptr< runnable_function_wrapper >( new runnable_function_wrapper( f ) );
}

} // namespace concurrency_

using concurrency_::runnable;
using concurrency_::run_fn;

} // namespace zi

#endif
