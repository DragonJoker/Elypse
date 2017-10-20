/*
See LICENSE file in root folder
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
