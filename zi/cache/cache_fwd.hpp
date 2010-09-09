/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_LIBS_CACHE_FWD_HPP
#define ZI_LIBS_CACHE_FWD_HPP 1

#include <zi/cache/config.hpp>

namespace zi {
namespace cache {

template<
    class CachedType,
    class KeyDefinition = identified_by<
        identity< CachedType >,
        std::less< identity< CachedType >::result_type >
        >,
    class CacheHandlers = handled_by,
    class CachePolicies
    >
struct cache;

} // namespace cache
} // namespace zi

#endif
