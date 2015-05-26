//***********************************************************************************************************
#ifndef ___Troll_SequenceProperties___
#define ___Troll_SequenceProperties___
//***********************************************************************************************************
#include "../../Project/Temporal/Module_Temporal.h"
#include "ObjectProperties.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class SequenceProperties : public ObjectProperties
		{
		private:
			Temporal::TrollSequence * m_sequence;
			wxTextCtrl * m_sequenceName;

		public:
			SequenceProperties( Temporal::TrollSequence * p_sequence, wxWindow * parent,
								const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
								long style = wxBORDER_NONE );
			~SequenceProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
