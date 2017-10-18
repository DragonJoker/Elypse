/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
