/*
See LICENSE file in root folder
*/
#ifndef ___ElypseFrame___
#define ___ElypseFrame___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/frame.h>

namespace Troll
{
	namespace GUI
	{
		class ElypseFrame
			: public wxFrame
		{
		public:
			ElypseFrame( wxWindow * p_parent, wxPoint const & p_position, wxSize const & p_size );
			virtual ~ElypseFrame();

		private:
			DECLARE_EVENT_TABLE();
			void OnSetFocus( wxFocusEvent & p_event );
			void OnKillFocus( wxFocusEvent & p_event );
			void OnShow( wxShowEvent & p_event );
			void OnActivate( wxActivateEvent & p_event );
			void OnIconise( wxIconizeEvent & p_event );
			void OnMouseLeftDown( wxMouseEvent & p_event );
			void OnMouseLeftUp( wxMouseEvent & p_event );
			void OnMouseMiddleDown( wxMouseEvent & p_event );
			void OnMouseMiddleUp( wxMouseEvent & p_event );
			void OnMouseRightDown( wxMouseEvent & p_event );
			void OnMouseRightUp( wxMouseEvent & p_event );
			void OnMouseWheel( wxMouseEvent & p_event );
			void OnMouseMove( wxMouseEvent & p_event );
			void OnMouseEnter( wxMouseEvent & p_event );
			void OnMouseLeave( wxMouseEvent & p_event );
			void OnKeyDown( wxKeyEvent & p_event );
			void OnKeyUp( wxKeyEvent & p_event );
			void OnChar( wxKeyEvent & p_event );

		private:
			ElypseCtrl * m_elypseCtrl;
		};
	}
}

#endif

