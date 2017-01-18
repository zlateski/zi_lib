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

#ifndef ZI_WATERSHED_DETAIL_RENEW_HPP
#define ZI_WATERSHED_DETAIL_RENEW_HPP 1

#include <zi/bits/type_traits.hpp>
#include <limits>
#include <cstddef>
#include <new>

namespace zi {
namespace watershed {
namespace detail {

template <typename T>
void renew(T& v)
{
    T n;
    n.swap(v);
}

template <typename T>
void reconstruct(T& v)
{
    T n;
    n.swap(v);
}

template <typename T, typename A1>
void reconstruct(T& v, A1 a1)
{
    T n(a1);
    n.swap(v);
}

template <typename T, typename A1, typename A2>
void reconstruct(T& v, A1 a1, A2 a2)
{
    T n(a1,a2);
    n.swap(v);
}

template <typename T, typename A1, typename A2, typename A3>
void reconstruct(T& v, A1 a1, A2 a2, A3 a3)
{
    T n(a1,a2,a3);
    n.swap(v);
}

} // namespace detail
} // namespace watershed
} // namespace zi

#endif

