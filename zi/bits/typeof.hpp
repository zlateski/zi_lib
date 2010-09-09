/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_TYPEOF_HPP
#define ZI_BITS_TYPEOF_HPP 1

#include <zi/config/config.hpp>
#
#if defined( ZI_CXX_GCC )
#  ifndef __typeof__
#    define __typeof__( expr ) typeof( expr )
#  endif
#
#elif defined( ZI_CXX_MSVC )
#  ifndef ZI_NO_BOOST
#    include <boost/typeof/typeof.hpp>
#    define __typeof__( expr ) BOOST_TYPEOF( expr )
#  else
#    error "no typeof implementation available"
#  endif
#
#else
#  ifndef __typeof__
#    error "no typeof implementation available"
#  endif
#endif
#
#endif
