/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_DETAIL_GET_INSTANCE_HPP
#define ZI_UTILITY_DETAIL_GET_INSTANCE_HPP 1

#include <zi/bits/type_traits.hpp>
#include <zi/utility/detail/bare_type.hpp>

namespace zi {
namespace detail {
namespace get_instance_ {

template< class Type >
struct get_instance
{
    typedef typename bare_type< Type >::type type;

    static type         static_instance;
    static type&        static_reference;
    static const type&  static_const_reference;
    static type*        static_pointer;
    static const type*  static_const_pointer;

};


} // namespace get_instance_

using get_instance_::get_instance;

} // namespace detail
} // namespace zi

#endif
