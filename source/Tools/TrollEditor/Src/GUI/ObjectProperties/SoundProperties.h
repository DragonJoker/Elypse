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
#ifndef ___SoundProperties___
#define ___SoundProperties___

#include "Project/Media/Module_Media.h"

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class SoundProperties
		: public ObjectProperties
	{
	public:
		SoundProperties( TROLL_PROJECT_MEDIA_NAMESPACE::TrollSound * p_sound, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
		~SoundProperties();

		virtual void UpdateProperties();

	private:
		DECLARE_EVENT_TABLE()
		void OnEditName( wxCommandEvent & p_event );
		void OnEditUrl( wxCommandEvent & p_event );
		void OnSelectSoundType( wxCommandEvent & p_event );
		void OnEditAttenuation( wxCommandEvent & p_event );
		void OnSelectLooped( wxCommandEvent & p_event );
		void OnEditTimer( wxCommandEvent & p_event );
		void OnEditFadeIn( wxCommandEvent & p_event );
		void OnEditFadeOut( wxCommandEvent & p_event );

	private:
		TROLL_PROJECT_MEDIA_NAMESPACE::TrollSound * m_sound;
		wxTextCtrl * m_soundName;
		wxButton * m_soundUrl;
		wxComboBox * m_soundType;
		wxTextCtrl * m_attenuation;
		wxComboBox * m_looped;
		wxTextCtrl * m_timer;
		wxTextCtrl * m_fadeInTime;
		wxTextCtrl * m_fadeOutTime;
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
