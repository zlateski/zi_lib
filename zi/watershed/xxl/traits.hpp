//
// Copyright (C) 2010  Aleksandar Zlateski <zlateski@mit.edu>
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

#ifndef ZI_WATERSHED_XXL_TRAITS_HPP
#define ZI_WATERSHED_XXL_TRAITS_HPP 1

#include <zi/watershed/detail/volume_ref.hpp>
#include <zi/bits/tuple.hpp>
#include <zi/bits/unordered_map.hpp>
#include <utility>
#include <vector>

namespace zi {
namespace watershed {

template< class T, class Id, class Count >
struct watershed_traits
{

    typedef T         value_type      ;
    typedef T*        value_ptr       ;
    typedef const T*  const_value_ptr ;

    typedef Id        id_type         ;
    typedef Id*       id_ptr          ;
    typedef const Id* const_id_ptr    ;

    typedef Count     count_type      ;

    typedef const_volume_ref< value_type, 4 > affinity_volume_ref;
    typedef volume_ref< id_type, 3 >          id_volume_ref;

    typedef std::pair< id_type, id_type > id_pair;
    typedef tuple< id_type, id_type, T >  edge_type;

    typedef std::vector< edge_type >      edge_vector;
    typedef unordered_map< id_pair, T >   edge_map;

};

} // namespace watershed
} // namespace zi

#endif
