//
// Copyright (C) 2011  Aleksandar Zlateski <zlateski@mit.edu>
// ----------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef ZI_UTILITY_DETAIL_MOST_SIGNIFICANT_BIT_HPP
#define ZI_UTILITY_DETAIL_MOST_SIGNIFICANT_BIT_HPP 1

#include <zi/bits/type_traits.hpp>
#include <zi/bits/cstdint.hpp>
#include <cstddef>
#include <limits>

namespace zi {
namespace detail {

template <typename T,typename>
struct most_significant_bit_helper2;

template <typename T>
struct most_significant_bit_helper2<T,true_type>
{
    static const T value =
        (integral_constant<T,1>::value<<std::numeric_limits<T>::digits );
};

template <typename T>
struct most_significant_bit_helper2<T,false_type>
{
    static const T value =
        (integral_constant<T,1>::value<<(std::numeric_limits<T>::digits-1));
};

template <typename T,typename>
struct most_significant_bit_helper;

template <typename T>
struct most_significant_bit_helper<T,true_type>
    : most_significant_bit_helper2<T,typename is_signed<T>::type>
{};

} // namespace detail

template <typename T>
struct most_significant_bit: detail::most_significant_bit_helper<T,typename is_integral<T>::type >
{};

} // namespace zi

#endif
