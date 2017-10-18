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
#ifndef ___EXECUTE_H___
#define ___EXECUTE_H___

#include <string>
#include "Macros.h"

namespace General
{
	namespace Utils
	{
		enum ExecuteStyle
		{
			ES_SYSTEM,
			ES_SHELL,
			ES_PROCESS
		};

		bool d_dll Execute( std::string const & p_name, std::string const & p_path, std::string const & p_params, ExecuteStyle p_style = ES_SYSTEM );

#ifndef __GNUG__

		bool d_dll Execute( std::wstring const & p_name, std::wstring const & p_path, std::wstring const & p_params, ExecuteStyle p_style = ES_SYSTEM );

#endif

	}
}

#endif
