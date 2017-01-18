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

#ifndef ZI_WATERSHED_THRESHOLDS_HPP
#define ZI_WATERSHED_THRESHOLDS_HPP 1

namespace zi {
namespace watershed {

template< class T, class R = std::size_t >
class thresholds
{

private:

    const T hi_ ;
    const T low_;
    const T mst_;
    const R size_;

public:
    explicit thresholds( const T& hi, const T& low, const T& mst, const R& size )
        : hi_ ( hi ),
          low_( low ),
          mst_( mst ),
          size_( size )
    {
    }

    template< class T1, class T2, class T3, class R1 >
    explicit thresholds( const T1& hi, const T2& low, const T3& mst, const R1& size )
        : hi_ ( static_cast< T >( hi ) ),
          low_( static_cast< T >( low ) ),
          mst_( static_cast< T >( mst ) ),
          size_( static_cast< R >( size ) )
    {
    }

    explicit thresholds( const thresholds< T, R > &rcp )
        : hi_ ( rcp.hi() ),
          low_( rcp.low() ),
          mst_( rcp.mst() ),
          size_( rcp.size() )
    {
    }

    template< class T1, class R1 >
    explicit thresholds( const thresholds< T1, R1 > &rcp )
        : hi_ ( static_cast< T >( rcp.hi() ) ),
          low_( static_cast< T >( rcp.low() ) ),
          mst_( static_cast< T >( rcp.mst() ) ),
          size_( static_cast< R >( rcp.size() ) )
    {
    }

    inline const T& hi() const
    {
        return hi_;
    }

    inline const T& low() const
    {
        return low_;
    }

    inline const T& mst() const
    {
        return mst_;
    }

    inline const R& size() const
    {
        return size_;
    }

};

} // namespace watershed
} // namespace zi

#endif
