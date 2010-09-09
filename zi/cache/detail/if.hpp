/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_DETAIL_IF_HPP
#define ZI_CACHE_DETAIL_IF_HPP 1

namespace zi {
namespace cache {
namespace detail {

struct false_ {};

template< bool Cond, class True, class False = false_ >
struct if_
{
    typedef True type;
};

template< class True, class False >
struct if_< false, True, False >
{
    typedef False type;
};

template< bool Cond, class True, class False = false_ >
struct if_not: if_< Cond, False, True >
{
};

template< class CondT, class True, class False = false_ >
struct if_t: if_< CondT::value, True, False >
{
};

template< class CondT, class True, class False = false_ >
struct if_not_t: if_< CondT::value, False, True >
{
};


} // namespace detail
} // namespace cache
} // namespace zi

#endif
