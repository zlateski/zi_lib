/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_DEFAULT_CONSTRUCTOR_HPP
#define ZI_CACHE_DEFAULT_CONSTRUCTOR_HPP 1

#include <zi/cache/config.hpp>
#include <zi/cache/detail/if.hpp>
#include <zi/cache/detail/enable_if.hpp>
#include <zi/cache/detail/type_traits.hpp>
#include <zi/cache/detail/ref.hpp>

namespace zi {
namespace cache {
namespace default_constructor_ {

template< class Key > struct key_instance
{
    static Key& refe;
};


namespace value_from_key_ctor {

template< class Value, class Key > char test( char(*)[ sizeof( Value( key_instance<Key>::refe ) ) ] );
template< class Value, class Key > int  test( ... );

}

template< class Value, class Key >
struct has_value_of_key_ctor
{
    static const bool value =
        ( sizeof( value_from_key_ctor::test< Value, Key >( 0 ) ) == 1 );
};

} // namespace default_constructor_

using default_constructor_::has_value_of_key_ctor;

} // namespace cache
} // namespace zi


#endif
