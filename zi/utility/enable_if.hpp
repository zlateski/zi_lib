/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_ENABLE_IF_HPP
#define ZI_UTILITY_ENABLE_IF_HPP 1

#include <zi/utility/detail/yes_no_type.hpp>

namespace zi {
namespace detail {
namespace enable_if_ {

template< bool Cond, class Type = void > struct enable_if
{
    typedef Type type;
};

template< class Type > struct enable_if< false, Type >
{
    /* No type defined */
};

template< class Cond, class Type = void > struct enable_if_t
{
    typedef Type type;
};

template< class Type > struct enable_if_t< detail::no_type, Type >
{
    /* No type defined */
};

} // namespace enable_if_


namespace disable_if_ {


template< bool Cond, class Type = void > struct disable_if
{
    typedef Type type;
};

template< class Type > struct disable_if< true, Type >
{
    /* No type defined */
};

template< class Cond, class Type = void > struct disable_if_t
{
    typedef Type type;
};

template< class Type > struct disable_if_t< detail::yes_type, Type >
{
    /* No type defined */
};


} // namespace disable_if_
} // namespace detail

using detail::enable_if_::enable_if ;
using detail::disable_if_::disable_if;
using detail::enable_if_::enable_if_t ;
using detail::disable_if_::disable_if_t;

} // namespace zi

#endif
