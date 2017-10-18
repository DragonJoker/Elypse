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
#include "TrollEditorPch.h"

#include "GUI/NewSceneFrame.h"

#include "GUI/ProjectFrame.h"
#include "GUI/FilesTree.h"

#include "Project/Scene.h"
#include "Project/Project.h"

namespace Troll
{
	namespace GUI
	{
		const wxString DEFAULT_SCENE_NAME = _( "Scene name" );

		BEGIN_EVENT_TABLE( NewSceneFrame, wxDialog )
			EVT_BUTTON( ID_okBtn, NewSceneFrame::_onOK )
			EVT_BUTTON( ID_clBtn, NewSceneFrame::_onCancel )
		END_EVENT_TABLE()

		NewSceneFrame::NewSceneFrame( ProjectFrame * p_parent, wxString const & title, wxPoint const & pos )
			: wxDialog( p_parent, -1, title, pos, wxSize( 200, 90 ) )
			, m_projectFrame{ p_parent }
		{
			SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			SetForegroundColour( PANEL_FOREGROUND_COLOUR );

			wxSize l_size = GetClientSize();
			wxPoint l_position( l_size.x / 2, 10 );
			m_sceneName = BuildTextCtrl( this, l_position, wxID_ANY, _( "Scene" ), DEFAULT_SCENE_NAME );
			m_sceneName->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			m_sceneName->SetForegroundColour( PANEL_FOREGROUND_COLOUR );

			l_position.x = 20;
			m_okBtn = new wxButton( this, ID_okBtn, _( "OK" ), l_position += wxPoint( 0, 25 ), wxSize( 70, 20 ) );
			m_cancelBtn = new wxButton( this, ID_clBtn, _( "Cancel" ), l_position += wxPoint( 90, 0 ), wxSize( 70, 20 ) );
		}

		NewSceneFrame::~NewSceneFrame()
		{
		}

		wxString NewSceneFrame::GetSceneName()
		{
			return wxEmptyString;
		}

		void NewSceneFrame::_onOK( wxCommandEvent & p_event )
		{
			wxString l_sceneName = m_sceneName->GetValue();

			if ( l_sceneName.empty() || l_sceneName == DEFAULT_SCENE_NAME )
			{
				wxMessageBox( _( "Enter a scene name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, this );
				return;
			}

			auto && l_project = m_projectFrame->GetProject();

			try
			{
				if ( l_project->AddScene( std::make_shared< ProjectComponents::Scene >( *l_project, l_sceneName ) ) )
				{
					m_projectFrame->GetFilesList()->AddSceneToProject( l_project->GetScene( l_sceneName ) );
					wxString l_path = m_projectFrame->GetProject()->GetPath();
					l_path = l_path + l_sceneName;
					wxFileName::Mkdir( l_path, 0777, wxPATH_MKDIR_FULL );
				}
			}
			catch ( bool )
			{
				LogDebug( _( "NewSceneFrame::OnNewScene - Adding problem" ) );
				return;
			}

			Destroy();
		}

		void NewSceneFrame::_onCancel( wxCommandEvent & p_event )
		{
			Destroy();
		}
	}
}
