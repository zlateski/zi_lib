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

#ifndef ZI_WATERSHED_DETAIL_FILE_PATH_HPP
#define ZI_WATERSHED_DETAIL_FILE_PATH_HPP 1

#include <string>
#include <sstream>

namespace zi {
namespace watershed {

class file_path
{
private:
    std::ostringstream oss_;

public:
    explicit file_path( const std::string& prefix = "" )
        : oss_()
    {
        oss_ << prefix;
    }

    operator std::string() const
    {
        return oss_.str();
    }

    template< class T >
    file_path& operator%=( const T& v )
    {
        oss_ << '.' << v;
        return *this;
    }

    template< class T >
    friend file_path operator,( const file_path& fp, const T& v )
    {
        file_path r( fp );
        return r %= v;
    }

    template< class T >
    friend file_path operator%( const file_path& fp, const T& v )
    {
        file_path r( fp );
        return r %= v;
    }

};


} // namespace watershed
} // namespace zi

#endif
