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
