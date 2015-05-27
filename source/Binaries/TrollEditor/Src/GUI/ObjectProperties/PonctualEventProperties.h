
#ifndef ___Troll_PonctualEventProperties___
#define ___Troll_PonctualEventProperties___

#include "../../Project/Temporal/Module_Temporal.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class PonctualEventProperties : public ObjectProperties
		{
		private:
			Temporal::TrollPonctualEvent * m_event;

			wxComboBox *	m_actionName;
			wxComboBox *	m_targetType;
			wxComboBox *	m_targetCB;
			wxTextCtrl *	m_targetTC;
			wxTextCtrl *	m_fireTime;
			wxTextCtrl *	m_v3ArgX;
			wxTextCtrl *	m_v3ArgY;
			wxTextCtrl *	m_v3ArgZ;

		public:
			PonctualEventProperties( Temporal::TrollPonctualEvent * p_event, wxWindow * parent,
									 const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
									 long style = wxBORDER_NONE );
			~PonctualEventProperties();

			virtual void UpdateProperties();

		protected:
			wxArrayString _getTargetTypes( const wxString & p_action );
			wxString _buildTargetChoice( const wxPoint & p_position, const wxString & p_targetType );
			void _buildActionArguments( wxPoint & p_position, const wxString & p_action );

		private:
			void _onSelectAction( wxCommandEvent & p_event );
			void _onSelectTargetType( wxCommandEvent & p_event );
			void _onEditTarget( wxCommandEvent & p_event );
			void _onEditTime( wxCommandEvent & p_event );
			void _onEditXParam( wxCommandEvent & p_event );
			void _onEditYParam( wxCommandEvent & p_event );
			void _onEditZParam( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

