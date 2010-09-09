/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_DETAIL_BARE_TYPE_HPP
#define ZI_UTILITY_DETAIL_BARE_TYPE_HPP 1

#include <zi/bits/type_traits.hpp>

namespace zi {
namespace detail {


template< class Type >
struct bare_type
{
    typedef typename remove_const<
        typename remove_cv<
            typename remove_reference<
                Type
                >::type
            >::type
        >::type type;
};


} // namespace detail
} // namespace zi

#endif
