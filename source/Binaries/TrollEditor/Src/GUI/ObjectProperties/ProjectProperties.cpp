
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/ProjectProperties.h"
#include "Project/Project.h"
/*
#include <wx/checkbox.h>
*/

using namespace Troll;
using namespace Troll::GUI;

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

BEGIN_EVENT_TABLE( ProjectProperties, wxFrame )
	EVT_COMBOBOX(	ppBackgroundType,	ProjectProperties::_onSelectBackgroundType )
	EVT_BUTTON(	ppBackground,		ProjectProperties::_onBackground )
	EVT_CHECKBOX(	ppShadows,			ProjectProperties::_onShadowsChange )
	EVT_COMBOBOX(	ppFSAA,				ProjectProperties::_onSelectFSAA )
	EVT_CHECKBOX(	ppShowDebugConsole,	ProjectProperties::_onShowConsoleChange )
	EVT_CHECKBOX(	ppShowFPS,			ProjectProperties::_onShowFPSChange )
	EVT_TEXT_ENTER(	ppStartupScript,	ProjectProperties::_onStartupScriptChange )
	EVT_BUTTON(	ppBack,				ProjectProperties::_onBack )
END_EVENT_TABLE()


ProjectProperties::ProjectProperties( Project * p_project, wxWindow * parent, const wxPoint & pos )
	:	wxFrame( parent, wxID_ANY, wxT( "Propriétés du projet" ), pos, wxSize( 200, 230 ),
				 wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT ),
	m_project( p_project )
{
	wxSize l_size = GetClientSize();
	wxPanel * l_panel = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), l_size, wxBORDER_NONE );
	l_size = l_panel->GetClientSize();
	wxPoint l_position( l_size.x / 2, 0 );

	if ( m_project )
	{
		SetLabel( wxT( "Propriétés du projet " ) + m_project->GetName() );
		wxArrayString l_values;
		l_values.push_back( wxT( "Couleur"	) );
		l_values.push_back( wxT( "Image"	) );
		wxString l_value;

		if ( m_project->GetBackgroundType() == bgColour )
		{
			l_value = wxT( "Couleur" );
		}
		else
		{
			l_value = wxT( "Image" );
		}

		m_backgroundType = BuildComboBox( l_panel, l_position, ppBackgroundType, wxT( "Type d'arrière plan" ), l_values, l_value );
		m_background = BuildButton( l_panel, l_position += wxPoint( 0, 25 ), ppBackground, wxT( "Sélectionner" ), l_value );
		m_shadows = new wxCheckBox( l_panel, ppShadows, wxT( "Ombres" ), l_position += wxPoint( 0, 25 ) );
		m_shadows->SetValue( m_project->GetShadows() );
		wxArrayString l_aaChoices;
		l_aaChoices.Add( wxT( "Aucun" ) );
		l_aaChoices.Add( wxT( "2x" ) );
		l_aaChoices.Add( wxT( "4x" ) );

		if ( m_project->GetFSAA() == aa0 )
		{
			l_value = wxT( "Aucun" );
		}
		else if ( m_project->GetFSAA() == aa2x )
		{
			l_value = wxT( "2x" );
		}
		else
		{
			l_value = wxT( "4x" );
		}

		m_fsaa = BuildComboBox( l_panel, l_position += wxPoint( 0, 25 ), ppFSAA, wxT( "Anti Aliasing : " ), l_aaChoices, l_value );
		m_showDebugConsole = new wxCheckBox( l_panel, ppShowDebugConsole, wxT( "Console" ), l_position += wxPoint( 0, 25 ) );
		m_showDebugConsole->SetValue( m_project->GetShowDebug() );
		m_showFPS = new wxCheckBox( l_panel, ppShowFPS, wxT( "FPS" ), l_position += wxPoint( 0, 25 ) );
		m_showFPS->SetValue( m_project->GetShowFPS() );
		m_startupScript = BuildTextCtrl( l_panel, l_position += wxPoint( 0, 25 ), ppStartupScript, wxT( "StartupScript" ) );
		m_startupScript->SetWindowStyle( wxTE_PROCESS_ENTER );
		m_startupScript->SetValue( m_project->GetStartupScript() );
	}
	else
	{
		l_panel->SetLabel( wxT( "Aucun projet en cours" ) );
	}

	m_back = new wxButton( l_panel, ppBack, wxT( "Retour" ), wxPoint( ( l_size.x / 2 ) - 35, l_size.y - 25 ), wxSize( 70, 20 ) );
}



ProjectProperties::~ProjectProperties()
{
}



void ProjectProperties::_onSelectBackgroundType( wxCommandEvent & p_event )
{
	if ( m_backgroundType->GetValue() == wxT( "Couleur" ) )
	{
		m_project->SetBackgroundType( bgColour );
	}
	else
	{
		m_project->SetBackgroundType( bgImage );
	}

	m_background->SetLabel( m_backgroundType->GetValue() );
}



void ProjectProperties::_onBackground( wxCommandEvent & p_event )
{
	if ( m_project->GetBackgroundType() == bgColour )
	{
		wxColourDialog l_colourDialor( this );

		if ( l_colourDialor.ShowModal() )
		{
			wxColourData retData = l_colourDialor.GetColourData();
			m_project->SetBackgroundColour( retData.GetColour().GetAsString() );
		}
	}
	else
	{
		wxString l_imagePath = wxFileSelector( wxT( "Choisissez une image" ), wxEmptyString, wxEmptyString, wxEmptyString,
											   wxT( "Fichiers BMP (*.bmp)|*.bmp|Fichiers PNG (*.png)|*.png|Fichiers JPEG (*.jpg)|*.jpg" ) );

		if ( ! l_imagePath.empty() )
		{
			m_project->SetBackgroundImage( l_imagePath );
		}
	}
}



void ProjectProperties::_onShadowsChange( wxCommandEvent & p_event )
{
	m_project->SetShadows( m_shadows->IsChecked() );
}



void ProjectProperties::_onSelectFSAA( wxCommandEvent & p_event )
{
	if ( m_fsaa->GetValue() == wxT( "Aucun" ) )
	{
		m_project->SetFSAA( aa0 );
	}
	else if ( m_fsaa->GetValue() == wxT( "2x" ) )
	{
		m_project->SetFSAA( aa2x );
	}
	else
	{
		m_project->SetFSAA( aa4x );
	}
}



void ProjectProperties::_onShowConsoleChange( wxCommandEvent & p_event )
{
	m_project->SetShowDebug( p_event.IsChecked() );
}



void ProjectProperties::_onShowFPSChange( wxCommandEvent & p_event )
{
	m_project->SetShowFPS( p_event.IsChecked() );
}



void ProjectProperties::_onStartupScriptChange( wxCommandEvent & p_event )
{
	m_project->SetStartupScript( m_startupScript->GetValue() );
}



void ProjectProperties::_onBack( wxCommandEvent & p_event )
{
	Destroy();
}


