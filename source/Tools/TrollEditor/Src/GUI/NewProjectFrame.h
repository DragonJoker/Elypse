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

#ifndef _NewProjectFrame_H
#define _NewProjectFrame_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Module_Project.h"

#include "Properties/Project/ProjectProperties.h"

#include <wx/dialog.h>

namespace Troll
{
	namespace GUI
	{
		class NewProjectProperties
			: public Properties::ProjectProperties
		{
		public:
			NewProjectProperties( ProjectComponents::ProjectSPtr project = std::make_shared< ProjectComponents::Project >() );
			~NewProjectProperties();
			/**
			 *\copydoc		Troll::GUI::Properties::ObjectProperty::OnValidate
			 */
			virtual bool OnValidate( wxWindow * p_parent );

		private:
			/**
			 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
			 */
			virtual void DoCreateProperties( wxPGEditor * p_editor, wxPropertyGrid * p_grid );
			/**
			 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoPropertyChange
			 */
			virtual void DoPropertyChange( wxPropertyGridEvent & p_event );

		private:
			bool DoSelectFolder( wxPGProperty * p_property );
			bool DoSelectBackground( wxPGProperty * p_property );
			bool DoChangeStartupScript( wxPGProperty * p_property );

		private:
			ProjectComponents::ProjectSPtr m_project;
			wxString m_bgColour{ wxT( "black" ) };
			wxString m_bgImage;
			wxString m_path;
			wxString m_startupScript;
			wxString m_name;
			wxString m_sceneName;
			wxSize m_resolution{ 800, 600 };
			bool m_shadows{ false };
			bool m_showDebug{ false };
			bool m_showFps{ false };
			ProjectComponents::AntiAliasing m_antiAliasing{ ProjectComponents::aa0 };
			ProjectComponents::BackgroundType m_backgroundType{ ProjectComponents::bgColour };
		};
	}
}

#endif

