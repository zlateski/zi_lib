/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_FUNCTION_HPP
#define ZI_BITS_FUNCTION_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <functional>
#  define ZI_FUNCTION_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/functional>
#    define ZI_FUNCTION_NAMESPACE ::std::tr1
#  else
#    include <boost/function.hpp>
#    define ZI_FUNCTION_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_FUNCTION_NAMESPACE::bad_function_call;
using ZI_FUNCTION_NAMESPACE::function;
using ZI_FUNCTION_NAMESPACE::swap;

} // namespace zi

#undef ZI_FUNCTION_NAMESPACE
#endif
