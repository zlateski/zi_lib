/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_CSTDINT_HPP
#define ZI_BITS_CSTDINT_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <cstdint>
#  define ZI_CSTDINT_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/cstdint>
#    define ZI_CSTDINT_NAMESPACE ::std::tr1
#  else
#    include <boost/cstdint.hpp>
#    define ZI_NEED_INTPTR_TYPES
#    define ZI_CSTDINT_NAMESPACE ::boost
#  endif
#endif

namespace zi {

using ZI_CSTDINT_NAMESPACE::int8_t;
using ZI_CSTDINT_NAMESPACE::int16_t;
using ZI_CSTDINT_NAMESPACE::int32_t;
using ZI_CSTDINT_NAMESPACE::int64_t;

using ZI_CSTDINT_NAMESPACE::int_fast8_t;
using ZI_CSTDINT_NAMESPACE::int_fast16_t;
using ZI_CSTDINT_NAMESPACE::int_fast32_t;
using ZI_CSTDINT_NAMESPACE::int_fast64_t;

using ZI_CSTDINT_NAMESPACE::int_least8_t;
using ZI_CSTDINT_NAMESPACE::int_least16_t;
using ZI_CSTDINT_NAMESPACE::int_least32_t;
using ZI_CSTDINT_NAMESPACE::int_least64_t;

using ZI_CSTDINT_NAMESPACE::intmax_t;

using ZI_CSTDINT_NAMESPACE::uint8_t;
using ZI_CSTDINT_NAMESPACE::uint16_t;
using ZI_CSTDINT_NAMESPACE::uint32_t;
using ZI_CSTDINT_NAMESPACE::uint64_t;

using ZI_CSTDINT_NAMESPACE::uint_fast8_t;
using ZI_CSTDINT_NAMESPACE::uint_fast16_t;
using ZI_CSTDINT_NAMESPACE::uint_fast32_t;
using ZI_CSTDINT_NAMESPACE::uint_fast64_t;

using ZI_CSTDINT_NAMESPACE::uint_least8_t;
using ZI_CSTDINT_NAMESPACE::uint_least16_t;
using ZI_CSTDINT_NAMESPACE::uint_least32_t;
using ZI_CSTDINT_NAMESPACE::uint_least64_t;

using ZI_CSTDINT_NAMESPACE::uintmax_t;

#if defined( ZI_NEED_INTPTR_TYPES )
using ::intptr_t;
using ::uintptr_t;
#else
using ZI_CSTDINT_NAMESPACE::uintptr_t;
using ZI_CSTDINT_NAMESPACE::intptr_t;
#endif

} // namespace zi

#undef ZI_NEED_INTPTR_TYPES
#undef ZI_CSTDINT_NAMESPACE
#endif
