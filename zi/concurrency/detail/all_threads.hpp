/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_DETAIL_ALL_THREADS_HPP
#define ZI_CONCURRENCY_DETAIL_ALL_THREADS_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/thread.hpp>

#include <cstddef>

namespace zi {

namespace concurrency_ {

namespace all_threads {



inline std::size_t finished()
{
    return all_threads_info.finished_count();
}

inline std::size_t started()
{
    return all_threads_info.started_count();
}

inline std::size_t active()
{
    return all_threads_info.active_count();
}

inline void join()
{
    all_threads_info.join_all();
}

inline void join_all()
{
    all_threads_info.join_all();
}



} // namespace all_threads

} // namespace concurrency_

} // namespace zi

#endif

