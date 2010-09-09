/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_REF_HPP
#define ZI_BITS_REF_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <functional>
#  define ZI_REF_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/functional>
#    define ZI_REF_NAMESPACE ::std::tr1
#  else
#    include <boost/ref.hpp>
#    define ZI_REF_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_REF_NAMESPACE::reference_wrapper;
using ZI_REF_NAMESPACE::ref;
using ZI_REF_NAMESPACE::cref;

} // namespace zi

#undef ZI_REF_NAMESPACE
#endif
