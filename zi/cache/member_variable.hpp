/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_MEMBER_VARIABLE_HPP
#define ZI_CACHE_MEMBER_VARIABLE_HPP 1

#include <zi/cache/config.hpp>
#include <zi/cache/detail/if.hpp>
#include <zi/cache/detail/enable_if.hpp>
#include <zi/cache/detail/type_traits.hpp>
#include <zi/cache/detail/ref.hpp>

namespace zi {
namespace cache {

namespace member_variable_ {

template< class CachedType,
          class Type,
          Type CachedType::*MemberVariablePtr
          >
struct non_const_member
{
    typedef Type result_type;

    template< class PtrToCachedType >
    typename disable_if<
        is_convertible< const PtrToCachedType&, const CachedType& >::type::value, Type&
    >::type
    operator() ( const PtrToCachedType& ptr ) const
    {
        return this->operator() ( *ptr );
    }

    Type& operator() ( CachedType& v ) const
    {
        return v.*MemberVariablePtr;
    }

    const Type& operator() ( const CachedType& v, void* = 0 ) const
    {
        return v.*MemberVariablePtr;
    }

    Type& operator() ( const reference_wrapper< CachedType >& v_ref )
    {
        return this->operator() ( v_ref.get() );
    }

    const Type& operator() ( const reference_wrapper< const CachedType >& v_ref )
    {
        return this->operator() ( v_ref.get() );
    }

};

template< class CachedType,
          class Type,
          Type CachedType::*MemberVariablePtr
          >
struct const_member
{
    typedef Type result_type;

    template< class PtrToCachedType >
    typename disable_if<
        is_convertible< const PtrToCachedType&, const CachedType& >::type::value, Type&
    >::type
    operator() ( const PtrToCachedType& ptr ) const
    {
        return this->operator() ( *ptr );
    }

    Type& operator() ( const CachedType& v ) const
    {
        return v.*MemberVariablePtr;
    }

    Type& operator() ( const reference_wrapper< const CachedType >& v_ref ) const
    {
        return this->operator() ( v_ref.get() );
    }

    Type& operator() ( const reference_wrapper< CachedType >& v_ref, void* = 0 ) const
    {
        return this->operator() ( v_ref.get() );
    }

};

} // namespace member_variable_

template< class CachedType,
          class Type,
          Type CachedType::*MemberVariablePtr
          >
struct member_variable:
    detail::if_<
        is_const< Type >::value,
        member_variable_::const_member    < CachedType, Type, MemberVariablePtr >,
        member_variable_::non_const_member< CachedType, Type, MemberVariablePtr >
    >::type
{
};

} // namespace cache
} // namespace zi


#endif
