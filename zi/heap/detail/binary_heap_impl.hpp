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

#ifndef ZI_HEAP_DETAIL_BINARY_HEAP_IMPL_HPP
#define ZI_HEAP_DETAIL_BINARY_HEAP_IMPL_HPP 1

#include <zi/bits/ref.hpp>
#include <zi/utility/exception.hpp>
#include <zi/utility/assert.hpp>
#include <zi/utility/for_each.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <functional>
#include <utility>
#include <algorithm>

#include <iostream>

namespace zi {
namespace heap {
namespace detail {

template< class Type,
          class KeyType,
          class ValueType,
          class KeyExtractor,
          class ValueExtractor,
          class ValueCompare,
          class Container,
          class Allocator
          >
class binary_heap_impl
{
private:

    std::size_t          size_           ;
    std::size_t          reserved_       ;
    KeyExtractor         key_extractor_  ;
    ValueExtractor       value_extractor_;
    ValueCompare         compare_        ;

    Allocator            alloc_   ;
    Container            keymap_  ;
    uint32_t*            heap_    ;
    uint32_t*            map_     ;
    Type*                store_   ;

private:

    void try_grow()
    {
        if ( size_ == reserved_ )
        {
            const std::size_t s = reserved_ << 1;

            Type* new_store = alloc_.allocate( s );

            for ( uint32_t i = 0; i < size_; ++i )
            {
                alloc_.construct( new_store + heap_[ i ], store_[ heap_[ i ] ] );
                alloc_.destroy( store_ + heap_[ i ] );
            }

            alloc_.deallocate( store_, reserved_ );

            store_ = new_store;

            uint32_t* nheap = new uint32_t[ s ];
            std::copy( heap_, heap_ + reserved_, nheap );
            delete [] heap_;
            heap_ = nheap;

            uint32_t* nmap = new uint32_t[ s ];
            std::copy( map_, map_ + reserved_, nmap );
            delete [] map_;
            map_ = nmap;

            for ( uint32_t i = static_cast< uint32_t >( reserved_ ); i < s; ++i )
            {
                heap_[ i ] = map_[ i ] = i;
            }

            reserved_ = s;
        }

        ZI_ASSERT( size_ < reserved_ );
    }

    void try_shrink()
    {
        if ( ( ( size_ << 2 ) < reserved_ ) && ( reserved_ > 4096 ) )
        {
            ZI_ASSERT( size_ == keymap_.size() );

            std::size_t s = reserved_ >> 1;

            Type* old = store_;

            store_ = alloc_.allocate( s );

            uint32_t si = 0;

            for ( uint32_t i = 0; i < size_; ++i )
            {
                if ( heap_[ i ] >= s )
                {
                    while ( map_[ si ] < s )
                    {
                        ++si;
                    }

                    keymap_[ key_extractor_( old[ heap_[ i ] ] ) ] = si;
                    alloc_.construct( store_ + si, old[ heap_[ i ] ] );
                    alloc_.destroy( old + heap_[ i ] );

                    heap_[ i ] = si;
                    map_[ si ] = i;
                }
                else // heap_[ i ] < s
                {
                    alloc_.construct( store_ + heap_[ i ], old[ heap_[ i ] ] );
                    alloc_.destroy( old + heap_[ i ] );
                }

            }

            for ( uint32_t i = size_; i < s; ++i )
            {
                if ( heap_[ i ] >= s )
                {
                    while ( map_[ si ] < s )
                    {
                        ++si;
                    }
                    heap_[ i ] = si;
                    map_[ si ] = i;
                }
            }

            alloc_.deallocate( old, reserved_ );

            uint32_t* nheap = new uint32_t[ s ];
            std::copy( heap_, heap_ + s, nheap );
            delete [] heap_;
            heap_ = nheap;

            uint32_t* nmap = new uint32_t[ s ];
            std::copy( map_, map_ + s, nmap );
            delete [] map_;
            map_ = nmap;

            reserved_ = s;
        }

        ZI_ASSERT( size_ < reserved_ );
    }

public:
    binary_heap_impl( const ValueCompare& compare   = ValueCompare(),
                      const Allocator   & allocator = Allocator(),
                      std::size_t init_reserved = 16 )
        : size_( 0 ),
          reserved_( init_reserved ),
          key_extractor_(),
          value_extractor_(),
          compare_( compare ),
          alloc_( allocator ),
          keymap_(),
          heap_( 0 ),
          map_( 0 ),
          store_( 0 )
    {
        if ( reserved_ < 16 )
        {
            reserved_ = 16;
        }
        heap_ = new uint32_t[ reserved_ ];
        map_  = new uint32_t[ reserved_ ];
        store_ = alloc_.allocate( reserved_ );

        for ( uint32_t i = 0; i < reserved_; ++i )
        {
            heap_[ i ] = map_[ i ] = i;
        }
    }

    ~binary_heap_impl()
    {
        for ( uint32_t i = 0; i < size_; ++i )
        {
            alloc_.destroy( store_ + heap_[ i ] );
        }

        alloc_.deallocate( store_, reserved_ );

        delete [] heap_;
        delete [] map_ ;

    }

    inline std::size_t size() const
    {
        return size_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    std::size_t count( const Type& v ) const
    {
        return keymap_.count( key_extractor_( const_cast< Type& >( v ) ) );
    }

    std::size_t key_count( const KeyType& v ) const
    {
        return keymap_.count( v );
    }

    const Type& top() const
    {
        if ( size_ == 0 )
        {
            throw ::zi::exception( "called pop on an empty heap" );
        }
        return store_[ heap_[ 0 ] ];
    }

    Type& top()
    {
        if ( size_ == 0 )
        {
            throw ::zi::exception( "called pop on an empty heap" );
        }
        return store_[ heap_[ 0 ] ];
    }

    void insert( const Type& v )
    {
        if ( !count( v ) )
        {
            insert_( v );
        }
    }

    std::size_t erase( const Type& v )
    {
        if ( count( v ) )
        {
            erase_( v );
            return 1;
        }
        return 0;
    }

    std::size_t erase_key( const KeyType& v )
    {
        if ( key_count( v ) )
        {
            erase_key_( v );
            return 1;
        }
        return 0;
    }

    void pop()
    {
        if ( size_ > 0 )
        {
            erase_( store_[ heap_[ 0 ] ] );
        }
    }

    void clear()
    {
        clear_();
    }


private:

    void swap_elements( uint32_t x, uint32_t y )
    {
        std::swap( heap_[ x ], heap_[ y ] );
        map_[ heap_[ x ] ] = x;
        map_[ heap_[ y ] ] = y;
    }

    uint32_t heap_up( uint32_t index )
    {
        uint32_t parent = ( index - 1 ) / 2;
        while ( index > 0 && compare_( value_extractor_( store_[ heap_[ index  ] ] ),
                                       value_extractor_( store_[ heap_[ parent ] ] )))
        {
            swap_elements( index, parent );
            index = parent;
            parent = ( index - 1 ) / 2;
        }
        return index;
    }

    void heap_down( uint32_t index )
    {
        ZI_ASSERT( index==0 ||
                   compare_(value_extractor_(store_[heap_[(index-1)/2]]),
                            value_extractor_(store_[heap_[index]])));

        uint32_t child = index * 2 + 1;
        while ( child < size_ )
        {
            if ( child + 1 < size_ &&
                 compare_( value_extractor_( store_[ heap_[ child + 1 ] ] ),
                           value_extractor_( store_[ heap_[ child ] ] )))
            {
                ++child;
            }

            if ( compare_( value_extractor_( store_[ heap_[ index ] ] ),
                           value_extractor_( store_[ heap_[ child ] ] )))
            {
                break;
            }

            swap_elements( index, child );
            index = child;
            child = ( index << 1 ) + 1;
        }
    }

    void insert_( const Type& v )
    {
        ZI_ASSERT( heap_[ size_ ] < reserved_ );

        alloc_.construct( store_ + heap_[ size_ ], v );

        keymap_.insert( std::make_pair( key_extractor_( const_cast< Type& >( v ) ),
                                        heap_[ size_ ] ));

        ZI_ASSERT( map_[ heap_[ size_ ] ] == size_ );

        heap_up( static_cast< uint32_t >( size_ ) );

        ++size_;
        try_grow();
    }

    void clear_()
    {
        for ( uint32_t i = 0; i < size_; ++i )
        {
            alloc_.destroy( store_ + heap_[ i ] );
        }

        if ( reserved_ > 16 )
        {

            alloc_.deallocate( store_, reserved_ );

            delete [] heap_;
            delete [] map_ ;

            heap_ = new uint32_t[ 16 ];
            map_  = new uint32_t[ 16 ];

            reserved_ = 16;
            store_ = alloc_.allocate( reserved_ );

            for ( uint32_t i = 0; i < reserved_; ++i )
            {
                heap_[ i ] = map_[ i ] = i;
            }
        }

        size_ = 0;
        keymap_.clear();
    }

    void erase_( const Type& v )
    {
        erase_at_( keymap_[ key_extractor_( const_cast< Type& > ( v ) ) ] );
    }

    void erase_key_( const KeyType& k )
    {
        erase_at_( keymap_[ k ] );
    }

    void erase_at_( const uint32_t pos )
    {
        ZI_VERIFY( keymap_.erase( key_extractor_( store_[ pos ] )));

        alloc_.destroy( store_ + pos );
        --size_;

        if ( map_[ pos ] < size_ )
        {
            uint32_t hp = map_[ pos ];
            swap_elements( map_[ pos ], size_ );
            hp = heap_up(hp);
            heap_down( hp );
        }
        try_shrink();
    }

};

} // namespace detail
} // namespace heap
} // namespace zi


#endif
