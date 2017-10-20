/*
See LICENSE file in root folder
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

