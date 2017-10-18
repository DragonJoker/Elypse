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
#ifndef ___Troll_SequencePanel___
#define ___Troll_SequencePanel___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class SequencePanel
				: public wxPanel
			{
			public:
				SequencePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~SequencePanel();

				ProjectComponents::Temporal::TrollSequence * SetSequence( ProjectComponents::Temporal::TrollSequence * p_sequence );
				void AddPonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event, Real p_time );
				void AddContinuousEvent( ProjectComponents::Temporal::TrollContinuousEvent * p_event );
				void RemovePonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event );
				void RemoveContinuousEvent( ProjectComponents::Temporal::TrollContinuousEvent * p_event );

			private:
				void _addPonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event, Real p_time );
				void _addContinuousEvent( ProjectComponents::Temporal::TrollContinuousEvent * p_event );
				int _getTopForContinuousEvent( ProjectComponents::Temporal::TrollContinuousEvent * p_event );
				int _getTopForPonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event );
				int _getTop( int p_left, int p_width );
				void _showContextMenu( wxPoint const & p_pos );

				DECLARE_EVENT_TABLE()
				void OnAddPonctualEvent( wxCommandEvent & p_event );
				void OnAddContinuousEvent( wxCommandEvent & p_event );
				void OnLeftMouseUp( wxMouseEvent & p_event );
				void OnRightMouseUp( wxMouseEvent & p_event );

			protected:
				LinePanel * m_sequencePanel;
				std::vector< PonctualEventPanel * > m_ponctualEvents;
				std::vector< ContinuousEventPanel * > m_continuousEvents;
				int m_currentLeft;
				ProjectComponents::Temporal::TrollSequence * m_sequence;
				DimensionMap m_addedPanelsDimensions;
				int m_maxTop;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
