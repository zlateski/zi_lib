/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_DEBUG_PRINTABLE_TYPE_HPP
#define ZI_DEBUG_PRINTABLE_TYPE_HPP 1

#include <zi/debug/detail/demangle.hpp>
#include <zi/utility/is_printable.hpp>

#include <iostream>
#include <string>

namespace zi {
namespace debug {

template< class Type > inline std::string
printable_type( Type t )
{
    return detail::demangle_type_of( t );
}

template< class Type > inline std::string
printable_type()
{
    return detail::demangle_type< Type >( );
}


} // namespace debug
} // namespace zi

#endif
