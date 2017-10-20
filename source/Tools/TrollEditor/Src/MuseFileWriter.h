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

	private:
		static wxString DoCreateMain( ProjectComponents::Project const & p_project, bool p_writeScripts );
		static wxString DoCreateSceneEditorMain( ProjectComponents::Project const & p_current, ProjectComponents::Project const & p_editor );
		static wxString DoCreateOverlayEditorMain( ProjectComponents::Project const & p_current, ProjectComponents::Project const & p_editor );
		static void DoAddSceneToMuse( ProjectComponents::Project const & p_project, bool p_createMain, bool p_withScripts, ProjectComponents::Scene const & p_scene, wxString const & p_mainFile );
		static void DoZipFolder( wxString const & p_folderPath, wxString const & p_zipFileName );
	};
}
