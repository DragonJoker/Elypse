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
	class MuseFileWriter
	{
	public:
		static void Musinate( ProjectComponents::Project const & p_project, bool p_createMain = true, bool p_withScripts = true );
		static void MusinateSceneEditor( ProjectComponents::Project const & p_current, ProjectComponents::Project const & p_editor );
		static void MusinateOverlayEditor( ProjectComponents::Project const & p_current, ProjectComponents::Project const & p_editor );
	};
}
