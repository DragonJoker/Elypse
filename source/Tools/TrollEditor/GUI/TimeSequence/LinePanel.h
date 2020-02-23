/*
See LICENSE file in root folder
*/
#ifndef ___Troll_LinePanel___
#define ___Troll_LinePanel___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class LinePanel
				: public wxPanel
			{
			public:
				LinePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~LinePanel();

				void Replace();

				void SetSequence( ProjectComponents::Temporal::TrollSequence * p_sequence );
				void AddPonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event, Real p_time );
				void AddContinuousEvent( ProjectComponents::Temporal::TrollContinuousEvent * p_event );

				inline ProjectComponents::Temporal::TrollSequence * GetSequence()const
				{
					return m_sequence;
				}

			private:
				void _showContextMenu( wxPoint const & p_pos );

				DECLARE_EVENT_TABLE()
				void OnAddPonctualEvent( wxCommandEvent & p_event );
				void OnAddContinuousEvent( wxCommandEvent & p_event );
				void OnLeftMouseUp( wxMouseEvent & p_event );
				void OnRightMouseUp( wxMouseEvent & p_event );
				void OnMouseEnter( wxMouseEvent & p_event );
				void OnMouseLeave( wxMouseEvent & p_event );

			private:
				ProjectComponents::Temporal::TrollSequence * m_sequence;
				int m_currentLeft;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
