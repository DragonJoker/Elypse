//***********************************************************************************************************
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/SoundProperties.h"

#include "Project/Media/Sound.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
/*
#include <General/File.h>
*/
//***********************************************************************************************************
using namespace Troll::GUI;
using namespace Troll::Media;
//***********************************************************************************************************
enum SoundPropsIDs
{
	soundName,
	soundUrl,
	soundType,
	soundAttenuation,
	soundLooped,
	soundTimer,
	soundFadeIn,
	soundFadeOut
};
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( SoundProperties, wxPanel )
	EVT_TEXT_ENTER(	soundName,				SoundProperties::_onEditName )
	EVT_BUTTON(	soundUrl,				SoundProperties::_onEditUrl )
	EVT_COMBOBOX(	soundType,				SoundProperties::_onSelectSoundType )
	EVT_TEXT_ENTER(	soundAttenuation,		SoundProperties::_onEditAttenuation )
	EVT_COMBOBOX(	soundLooped,			SoundProperties::_onSelectLooped )
	EVT_TEXT_ENTER(	soundTimer,				SoundProperties::_onEditTimer )
	EVT_TEXT_ENTER(	soundFadeIn,			SoundProperties::_onEditFadeIn )
	EVT_TEXT_ENTER(	soundFadeOut,			SoundProperties::_onEditFadeOut )
END_EVENT_TABLE()
//***********************************************************************************************************

SoundProperties :: SoundProperties( TrollSound * p_sound, wxWindow * parent, const wxPoint & pos,
									const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_sound( p_sound )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_soundName = BuildTextCtrl( this, l_position, soundName, wxT( "Name" ), m_sound->GetName() );
	m_soundUrl = BuildButton( this, l_position += wxPoint( 0, 20 ), soundUrl, wxT( "Sound URL" ), m_sound->GetUrl() );
	wxArrayString l_types;
	l_types.Add( wxT( "Music" ) );
	l_types.Add( wxT( "SFX" ) );
	m_soundType = BuildComboBox( this, l_position += wxPoint( 0, 20 ), soundType, wxT( "Sound Type" ), l_types, ( m_sound->GetSoundType() == Sfx ? wxT( "SFX" ) : wxT( "Music" ) ) );

	if ( ! m_sound->IsGlobal() )
	{
		return;
	}

	m_attenuation = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundAttenuation, wxT( "Attenuation" ), m_sound->GetGlobal()->m_attenuation );
	wxArrayString l_looped;
	l_looped.Add( wxT( "Oui" ) );
	l_looped.Add( wxT( "Non" ) );
	m_looped = BuildComboBox( this, l_position += wxPoint( 0, 20 ), soundLooped, wxT( "Looped" ), l_looped, ( m_sound->GetGlobal()->m_looped ? wxT( "Oui" ) : wxT( "Non" ) ) );
	m_timer = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundTimer, wxT( "Timer time" ), m_sound->GetGlobal()->m_timer );
	m_fadeInTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeIn, wxT( "Fade In Time" ), m_sound->GetGlobal()->m_fadeInTime );
	m_fadeOutTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeOut, wxT( "Fade Out Time" ), m_sound->GetGlobal()->m_fadeOutTime );
}

//***********************************************************************************************************

SoundProperties :: ~SoundProperties()
{
}

//***********************************************************************************************************

void SoundProperties :: UpdateProperties()
{
	m_updatingProperties = true;
	m_updatingProperties = false;
}

//***********************************************************************************************************

void SoundProperties :: _onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_sound->SetName( m_soundName->GetValue() );
	}
}

//***********************************************************************************************************

void SoundProperties :: _onEditUrl( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxFileDialog * l_dialog = new wxFileDialog( this, wxT( "Choisissez un fichier son" ), wxEmptyString,
				wxEmptyString,
				wxT( "Audio files (*.wav;*.mp3;*.ogg;*.wma)|*.wav;*.mp3;*.ogg;*.wma" ) );

		if ( l_dialog->ShowModal() )
		{
			Path l_filePath = l_dialog->GetPath().char_str().data();
			Path l_folderPath = wxString( g_mainFrame->GetCurrentProject()->GetPath() + wxT( "Sons" ) ).char_str().data();

			if ( ! DirectoryExists( l_folderPath ) )
			{
				DirectoryCreate( l_folderPath );
			}

			wxString text = wxT( "Sons" );
			wxTreeItemId l_folderId = g_mainFrame->m_filesList->GetFolderId( wxT( "Datas non 3D (Dossiers)" ) );
			wxTreeItemId l_itemId = g_mainFrame->m_filesList->AddExistingFileToProjet( l_folderId, TITDataFolder, text );

			if ( ! g_mainFrame->GetCurrentProject()->GetMainScene()->AddFile( l_folderId, l_itemId, text, true, false ) )
			{
				g_mainFrame->m_filesList->Delete( l_itemId );
			}

			FileCopy( l_filePath, l_folderPath / l_filePath.GetLeaf() );
			Path l_relPath = g_mainFrame->GetCurrentProject()->GetMainScene()->GetName().char_str().data();
			l_relPath /= "Sons/";
			m_sound->SetUrl( make_wxString( l_relPath ) );
		}
	}
}

//***********************************************************************************************************

void SoundProperties :: _onSelectSoundType( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		int l_index = p_event.GetId() + 1;
		m_sound->SetSoundType( static_cast <TrollSoundType>( l_index ) );
	}
}

//***********************************************************************************************************

void SoundProperties :: _onEditAttenuation( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_attenuation->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				m_sound->SetGlobalAttenuation( l_res );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0.0 et 1.0" ), wxT( "Erreur" ) );
			}
		}
	}
}

//***********************************************************************************************************

void SoundProperties :: _onSelectLooped( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_looped->GetValue();
		m_sound->SetGlobalLooped( l_value == wxT( "Oui" ) );
	}
}

//***********************************************************************************************************

void SoundProperties :: _onEditTimer( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_timer->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_sound->SetGlobalTimer( l_res );
		}
	}
}

//***********************************************************************************************************

void SoundProperties :: _onEditFadeIn( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_fadeInTime->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_sound->SetGlobalFadeIn( l_res );
		}
	}
}

//***********************************************************************************************************

void SoundProperties :: _onEditFadeOut( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_fadeOutTime->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_sound->SetGlobalFadeOut( l_res );
		}
	}
}

//***********************************************************************************************************
