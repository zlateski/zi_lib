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

#ifndef ZI_WATERSHED_DETAIL_MEMORY_MANAGER_HPP
#define ZI_WATERSHED_DETAIL_MEMORY_MANAGER_HPP 1

#include <zi/watershed/detail/mmap_file.hpp>
#include <zi/watershed/detail/exception.hpp>

#include <zi/bits/unordered_map.hpp>

//
// 1. one read/write thread
// 2. request real file or temp file
//

#include <string>
#include <iterator>
#include <cerrno>
#include <cstring>
#include <sys/mman.h>

namespace zi {
namespace watershed {

class memory_manager: public mmap_file
{
public:
    static const std::size_t page_size = 4096;

private:

    inline void* mmap_( std::size_t len, int prot, std::size_t offset )
    {
        void *r = mmap( NULL, len, prot, MAP_SHARED, fd_, offset );

        if ( r == MAP_FAILED || r == NULL )
        {
            ZI_THROWF( resource_error, "can't mmap file %s", filename_.c_str() );
        }

        return r;
    }

    inline void munmap_( void *m, std::size_t len )
    {
        if ( munmap( m, len ) < 0 )
        {
            ZI_THROWF( resource_error, "can't munmap file %s", filename_.c_str() );
        }
    }

public:

    inline memory_manager( const std::string& filename, int mode )
        : file( filename, mode )
    { }


    void read( void *buffer, std::size_t len, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_READ, offset );

        if ( padding )
        {
            void *vm = reinterpret_cast< void* >
                ( reinterpret_cast< char* >( r ) + padding );

            std::memcpy( buffer, vm, len );
        }
        else
        {
            std::memcpy( buffer, r, len );
        }

        munmap_( r, len );
    }

    void write( const void *buffer, std::size_t len, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_WRITE, offset );

        if ( padding )
        {
            void *vm = reinterpret_cast< void* >
                ( reinterpret_cast< char* >( r ) + padding );

            std::memcpy( vm, buffer, len );
        }
        else
        {
            std::memcpy( r, buffer, len );
        }

        munmap_( r, len );
    }

    template< class OutputIterator >
    void read_n( OutputIterator first, std::size_t count, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        typedef typename std::iterator_traits< OutputIterator >::value_type value_type;

        std::size_t len     = count * sizeof( value_type );
        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_READ, offset );

        value_type *vm = reinterpret_cast< value_type* >
            ( reinterpret_cast< char* >( r ) + padding );

        std::copy( vm, vm + count, first );

        munmap_( r, len );
    }

    template< class InputIterator >
    void write( InputIterator first, InputIterator last, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        typedef typename std::iterator_traits< InputIterator >::value_type value_type;

        std::size_t len = static_cast< std::size_t >
            ( last - first + 1 ) * sizeof( value_type );

        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_WRITE, offset );

        value_type *vm = reinterpret_cast< value_type* >
            ( reinterpret_cast< char* >( r ) + padding );

        std::copy( first, last, vm );

        munmap_( r, len );
    }

    template< class InputIterator >
    void write_n( InputIterator first, std::size_t count, std::size_t offset = 0 )
    {
        mutex::guard g( mutex_ );

        typedef typename std::iterator_traits< InputIterator >::value_type value_type;

        std::size_t len     = count * sizeof( value_type );
        std::size_t padding = 0;

        if ( offset )
        {
            padding  = offset % page_size;
            offset  -= padding;
            len     += padding;
        }

        void *r = mmap_( len, PROT_WRITE, offset );

        value_type *vm = reinterpret_cast< value_type* >
            ( reinterpret_cast< char* >( r ) + padding );

        for ( std::size_t i = 0; i < count; ++i )
        {
            vm[ i ] = *( first++ );
        }

        munmap_( r, len );
    }

    static void write( const std::string& filename,
                       const void *buffer,
                       std::size_t len,
                       std::size_t offset = 0 )
    {
        memory_manager f( filename, file::CREAT | file::RDWR | file::TRUNC );
        f.size( len );
        return f.write( buffer, len, offset );
    }

    template< class InputIterator >
    static void write_n( const std::string& filename,
                         InputIterator first,
                         std::size_t count,
                         std::size_t offset = 0 )
    {
        typedef typename std::iterator_traits< InputIterator >::value_type value_type;
        memory_manager f( filename, file::CREAT | file::RDWR | file::TRUNC );
        f.size( count * sizeof( value_type ) );
        return f.template write_n< InputIterator >( first, count, offset );
    }

    template< class InputIterator >
    static void write( const std::string& filename,
                       InputIterator first,
                       InputIterator last,
                       std::size_t offset = 0 )
    {
        typedef typename std::iterator_traits< InputIterator >::value_type value_type;

        std::size_t len =
            static_cast< std::size_t >( last - first + 1 ) * sizeof( value_type );

        memory_manager f( filename, file::CREAT | file::RDWR | file::TRUNC );
        f.size( len );
        return f.template write< InputIterator >( first, last, offset );
    }

    static void read( const std::string& filename,
                      void *buffer,
                      std::size_t len,
                      std::size_t offset = 0 )
    {
        memory_manager f( filename, file::RDONLY );
        return f.read( buffer, len, offset );
    }

    template< class OutputIterator >
    static void read_n( const std::string& filename,
                        OutputIterator first,
                        std::size_t count,
                        std::size_t offset = 0 )
    {
        memory_manager f( filename, file::RDONLY );
        return f.template read_n< OutputIterator >( first, count, offset );
    }

    static void erase( const std::string& filename )
    {
        ::remove( filename.c_str() );
    }

};

} // namespace watershed
} // namespace zi

#endif
