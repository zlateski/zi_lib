/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_DETAIL_NOT_THIS_TYPE_HPP
#define ZI_UTILITY_DETAIL_NOT_THIS_TYPE_HPP 1

namespace zi {
namespace detail {

template< class Type >
struct not_this_type
{
    typedef char ( &type )[ sizeof( Type ) + 1 ];
};

} // namespace detail
} // namespace zi

#endif
