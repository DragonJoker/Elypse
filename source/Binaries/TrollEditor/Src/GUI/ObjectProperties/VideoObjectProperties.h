//***********************************************************************************************************
#ifndef ___VideoObjectProperties___
#define ___VideoObjectProperties___
//***********************************************************************************************************
#include "../../Project/Media/Module_Media.h"
#include "ObjectProperties.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class VideoObjectProperties : public ObjectProperties
		{
		public:
			Media::TrollVideoObject * m_videoObject;
			wxTextCtrl * m_attenuation;

			VideoObjectProperties( Media::TrollVideoObject * p_videoObject, wxWindow * parent,
								   const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
								   long style = wxBORDER_NONE );
			~VideoObjectProperties();

			virtual void UpdateProperties();

		private:
			void _onEditAttenuation( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
