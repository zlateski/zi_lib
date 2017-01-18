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

#ifndef ZI_GRAPH_BICONNECTED_COMPONENTS_HPP
#define ZI_GRAPH_BICONNECTED_COMPONENTS_HPP 1

#include <cstddef>
#include <vector>
#include <utility>
#include <list>

#include <zi/utility/assert.hpp>
#include <zi/utility/for_each.hpp>

#include <zi/bits/function.hpp>
#include <zi/bits/bind.hpp>

namespace zi {
namespace graph {

namespace detail {


template< class ID >
class biconnected_components_impl
{
private:
    typedef ID                                  id_type   ;
    typedef std::vector< std::list< id_type > > graph_type;

    graph_type graph_;

public:
    std::vector< id_type > lowpoints_;
    std::vector< id_type > depths_   ;
    std::vector< id_type > ncomps_   ;

    biconnected_components_impl( const graph_type& graph )
        : graph_( graph ),
          lowpoints_( graph.size() ),
          depths_( graph.size() )
    {
    }

    id_type visit( id_type node, id_type parent, id_type depth )
    {
        if ( depths_[node] )
        {
            return depths_[node];
        }

        depths_[node]    = depth;
        lowpoints_[node] = depth;

        FOR_EACH( it, graph_[node] )
        {
            if ( *it != parent )
            {
                id_type res = visit( *it, node, depth+1 );
                lowpoints_[node] = std::min( lowpoints_[node], res );
            }
        }

        return lowpoints_[node];
    }

    void operator()()
    {
        for ( std::size_t i = 0; i < graph_.size(); ++i )
        {
            if ( depths_[i] == 0 )
            {
                visit( i, 0, 1 );
            }
        }

        /*
        std::vector< id_type > depth  ( graph_.size() );
        std::deque < id_type > dfs;

        for ( std::size_t i = 0; i < graph_.size(); ++i )
        {
            if ( depth[i] == 0 )
            {
                depth[i] = 1;
                dfs.push_back(i);

                while ( !dfs.empty() )
                {
                    id_type current = dfs.back();
                    //dfs.pop_back();

                    FOR_EACH( it, graph_[current] )
                    {
                        //if (
                    }
                }
            }
            }*/
    }
};

//std::vector< std::list< id_type > > children( new_index );


} // namespace detail

/*template< class T >
T munkres( const std::vector< std::vector< T > >& m )
{
    detail::munkres_impl< T > mkr( m );
    return mkr();
    }*/

} // namespace graph
} // namespace zi

#endif

