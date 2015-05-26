//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/NewListbox.h"
#include "GUI/MainFrame.h"
#include "GUI/TextPanel.h"
#include "GUI/TextCtrl.h"
#include "GUI/StcTextEditor.hpp"
//***********************************************************************************************************
using namespace Troll::GUI;
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( AutoCompletionListBox, wxListBox )
	EVT_LEFT_DCLICK(	AutoCompletionListBox :: _onMouseLeftDown )
	EVT_KEY_DOWN(	AutoCompletionListBox :: _onKeyDown )
END_EVENT_TABLE()
//***********************************************************************************************************

AutoCompletionListBox :: AutoCompletionListBox( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size,
		const wxArrayString & choices )
	:	wxListBox( parent, id, pos, size, choices, wxLB_ALWAYS_SB | wxWANTS_CHARS )
{
}

//***********************************************************************************************************

void AutoCompletionListBox :: _onMouseLeftDown( wxMouseEvent & p_event )
{
//	g_mainFrame->m_editText->GetText()->AutoWord();
}

//***********************************************************************************************************

void AutoCompletionListBox :: _onKeyDown( wxKeyEvent & p_event )
{
	int l_key = p_event.GetKeyCode();

	switch ( p_event.GetKeyCode() )
	{
	case WXK_RIGHT:
	{
//				g_mainFrame->m_editText->GetText()->AutoWord();
		return;
	}
	break;

	case WXK_RETURN:
	{
//				g_mainFrame->m_editText->GetText()->AutoWord();
		return;
	}
	break;

	case WXK_ESCAPE:
	{
		g_mainFrame->m_listBox->Hide();
//				g_mainFrame->m_editText->GetText()->SetFocus();
		return;
	}
	break;
	}

	p_event.Skip();
}

//***********************************************************************************************************
