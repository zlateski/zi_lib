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
#include <zi/bits/cstdint.hpp>

namespace zi {

class wall_timer
{
private:
    int64_t start_, lap_start_;
    static const int64_t TICKS_PER_SEC = 1000000;

public:
    wall_timer()
    {
        restart();
    }

    inline void restart()
    {
        start_ = lap_start_ = now::usec();
    }

    inline void reset()
    {
        restart();
    }

    template< class T > inline T lap()
    {
        int64_t last = lap_start_;
        lap_start_ = now::usec();
        return T( lap_start_ - last ) / TICKS_PER_SEC;
    }

    template< class T > inline T elpased()
    {
        return T( now::usec() - start_ ) / TICKS_PER_SEC;
    }

    template< class T > inline T lap_elpased()
    {
        return T( now::usec() - lap_start_ ) / TICKS_PER_SEC;
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
