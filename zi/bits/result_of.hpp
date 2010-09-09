/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_RESULT_OF_HPP
#define ZI_BITS_RESULT_OF_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <functional>
#  define ZI_RESULT_OF_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/functional>
#    define ZI_RESULT_OF_NAMESPACE ::std::tr1
#  else
#    include <boost/utility/result_of.hpp>
#    define ZI_RESULT_OF_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_RESULT_OF_NAMESPACE::result_of;

} // namespace zi

#undef ZI_RESULT_OF_NAMESPACE
#endif
