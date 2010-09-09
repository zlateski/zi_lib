/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_BARRIER_HPP
#define ZI_CONCURRENCY_BARRIER_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/condition_variable.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/assert.hpp>

namespace zi {
namespace concurrency_ {


class barrier: non_copyable
{
private:

    mutable int32_t threshold_;
    mutable int32_t count_    ;
    mutable int32_t round_    ;

    condition_variable cv_;
    mutex              m_ ;

public:

    explicit barrier( int32_t count = 1 ):
        threshold_( count ), count_( 0 ), round_( 0 ), cv_(), m_()
    {
        ZI_VERIFY( count > 0 );
    }

    bool wait() const
    {
        mutex::guard g( m_ );

        int32_t round = round_;

        if ( ++count_ == threshold_ )
        {
            ++round_   ;
            count_ = 0 ;
            cv_.notify_all();

            return true;
        }

        while ( round_ == round )
        {
            cv_.wait( m_ );
        }

        return false;
    }

};



} // namespace concurrency_

using concurrency_::barrier;

} // namespace zi


#endif
