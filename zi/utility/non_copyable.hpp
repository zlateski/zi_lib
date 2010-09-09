/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_NON_COPYABLE_HPP
#define ZI_UTILITY_NON_COPYABLE_HPP 1

namespace zi {
namespace non_copyable_ {

class non_copyable
{
protected:
     non_copyable() {}
    ~non_copyable() {}

private:

    non_copyable( const non_copyable& );
    const non_copyable& operator=( const non_copyable& );

};

} // namespace non_copyable_

using non_copyable_::non_copyable;

} // namespace zi

#endif
