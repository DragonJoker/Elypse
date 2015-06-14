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

#include "SoundObjectProperties.h"

#include "Project/Media/SoundObject.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum SoundObjectPropsIDs
	{
		soundAttenuation,
		soundLooped,
		soundTimer,
		soundFadeIn,
		soundFadeOut
	};

	BEGIN_EVENT_TABLE( SoundObjectProperties, wxPanel )
		EVT_TEXT_ENTER( soundAttenuation, SoundObjectProperties::OnEditAttenuation )
		EVT_COMBOBOX( soundLooped, SoundObjectProperties::OnSelectLooped )
		EVT_TEXT_ENTER( soundTimer, SoundObjectProperties::OnEditTimer )
		EVT_TEXT_ENTER( soundFadeIn, SoundObjectProperties::OnEditFadeIn )
		EVT_TEXT_ENTER( soundFadeOut, SoundObjectProperties::OnEditFadeOut )
	END_EVENT_TABLE()

	SoundObjectProperties::SoundObjectProperties( TROLL_PROJECT_MEDIA_NAMESPACE::TrollSoundObject * p_soundObject, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_soundObject( p_soundObject )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_attenuation = BuildTextCtrl( this, l_position, soundAttenuation, _( "Attenuation" ), m_soundObject->GetInstance().m_attenuation );
		wxArrayString l_looped;
		l_looped.Add( TE_YES );
		l_looped.Add( TE_NO );
		m_looped = BuildComboBox( this, l_position += wxPoint( 0, 20 ), soundLooped, _( "Looped" ), l_looped, ( m_soundObject->GetInstance().m_looped ? TE_YES : TE_NO ) );
		m_timer = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundTimer, _( "Timer time" ), m_soundObject->GetInstance().m_timer );
		m_fadeInTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeIn, _( "Fade In Time" ), m_soundObject->GetInstance().m_fadeInTime );
		m_fadeOutTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeOut, _( "Fade Out Time" ), m_soundObject->GetInstance().m_fadeOutTime );
	}

	SoundObjectProperties::~SoundObjectProperties()
	{
	}

	void SoundObjectProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_updatingProperties = false;
	}

	void SoundObjectProperties::OnEditAttenuation( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_attenuation->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				m_soundObject->SetAttenuation( l_res );
			}
		}
	}

	void SoundObjectProperties::OnSelectLooped( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_looped->GetValue();
			m_soundObject->SetLooped( l_value == TE_YES );
		}
	}

	void SoundObjectProperties::OnEditTimer( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_timer->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				m_soundObject->SetTimer( l_res );
			}
		}
	}

	void SoundObjectProperties::OnEditFadeIn( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_fadeInTime->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				m_soundObject->SetFadeIn( l_res );
			}
		}
	}

	void SoundObjectProperties::OnEditFadeOut( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_fadeOutTime->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				m_soundObject->SetFadeOut( l_res );
			}
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
