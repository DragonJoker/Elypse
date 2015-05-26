//***********************************************************************************************************
#ifndef ___VideoProperties___
#define ___VideoProperties___
//***********************************************************************************************************
#include "../../Project/Media/Module_Media.h"
#include "ObjectProperties.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class VideoProperties : public ObjectProperties
		{
		public:
			Media::TrollVideo * m_video;
			wxTextCtrl * m_videoName;
			wxButton * m_videoUrl;
			wxComboBox * m_looped;
			wxComboBox * m_enabled;

			VideoProperties( Media::TrollVideo * p_video, wxWindow * parent,
							 const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							 long style = wxBORDER_NONE );
			~VideoProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );
			void _onEditUrl( wxCommandEvent & p_event );
			void _onSelectLooped( wxCommandEvent & p_event );
			void _onSelectEnabled( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
