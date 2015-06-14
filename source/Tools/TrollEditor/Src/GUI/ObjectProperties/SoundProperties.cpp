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

#include "SoundProperties.h"

#include "Project/Media/Sound.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"

using namespace TROLL_PROJECT_MEDIA_NAMESPACE;

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
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

	BEGIN_EVENT_TABLE( SoundProperties, wxPanel )
		EVT_TEXT_ENTER( soundName, SoundProperties::OnEditName )
		EVT_BUTTON( soundUrl, SoundProperties::OnEditUrl )
		EVT_COMBOBOX( soundType, SoundProperties::OnSelectSoundType )
		EVT_TEXT_ENTER( soundAttenuation, SoundProperties::OnEditAttenuation )
		EVT_COMBOBOX( soundLooped, SoundProperties::OnSelectLooped )
		EVT_TEXT_ENTER( soundTimer, SoundProperties::OnEditTimer )
		EVT_TEXT_ENTER( soundFadeIn, SoundProperties::OnEditFadeIn )
		EVT_TEXT_ENTER( soundFadeOut, SoundProperties::OnEditFadeOut )
	END_EVENT_TABLE()

	SoundProperties::SoundProperties( TrollSound * p_sound, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_sound( p_sound )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_soundName = BuildTextCtrl( this, l_position, soundName, _( "Name" ), m_sound->GetObjectName() );
		m_soundUrl = BuildButton( this, l_position += wxPoint( 0, 20 ), soundUrl, _( "Sound URL" ), m_sound->GetUrl() );
		wxArrayString l_types;
		l_types.Add( _( "Music" ) );
		l_types.Add( _( "SFX" ) );
		m_soundType = BuildComboBox( this, l_position += wxPoint( 0, 20 ), soundType, _( "Sound Type" ), l_types, ( m_sound->GetSoundType() == Sfx ? _( "SFX" ) : _( "Music" ) ) );

		if ( m_sound->IsGlobal() )
		{
			m_attenuation = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundAttenuation, _( "Attenuation" ), m_sound->GetGlobal()->m_attenuation );
			wxArrayString l_looped;
			l_looped.Add( TE_YES );
			l_looped.Add( TE_NO );
			m_looped = BuildComboBox( this, l_position += wxPoint( 0, 20 ), soundLooped, _( "Looped" ), l_looped, ( m_sound->GetGlobal()->m_looped ? TE_YES : TE_NO ) );
			m_timer = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundTimer, _( "Timer time" ), m_sound->GetGlobal()->m_timer );
			m_fadeInTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeIn, _( "Fade In Time" ), m_sound->GetGlobal()->m_fadeInTime );
			m_fadeOutTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeOut, _( "Fade Out Time" ), m_sound->GetGlobal()->m_fadeOutTime );
		}
	}

	SoundProperties::~SoundProperties()
	{
	}

	void SoundProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_updatingProperties = false;
	}

	void SoundProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_sound->SetObjectName( m_soundName->GetValue() );
		}
	}

	void SoundProperties::OnEditUrl( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxFileDialog l_dialog( this, _( "Choose an audio file" ), wxEmptyString,
					wxEmptyString,
					wxString() << _( "Audio files" ) << wxT( " (*.wav;*.mp3;*.ogg;*.wma)|*.wav;*.mp3;*.ogg;*.wma" ) );

			if ( l_dialog.ShowModal() )
			{
				Path l_filePath = make_string( l_dialog.GetPath() );
				Path l_folderPath = make_string( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetPath() + wxT( "Sons" ) );

				if ( ! DirectoryExists( l_folderPath ) )
				{
					DirectoryCreate( l_folderPath );
				}

				wxString text = _( "Sounds" );
				wxTreeItemId l_folderId = GUI::MainFrame::GetInstance()->GetFilesList()->GetFolderId( OTHER_DATA_FOLDERS );
				wxTreeItemId l_itemId = GUI::MainFrame::GetInstance()->GetFilesList()->AddExistingFileToProjet( l_folderId, TITDataFolder, text );

				if ( ! GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddFile( l_folderId, l_itemId, text, true, false ) )
				{
					GUI::MainFrame::GetInstance()->GetFilesList()->Delete( l_itemId );
				}

				FileCopy( l_filePath, l_folderPath / l_filePath.GetLeaf() );
				Path l_relPath = make_string( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetName() );
				l_relPath /= "Sons/";
				m_sound->SetUrl( make_wxString( l_relPath ) );
			}
		}
	}

	void SoundProperties::OnSelectSoundType( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			int l_index = p_event.GetId() + 1;
			m_sound->SetSoundType( static_cast <TrollSoundType>( l_index ) );
		}
	}

	void SoundProperties::OnEditAttenuation( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_attenuation->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				m_sound->SetGlobalAttenuation( l_res );
			}
		}
	}

	void SoundProperties::OnSelectLooped( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_looped->GetValue();
			m_sound->SetGlobalLooped( l_value == TE_YES );
		}
	}

	void SoundProperties::OnEditTimer( wxCommandEvent & p_event )
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

	void SoundProperties::OnEditFadeIn( wxCommandEvent & p_event )
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

	void SoundProperties::OnEditFadeOut( wxCommandEvent & p_event )
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
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
