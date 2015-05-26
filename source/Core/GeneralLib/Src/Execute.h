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

		bool d_dll Execute(	const std::string & p_name, const std::string & p_path,
							const std::string & p_params, ExecuteStyle p_style = ES_SYSTEM );

#ifndef __GNUG__

		bool d_dll Execute(	const std::wstring & p_name, const std::wstring & p_path,
							const std::wstring & p_params, ExecuteStyle p_style = ES_SYSTEM );

#endif

	}
}

#endif
