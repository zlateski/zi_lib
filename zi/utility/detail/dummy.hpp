/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_DETAIL_DUMMY_HPP
#define ZI_UTILITY_DETAIL_DUMMY_HPP 1

namespace zi {
namespace detail {

template< int > struct dummy
{
    dummy( int ) {}
};

} // namespace detail
} // namespace zi

#endif
