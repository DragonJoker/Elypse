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

#include "VideoProperties.h"

#include "Project/Media/Video.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum VideoPropsIDs
	{
		videoName,
		videoUrl,
		videoLooped,
		videoEnabled
	};

	BEGIN_EVENT_TABLE( VideoProperties, wxPanel )
		EVT_TEXT_ENTER( videoName, VideoProperties::OnEditName )
		EVT_BUTTON( videoUrl, VideoProperties::OnEditUrl )
		EVT_COMBOBOX( videoLooped, VideoProperties::OnSelectLooped )
		EVT_COMBOBOX( videoEnabled, VideoProperties::OnSelectEnabled )
	END_EVENT_TABLE()

	VideoProperties::VideoProperties( TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideo * p_video, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_video( p_video )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_videoName = BuildTextCtrl( this, l_position, videoName, _( "Material Name" ), m_video->GetObjectName() );
		m_videoUrl = BuildButton( this, l_position += wxPoint( 0, 20 ), videoUrl, _( "Video URL" ), m_video->GetUrl() );
		wxArrayString l_looped;
		l_looped.Add( TE_YES );
		l_looped.Add( TE_NO );
		m_looped = BuildComboBox( this, l_position += wxPoint( 0, 20 ), videoLooped, _( "Looped" ), l_looped, ( m_video->IsLooped() ? TE_NO : TE_NO ) );
		m_enabled = BuildComboBox( this, l_position += wxPoint( 0, 20 ), videoEnabled, _( "Enabled" ), l_looped, ( m_video->IsEnabled() ? TE_NO : TE_NO ) );
	}

	VideoProperties::~VideoProperties()
	{
	}

	void VideoProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_updatingProperties = false;
	}

	void VideoProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_video->SetObjectName( m_videoName->GetValue() );
		}
	}

	void VideoProperties::OnEditUrl( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxFileDialog l_dialog( this, _( "Choose a video file" ), wxEmptyString, wxEmptyString,
					wxString() << _( "Video files" ) << wxT( " (*.mpeg;*.avi;*.ogm;*.wmv)|*.mpeg;*.avi;*.ogm;*.wmv" ) );

			if ( l_dialog.ShowModal() )
			{
				wxString l_filePath = l_dialog.GetPath();
				m_video->SetUrl( l_filePath );
			}
		}
	}

	void VideoProperties::OnSelectLooped( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_looped->GetValue();
			m_video->SetLooped( l_value == TE_YES );
		}
	}

	void VideoProperties::OnSelectEnabled( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = p_event.GetString();
			m_video->SetEnabled( l_value == TE_YES );
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
