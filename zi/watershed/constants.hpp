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

#ifndef ZI_WATERSHED_CONSTANTS_HPP
#define ZI_WATERSHED_CONSTANTS_HPP 1

namespace zi {
namespace watershed {

enum chunk_position
{
    MIDDLE = 0x00,
    XFRONT = 0x01,
    XBACK  = 0x02,
    YFRONT = 0x04,
    YBACK  = 0x08,
    ZFRONT = 0x10,
    ZBACK  = 0x20,
};

} // namespace watershed
} // namespace zi

#endif
