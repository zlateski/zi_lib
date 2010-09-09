/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_MEMBER_FUNCTION_HPP
#define ZI_CACHE_MEMBER_FUNCTION_HPP 1

#include <zi/cache/config.hpp>
#include <zi/cache/detail/enable_if.hpp>
#include <zi/cache/detail/type_traits.hpp>
#include <zi/cache/detail/ref.hpp>

namespace zi {
namespace cache {

template< class CachedType,
          class Return,
          Return (CachedType::*MemberFunctionPtr)()
          >
struct member_function
{
    typedef typename remove_reference< Return >::type result_type;

    template< class PtrToCachedType >
    typename disable_if<
        is_convertible< PtrToCachedType&, CachedType& >::type::value,
        Return
    >::type
    operator() ( const PtrToCachedType& ptr ) const
    {
        return this->operator() ( *ptr );
    }

    Return operator() ( CachedType& v ) const
    {
        return (v.*MemberFunctionPtr)();
    }

    Return operator() ( const reference_wrapper< CachedType >& v_ref )
    {
        return this->operator() ( v_ref.get() );
    }

};

template< class CachedType,
          class Return,
          Return (CachedType::*MemberFunctionPtr)() const
          >
struct const_member_function
{
    typedef typename remove_reference< Return >::type result_type;

    template< class PtrToCachedType >
    typename disable_if<
        is_convertible< const PtrToCachedType&, const CachedType& >::type::value,
        Return
    >::type
    operator() ( const PtrToCachedType& ptr ) const
    {
        return this->operator() ( *ptr );
    }

    Return operator() ( const CachedType& v ) const
    {
        return (v.*MemberFunctionPtr)();
    }

    Return operator() ( const reference_wrapper< CachedType >& v_ref ) const
    {
        return this->operator() ( v_ref.get() );
    }

    Return operator() ( const reference_wrapper< const CachedType >& v_ref ) const
    {
        return this->operator() ( v_ref.get() );
    }

};



} // namespace cache
} // namespace zi


#endif
