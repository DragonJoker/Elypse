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

#include "ProjectProperties.h"

#include "Project/Project.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum projectPropertiesID
	{
		ppProjectName,
		ppBackgroundType,
		ppBackground,
		ppShadows,
		ppFSAA,
		ppShowDebugConsole,
		ppShowFPS,
		ppStartupScript,
		ppBack
	};

	BEGIN_EVENT_TABLE( ProjectProperties, wxDialog )
		EVT_COMBOBOX( ppBackgroundType, ProjectProperties::OnSelectBackgroundType )
		EVT_BUTTON( ppBackground, ProjectProperties::OnBackground )
		EVT_CHECKBOX( ppShadows, ProjectProperties::OnShadowsChange )
		EVT_COMBOBOX( ppFSAA, ProjectProperties::OnSelectFSAA )
		EVT_CHECKBOX( ppShowDebugConsole, ProjectProperties::OnShowConsoleChange )
		EVT_CHECKBOX( ppShowFPS, ProjectProperties::OnShowFPSChange )
		EVT_TEXT_ENTER( ppStartupScript, ProjectProperties::OnStartupScriptChange )
		EVT_BUTTON( ppBack, ProjectProperties::OnBack )
	END_EVENT_TABLE()

	ProjectProperties::ProjectProperties( TROLL_PROJECT_NAMESPACE::Project & p_project, wxWindow * parent, const wxPoint & pos )
		: wxDialog( parent, wxID_ANY, _( "Project properties" ), pos, wxSize( 200, 230 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT )
		, m_project( p_project )
	{
		wxSize l_size = GetClientSize();
		wxPanel * l_panel = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), l_size, wxBORDER_NONE );
		l_size = l_panel->GetClientSize();
		wxPoint l_position( l_size.x / 2, 0 );

		SetLabel( _( "Project properties: " ) + m_project.GetName() );
		wxArrayString l_values;
		l_values.push_back( _( "Colour" ) );
		l_values.push_back( _( "Image" ) );
		wxString l_value;

		if ( m_project.GetBackgroundType() == TROLL_PROJECT_NAMESPACE::bgColour )
		{
			l_value = _( "Colour" );
		}
		else
		{
			l_value = _( "Image" );
		}

		m_backgroundType = BuildComboBox( l_panel, l_position, ppBackgroundType, _( "Background type" ), l_values, l_value );
		m_background = BuildButton( l_panel, l_position += wxPoint( 0, 25 ), ppBackground, _( "Choose" ), l_value );
		m_shadows = new wxCheckBox( l_panel, ppShadows, wxT( "Ombres" ), l_position += wxPoint( 0, 25 ) );
		m_shadows->SetValue( m_project.GetShadows() );
		wxArrayString l_aaChoices;
		l_aaChoices.Add( _( "None" ) );
		l_aaChoices.Add( _( "2x" ) );
		l_aaChoices.Add( _( "4x" ) );

		if ( m_project.GetFSAA() == TROLL_PROJECT_NAMESPACE::aa0 )
		{
			l_value = _( "None" );
		}
		else if ( m_project.GetFSAA() == TROLL_PROJECT_NAMESPACE::aa2x )
		{
			l_value = _( "2x" );
		}
		else
		{
			l_value = _( "4x" );
		}

		m_fsaa = BuildComboBox( l_panel, l_position += wxPoint( 0, 25 ), ppFSAA, _( "Anti Aliasing : " ), l_aaChoices, l_value );
		m_showDebugConsole = new wxCheckBox( l_panel, ppShowDebugConsole, _( "Console" ), l_position += wxPoint( 0, 25 ) );
		m_showDebugConsole->SetValue( m_project.GetShowDebug() );
		m_showFPS = new wxCheckBox( l_panel, ppShowFPS, _( "FPS" ), l_position += wxPoint( 0, 25 ) );
		m_showFPS->SetValue( m_project.GetShowFPS() );
		m_startupScript = BuildTextCtrl( l_panel, l_position += wxPoint( 0, 25 ), ppStartupScript, _( "StartupScript" ) );
		m_startupScript->SetWindowStyle( wxTE_PROCESS_ENTER );
		m_startupScript->SetValue( m_project.GetStartupScript() );
		m_back = new wxButton( l_panel, ppBack, _( "Back" ), wxPoint( ( l_size.x / 2 ) - 35, l_size.y - 25 ), wxSize( 70, 20 ) );
	}

	ProjectProperties::~ProjectProperties()
	{
	}

	void ProjectProperties::OnSelectBackgroundType( wxCommandEvent & p_event )
	{
		if ( m_backgroundType->GetValue() == _( "Colour" ) )
		{
			m_project.SetBackgroundType( TROLL_PROJECT_NAMESPACE::bgColour );
		}
		else
		{
			m_project.SetBackgroundType( TROLL_PROJECT_NAMESPACE::bgImage );
		}

		m_background->SetLabel( m_backgroundType->GetValue() );
	}

	void ProjectProperties::OnBackground( wxCommandEvent & p_event )
	{
		if ( m_project.GetBackgroundType() == TROLL_PROJECT_NAMESPACE::bgColour )
		{
			wxColourDialog l_colourDialor( this );

			if ( l_colourDialor.ShowModal() )
			{
				wxColourData retData = l_colourDialor.GetColourData();
				m_project.SetBackgroundColour( retData.GetColour().GetAsString() );
			}
		}
		else
		{
			wxString l_imagePath = wxFileSelector( _( "Choose an image" ), wxEmptyString, wxEmptyString, wxEmptyString,
												   wxString() << _( "BMP files" ) << wxT( " (*.bmp)|*.bmp|" ) << _( "PNG files" ) << wxT( " (*.png)|*.png|" ) << _( "JPEG files" ) << wxT( " (*.jpg)|*.jpg" ) );

			if ( ! l_imagePath.empty() )
			{
				m_project.SetBackgroundImage( l_imagePath );
			}
		}
	}

	void ProjectProperties::OnShadowsChange( wxCommandEvent & p_event )
	{
		m_project.SetShadows( m_shadows->IsChecked() );
	}

	void ProjectProperties::OnSelectFSAA( wxCommandEvent & p_event )
	{
		if ( m_fsaa->GetValue() == _( "None" ) )
		{
			m_project.SetFSAA( TROLL_PROJECT_NAMESPACE::aa0 );
		}
		else if ( m_fsaa->GetValue() == _( "2x" ) )
		{
			m_project.SetFSAA( TROLL_PROJECT_NAMESPACE::aa2x );
		}
		else
		{
			m_project.SetFSAA( TROLL_PROJECT_NAMESPACE::aa4x );
		}
	}

	void ProjectProperties::OnShowConsoleChange( wxCommandEvent & p_event )
	{
		m_project.SetShowDebug( p_event.IsChecked() );
	}

	void ProjectProperties::OnShowFPSChange( wxCommandEvent & p_event )
	{
		m_project.SetShowFPS( p_event.IsChecked() );
	}

	void ProjectProperties::OnStartupScriptChange( wxCommandEvent & p_event )
	{
		m_project.SetStartupScript( m_startupScript->GetValue() );
	}

	void ProjectProperties::OnBack( wxCommandEvent & p_event )
	{
		Destroy();
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE