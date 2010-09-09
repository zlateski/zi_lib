/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_SIMPLE_TASK_CONTAINER_HPP
#define ZI_CONCURRENCY_DETAIL_SIMPLE_TASK_CONTAINER_HPP 1

#include <zi/concurrency/runnable.hpp>
#include <zi/bits/shared_ptr.hpp>
#include <deque>

namespace zi {
namespace concurrency_ {
namespace detail {

typedef std::deque< shared_ptr< runnable > > simple_task_container;

} // namespace detail
} // namespace concurrency_
} // namespace zi

#endif

