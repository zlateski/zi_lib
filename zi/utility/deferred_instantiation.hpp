/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_DEFERRED_INSTANTIATION_HPP
#define ZI_UTILITY_DEFERRED_INSTANTIATION_HPP 1

#include <zi/config/config.hpp>

namespace zi {

template< class TypeToInstantiate,
          class WaitOn1 = void, class WaitOn2 = void, class WaitOn3 = void,
          class WaitOn4 = void, class WaitOn5 = void, class WaitOn6 = void,
          class WaitOn7 = void, class WaitOn8 = void, class WaitOn9 = void,
          class WaitOna = void, class WaitOnb = void, class WaitOnc = void,
          class WaitOnd = void, class WaitOne = void, class WaitOnf = void
          >
struct deferred_instantiation
{
    typedef TypeToInstantiate type;
};

} // namespace zi

#if defined( ZI_CXX_MSVC )
#  define ZI_DEFERRED_INSTANTIATION( _c, ... ) _c
#else
#  define ZI_DEFERRED_INSTANTIATION( _c, ... )                  \
    ::zi::deferred_instantiation< _c, ##__VA_ARGS__ >::type
#endif

#endif
