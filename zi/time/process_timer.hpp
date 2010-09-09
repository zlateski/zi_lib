/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_TIME_PROCESS_TIMER_HPP
#define ZI_TIME_PROCESS_TIMER_HPP 1

#include <zi/time/config.hpp>
#include <zi/bits/cstdint.hpp>

#include <ctime>

namespace zi {

class process_timer
{
private:
    std::clock_t start_, lap_start_;

public:

    process_timer()
    {
        restart();
    }

    inline void restart()
    {
        start_ = lap_start_ = std::clock();
    }

    inline void reset()
    {
        restart();
    }

    template< class T > inline T lap()
    {
        int64_t last = lap_start_;
        lap_start_ = std::clock();
        T ret( lap_start_ - last );
        ret /= static_cast< T >( CLOCKS_PER_SEC );
        return ret < 0 ? 0 : ret;
    }

    template< class T > inline T elpased()
    {
        return T( std::clock() - start_ ) / CLOCKS_PER_SEC;
    }

    template< class T > inline T lap_elpased()
    {
        return T( std::clock() - lap_start_ ) / CLOCKS_PER_SEC;
    }

    inline int64_t lap()
    {
        return lap< int64_t >();
    }

    inline int64_t elpased()
    {
        return elpased< int64_t >();
    }

    inline int64_t lap_elpased()
    {
        return lap_elpased< int64_t >();
    }

};

} // namespace zi

#endif
