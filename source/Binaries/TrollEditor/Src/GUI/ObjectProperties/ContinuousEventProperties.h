//***********************************************************************************************************
#ifndef ___Troll_ContinuousEventProperties___
#define ___Troll_ContinuousEventProperties___
//***********************************************************************************************************
#include "../../Project/Temporal/Module_Temporal.h"
#include "ObjectProperties.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class ContinuousEventProperties : public ObjectProperties
		{
		private:
			Temporal::TrollContinuousEvent * m_event;

			wxComboBox *	m_actionName;
			wxComboBox *	m_targetType;
			void 	*		m_target;
			wxComboBox *	m_interpolation;
			wxComboBox *	m_keys;
			wxPanel 	*	m_keyPanel;

			Temporal::TrollKeyFrame * m_viewedKeyFrame;


		public:
			ContinuousEventProperties( Temporal::TrollContinuousEvent * p_event, wxWindow * parent,
									   const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
									   long style = wxBORDER_NONE );
			~ContinuousEventProperties();

			virtual void UpdateProperties();

		protected:
			wxArrayString _getTargetTypes( const wxString & p_action );
			wxString _buildTargetChoice( const wxPoint & p_position, const wxString & p_targetType );
			wxString _buildInterpolationChoice( const wxPoint & p_position );
			void _buildKeyFramesList( const wxPoint & p_position );

		private:
			void _onSelectAction( wxCommandEvent & p_event );
			void _onSelectTargetType( wxCommandEvent & p_event );
			void _onEditTarget( wxCommandEvent & p_event );
			void _onSelectInterpolation( wxCommandEvent & p_event );
			void _onSelectKey( wxCommandEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
