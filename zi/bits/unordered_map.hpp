/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_UNORDERED_MAP_HPP
#define ZI_BITS_UNORDERED_MAP_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <unordered_map>
#  define ZI_UNORDERED_MAP_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/unordered_map>
#    define ZI_UNORDERED_MAP_NAMESPACE ::std::tr1
#  else
#    include <boost/unordered_map.hpp>
#    define ZI_UNORDERED_MAP_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_UNORDERED_MAP_NAMESPACE::unordered_map;
using ZI_UNORDERED_MAP_NAMESPACE::unordered_multimap;
using ZI_UNORDERED_MAP_NAMESPACE::swap;

} // namespace zi

#undef ZI_UNORDERED_MAP_NAMESPACE
#endif
