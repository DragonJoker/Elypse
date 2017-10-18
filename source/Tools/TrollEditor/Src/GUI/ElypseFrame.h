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

