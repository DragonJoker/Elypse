/*
See LICENSE file in root folder
*/
#ifndef ___Troll_ProjectFileWriter___
#define ___Troll_ProjectFileWriter___

#include "Module_Project.h"

#include "GUI/FileParser.h"

#include <map>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace ProjectFileWriter
		{
			bool WriteProject( Project const & p_project, wxTextOutputStream & p_stream );
			bool WriteScene( Scene const & p_scene, wxTextOutputStream & p_stream );
		}
	}
}

#endif
