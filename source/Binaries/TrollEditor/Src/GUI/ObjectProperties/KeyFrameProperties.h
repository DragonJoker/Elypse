//***********************************************************************************************************
#ifndef ___Troll_KeyFrameProperties___
#define ___Troll_KeyFrameProperties___
//***********************************************************************************************************
#include "../../Project/Temporal/Module_Temporal.h"
#include "ObjectProperties.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class KeyFrameProperties : public ObjectProperties
		{
		private:
			Temporal::TrollKeyFrame * m_key;

			wxTextCtrl *	m_fireTime;
			wxTextCtrl *	m_real;
			wxTextCtrl *	m_v3X;
			wxTextCtrl *	m_v3Y;
			wxTextCtrl *	m_v3Z;


		public:
			KeyFrameProperties( Temporal::TrollKeyFrame * p_keyFrame, wxWindow * parent,
								const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
								long style = wxBORDER_NONE );
			~KeyFrameProperties();

			virtual void UpdateProperties();

		protected:
			void _buildArguments( const wxPoint & p_position );

		private:
			void _onEditFireTime( wxCommandEvent & p_event );
			void _onEditReal( wxCommandEvent & p_event );
			void _onEditV3X( wxCommandEvent & p_event );
			void _onEditV3Y( wxCommandEvent & p_event );
			void _onEditV3Z( wxCommandEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
