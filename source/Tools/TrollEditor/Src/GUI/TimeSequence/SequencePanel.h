/*
See LICENSE file in root folder
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
