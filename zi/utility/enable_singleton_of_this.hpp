/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_ENABLE_SINGLETON_OF_THIS_HPP
#define ZI_UTILITY_ENABLE_SINGLETON_OF_THIS_HPP 1

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/singleton.hpp>

namespace zi {
namespace singleton_ {

template< class Type >
class enable_singleton_of_this: non_copyable
{
public:

    template< class Tag > static Type& instance()
    {
        return singleton< Type >::template instance< Tag >();
    }

    static Type& instance()
    {
        return singleton< Type >::instance();
    }

};

} // namespace singleton_

using singleton_::enable_singleton_of_this;

} // namespace zi

#endif
