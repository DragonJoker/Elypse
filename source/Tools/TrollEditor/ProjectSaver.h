/*
See LICENSE file in root folder
*/
#pragma once

#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>

#include <DataWriter.h>

#include "Project/Module_Project.h"

namespace Troll
{
	class ProjectSaver
	{
	public:
		static void SaveProject( ProjectComponents::Project & p_project );
		static void SaveScene( ProjectComponents::Project & p_project, ProjectComponents::Scene const & p_scene );
		static wxString SaveFileAs( ProjectComponents::Project & p_project, ProjectComponents::File const & p_file );
		static void SaveFile( ProjectComponents::Project & p_project, ProjectComponents::File const & p_file, wxString const & p_fileName );
	};
}
