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

#ifndef ZI_WATERSHED_CHUNK_PROCESSOR_HPP
#define ZI_WATERSHED_CHUNK_PROCESSOR_HPP 1

#include <zi/watershed/quickie_watershed.hpp>
#include <zi/bits/shared_ptr.hpp>
#include <zi/utility/string_printf.hpp>

namespace zi {
namespace watershed {

template< class T, class R >
class chunk_processor: non_copyable
{
private:

    const std::string     file_prefix_ ;
    chunk< R >            chunk_   ;
    const std::size_t     xpos_    ;
    const std::size_t     ypos_    ;
    const std::size_t     zpos_    ;
    affinity_matrix< T >  affinity_;
    mem::mmap_file        storage_ ;
    const int             position_;

public:

    chunk_processor( const std::string& file_prefix,
                     const std::size_t  x,
                     const std::size_t  y,
                     const std::size_t  z,
                     const std::size_t  xpos,
                     const std::size_t  ypos,
                     const std::size_t  zpos,
                     const int          position )

        : file_prefix_( file_prefix ),
          chunk_( x, y, z, false ),
          xpos_( xpos ),
          ypos_( ypos ),
          zpos_( zpos ),
          storage_( string_printf( "%s.%.3d.%.3d.%.3d.chunk_data", file_prefix_.c_str(),
                                   xpos, ypos, zpos ),
                    mem::file::CREAT | mem::file::RDWR  | mem::file::TRUNC ),
          affinity_( x, y, z,
                     string_printf( "%s.%.3d.%.3d.%.3d.affinity", file_prefix_.c_str(),
                                    xpos, ypos, zpos ) ),
          position_( position )
    {
        storage_.size( x * y * z * sizeof( R ) );
    }

};

} // namespace watershed
} // namespace zi

#endif

