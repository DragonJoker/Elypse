
#ifndef ___Troll_PonctualEventPanel___
#define ___Troll_PonctualEventPanel___

#include <wx/panel.h>

#include "GUI/Module_GUI.h"
#include "Project/Temporal/Module_Temporal.h"

namespace Troll
{
	namespace GUI
	{
		class PonctualEventPanel
			: public wxPanel
		{
		protected:
			Temporal::TrollPonctualEvent * m_event;
			ArrowPanel * m_arrow;
			Real m_time;
			bool m_mouseLeftDown;
			int m_top;

		public:
			PonctualEventPanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
								const wxSize & p_size = wxDefaultSize );
			~PonctualEventPanel();

			void SetPonctualEvent( Temporal::TrollPonctualEvent * p_event, Real p_time );

			void Replace( bool p_init = false );

			void SetTop( int p_top );

		public:
			void SetMouseLeftDown( bool p_leftDown )
			{
				m_mouseLeftDown = p_leftDown;
			}

		private:
			void _release( bool p_timeChanged );
			void _showContextMenu( const wxPoint & p_pos );

			void _onLeftMouseDown( wxMouseEvent & p_event );
			void _onLeftMouseUp( wxMouseEvent & p_event );
			void _onRightMouseUp( wxMouseEvent & p_event );
			void _onMouseEnter( wxMouseEvent & p_event );
			void _onMouseMove( wxMouseEvent & p_event );
			void _onMouseLeave( wxMouseEvent & p_event );
			void _onDeleteEvent( wxCommandEvent & p_event );
			void _onKillFocus( wxFocusEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

