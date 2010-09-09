/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_DETAIL_MULTI_INDEX_HPP
#define ZI_CACHE_DETAIL_MULTI_INDEX_HPP 1

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

namespace zi {
namespace cache {
namespace multi_index  {

using namespace boost::multi_index;

} // namespace multi_array

using multi_index::multi_index_container;

} // namespace cache
} // namespace zi

#define MULTI_INDEX_MEMBER BOOST_MULTI_INDEX_MEMBER

#endif

