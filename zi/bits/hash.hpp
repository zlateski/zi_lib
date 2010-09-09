/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_HASH_HPP
#define ZI_BITS_HASH_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <functional>
#  define ZI_HASH_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/functional>
#    define ZI_HASH_NAMESPACE ::std::tr1
#  else
#    include <boost/functional/hash.hpp>
#    define ZI_HASH_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_HASH_NAMESPACE::hash;

} // namespace zi

#undef ZI_HASH_NAMESPACE
#endif
