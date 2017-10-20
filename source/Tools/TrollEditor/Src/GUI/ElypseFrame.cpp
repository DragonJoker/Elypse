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
#include "TrollEditorPch.h"

#include "GUI/ElypseFrame.h"
#include "GUI/ElypseCtrl.h"

namespace Troll
{
	namespace GUI
	{
		ElypseFrame::ElypseFrame( wxWindow * p_parent, wxPoint const & p_position, wxSize const & p_size )
			: wxFrame( p_parent, wxID_ANY, _( "Elypse Window" ), p_position, p_size, wxWANTS_CHARS | wxSYSTEM_MENU | wxCAPTION )
			, m_elypseCtrl( NULL )
		{
			SetClientSize( p_size );
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );
		}

		ElypseFrame::~ElypseFrame()
		{
		}

		BEGIN_EVENT_TABLE( ElypseFrame, wxFrame )
			EVT_SET_FOCUS( ElypseFrame::OnSetFocus )
			EVT_KILL_FOCUS( ElypseFrame::OnKillFocus )
			EVT_SHOW( ElypseFrame::OnShow )
			EVT_ACTIVATE( ElypseFrame::OnActivate )
			EVT_ICONIZE( ElypseFrame::OnIconise )
			EVT_LEFT_DOWN( ElypseFrame::OnMouseLeftDown )
			EVT_LEFT_UP( ElypseFrame::OnMouseLeftUp )
			EVT_MIDDLE_DOWN( ElypseFrame::OnMouseMiddleDown )
			EVT_MIDDLE_UP( ElypseFrame::OnMouseMiddleUp )
			EVT_RIGHT_DOWN( ElypseFrame::OnMouseRightDown )
			EVT_RIGHT_UP( ElypseFrame::OnMouseRightUp )
			EVT_LEFT_DCLICK( ElypseFrame::OnMouseLeftDown )
			EVT_MIDDLE_DCLICK( ElypseFrame::OnMouseMiddleDown )
			EVT_RIGHT_DCLICK( ElypseFrame::OnMouseRightDown )
			EVT_MOUSEWHEEL( ElypseFrame::OnMouseWheel )
			EVT_MOTION( ElypseFrame::OnMouseMove )
			EVT_KEY_DOWN( ElypseFrame::OnKeyDown )
			EVT_KEY_UP( ElypseFrame::OnKeyUp )
			EVT_CHAR( ElypseFrame::OnChar )
			EVT_ENTER_WINDOW( ElypseFrame::OnMouseEnter )
			EVT_LEAVE_WINDOW( ElypseFrame::OnMouseLeave )
		END_EVENT_TABLE()

		void ElypseFrame::OnSetFocus( wxFocusEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnSetFocus( p_event );
			}
		}

		void ElypseFrame::OnKillFocus( wxFocusEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnKillFocus( p_event );
			}
		}

		void ElypseFrame::OnShow( wxShowEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnShow( p_event );
			}
		}

		void ElypseFrame::OnActivate( wxActivateEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnActivate( p_event );
			}
		}

		void ElypseFrame::OnIconise( wxIconizeEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnIconise( p_event );
			}
		}

		void ElypseFrame::OnMouseLeftDown( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnButtonLeftDown( p_event );
			}
		}

		void ElypseFrame::OnMouseLeftUp( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnButtonLeftUp( p_event );
			}
		}

		void ElypseFrame::OnMouseMiddleDown( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnButtonMiddleDown( p_event );
			}
		}

		void ElypseFrame::OnMouseMiddleUp( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnButtonMiddleUp( p_event );
			}
		}

		void ElypseFrame::OnMouseRightDown( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnButtonRightDown( p_event );
			}
		}

		void ElypseFrame::OnMouseRightUp( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnButtonRightUp( p_event );
			}
		}

		void ElypseFrame::OnMouseWheel( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnMouseWheel( p_event );
			}
		}

		void ElypseFrame::OnMouseMove( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnMouseMove( p_event );
			}
		}

		void ElypseFrame::OnMouseEnter( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnMouseEnter( p_event );
			}
		}

		void ElypseFrame::OnMouseLeave( wxMouseEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnMouseLeave( p_event );
			}
		}

		void ElypseFrame::OnKeyDown( wxKeyEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnKeyDown( p_event );
			}
		}

		void ElypseFrame::OnKeyUp( wxKeyEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnKeyUp( p_event );
			}
		}

		void ElypseFrame::OnChar( wxKeyEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnChar( p_event );
			}
		}
	}
}
