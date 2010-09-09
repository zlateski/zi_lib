/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_SHARED_PTR_HPP
#define ZI_BITS_SHARED_PTR_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <memory>
#  define ZI_SHARED_PTR_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/memory>
#    define ZI_SHARED_PTR_NAMESPACE ::std::tr1
#  else
#    include <boost/shared_ptr.hpp>
#    define ZI_SHARED_PTR_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_SHARED_PTR_NAMESPACE::shared_ptr ;
using ZI_SHARED_PTR_NAMESPACE::weak_ptr   ;

} // namespace zi

#undef ZI_SHARED_PTR_NAMESPACE
#endif
