
#ifndef ___SoundProperties___
#define ___SoundProperties___

#include "Project/Media/Module_Media.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class SoundProperties
			: public ObjectProperties
		{
		public:
			Media::TrollSound * m_sound;
			wxTextCtrl * m_soundName;
			wxButton * m_soundUrl;
			wxComboBox * m_soundType;
			wxTextCtrl * m_attenuation;
			wxComboBox * m_looped;
			wxTextCtrl * m_timer;
			wxTextCtrl * m_fadeInTime;
			wxTextCtrl * m_fadeOutTime;

			SoundProperties( Media::TrollSound * p_sound, wxWindow * parent,
							 const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							 long style = wxBORDER_NONE );
			~SoundProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );
			void _onEditUrl( wxCommandEvent & p_event );
			void _onSelectSoundType( wxCommandEvent & p_event );
			void _onEditAttenuation( wxCommandEvent & p_event );
			void _onSelectLooped( wxCommandEvent & p_event );
			void _onEditTimer( wxCommandEvent & p_event );
			void _onEditFadeIn( wxCommandEvent & p_event );
			void _onEditFadeOut( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

