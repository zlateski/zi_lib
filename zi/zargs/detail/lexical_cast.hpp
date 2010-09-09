/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZARGS_DETAIL_LEXICAL_CAST_HPP
#define ZI_ZARGS_DETAIL_LEXICAL_CAST_HPP 1

#include <zi/config/config.hpp>

#if !defined( ZI_NO_BOOST )
#  include <zi/zargs/detail/boost_lexical_cast.hpp>
#
#else
#  include <zi/zargs/detail/simple_lexical_cast.hpp>
#
#endif

#endif
