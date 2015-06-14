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
#ifndef ___ELYPSE_CTRL_H___
#define ___ELYPSE_CTRL_H___

#include "ElypseBinaryPrerequisites.h"

#include <Module_Main.h>
#include <OwnedBy.h>

BEGIN_ELYPSE_BINARY_NAMESPACE
{
	class ElypseCtrl
		: public wxControl
		, public owned_by< ElypseApp >
	{
	public:
		ElypseCtrl( ElypseApp * p_owner, wxWindow * p_parent, COMMON_GUI_NAMESPACE::wxElypsePluginSPtr p_plugin, std::shared_ptr< ElypseInstance > p_elypse );
		~ElypseCtrl();

		const String GetWHandle();
			
	private:
		static wxSize GetClientSize( wxWindow * p_window )
		{
			int l_height = 0;
			int l_width = 0;

			p_window->GetClientSize( & l_width, & l_height );

			return wxSize( l_width, l_height );
		}
		inline 	std::shared_ptr< ElypseInstance > GetElypse()
		{
			return m_elypse.lock();
		}

		DECLARE_EVENT_TABLE()
		void OnKeyDown( wxKeyEvent & );
		void OnKeyUp( wxKeyEvent & );
		void OnChar( wxKeyEvent & );
		void OnKillFocus( wxFocusEvent & );
		void OnMouseMove( wxMouseEvent & );
		void OnMouseWheel( wxMouseEvent & );
		void OnMButtonDown( wxMouseEvent & );
		void OnMButtonUp( wxMouseEvent & );
		void OnLButtonDown( wxMouseEvent & );
		void OnLButtonUp( wxMouseEvent & );
		void OnRButtonDown( wxMouseEvent & );
		void OnRButtonUp( wxMouseEvent & );
		void OnClose( wxCloseEvent & );
		void OnPaint( wxPaintEvent & p_event );
		void OnEraseBackground( wxEraseEvent & p_event );
		void OnRenderTimer( wxTimerEvent & p_event );

	private:
		std::weak_ptr< ElypseInstance > m_elypse;
		COMMON_GUI_NAMESPACE::wxElypsePluginWPtr m_plugin;

		Real m_width;
		Real m_height;

		bool m_rightButton;
		bool m_leftButton;
		bool m_middleButton;
	};
}
END_ELYPSE_BINARY_NAMESPACE

#endif
