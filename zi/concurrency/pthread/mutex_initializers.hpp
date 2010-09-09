/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_PTHREAD_MUTEX_INITIALIZERS_HPP
#define ZI_CONCURRENCY_PTHREAD_MUTEX_INITIALIZERS_HPP 1

#include <zi/concurrency/config.hpp>
#include <zi/concurrency/pthread/mutex_tags.hpp>

#include <pthread.h>

namespace zi {

namespace concurrency_ {


template< class PtMutexTag > struct mutex_initializer;

template<> struct mutex_initializer< mutex_default_tag >
{
    static void initialize( pthread_mutex_t &ptm)
    {
        static const pthread_mutex_t stored_initializer =
            PTHREAD_MUTEX_INITIALIZER;
        ptm = stored_initializer;
    }
};

template<> struct mutex_initializer< mutex_adaptive_tag >
{
    static void initialize( pthread_mutex_t &ptm)
    {
        static const pthread_mutex_t stored_initializer =
            PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP;
        ptm = stored_initializer;
    }
};

template<> struct mutex_initializer< mutex_recursive_tag >
{
    static void initialize( pthread_mutex_t &ptm)
    {
        static const pthread_mutex_t stored_initializer =
            PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
        ptm = stored_initializer;
    }
};



} // namespace concurrency_

} // namespace zi

#endif
