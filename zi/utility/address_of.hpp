/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_ADDRESS_OF_HPP
#define ZI_UTILITY_ADDRESS_OF_HPP 1

#include <zi/utility/detail/dummy.hpp>

namespace zi {
namespace address_of_ {

template< class T > struct address_of_helper
{

    static inline T* get_address( T &t, detail::dummy< 0 > = 0 )
    {
        return reinterpret_cast< T* >
            ( &const_cast< char& >
              ( reinterpret_cast< const volatile char& >( t ) ));
    }

    static inline T* get_address( T *t, detail::dummy< 1 > = 0 )
    {
        return t;
    }
};

template< class T > T* address_of( T &t )
{
    return address_of_helper< T >::get_address( t );
}

} // namespace address_of_

using address_of_::address_of;

} // namespace zi

#endif
