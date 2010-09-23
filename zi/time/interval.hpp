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

#ifndef ZI_TIME_INTERVAL_HPP
#define ZI_TIME_INTERVAL_HPP 1

#include <zi/time/config.hpp>
#include <zi/bits/cstdint.hpp>

namespace zi {
namespace interval {
namespace detail {

template< int64_t Factor > struct interval_tpl
{
private:

    int64_t value_;

    interval_tpl( int64_t v, void* distinguish_constructor ): value_( v )
    {
    }

public:

    template< int64_t F > friend struct interval_tpl;


    interval_tpl(): value_( 0 )
    {
    }

    interval_tpl( int64_t v ): value_( Factor * v )
    {
    }

    template< int64_t F > interval_tpl( const interval_tpl< F > &r ):
        value_( r.value_ )
    {
    }

    operator bool() const
    {
        return static_cast< bool >( value_ );
    }

    interval_tpl operator-() const
    {
        return interval_tpl( value_, 0 );
    }

    interval_tpl& operator*=( int64_t v )
    {
        value_ *= v;
        return *this;
    }

    interval_tpl& operator/=( int64_t v )
    {
        value_ /= v;
        return *this;
    }

    template< int64_t R >
    interval_tpl& operator+=( const interval_tpl< R >& r )
    {
        value_ += r.value_;
        return *this;
    }

    template< int64_t R >
    interval_tpl& operator-=( const interval_tpl< R >& r )
    {
        value_ -= r.value_;
        return *this;
    }

    template< int64_t R >
    interval_tpl& operator=( const interval_tpl< R >& r )
    {
        value_ = r.value_;
        return *this;
    }

    interval_tpl operator*( int64_t v )
    {
        return interval_tpl( value_ * v, 0 );
    }

    interval_tpl operator/( int64_t v )
    {
        return interval_tpl( value_ / v, 0 );
    }

    template< int64_t R >
    interval_tpl operator+( const interval_tpl< R >& r )
    {
        return interval_tpl( value_ + r.value_, 0 );
    }

    template< int64_t R >
    interval_tpl operator-( const interval_tpl< R >& r )
    {
        return interval_tpl( value_ - r.value_, 0 );
    }

#define INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( _op )          \
                                                                        \
    template< int64_t R >                                               \
    bool operator _op ( const interval_tpl< R >& r ) const              \
    {                                                                   \
        return value_ _op r.value_;                                     \
    }


    INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( <= )
    INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( <  )
    INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( >= )
    INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( >  )
    INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( == )
    INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR( != )

#undef INTERVAL_TPL_DEFINE_FRIEND_BOOL_BINARY_OPERATOR

    int64_t nsecs() const
    {
        return value_;
    }

    int64_t nanoseconds() const
    {
        return value_;
    }

#define INTERVAL_TPL_DEFINE_FUNCTION( _name, _factor )                  \
                                                                        \
    int64_t _name() const                                               \
    {                                                                   \
        return ( value_ / _factor ) +                                   \
            ( ( value_ % _factor ) > ( _factor >> 1 ) ? 1 : 0 );        \
    }

    INTERVAL_TPL_DEFINE_FUNCTION( usecs, 1000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( microseconds, 1000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( msecs, 1000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( milliseconds, 1000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( secs, 1000000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( seconds, 1000000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( mins, 60000000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( minutes, 60000000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( hours, 3600000000000LL )
    INTERVAL_TPL_DEFINE_FUNCTION( days, 86400000000000LL )

#undef INTERVAL_TPL_DEFINE_FUNCTION

};

} // namespace detail

typedef detail::interval_tpl< 1LL >              nsecs;
typedef detail::interval_tpl< 1LL >              nanoseconds;
typedef detail::interval_tpl< 1000LL >           usecs;
typedef detail::interval_tpl< 1000LL >           microseconds;
typedef detail::interval_tpl< 1000000LL >        msecs;
typedef detail::interval_tpl< 1000000LL >        milliseconds;
typedef detail::interval_tpl< 1000000000LL >     secs;
typedef detail::interval_tpl< 1000000000LL >     seconds;
typedef detail::interval_tpl< 60000000000LL >    mins;
typedef detail::interval_tpl< 60000000000LL >    minutes;
typedef detail::interval_tpl< 3600000000000LL >  hours;
typedef detail::interval_tpl< 86400000000000LL > days;


} // namespace interval
} // namespace zi

#endif
