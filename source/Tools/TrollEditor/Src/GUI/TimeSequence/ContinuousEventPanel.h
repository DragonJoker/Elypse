
#ifndef ___Troll_ContinuousEventPanel___
#define ___Troll_ContinuousEventPanel___

#include <wx/panel.h>

#include "GUI/Module_GUI.h"
#include "Project/Temporal/Module_Temporal.h"

namespace Troll
{
	namespace GUI
	{
		class ContinuousEventPanel
			: public wxPanel
		{
		protected:
			Temporal::TrollContinuousEvent * m_event;
			ArrowPanel * m_arrow;
			Real m_time;
			bool m_mouseLeftDown;
			bool m_hasMoved;
			int m_top;

		public:
			ContinuousEventPanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
								  const wxSize & p_size = wxDefaultSize );
			~ContinuousEventPanel();

			void SetContinuousEvent( Temporal::TrollContinuousEvent * p_event );
			void Replace( bool p_init = false );
			void SetTop( int p_top );

		private:
			void _showContextMenu( const wxPoint & p_pos );
			void _release( bool p_timeChanged );

			void _onLeftMouseDown( wxMouseEvent & p_event );
			void _onLeftMouseUp( wxMouseEvent & p_event );
			void _onRightMouseUp( wxMouseEvent & p_event );
			void _onMouseEnter( wxMouseEvent & p_event );
			void _onMouseLeave( wxMouseEvent & p_event );
			void _onMouseMove( wxMouseEvent & p_event );
			void _onNewFrame( wxCommandEvent & p_event );
			void _onDeleteEvent( wxCommandEvent & p_event );
			void _onKillFocus( wxFocusEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

