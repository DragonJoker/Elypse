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

#include "GUI/NewProjectFrame.h"

#include "GUI/MainFrame.h"

#include "Project/Project.h"

BEGIN_TROLL_GUI_NAMESPACE
{
	const wxString DEFAULT_PROJECT_NAME = _( "Project name" );
	const wxString DEFAULT_SCENE_NAME = _( "Scene name" );
	const wxString DEFAULT_FOLDER = _( "Path" );

	enum NewProjectFrameIDs
	{
		ID_pfBtn,
		ID_ppETxt,
		ID_ppSTxt,
		ID_clBtn,
		ID_okBtn,
		ID_pnETxt,
		ID_pnSTxt,
		ID_bgBtn,
		ID_bgRBox,
		ID_aaRBox,
		ID_aaSTxt,
		ID_msETxt,
		ID_msSTxt,
		ID_shCkb,
		ID_bgSTxt,
		ID_pwTxt,
		ID_phTxt
	};

	BEGIN_EVENT_TABLE( NewProjectFrame, wxDialog )
		EVT_COMBOBOX( ID_bgRBox, NewProjectFrame::OnSelectBackgroundType )
		EVT_BUTTON( ID_bgBtn, NewProjectFrame::OnSelectBackgroundButton )
		EVT_CHECKBOX( ID_shCkb, NewProjectFrame::OnShadowsChange )
		EVT_COMBOBOX( ID_aaRBox, NewProjectFrame::OnAntiAliasingChange )
		EVT_BUTTON( ID_okBtn, NewProjectFrame::OnOK )
		EVT_BUTTON( ID_clBtn, NewProjectFrame::OnCancel )
		EVT_BUTTON( ID_pfBtn, NewProjectFrame::OnBrowse )
	END_EVENT_TABLE()
	
	NewProjectFrame::NewProjectFrame( wxWindow * p_parent, const wxString & title, const wxPoint & pos )
		: wxDialog( p_parent, -1, title, pos, wxSize( 220, 300 ) )
		, m_antiAliasing( TROLL_PROJECT_NAMESPACE::aa0 )
		, m_shadows( false )
		, m_backgroundType( TROLL_PROJECT_NAMESPACE::bgColour )
		, m_bgColour( wxT( "black" ) )
	{
		SetClientSize( 240, 270 );
		wxSize l_size = GetClientSize();
		wxPoint l_position( l_size.x / 2, 10 );
		wxPoint l_ptOffset( 0, 25 );
		wxArrayString l_values;
		l_values.Add( _( "Colour" ) );
		l_values.Add( _( "Image" ) );
		wxString l_strValue = _( "Colour" );
		wxArrayString l_aaChoices;
		l_aaChoices.Add( _( "None" ) );
		l_aaChoices.Add( _( "2x" ) );
		l_aaChoices.Add( _( "4x" ) );
		wxString l_strAaValue = _( "None" );
		m_projectName = BuildTextCtrl( this, l_position, ID_pnETxt, _( "Project Nmae:" ), DEFAULT_PROJECT_NAME );
		l_position += l_ptOffset;
		m_folderButton = BuildButton( this, l_position, ID_pfBtn, _( "Project Folder" ), _( "Browse..." ) );
		l_position += l_ptOffset;
		m_projectFolder = new wxTextCtrl( this, ID_ppETxt, DEFAULT_FOLDER, l_position - wxPoint( ( l_size.x / 2 ) - 5, 0 ), wxSize( l_size.x - 10, 20 ), wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
		l_position += l_ptOffset;
		m_bgRBox = BuildComboBox( this, l_position, ID_bgRBox, _( "Background Type:" ), l_values, l_strValue );
		l_position += l_ptOffset;
		m_bgSelect = BuildButton( this, l_position, ID_bgBtn, _( "Choose" ), l_strValue );
		l_position += l_ptOffset;
		m_mainSceneName = BuildTextCtrl( this, l_position, ID_msETxt, _( "Main Scene" ), DEFAULT_SCENE_NAME );
		l_position += l_ptOffset;
		m_shCkb = new wxCheckBox( this, ID_shCkb, _( "Shadows" ), l_position - wxPoint( ( l_size.x / 2 ) - 5, 0 ) );
		l_position += l_ptOffset;
		m_antiAlias = BuildComboBox( this, l_position, ID_aaRBox, _( "Anti Aliasing : " ), l_aaChoices, l_strAaValue );
		l_position += l_ptOffset;
		wxStaticText * l_text = new wxStaticText( this, wxID_ANY, _( "Dimensions" ), wxPoint( 0, l_position.y + 3 ) );
		m_width = new wxTextCtrl( this, ID_pwTxt, wxT( "640" ), wxPoint( 70, l_position.y ), wxSize( 30, 20 ), wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
		wxStaticText * l_x = new wxStaticText( this, wxID_ANY, wxT( "x" ), wxPoint( 105, l_position.y + 3 ) );
		m_height = new wxTextCtrl( this, ID_phTxt, wxT( "480" ), wxPoint( 115, l_position.y ), wxSize( 30, 20 ), wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
		l_position.x = 20;
		m_okBtn = new wxButton( this, ID_okBtn, _( "OK" ), l_position += wxPoint( 0, 25 ), wxSize( 70, 20 ) );
		m_cancelBtn = new wxButton( this, ID_clBtn, _( "Cancel" ), l_position += wxPoint( 90, 0 ), wxSize( 70, 20 ) );
	}



	NewProjectFrame::~NewProjectFrame()
	{
	}



	void NewProjectFrame::OnSelectBackgroundType( wxCommandEvent & p_event )
	{
		if ( m_bgRBox->GetValue() == _( "Colour" ) )
		{
			m_backgroundType = TROLL_PROJECT_NAMESPACE::bgColour;
		}
		else
		{
			m_backgroundType = TROLL_PROJECT_NAMESPACE::bgImage;
		}

		m_bgSelect->SetLabel( m_bgRBox->GetValue() );
	}



	void NewProjectFrame::OnSelectBackgroundButton( wxCommandEvent & p_event )
	{
		if ( m_backgroundType == TROLL_PROJECT_NAMESPACE::bgColour )
		{
			wxColourDialog l_colourDialor( this );

			if ( l_colourDialor.ShowModal() )
			{
				wxColourData retData = l_colourDialor.GetColourData();
				m_bgColour = retData.GetColour().GetAsString();
			}
		}
		else
		{
			wxString l_imagePath = wxFileSelector( _( "Choose an image" ), wxEmptyString, wxEmptyString, wxEmptyString,
												   wxString() << _( "BMP Files" ) << wxT( " (*.bmp)|*.bmp|" ) << _( "PNG Files" ) << wxT( " (*.png)|*.png|" ) << _( "JPEG Files" ) << wxT( " (*.jpg)|*.jpg" ) );

			if ( ! l_imagePath.empty() )
			{
				m_bgImage = l_imagePath;
			}
		}
	}

	void NewProjectFrame::OnShadowsChange( wxCommandEvent & p_event )
	{
		m_shadows = m_shCkb->IsChecked();
	}

	void NewProjectFrame::OnAntiAliasingChange( wxCommandEvent & p_event )
	{
		if ( m_antiAlias->GetValue() == _( "None" ) )
		{
			m_antiAliasing = TROLL_PROJECT_NAMESPACE::aa0;
		}
		else if ( m_antiAlias->GetValue() == _( "2x" ) )
		{
			m_antiAliasing = TROLL_PROJECT_NAMESPACE::aa2x;
		}
		else
		{
			m_antiAliasing = TROLL_PROJECT_NAMESPACE::aa4x;
		}
	}

	void NewProjectFrame::OnOK( wxCommandEvent & p_event )
	{
		wxString l_projectName = m_projectName->GetLineText( 0 );

		if ( l_projectName.empty() || l_projectName == DEFAULT_PROJECT_NAME )
		{
			wxMessageBox( _( "Enter a project name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, this );
			return;
		}

		wxString l_mainSceneName = m_mainSceneName->GetLineText( 0 );

		if ( l_mainSceneName.empty() || l_mainSceneName == DEFAULT_SCENE_NAME )
		{
			wxMessageBox( _( "Enter the main scene name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, this );
			return;
		}

		wxString l_projectPath = m_projectFolder->GetLineText( 0 );

		if ( l_projectPath.empty() || l_projectPath == DEFAULT_FOLDER )
		{
			wxMessageBox( _( "Choose the project's folder" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, this );
			return;
		}

		if ( l_projectPath[l_projectPath.size() - 1] != wxT( '\\' ) && l_projectPath[l_projectPath.size() - 1] != wxT( '/' ) )
		{
			l_projectPath.Append( wxFileName::GetPathSeparator() );
		}

		if ( ! DirectoryExists( make_string( l_projectPath ) ) )
		{
			DirectoryCreate( make_string( l_projectPath ) );
		}

		if ( m_bgColour.empty() && m_bgImage.empty() )
		{
			wxMessageBox( _( "Choose a background colour or image" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, this );
			return;
		}

		wxSize l_resolution;
		long l_width, l_height;
		m_width->GetValue().ToLong( & l_width );
		m_height->GetValue().ToLong( & l_height );
		l_resolution = wxSize( l_width, l_height );

		if ( m_backgroundType == TROLL_PROJECT_NAMESPACE::bgColour )
		{
			GUI::MainFrame::GetInstance()->SetProject( l_projectName, l_projectPath, l_mainSceneName, m_backgroundType, m_bgColour, m_shadows, m_antiAliasing, l_resolution );
		}
		else
		{
			GUI::MainFrame::GetInstance()->SetProject( l_projectName, l_projectPath, l_mainSceneName, m_backgroundType, m_bgImage, m_shadows, m_antiAliasing, l_resolution );
		}

		Hide();
	}

	void NewProjectFrame::OnCancel( wxCommandEvent & p_event )
	{
		Destroy();
	}

	void NewProjectFrame::OnBrowse( wxCommandEvent & p_event )
	{
		wxDirDialog l_dialog( NULL, _( "Choose a directory" ) );

		if ( l_dialog.ShowModal() == wxID_OK )
		{
			m_path = l_dialog.GetPath();
			m_projectFolder->Clear();
			m_projectFolder->AppendText( m_path );
		}
	}
}
END_TROLL_GUI_NAMESPACE
