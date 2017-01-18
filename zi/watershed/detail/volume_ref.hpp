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

#ifndef ZI_WATERSHED_DETAIL_VOLUME_REF_HPP
#define ZI_WATERSHED_DETAIL_VOLUME_REF_HPP 1

#include <zi/watershed/detail/types.hpp>
#include <boost/multi_array.hpp>
#include <boost/multi_array/types.hpp>

namespace zi {
namespace watershed {

namespace volume_types = boost::multi_array_types;

namespace {

volume_types::extent_gen extents;
volume_types::index_gen  indices;

boost::fortran_storage_order matlab_storage_order;

}

using   boost::multi_array_types::index_range;
typedef boost::multi_array_types::index_range range;

template< typename T, std::size_t D >
class volume_ref: public boost::multi_array_ref< T, D >
{
private:
    typedef boost::multi_array_ref< T, D > super_type    ;

public:
    typedef volume_ref< T, D > type;

    typedef typename super_type::value_type value_type;
    typedef typename super_type::const_reference const_reference;
    typedef typename super_type::const_iterator const_iterator;
    typedef typename super_type::const_reverse_iterator const_reverse_iterator;
    typedef typename super_type::element element;
    typedef typename super_type::size_type size_type;
    typedef typename super_type::difference_type difference_type;
    typedef typename super_type::index index;

public:

    template< typename ExtentList >
    volume_ref( T* data, const ExtentList& extents )
        : super_type( data, extents, matlab_storage_order )
    { }

    const T& at( index i ) const
    {
        return super_type::base_[ i ];
    }

    T& at( index i )
    {
        return super_type::base_[ i ];
    }

};

template< typename T, std::size_t D >
class const_volume_ref: public boost::const_multi_array_ref< T, D >
{
private:
    typedef boost::const_multi_array_ref< T, D > super_type;

public:
    typedef volume_ref< T, D > type;

    typedef typename super_type::value_type value_type;
    typedef typename super_type::const_reference const_reference;
    typedef typename super_type::const_iterator const_iterator;
    typedef typename super_type::const_reverse_iterator const_reverse_iterator;
    typedef typename super_type::element element;
    typedef typename super_type::size_type size_type;
    typedef typename super_type::difference_type difference_type;
    typedef typename super_type::index index;

public:

    template< typename ExtentList >
    const_volume_ref( const T* data, const ExtentList& extents )
        : super_type( data, extents, matlab_storage_order )
    { }

    const T& at( index i ) const
    {
        return super_type::base_[ i ];
    }
};

using boost::multi_array;

} // namespace watershed
} // namespace zi

#endif
