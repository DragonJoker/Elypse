#ifndef ___Troll_ContinuousEventProperties___
#define ___Troll_ContinuousEventProperties___

#include "Project/Temporal/Module_Temporal.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class ContinuousEventProperties : public ObjectProperties
		{
		public:
			ContinuousEventProperties( Temporal::TrollContinuousEvent & p_event, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
			~ContinuousEventProperties();

			virtual void UpdateProperties();

		protected:
			wxArrayString DoGetTargetTypes( const wxString & p_action );
			wxString DoBuildTargetChoice( const wxPoint & p_position, const wxString & p_targetType );
			wxString DoBuildInterpolationChoice( const wxPoint & p_position );
			void DoBuildKeyFramesList( const wxPoint & p_position );

		private:
			void OnSelectAction( wxCommandEvent & p_event );
			void OnSelectTargetType( wxCommandEvent & p_event );
			void OnEditTarget( wxCommandEvent & p_event );
			void OnSelectInterpolation( wxCommandEvent & p_event );
			void OnSelectKey( wxCommandEvent & p_event );
			DECLARE_EVENT_TABLE()

		private:
			Temporal::TrollContinuousEvent & m_event;
			Temporal::TrollKeyFrame * m_viewedKeyFrame;

			wxComboBox * m_actionName;
			wxComboBox * m_targetType;
			void * m_target;
			wxComboBox * m_interpolation;
			wxComboBox * m_keys;
			wxPanel * m_keyPanel;

		};
	}
}

#endif

