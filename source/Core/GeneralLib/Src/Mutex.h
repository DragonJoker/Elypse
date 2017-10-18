/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERAL_LIB_MUTEX_H___
#define ___GENERAL_LIB_MUTEX_H___

#include "MultiThreadConfig.h"

#include <thread>
#include <mutex>

namespace General
{
	namespace MultiThreading
	{
		template< typename T_mutex >
		std::unique_lock< T_mutex > make_unique_lock( T_mutex & p_mutex )
		{
			return std::unique_lock< T_mutex > { p_mutex };
		}
	}
}

#endif
