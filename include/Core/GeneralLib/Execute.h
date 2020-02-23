/*
See LICENSE file in root folder
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
