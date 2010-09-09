/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_SINGLETON_HPP
#define ZI_UTILITY_SINGLETON_HPP 1

namespace zi {
namespace singleton_ {

template< class Type > class singleton: private Type
{
private:

    inline  singleton() {}
    inline ~singleton() {}

public:

    typedef singleton< Type > type;

    template< class Tag > static Type& instance()
    {
        static singleton< Type > instance;
        return instance;
    }

    static Type& instance()
    {
        static singleton< Type > instance;
        return instance;
    }

};

template< class Type > Type& singleton_of()
{
    return singleton< Type >::instance();
}

} // namespace singleton_

using singleton_::singleton;
using singleton_::singleton_of;

} // namespace zi

#endif
