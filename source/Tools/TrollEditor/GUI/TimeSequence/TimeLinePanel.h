/*
See LICENSE file in root folder
*/
#ifndef ___TimeLineViewer___
#define ___TimeLineViewer___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class TimeLinePanel
				: public wxPanel
			{
			public:
				TimeLinePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~TimeLinePanel();

				void UpdateSequence( ProjectComponents::Temporal::TrollSequence * p_sequence );
				void UpdateSequences( float p_time );
				ProjectComponents::Temporal::TrollSequence * AddSequence( ProjectComponents::Temporal::TrollSequence * p_sequence );
				LinePanel * GetSequencePanel( wxString const & p_name );

			private:
				DECLARE_EVENT_TABLE()

			protected:
				int m_totalWidth;
				int m_totalHeight;
				int m_currentTop;
				int m_currentLeft;
				ProjectComponents::Temporal::TrollSequenceStrMap m_alreadyAddedSequences;
				std::vector< SequencePanel * > m_sequences;
				uint32_t m_displayedSequences;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
