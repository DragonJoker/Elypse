
#ifndef ___SoundObjectProperties___
#define ___SoundObjectProperties___

#include "../../Project/Media/Module_Media.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class SoundObjectProperties : public ObjectProperties
		{
		public:
			Media::TrollSoundObject * m_soundObject;
			wxTextCtrl * m_attenuation;
			wxComboBox * m_looped;
			wxTextCtrl * m_timer;
			wxTextCtrl * m_fadeInTime;
			wxTextCtrl * m_fadeOutTime;

			SoundObjectProperties( Media::TrollSoundObject * p_soundObject, wxWindow * parent,
								   const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
								   long style = wxBORDER_NONE );
			~SoundObjectProperties();

			virtual void UpdateProperties();

		private:
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

