/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_DEBUG_DETAIL_DEMANGLE_HPP
#define ZI_DEBUG_DETAIL_DEMANGLE_HPP 1

#include <zi/config/config.hpp>

#include <cstddef>
#include <cstdlib>
#include <typeinfo>
#include <string>

#if defined( __GLIBCXX__ ) || defined( __GLIBCPP__ )
#  define ZI_GLIBC_DEMANGLING_AVAILABLE
#  include <cxxabi.h>
#endif

namespace zi {
namespace detail {

inline std::string demangle_name( const char* mangled )
{

#ifdef ZI_GLIBC_DEMANGLING_AVAILABLE

    char*       demangled_cstr;
    std::size_t length;
    int         ignore;

    demangled_cstr = abi::__cxa_demangle( mangled, NULL, &length, &ignore );

    if ( demangled_cstr )
    {
        std::string demangled( demangled_cstr );
        std::free( demangled_cstr );
        return demangled;
    }
    else
    {
        return std::string("non-demangleable symbol: ") + mangled;
    }

#else

    return std::string( mangled );

#endif

}

template< class Type > inline std::string demangle_type_of( const Type& )
{
    return demangle_name( typeid( Type ).name() );
}

template< class Type > inline std::string demangle_type( )
{
    return demangle_name( typeid( Type ).name() );
}


} // namespace detail
} // namespace zi

#if defined ZI_GLIBC_DEMANGLING_AVAILABLE
#  undef ZI_GLIBC_DEMANGLING_AVAILABLE
#endif

#endif
