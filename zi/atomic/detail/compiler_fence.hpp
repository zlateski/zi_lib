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

#ifndef ZI_ATOMIC_DETAIL_COMPILER_FENCE_HPP
#define ZI_ATOMIC_DETAIL_COMPILER_FENCE_HPP 1

#include <zi/atomic/config.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#  pragma once
#endif
#
#if defined( _MSC_VER ) && _MSC_VER >= 1310
#

extern "C" void _ReadWriteBarrier();

#
#pragma intrinsic( _ReadWriteBarrier )
#
#define ZI_CONCURRENCY_COMPILER_FENCE _ReadWriteBarrier();
#
#elif defined(__GNUC__)
#
#define ZI_CONCURRENCY_COMPILER_FENCE __asm__ __volatile__( "" : : : "memory" );
#
#else
#
#define ZI_CONCURRENCY_COMPILER_FENCE
#
#endif

#endif
