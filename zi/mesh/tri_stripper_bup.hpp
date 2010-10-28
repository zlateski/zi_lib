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

#ifndef ZI_MESH_TRI_STRIPPER_HPP
#define ZI_MESH_TRI_STRIPPER_HPP 1

#include <zi/mesh/tri_mesh.hpp>
#include <zi/heap/binary_heap.hpp>

namespace zi {
namespace mesh {

class tri_stripper_impl
{
private:
    static const uint32_t none = 0x80000000;

    inline uint32_t next_edge( uint32_t e ) const
    {
        static const uint32_t map_helper[ 4 ] = { 0, 2, 3, 1 };
        return ( e & 0x7ffffffc ) + map_helper[ e & 3 ];
    }

    inline uint32_t pair_edge( uint32_t e ) const
    {
        return data_[ e & 0x7fffffff ];
    }

    inline uint32_t valid_edge( uint32_t e ) const
    {
        return ( data_[ e ] ^ 0x80000000 ) >> 31;
    }

    inline uint32_t valid_edge( uint32_t f, uint32_t n ) const
    {
        return ( data_[ f | n ] ^ 0x80000000 ) >> 31;
    }

    inline uint32_t edge_face( uint32_t e ) const
    {
        return ( e >> 2 );
    }

    inline uint32_t adj_face( uint32_t f, uint32_t x ) const
    {
        return data_[ f | x ] >> 2;
    }

    inline void remove_edge( uint32_t f, uint32_t no )
    {
        data_[ f | no ] |= 0x80000000;
    }

    inline void remove_edge( uint32_t e )
    {
        data_[ e ] |= 0x80000000;
    }

    struct face_data
    {
        uint32_t v1, v2, v3;
    };

    std::vector< uint32_t >   data_      ;
    std::vector< face_data >  face_data_ ;
    std::size_t               size_      ;

    struct heap_entry
    {
        const uint32_t face_ ;
        const uint32_t value_;

        explicit heap_entry( const uint32_t f, const uint32_t v )
            : face_( f ), value_( v )
        {
        }

    };

    typedef binary_heap<
        heap_entry,
        zi::heap::hashed_index<
            zi::heap::member_variable<
                heap_entry,
                const uint32_t,
                &heap_entry::face_
            >
        >,

        zi::heap::value<
            zi::heap::member_variable<
                heap_entry,
                const uint32_t,
                &heap_entry::value_
            >,
            std::less< uint32_t >
        >
    > heap_type;

    heap_type heap_;


    void add_to_heap( uint32_t f )
    {
        if ( f & 0x80000000 )
        {
            return;
        }

        uint32_t v = 0;
        v += valid_edge( f, 1 );
        v += valid_edge( f, 2 );
        v += valid_edge( f, 3 );

        v <<= 1;
        if ( data_[ f ] != f )
        {
            ++v;
        }

        if ( v > 0 )
        {
            heap_.insert( heap_entry( f, v ) );
        }
    }

    void update_heap( uint32_t f )
    {
        heap_.erase_key( f );
        add_to_heap( f );
    }

    void apply_edge( uint32_t e1 )
    {

        uint32_t f1 = e1 & 0x7ffffffc;
        uint32_t e2 = data_[ e1 & 0x7fffffff ];
        uint32_t f2 = e2 & 0x7ffffffc;

        std::cout << "APPLYING EDGE: " << ( f1 >> 2  ) << "-" << ( f2 >> 2 ) << "\n";

        if ( data_[ f1 ] != f1 )
        {
            data_[ f1 ] |= 0x80000000;
            uint32_t ne = next_edge( e1 );
            if ( ( data_[ ne ] & 0x80000000 ) != 0x80000000 )
            {
                data_[ data_[ ne ] ] = 0x80000000;
                data_[ ne ] |= 0x80000000;
                update_heap( data_[ ne ] & 0x7ffffffc );
            }
            ne = next_edge( ne );
            if ( ( data_[ ne ] & 0x80000000 ) != 0x80000000 )
            {
                data_[ data_[ ne ] ] = 0x80000000;
                data_[ ne ] |= 0x80000000;
                update_heap( data_[ ne ] & 0x7ffffffc );
            }
        }

        if ( data_[ f2 ] != f2 )
        {

            data_[ f2 ] |= 0x80000000;
            uint32_t ne = next_edge( e1 );
            if ( ( data_[ ne ] & 0x80000000 ) != 0x80000000 )
            {
                data_[ data_[ ne ] ] = 0x80000000;
                data_[ ne ] |= 0x80000000;
                update_heap( data_[ ne ] & 0x7ffffffc );
            }
            ne = next_edge( ne );
            if ( ( data_[ ne ] & 0x80000000 ) != 0x80000000 )
            {
                data_[ data_[ ne ] ] = 0x80000000;
                data_[ ne ] |= 0x80000000;
                update_heap( data_[ ne ] & 0x7ffffffc );
            }
        }

        data_[ e1 ] |= 0x80000000;
        data_[ e2 ] |= 0x80000000;

        uint32_t tmp1 = data_[ f1 ] & 0x7ffffffc;
        uint32_t tmp2 = data_[ f2 ] & 0x7ffffffc;
        data_[ tmp1 ] &= 0x80000000;
        data_[ tmp1 ] |= tmp2;

        data_[ tmp2 ] &= 0x80000000;
        data_[ tmp2 ] |= tmp1;

        update_heap( f1 );
        update_heap( f2 );
    }

    void iteration()
    {
        heap_entry t = heap_.top();
        heap_.pop();

        std::cout << "POPPED: " << ( t.face_ << 2 ) << " : " << t.value_  << "\n";

        if ( t.value_ == 0 )
        {
            std::cout << "TRIANGLE: " << t.face_ << "\n";
            data_[ t.face_ ] |= 0x80000000;
        }
        else
        {
            for ( uint32_t i = 1; i < 4; ++i )
            {
                if ( ( data_[ t.face_ | i ] & 0x80000000 ) != 0x80000000 )
                {
                    if ( ( data_[ t.face_ ] & 0x7ffffffc ) !=
                         ( data_[ t.face_ | i ] & 0x7ffffffc ) )
                    {
                        apply_edge( t.face_ | i );
                        return;
                    }
                    else
                    {
                        uint32_t se = t.face_ | i;
                        data_[ se ] |= 0x80000000;
                        data_[ data_[ se ] & 0x7ffffffc ] |= 0x80000000;
                    }
                }
            }
        }
    }

public:
    explicit tri_stripper_impl( const tri_mesh& mesh )
        : data_( mesh.face_count() * 4 ),
          face_data_( mesh.face_count() ),
          size_( mesh.face_count() ),
          heap_()
    {
        std::fill_n( data_.begin(), data_.size(), 0x80000000 );

        unordered_map< uint64_t, uint32_t > edges;
        unordered_map< uint64_t, uint32_t >::const_iterator eit;

        uint32_t idx = 0;
        FOR_EACH( it, mesh.faces )
        {
            const uint32_t& v1 = it->v0();
            const uint32_t& v2 = it->v1();
            const uint32_t& v3 = it->v2();

            const uint32_t ioff = idx << 2;

            face_data_[ idx ].v1 = v1;
            face_data_[ idx ].v2 = v2;
            face_data_[ idx ].v3 = v3;

            edges.insert( std::make_pair( detail::make_edge( v1, v2 ), ioff + 3 ));
            edges.insert( std::make_pair( detail::make_edge( v2, v3 ), ioff + 1 ));
            edges.insert( std::make_pair( detail::make_edge( v3, v1 ), ioff + 2 ));

            eit = edges.find( detail::make_edge( v2, v1 ) );
            if ( eit != edges.end() )
            {
                data_[ ioff + 3 ] = eit->second;
                data_[ eit->second ] = ioff + 3;
            }

            eit = edges.find( detail::make_edge( v3, v2 ) );
            if ( eit != edges.end() )
            {
                data_[ ioff + 1 ] = eit->second;
                data_[ eit->second ] = ioff + 1;
            }

            eit = edges.find( detail::make_edge( v1, v3 ) );
            if ( eit != edges.end() )
            {
                data_[ ioff + 2 ] = eit->second;
                data_[ eit->second ] = ioff + 2;
            }

            data_[ ioff ] = ioff;
            ++idx;
        }

    }

    void execute()
    {

        for ( uint32_t i = 0; i < size_; ++i )
        {
            add_to_heap( i << 2 );
        }

        std::cout << "HEAP SIZE: " << heap_.size() << "\n";

        while ( heap_.size() > 0 )
        {
            iteration();
        }

    }

};

} // namespace mesh
} // namespace zi

#endif
