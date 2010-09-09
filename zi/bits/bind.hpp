/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_BIND_HPP
#define ZI_BITS_BIND_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <functional>
#  define ZI_BIND_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/functional>
#    define ZI_BIND_NAMESPACE ::std::tr1
#  else
#    include <boost/bind.hpp>
#    define ZI_BIND_NAMESPACE ::boost
#    define ZI_PLACEHOLDERS_IN_UNNAMED
#  endif
#endif

namespace zi {

using ZI_BIND_NAMESPACE::is_bind_expression;
using ZI_BIND_NAMESPACE::is_placeholder;
using ZI_BIND_NAMESPACE::bind;

namespace placeholders {

#ifndef ZI_PLACEHOLDERS_IN_UNNAMED

using namespace ZI_BIND_NAMESPACE::placeholders;

#else

using ::_1;
using ::_2;
using ::_3;
using ::_4;
using ::_5;
using ::_6;
using ::_7;
using ::_8;
using ::_9;

#endif

}

} // namespace zi

namespace {

#ifndef ZI_PLACEHOLDERS_IN_UNNAMED

using ZI_BIND_NAMESPACE::placeholders::_1;
using ZI_BIND_NAMESPACE::placeholders::_2;
using ZI_BIND_NAMESPACE::placeholders::_3;
using ZI_BIND_NAMESPACE::placeholders::_4;
using ZI_BIND_NAMESPACE::placeholders::_5;
using ZI_BIND_NAMESPACE::placeholders::_6;
using ZI_BIND_NAMESPACE::placeholders::_7;
using ZI_BIND_NAMESPACE::placeholders::_8;
using ZI_BIND_NAMESPACE::placeholders::_9;

#else
#  undef ZI_PLACEHOLDERS_IN_UNNAMED
#endif

}

#undef ZI_BIND_NAMESPACE
#endif
