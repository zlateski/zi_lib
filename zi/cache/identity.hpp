/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_IDENTITY_HPP
#define ZI_CACHE_IDENTITY_HPP 1

#include <zi/cache/config.hpp>
#include <zi/cache/detail/if.hpp>
#include <zi/cache/detail/enable_if.hpp>
#include <zi/cache/detail/type_traits.hpp>
#include <zi/cache/detail/ref.hpp>

namespace zi {
namespace cache {

namespace identity_ {

template< class Type >
struct non_const_identity
{
    typedef Type result_type;

    template< class PtrToType >
    typename disable_if<
        is_convertible< const PtrToType&, const Type& >::type::value, Type&
    >::type
    operator() ( const PtrToType& ptr ) const
    {
        return this->operator() ( *ptr );
    }

    Type& operator() ( Type& v ) const
    {
        return v;
    }

    const Type& operator() ( const Type& v, void* = 0 ) const
    {
        return v;
    }

    Type& operator() ( const reference_wrapper< Type >& v_ref )
    {
        return v_ref.get();
    }

    const Type& operator() ( const reference_wrapper< const Type >& v_ref )
    {
        return v_ref.get();
    }

};

template< class Type >
struct const_identity
{
    typedef Type result_type;

    template< class PtrToType >
    typename disable_if<
        is_convertible< const PtrToType&, const Type& >::type::value, Type&
    >::type
    operator() ( const PtrToType& ptr ) const
    {
        return this->operator() ( *ptr );
    }

    Type& operator() ( Type& v ) const
    {
        return v;
    }

    Type& operator() ( const reference_wrapper< Type >& v_ref ) const
    {
        return v_ref.get();
    }

    Type& operator() ( const reference_wrapper<
                           typename remove_const< Type >::type
                       >& v_ref, void* = 0 ) const
    {
        return v_ref.get();
    }

};

} // namespace identity_

template< class Type >
struct identity:
    detail::if_<
        is_const< Type >::value,
        identity_::const_identity    < Type >,
        identity_::non_const_identity< Type >
    >::type
{
};

} // namespace cache
} // namespace zi


#endif
