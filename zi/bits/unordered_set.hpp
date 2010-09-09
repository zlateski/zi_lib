/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_UNORDERED_SET_HPP
#define ZI_BITS_UNORDERED_SET_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <unordered_set>
#  define ZI_UNORDERED_SET_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/unordered_set>
#    define ZI_UNORDERED_SET_NAMESPACE ::std::tr1
#  else
#    include <boost/unordered_set.hpp>
#    define ZI_UNORDERED_SET_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_UNORDERED_SET_NAMESPACE::unordered_set;
using ZI_UNORDERED_SET_NAMESPACE::unordered_multiset;
using ZI_UNORDERED_SET_NAMESPACE::swap;

} // namespace zi

#undef ZI_UNORDERED_SET_NAMESPACE
#endif
