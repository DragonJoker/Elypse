
#include "PrecompiledHeader.h"

#include "NewProjectFrame.h"

#include "MainFrame.h"
#include "Project/Project.h"

using namespace Troll;
using namespace Troll::GUI;

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
	EVT_COMBOBOX(	ID_bgRBox,		NewProjectFrame::_onSelectBackgroundType )
	EVT_BUTTON(	ID_bgBtn,		NewProjectFrame::_onSelectBackgroundButton )
	EVT_CHECKBOX(	ID_shCkb,		NewProjectFrame::_onShadowsChange )
	EVT_COMBOBOX(	ID_aaRBox,		NewProjectFrame::_onAntiAliasingChange )
	EVT_BUTTON(	ID_okBtn,		NewProjectFrame::_onOK )
	EVT_BUTTON(	ID_clBtn,		NewProjectFrame::_onCancel )
	EVT_BUTTON(	ID_pfBtn,		NewProjectFrame::_onBrowse )
END_EVENT_TABLE()


NewProjectFrame::NewProjectFrame( wxWindow * p_parent, const wxString & title, const wxPoint & pos )
	:	wxDialog( p_parent, -1, title, pos, wxSize( 220, 300 )	)
	,	m_antiAliasing( aa0	)
	,	m_shadows( false	)
	,	m_backgroundType( bgColour	)
	,	m_bgColour( wxT( "black" )	)
{
	SetClientSize( 240, 270 );
	wxSize l_size = GetClientSize();
	wxPoint l_position( l_size.x / 2, 10 );
	wxPoint l_ptOffset( 0, 25 );
	wxArrayString l_values;
	l_values.Add( wxT( "Couleur" ) );
	l_values.Add( wxT( "Image" ) );
	wxString l_strValue = wxT( "Couleur" );
	wxArrayString l_aaChoices;
	l_aaChoices.Add( wxT( "Aucun" ) );
	l_aaChoices.Add( wxT( "2x" ) );
	l_aaChoices.Add( wxT( "4x" ) );
	wxString l_strAaValue = wxT( "Aucun" );
	m_projectName = BuildTextCtrl( this, l_position, ID_pnETxt, wxT( "Nom du projet :" ), wxT( "Nom du projet" ) );
	l_position += l_ptOffset;
	m_folderButton = BuildButton( this, l_position, ID_pfBtn, wxT( "Dossier du projet" ), wxT( "Parcourir..." ) );
	l_position += l_ptOffset;
	m_projectFolder = new wxTextCtrl( this, ID_ppETxt, wxT( "Chemin" ), l_position - wxPoint( ( l_size.x / 2 ) - 5, 0 ), wxSize( l_size.x - 10, 20 ), wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
	l_position += l_ptOffset;
	m_bgRBox = BuildComboBox( this, l_position, ID_bgRBox, wxT( "Type d'arrière plan :" ), l_values, l_strValue );
	l_position += l_ptOffset;
	m_bgSelect = BuildButton( this, l_position, ID_bgBtn, wxT( "Sélectionner" ), l_strValue );
	l_position += l_ptOffset;
	m_mainSceneName = BuildTextCtrl( this, l_position, ID_msETxt, wxT( "Scène Principale" ), wxT( "Nom de la scene" ) );
	l_position += l_ptOffset;
	m_shCkb = new wxCheckBox( this, ID_shCkb, wxT( "Ombres" ), l_position - wxPoint( ( l_size.x / 2 ) - 5, 0 ) );
	l_position += l_ptOffset;
	m_antiAlias = BuildComboBox( this, l_position, ID_aaRBox, wxT( "Anti Aliasing : " ), l_aaChoices, l_strAaValue );
	l_position += l_ptOffset;
	wxStaticText * l_text = new wxStaticText( this, wxID_ANY, wxT( "Résolution" ), wxPoint( 0, l_position.y + 3 ) );
	m_width = new wxTextCtrl( this, ID_pwTxt, wxT( "640" ), wxPoint( 70, l_position.y ), wxSize( 30, 20 ), wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
	wxStaticText * l_x = new wxStaticText( this, wxID_ANY, wxT( "x" ), wxPoint( 105, l_position.y + 3 ) );
	m_height = new wxTextCtrl( this, ID_phTxt, wxT( "480" ), wxPoint( 115, l_position.y ), wxSize( 30, 20 ), wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
	l_position.x = 20;
	m_okBtn = new wxButton( this, ID_okBtn, wxT( "Valider" ), l_position += wxPoint( 0, 25 ), wxSize( 70, 20 ) );
	m_cancelBtn = new wxButton( this, ID_clBtn, wxT( "Annuler" ), l_position += wxPoint( 90, 0 ), wxSize( 70, 20 ) );
}



NewProjectFrame::~NewProjectFrame()
{
}



void NewProjectFrame::_onSelectBackgroundType( wxCommandEvent & p_event )
{
	if ( m_bgRBox->GetValue() == wxT( "Couleur" ) )
	{
		m_backgroundType = bgColour;
	}
	else
	{
		m_backgroundType = bgImage;
	}

	m_bgSelect->SetLabel( m_bgRBox->GetValue() );
}



void NewProjectFrame::_onSelectBackgroundButton( wxCommandEvent & p_event )
{
	if ( m_backgroundType == bgColour )
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
		wxString l_imagePath = wxFileSelector( wxT( "Choisissez une image" ), wxEmptyString, wxEmptyString, wxEmptyString,
											   wxT( "Fichiers BMP (*.bmp)|*.bmp|Fichiers PNG (*.png)|*.png|Fichiers JPEG (*.jpg)|*.jpg" ) );

		if ( ! l_imagePath.empty() )
		{
			m_bgImage = l_imagePath;
		}
	}
}



void NewProjectFrame::_onShadowsChange( wxCommandEvent & p_event )
{
	m_shadows = m_shCkb->IsChecked();
}



void NewProjectFrame::_onAntiAliasingChange( wxCommandEvent & p_event )
{
	if ( m_antiAlias->GetValue() == wxT( "Aucun" ) )
	{
		m_antiAliasing = aa0;
	}
	else if ( m_antiAlias->GetValue() == wxT( "2x" ) )
	{
		m_antiAliasing = aa2x;
	}
	else
	{
		m_antiAliasing = aa4x;
	}
}



void NewProjectFrame::_onOK( wxCommandEvent & p_event )
{
	wxString l_projectName = m_projectName->GetLineText( 0 );

	if ( l_projectName.empty() || l_projectName == wxT( "Nom du projet" ) )
	{
		wxMessageBox( wxT( "Entrez un nom de projet" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_EXCLAMATION, this );
		return;
	}

	wxString l_mainSceneName = m_mainSceneName->GetLineText( 0 );

	if ( l_mainSceneName.empty() || l_mainSceneName == wxT( "Nom de la scène" ) )
	{
		wxMessageBox( wxT( "Entrez un nom de scène principale" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_EXCLAMATION, this );
		return;
	}

	wxString l_projectPath = m_projectFolder->GetLineText( 0 );

	if ( l_projectPath.empty() || l_projectPath == wxT( "Chemin" ) )
	{
		wxMessageBox( wxT( "Choisissez un dossier de projet" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_EXCLAMATION, this );
		return;
	}

	if ( l_projectPath[l_projectPath.size() - 1] != wxT( '\\' ) && l_projectPath[l_projectPath.size() - 1] != wxT( '/' ) )
	{
		l_projectPath.Append( wxFileName::GetPathSeparator() );
	}

	if ( ! DirectoryExists( l_projectPath.char_str().data() ) )
	{
		DirectoryCreate( l_projectPath.char_str().data() );
	}

	if ( m_bgColour.empty() && m_bgImage.empty() )
	{
		wxMessageBox( wxT( "Choisissez une couleur ou image de fond" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_EXCLAMATION, this );
		return;
	}

	wxSize l_resolution;
	long l_width, l_height;
	m_width->GetValue().ToLong( & l_width );
	m_height->GetValue().ToLong( & l_height );
	l_resolution = wxSize( l_width, l_height );

	if ( m_backgroundType == bgColour )
	{
		g_mainFrame->SetProject( l_projectName, l_projectPath, l_mainSceneName, m_backgroundType,
								 m_bgColour, m_shadows, m_antiAliasing, l_resolution );
	}
	else
	{
		g_mainFrame->SetProject( l_projectName, l_projectPath, l_mainSceneName, m_backgroundType,
								 m_bgImage, m_shadows, m_antiAliasing, l_resolution );
	}

	Hide();
}



void NewProjectFrame::_onCancel( wxCommandEvent & p_event )
{
	Destroy();
}



void NewProjectFrame::_onBrowse( wxCommandEvent & p_event )
{
	wxDirDialog l_dialog( NULL, wxT( "Sélectionnez un dossier" ) );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		m_path = l_dialog.GetPath();
		m_projectFolder->Clear();
		m_projectFolder->AppendText( m_path );
	}
}


