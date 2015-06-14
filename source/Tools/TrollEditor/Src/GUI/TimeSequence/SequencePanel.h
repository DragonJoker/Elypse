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

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	class SequencePanel
		: public wxPanel
	{
	public:
		SequencePanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition, const wxSize & p_size = wxDefaultSize );
		~SequencePanel();

		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * SetSequence( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence );
		void AddPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event, Real p_time );
		void AddContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event );
		void RemovePonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event );
		void RemoveContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event );

	private:
		void _addPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event, Real p_time );
		void _addContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event );
		int _getTopForContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event );
		int _getTopForPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event );
		int _getTop( int p_left, int p_width );
		void _showContextMenu( const wxPoint & p_pos );
		
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
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * m_sequence;
		DimensionMap m_addedPanelsDimensions;
		int m_maxTop;
	};
}
END_TROLL_GUI_TIME_NAMESPACE

#endif
