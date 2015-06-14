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

#include "GUI/NewOverlayFrame.h"

#include "GUI/ElypseCtrl.h"
#include "GUI/ElypseFrame.h"
#include "GUI/ElypsePanel.h"
#include "GUI/MainFrame.h"
#include "GUI/ObjectsTree.h"

#include "Project/2D/Overlay.h"
#include "Project/Project.h"
#include "Project/Scene.h"

using namespace Ogre;

BEGIN_TROLL_GUI_NAMESPACE
{
	enum NowOverlayFrameIDs
	{
		noIdTemplate,
		noIdOK,
		noIdCancel
	};

	BEGIN_EVENT_TABLE( NewOverlayFrame, wxFrame )
		EVT_COMBOBOX( noIdTemplate, NewOverlayFrame::OnSelectTemplate )
		EVT_BUTTON( noIdOK, NewOverlayFrame::OnOK )
		EVT_BUTTON( noIdCancel, NewOverlayFrame::OnCancel )
	END_EVENT_TABLE()

	NewOverlayFrame::NewOverlayFrame( wxWindow * p_parent, const wxPoint & p_position, EMOverlay * p_parentOverlay, const wxString & p_name )
		: wxFrame( p_parent, wxID_ANY, _( "New Overlay" ), p_position, wxSize( 250, 422 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT )
		, m_parentOverlay( p_parentOverlay )
		, m_parentGroup( NULL )
		, m_gui( ScriptEngine::GetContext()->gui )
		, m_overlay( NULL )
		, m_mustBeClosed( false )
		, m_overlayName( p_name )
	{
		if ( m_overlayName.empty() )
		{
			static int sl_number = 0;
			m_overlayName = _( "NewOverlay" );
			m_overlayName << sl_number++;
		}

		if ( m_parentOverlay != NULL )
		{
			m_parentGroup = m_parentOverlay->GetOwner();
			wxString l_strName( make_wxString( m_parentOverlay->GetName() ) );
			TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * l_parentTrollOverlay = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( l_strName );

			if ( l_parentTrollOverlay->GetTextInfos() != NULL )
			{
				wxMessageBox( _( "Can't create an aoverlay in a text" ), TROLL_EDITOR_ERROR, wxID_OK );
				m_mustBeClosed = true;
			}
		}
		else
		{
			m_parentGroup = ScriptEngine::GetContext()->gui->CreateOverlayGroup( make_string( m_overlayName ) );
		}

		wxSize l_size = GetClientSize();
		wxArrayString l_choices;
		_getOverlayTemplates( l_choices );
		wxComboBox * l_box = BuildComboBox( this, wxPoint( 100, 10 ), noIdTemplate, _( "Template" ), l_choices, ( l_choices.size() > 0 ? l_choices[0] : wxT( "" ) ) );
		wxButton * l_button = new wxButton( this, noIdOK, _( "OK" ), wxPoint( 10, 370 ), wxSize( 100, 20 ), wxBORDER_SIMPLE );
		l_button = new wxButton( this, noIdCancel, _( "Cancel" ), wxPoint( 135, 370 ), wxSize( 100, 20 ), wxBORDER_SIMPLE );
		m_propertiesPanel = new wxPanel( this, wxID_ANY, wxPoint( 5, 35 ), wxSize( l_size.x - 10, l_size.y - 80 ) );
	}



	void NewOverlayFrame::_getOverlayTemplates( wxArrayString & p_array )
	{
		p_array.clear();
		OverlayManager::TemplateIterator l_it = OverlayManager::getSingletonPtr()->getTemplateIterator();

		while ( l_it.hasMoreElements() )
		{
			p_array.push_back( make_wxString( l_it.getNext()->getName().c_str() ) );
		}
	}

	void NewOverlayFrame::OnSelectTemplate( wxCommandEvent & p_event )
	{
		EMOverlay * l_emov;
		String l_templateName = make_string( p_event.GetString() );

		if ( m_overlay != NULL )
		{
			m_overlay->Reinitialise( make_wxString( l_templateName.c_str() ) );
		}
		else
		{
			TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * l_parentTrollOverlay = NULL;
			wxArrayString l_list;
			GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlaysNames( l_list );
			wxString l_fileName;

			if ( l_list.size() > 0 )
			{
				l_fileName = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( l_list[0] )->GetFileName();
			}
			else
			{
				l_fileName = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneFile( 0 )->FileName;
			}

			if ( m_parentOverlay != NULL )
			{
				l_emov = m_parentOverlay->CreateChild( make_string( m_overlayName ) );
				l_emov->Initialise( l_templateName );
				l_parentTrollOverlay = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( m_parentOverlay->GetName() ) );
			}
			else
			{
				l_emov = m_parentGroup->GetFirstElement();
				l_emov->Initialise( l_templateName );
			}

			m_overlay = new TROLL_PROJECT_2D_NAMESPACE::TrollOverlay( m_overlayName, l_parentTrollOverlay, l_fileName );
			m_overlay->SetMuseOverlay( l_emov );
		}

		m_overlay->BuildPanel( m_propertiesPanel, m_propertiesPanel->GetClientSize().x );
	}

	void NewOverlayFrame::OnOK( wxCommandEvent & p_event )
	{
		if ( m_overlay != NULL )
		{
			GUI::MainFrame::GetInstance()->AddOverlay( m_overlay, m_parentOverlay );
			m_overlay->HideProperties();
		}

		Close();
		GUI::MainFrame::GetInstance()->GetElypseCtrl()->SetNewOverlayFrame( NULL );
	}

	void NewOverlayFrame::OnCancel( wxCommandEvent & p_event )
	{
		if ( m_overlay != NULL )
		{
			EMOverlayGroup * l_group = m_overlay->GetMuseOverlay()->GetOwner();

			if ( m_parentOverlay == NULL )
			{
				m_gui->DestroyOverlayGroup( l_group->GetName() );
			}
			else
			{
				l_group->DestroyElement( m_overlay->GetMuseOverlay()->GetName() );
			}

			delete m_overlay;
		}

		Close();
		GUI::MainFrame::GetInstance()->GetElypseCtrl()->SetNewOverlayFrame( NULL );
	}
}
END_TROLL_GUI_NAMESPACE
