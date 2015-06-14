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
		typedef std::recursive_mutex RecursiveMutex;
		typedef std::mutex Mutex;
	}
}

#define GENLIB_AUTO_SCOPED_LOCK() std::unique_lock< std::mutex > scoped_lock_m_mutex( m_mutex )
#define GENLIB_SCOPED_LOCK( p_mutex ) std::unique_lock< std::mutex > scoped_lock_ ## p_mutex( p_mutex )
#define GENLIB_SCOPED_RECURSIVE_LOCK( p_mutex ) std::unique_lock< std::recursive_mutex > scoped_lock_ ## p_mutex( p_mutex )

#define GENLIB_LOCK_MUTEX( p_mutex ) ( p_mutex ).lock()
#define GENLIB_UNLOCK_MUTEX( p_mutex ) ( p_mutex ).unlock()
#define GENLIB_RECURSIVE_LOCK_MUTEX( p_mutex ) ( p_mutex ).lock()
#define GENLIB_RECURSIVE_UNLOCK_MUTEX( p_mutex ) ( p_mutex ).unlock()

#endif
