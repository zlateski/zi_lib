/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_DEBUG_PRINTABLE_VALUE_HPP
#define ZI_DEBUG_PRINTABLE_VALUE_HPP 1

#include <zi/debug/detail/demangle.hpp>

#include <zi/utility/enable_if.hpp>
#include <zi/utility/is_printable.hpp>
#include <zi/utility/address_of.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cstddef>

namespace zi {
namespace debug {

template< class Type > inline std::string
printable_value( const Type& t,
                 typename enable_if< is_printable< Type >::value >::type* = 0 )
{
    std::ostringstream ss;
    ss << t;
    return ss.str();
}

template< class Type > inline std::string
printable_value( const Type& t,
                 typename disable_if< is_printable< Type >::value >::type* = 0 )
{
    std::ostringstream ss;
    ss << "[obj@" << address_of( t ) << "]";
    return ss.str();
}

template< class Type > inline std::string
printable_value( Type* t )
{
    std::ostringstream ss;
    ss << "["  << reinterpret_cast< const void* >( t ) << "]";
    return ss.str();
}

template< class Type, std::size_t S > inline std::string
printable_value( Type t[S] )
{
    std::ostringstream ss;
    ss << "[" << reinterpret_cast< const void* >( t ) << "]";
    return ss.str();
}


} // namespace debug
} // namespace zi

#endif
