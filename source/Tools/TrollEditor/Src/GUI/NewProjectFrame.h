
#ifndef _NewProjectFrame_H
#define _NewProjectFrame_H

#include <wx/dialog.h>

#include "Module_GUI.h"
#include "Project/Module_Project.h"

namespace Troll
{
	namespace GUI
	{
		class NewProjectFrame
			: public wxDialog
		{
		private:
			bool m_shadows;
			AntiAliasing m_antiAliasing;
			BackgroundType m_backgroundType;
			wxString m_bgColour;
			wxString m_bgImage;
			wxString m_path;

			// GUI controls
			wxTextCtrl * m_projectName;
			wxButton * m_folderButton;
			wxTextCtrl * m_projectFolder;
			wxComboBox * m_bgRBox;
			wxButton * m_bgSelect;
			wxTextCtrl * m_mainSceneName;
			wxCheckBox * m_shCkb;
			wxComboBox * m_antiAlias;
			wxTextCtrl * m_width;
			wxTextCtrl * m_height;
			wxButton * m_okBtn;
			wxButton * m_cancelBtn;

		public:
			NewProjectFrame( wxWindow * p_parent, const wxString & title = wxT( "Nouveau Projet" ), const wxPoint & pos = wxDefaultPosition );
			~NewProjectFrame();

		private:
			void _onSelectBackgroundType( wxCommandEvent & p_event );
			void _onSelectBackgroundButton( wxCommandEvent & p_event );
			void _onMainSceneChange( wxCommandEvent & p_event );
			void _onShadowsChange( wxCommandEvent & p_event );
			void _onAntiAliasingChange( wxCommandEvent & p_event );
			void _onOK( wxCommandEvent & p_event );
			void _onCancel( wxCommandEvent & p_event );
			void _onBrowse( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

