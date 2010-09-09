/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_ARRAY_HPP
#define ZI_BITS_ARRAY_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <array>
#  define ZI_ARRAY_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/array>
#    define ZI_ARRAY_NAMESPACE ::std::tr1
#  else
#    include <boost/array.hpp>
#    define ZI_ARRAY_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_ARRAY_NAMESPACE::array;
using ZI_ARRAY_NAMESPACE::swap;

} // namespace zi

#undef ZI_ARRAY_NAMESPACE
#endif
