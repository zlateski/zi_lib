/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_IS_PRINTABLE_HPP
#define ZI_UTILITY_IS_PRINTABLE_HPP 1

#include <zi/utility/enable_if.hpp>

#include <zi/utility/detail/get_instance.hpp>
#include <zi/utility/detail/whatever.hpp>
#include <zi/utility/detail/not_this_type.hpp>

#include <iostream>
#include <cstddef>
#include <string>

namespace zi {
namespace detail {
namespace is_printable_ {

// sizeof( not_ostream ) != sizeof( std::ostream)
typedef detail::not_this_type< std::ostream >::type not_ostream;

// overload the operator<< ( std::ostream&, type ) with a
// dummy type that has different size than std::ostream
not_ostream operator<< ( std::ostream&, detail::whatever );


template< class Type, std::size_t S >
struct is_printable_helper
{
    static const bool value = 0;
};

template< std::size_t S >
struct is_printable_helper< void, S >
{
    static const bool value = 0;
};

template< class Type > struct
is_printable_helper< Type, sizeof( std::ostream ) >
{
    static const bool value = 1;
};


template< class Type >
struct is_printable : is_printable_helper<
    Type,
    sizeof( detail::get_instance< std::ostream >::static_reference <<
            detail::get_instance< Type >::static_const_reference ) >
{
};

// specialization for void

template<>
struct is_printable< void > : is_printable_helper< void, 0 >
{
};

template<>
struct is_printable< const void > : is_printable_helper< const void, 0 >
{
};

template<>
struct is_printable< const volatile void > : is_printable_helper< const volatile void, 0 >
{
};


} // namespace is_printable_
} // namespace detail

using detail::is_printable_::is_printable ;

} // namespace zi

#endif
