//***********************************************************************************************************
#ifndef ___TimeLineViewer___
#define ___TimeLineViewer___
//***********************************************************************************************************
#include "../Module_GUI.h"
#include "../../Project/Temporal/Module_Temporal.h"
#include <wx/panel.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class TimeLinePanel : public wxPanel
		{
		protected:
			int m_totalWidth;
			int m_totalHeight;
			int m_currentTop;
			int m_currentLeft;
			TrollSequenceStrMap m_alreadyAddedSequences;
			std::vector <SequencePanel *> m_sequences;
			unsigned int m_displayedSequences;

		public:
			TimeLinePanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
						   const wxSize & p_size = wxDefaultSize );
			~TimeLinePanel();

			void UpdateSequence( Temporal::TrollSequence * p_sequence );
			void UpdateSequences( float p_time );
			Temporal::TrollSequence * AddSequence( Temporal::TrollSequence * p_sequence );
			LinePanel * GetSequencePanel( const wxString & p_name );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************