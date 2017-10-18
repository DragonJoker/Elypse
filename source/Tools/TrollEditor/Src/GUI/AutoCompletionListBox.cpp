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

#include "GUI/AutoCompletionListBox.h"

#include "GUI/MainFrame.h"
#include "GUI/StcTextEditor.h"

namespace Troll
{
	namespace GUI
	{
		BEGIN_EVENT_TABLE( AutoCompletionListBox, wxListBox )
			EVT_LEFT_DCLICK( AutoCompletionListBox::OnMouseLeftDown )
			EVT_KEY_DOWN( AutoCompletionListBox::OnKeyDown )
		END_EVENT_TABLE()

		AutoCompletionListBox::AutoCompletionListBox( wxWindow * parent, wxWindowID id, wxPoint const & pos, wxSize const & size, const wxArrayString & choices )
			: wxListBox( parent, id, pos, size, choices, wxLB_ALWAYS_SB | wxWANTS_CHARS )
		{
		}

		void AutoCompletionListBox::OnMouseLeftDown( wxMouseEvent & p_event )
		{
			//wxGetApp().GetMainFrame()->m_editText->GetText()->AutoWord();
		}

		void AutoCompletionListBox::OnKeyDown( wxKeyEvent & p_event )
		{
			int l_key = p_event.GetKeyCode();

			switch ( p_event.GetKeyCode() )
			{
			case WXK_RIGHT:
			{
				//wxGetApp().GetMainFrame()->m_editText->GetText()->AutoWord();
				return;
			}
			break;

			case WXK_RETURN:
			{
				//wxGetApp().GetMainFrame()->m_editText->GetText()->AutoWord();
				return;
			}
			break;

			case WXK_ESCAPE:
			{
				//wxGetApp().GetMainFrame()->m_listBox->Hide();
				//wxGetApp().GetMainFrame()->m_editText->GetText()->SetFocus();
				return;
			}
			break;
			}

			p_event.Skip();
		}
	}
}
