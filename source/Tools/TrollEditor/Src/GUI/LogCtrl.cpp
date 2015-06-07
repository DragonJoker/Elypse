#include "PrecompiledHeader.h"

#include "LogCtrl.h"

#include "MainFrame.h"
#include "TextPanel.h"
#include "TextCtrl.h"
#include "Project/Scene.h"

using namespace Troll::GUI;
using namespace Troll;

BEGIN_EVENT_TABLE( LogCtrl, wxListBox )
	EVT_LEFT_DCLICK( LogCtrl::OnDoubleClick )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


LogCtrl::LogCtrl( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style )
	: wxListBox( parent, id, pos, size, wxArrayString(), style )
{
}



void LogCtrl::OnDoubleClick( wxMouseEvent & p_event )
{
	/*
		long x;
		long y;
		PositionToXY( GetInsertionPoint(), & x, & y);
		wxString txt = GetLineText( y);
		TrollFile * l_file ;

		if(txt.Contains( ".emscript"))
		{
			size_t csize = txt.find( "[");
			size_t fsize = txt.find( ".emscript");
			txt = txt.substr( csize + 1, (fsize - csize + 8) );
			std::cout << "LogCtrl::_onDoubleClick - " << txt << "\n";

			l_file = g_mainFrame->m_filesList->GetSelectedScene()->GetFile( g_mainFrame->m_filesList->GetItemByName( txt));

		}

		txt = GetLineText( y);
		if (txt.Contains( "L#"))
		{
			size_t csize = txt.find( "L#");
			size_t fsize = txt.find( "]");
			txt = txt.substr( csize + 3, (fsize - csize - 4) );

			g_mainFrame->m_mainTabsContainer->SetSelection( l_file->IdOnglet);
			std::cout << "LogCtrl::_onDoubleClick - " << l_file->IdOnglet << "\n";
			l_file->EditPanel->SetFocus();
			l_file->EditPanel->GetText()->SetInsertionPoint( l_file->EditPanel->GetText()->XYToPosition( 0, atol( txt) - 1));
		}
	*/
	p_event.Skip();
}


