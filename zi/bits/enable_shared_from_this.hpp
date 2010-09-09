/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_ENABLE_SHARED_FROM_THIS_HPP
#define ZI_BITS_ENABLE_SHARED_FROM_THIS_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <memory>
#  define ZI_ENABLE_SHARED_FROM_THIS_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/memory>
#    define ZI_ENABLE_SHARED_FROM_THIS_NAMESPACE ::std::tr1
#  else
#    include <boost/enable_shared_from_this.hpp>
#    define ZI_ENABLE_SHARED_FROM_THIS_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_ENABLE_SHARED_FROM_THIS_NAMESPACE::enable_shared_from_this;

} // namespace zi

#undef ZI_ENABLE_SHARED_FROM_THIS_NAMESPACE
#endif
