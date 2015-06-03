#ifndef ___ElypseFrame___
#define ___ElypseFrame___

#include <wx/frame.h>

#include "Module_GUI.h"

namespace Troll
{
	namespace GUI
	{
		class ElypseFrame
			: public wxFrame
		{
		private:
			ElypseCtrl * m_elypseCtrl;
		public:
			ElypseFrame( wxWindow * p_parent, const wxPoint & p_position, const wxSize & p_size )
				:	wxFrame( p_parent, wxID_ANY, _( "Elypse Window" ), p_position, p_size, wxWANTS_CHARS | wxSYSTEM_MENU | wxCAPTION )
				,	m_elypseCtrl( NULL )
			{
			}
			~ElypseFrame()
			{
			}

			void OnSetFocus( wxFocusEvent & p_event );
			void OnKillFocus( wxFocusEvent & p_event );
			void OnShow( wxShowEvent & p_event );
			void OnActivate( wxActivateEvent & p_event );
			void _onIconise( wxIconizeEvent & p_event );
			void _onMouseLeftDown( wxMouseEvent & p_event );
			void _onMouseLeftUp( wxMouseEvent & p_event );
			void _onMouseMiddleDown( wxMouseEvent & p_event );
			void _onMouseMiddleUp( wxMouseEvent & p_event );
			void _onMouseRightDown( wxMouseEvent & p_event );
			void _onMouseRightUp( wxMouseEvent & p_event );
			void _onMouseWheel( wxMouseEvent & p_event );
			void _onMouseMove( wxMouseEvent & p_event );
			void _onMouseEnter( wxMouseEvent & p_event );
			void _onMouseLeave( wxMouseEvent & p_event );
			void _onKeyDown( wxKeyEvent & p_event );
			void _onKeyUp( wxKeyEvent & p_event );
			void _onChar( wxKeyEvent & p_event );

			DECLARE_EVENT_TABLE();
		};
	}
}

#endif

