//
// Copyright (C) 2010  Aleksandar Zlateski <zlateski@mit.edu>
// ----------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef ZI_TIME_WALL_TIMER_HPP
#define ZI_TIME_WALL_TIMER_HPP 1

#include <zi/time/config.hpp>
#include <zi/time/now.hpp>
#include <zi/time/time_units.hpp>
#include <zi/utility/enable_if.hpp>
#include <zi/bits/cstdint.hpp>
#include <zi/bits/type_traits.hpp>

namespace zi {

class wall_timer
{
private:
    int64_t start_, lap_start_;
    static const int64_t TICKS_PER_SEC = 1000000000;

    static inline int64_t convert_( int64_t value, int64_t factor )
    {
        return ( value / factor ) + ( ( value % factor ) > ( factor >> 1 ) ? 1 : 0 );
    }

public:
    wall_timer()
    {
        restart();
    }

    inline void restart()
    {
        start_ = lap_start_ = now::nsec();
    }

    inline void reset()
    {
        restart();
    }

    template< class T >
    inline T lap( typename enable_if< is_fundamental< T >::value >::type* = 0 )
    {
        int64_t last = lap_start_;
        lap_start_ = now::nsec();
        return T( lap_start_ - last ) / TICKS_PER_SEC;
    }

    template< class T >
    inline T elpased( typename enable_if< is_fundamental< T >::value >::type* = 0 )
    {
        return T( now::nsec() - start_ ) / TICKS_PER_SEC;
    }

    template< class T >
    inline T lap_elpased( typename enable_if< is_fundamental< T >::value >::type* = 0 )
    {
        return T( now::nsec() - lap_start_ ) / TICKS_PER_SEC;
    }

    template< class T >
    inline int64_t lap( typename disable_if< is_fundamental< T >::value >::type* = 0 )
    {
        int64_t last = lap_start_;
        lap_start_ = now::nsec();
        return convert_( lap_start_ - last, T::factor );
    }

    template< class T >
    inline int64_t elpased( typename disable_if< is_fundamental< T >::value >::type* = 0 )
    {
        return convert_( now::nsec() - start_, T::factor );
    }

    template< class T >
    inline int64_t lap_elpased( typename disable_if< is_fundamental< T >::value >::type* = 0 )
    {
        return convert_( now::nsec() - lap_start_, T::factor );
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
