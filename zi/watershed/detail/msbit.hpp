//
// Copyright (C) 2010-2011  Aleksandar Zlateski <zlateski@mit.edu>
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

#ifndef ZI_WATERSHED_DETAIL_MSBIT_HPP
#define ZI_WATERSHED_DETAIL_MSBIT_HPP 1

#include <zi/bits/type_traits.hpp>
#include <limits>
#include <cstddef>
#include <climits>

namespace zi {
namespace watershed {
namespace detail {


template <typename T,T one,class>
struct msbit_helper;

template <typename T,T one>
struct msbit_helper<T,one,true_type>
{
    typedef msbit_helper type;
    static const T value = (one << ((sizeof(T)*CHAR_BIT)-1));
};

template <typename T>
struct msbit: msbit_helper<T,1,typename is_integral< T >::type >
{};

} // namespace detail
} // namespace watershed
} // namespace zi

#endif

