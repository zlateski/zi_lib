/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_STATIC_IF_HPP
#define ZI_UTILITY_STATIC_IF_HPP 1

namespace zi {

template< bool Cond, class IfTrueType, class IfFalseType >
struct static_if
{
    typedef IfTrueType type;
};

template< class IfTrueType, class IfFalseType >
struct static_if< false, IfTrueType, IfFalseType >
{
    typedef IfFalseType type;
};

template< bool Cond, class IfTrueType, class IfFalseType >
struct if_
{
    typedef IfTrueType type;
};

template< class IfTrueType, class IfFalseType >
struct if_< false, IfTrueType, IfFalseType >
{
    typedef IfFalseType type;
};

} // namespace zi

#endif
