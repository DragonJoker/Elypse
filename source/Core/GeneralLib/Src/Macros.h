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
#ifndef ___MACROS_H___
#define ___MACROS_H___

#include "Config.h"

#ifndef ___MUSE_DEBUG___
#	define d_debug_only
#	define d_debug_assert( X)
#else
#	define d_debug_only if( 0)
#	define d_debug_assert( X) Assert( X)
#endif

#ifndef __FUNCTION__
#	define __FUNCTION__ ""
#endif

#ifndef __COUNTER__
#	define __COUNTER__ ""
#endif

#ifndef ___GENLIB_DLL___
#	define d_dll
#else
#	define d_dll d_dllexport
#endif

#define d_coucou std::cout << "Coucou ~ " << __FILE__ << "::" << __FUNCTION__ << " @ line " << __LINE__ << std::endl

#endif
