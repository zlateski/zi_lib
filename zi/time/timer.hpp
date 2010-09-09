/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_TIME_TIMER_HPP
#define ZI_TIME_TIMER_HPP 1

#include <zi/time/config.hpp>
#include <zi/time/wall_timer.hpp>
#include <zi/time/process_timer.hpp>
#include <zi/bits/cstdint.hpp>

#include <ctime>

namespace zi {

class timer
{
public:
    typedef wall_timer    wall   ;
    typedef process_timer process;


private:
    wall_timer    wall_   ;
    process_timer process_;

public:

    template< class T > struct tv
    {
        T wall, process;
    };

    timer(): wall_(), process_()
    {
        restart();
    }

    inline void restart()
    {
        wall_.restart();
        process_.restart();
    }

    inline void reset()
    {
        wall_.reset();
        process_.reset();
    }

    template< class T > inline void lap( ::zi::timer::tv< T > &v )
    {
        v.wall    = wall_.template lap< T >();
        v.process = process_.template lap< T >();
    }

    template< class T > inline void elpased( ::zi::timer::tv< T > &v )
    {
        v.wall    = wall_.template elpased< T >();
        v.process = process_.template elpased< T >();
    }

    template< class T > inline void lap_elpased( ::zi::timer::tv< T > &v )
    {
        v.wall    = wall_.template lap_elpased< T >();
        v.process = process_.template lap_elpased< T >();
    }

};

} // namespace zi

#endif
