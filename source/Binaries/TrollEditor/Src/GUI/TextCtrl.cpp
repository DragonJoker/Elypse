
#include "PrecompiledHeader.h"

#include "GUI/TextCtrl.h"
#include "GUI/TextPanel.h"
#include "GUI/TextLinesCtrl.h"
#include "GUI/MainFrame.h"
#include "Project/Scene.h"
#include "Project/Project.h"

using namespace Troll::GUI;
using namespace Troll;

BEGIN_EVENT_TABLE( TextCtrl, wxTextCtrl )
	EVT_KEY_UP(	TextCtrl::_onKeyUp )
	EVT_KEY_DOWN(	TextCtrl::_onKeyDown )
//	EVT_CHAR(					TextCtrl::_onEventText)
	EVT_MOUSEWHEEL(	TextCtrl::_onMouseWheel )
	EVT_SCROLLWIN_TOP(	TextCtrl::_onScrollToTop )
	EVT_SCROLLWIN_BOTTOM(	TextCtrl::_onScrollToBottom )
	EVT_SCROLLWIN_LINEUP(	TextCtrl::_onScrollLineUp )
	EVT_SCROLLWIN_LINEDOWN(	TextCtrl::_onScrollLineDown )
	EVT_SCROLLWIN_PAGEUP(	TextCtrl::_onScrollPageUp )
	EVT_SCROLLWIN_PAGEDOWN(	TextCtrl::_onScrollPageDown )
	EVT_SCROLLWIN_THUMBTRACK(	TextCtrl::_onScrollThumbTrack )
	EVT_SCROLLWIN_THUMBRELEASE(	TextCtrl::_onScrollThumbRelease )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


TextCtrl :: TextCtrl( wxWindow * parent, wxWindowID id, const wxString & value,
					  const wxPoint & pos, const wxSize & size, long style,
					  TrollFile * p_file )
	:	wxTextCtrl( parent, id, value, pos, size, style ),
		m_posDeb( 0 ),
		m_posFin( 0 ),
		m_file( p_file ),
		m_linesNumber( 0 ),
		m_scrollByWheel( false ),
		m_difference( 0 ),
		m_precPosition( 0 ),
		m_modified( false )
{
	m_panel = reinterpret_cast <TextPanel *>( GetParent()->GetParent() );
}



TextCtrl :: ~TextCtrl()
{
//	std::cout << "TextCtrl :: ~TextCtrl - " << m_file << " - Modified : " << m_modified << " - Saved : " << m_file->Saved << "\n";
	if ( m_file != NULL && ( m_modified || ! m_file->Saved ) )
	{
		if ( wxMessageBox( wxT( "Enregistrer les modifications?" ), m_file->FileName, wxYES_NO, this ) == wxYES )
		{
			g_mainFrame->SaveFile( m_file, m_file->FileName );
		}

		m_file->EditPanel = NULL;
	}
}



void TextCtrl :: AutoWord()
{
	/*
	std::map <wxString, wxString>::const_iterator l_it, l_end;
	l_it = g_mainFrame->m_map.find( g_mainFrame->m_listBox->GetStringSelection());
	l_end = g_mainFrame->m_map.end();

	if (l_it != l_end)
	{
		wxString l_itemDesc = l_it->second;

		size_t l_lastNull;
		wxString l_ajout = "";

		while (l_itemDesc.Contains(",null"))
		{
			l_lastNull = l_itemDesc.find(",null");
			l_itemDesc = l_itemDesc.replace( l_lastNull, 5, "");
		}

		if ( l_itemDesc.Contains("null"))
		{
			l_lastNull = l_itemDesc.find("null");
			l_itemDesc = l_itemDesc.replace( l_lastNull, 4, "");
		}

		while( l_itemDesc.Contains(",,"))
		{
			l_lastNull = l_itemDesc.find(",,");
			l_itemDesc = l_itemDesc.replace( l_lastNull, 2, ",");
		}

		if ( l_itemDesc.Contains(",)"))
		{
			l_lastNull = l_itemDesc.find(",)");
			l_itemDesc = l_itemDesc.replace( l_lastNull, 2, ")");
		}

		if ( l_itemDesc.Contains("code)"))
		{
			l_ajout = "{\r\n\tcode;\r\n}\r\n";
		}

		Replace( m_posDeb, m_posFin + 1, l_itemDesc + l_ajout);
		g_mainFrame->m_listBox->Hide();
		SetFocus();
	}
	*/
}



void TextCtrl :: _onMouseWheel( wxMouseEvent & p_event )
{
	m_scrollByWheel = true;

	if ( p_event.GetWheelRotation() < 0 )
	{
		reinterpret_cast <TextPanel *>( GetParent()->GetParent() )->ScrollLines( 3 );
	}
	else
	{
		reinterpret_cast <TextPanel *>( GetParent()->GetParent() )->ScrollLines( -3 );
	}

	m_scrollByWheel = false;
}



void TextCtrl :: SaveFile( const wxString & p_path, int p_fileType )
{
	m_modified = false;
	m_file->Saved = true;
//	std::cout << "TextCtrl :: SaveFile - " << p_path << " - Saved : " << (m_file->Saved ? "true" : "false") << "\n";
//	std::cout << "TextCtrl :: ~SaveFile - " << m_file << " - Modified : " << m_modified << " - Saved : " << m_file->Saved << "\n";
	wxTextCtrl::SaveFile( p_path, p_fileType );
}



void TextCtrl :: GoToLine( int p_line )
{
	int l_currentLine = 1;
	SetInsertionPoint( 0 );
	wxString l_txt = GetValue();
	long l_currentPosition = 0;

	while ( l_currentLine < p_line )
	{
		while ( l_currentPosition < GetLastPosition() && l_txt.at( l_currentPosition ) != '\n' )
		{
			l_currentPosition++;
		}

		if ( l_currentPosition < GetLastPosition() )
		{
			l_currentLine++;
		}
		else
		{
			break;
		}
	}

	if ( l_currentPosition < GetLastPosition() )
	{
//		std::cout << "TextCtrl :: GoToLine - Wanted : " << p_line << " - Retrieved : " << l_currentLine << " - Cursor : " << l_currentPosition << "\n";
		SetInsertionPoint( l_currentPosition );
	}
}



void TextCtrl :: CompleteCode( int p_keyCode )
{
	/*
	if (m_file->FileName.find( ".emscript") == wxString::npos)
	{
		return;
	}

	wxString Tex;
	wxString theText;
	wxString key;
	wxSize autoSize( 180, 120);

	switch (p_keyCode)
	{
		case WXK_RETURN:
			{
				m_posDeb = GetInsertionPoint();
				g_mainFrame->m_listBox->Hide();
				return;
			}
			break;

		case WXK_TAB:
			{
				m_posDeb = GetInsertionPoint() + 1;
				g_mainFrame->m_listBox->Hide();
				return;
			}
			break;

		case WXK_SPACE:
			{
				m_posDeb = GetInsertionPoint() + 1;
				m_posFin = GetInsertionPoint() + 1;
				g_mainFrame->m_listBox->Hide();
				return;
			}
			break;

		case WXK_DOWN:
			{
				std::cout << GetInsertionPoint() << std::endl;
				if ( g_mainFrame->m_listBox->IsShown())
				{
					g_mainFrame->m_listBox->SetFocus();
					//moche mais ça marche
					keybd_event( VK_DOWN,0,1,0);
					keybd_event( VK_DOWN,0,2,0);
				}
				return;
			}
			break;
	}

	wxString cha = cha.FromAscii( p_keyCode);

	g_mainFrame->m_choices.Clear();
	m_posFin = GetInsertionPoint();
	theText = GetRange( m_posDeb, m_posFin);

	theText = theText+cha ;

	if (theText.Length()>0)
	{
		wxString l_upperBoundString = theText;
		l_upperBoundString[l_upperBoundString.size() - 1] ++;

		MapType::iterator ifind = g_mainFrame->m_map.lower_bound( theText);
		MapType::iterator ifind2 = g_mainFrame->m_map.lower_bound( l_upperBoundString);

		for( ; ifind != ifind2 ; ifind ++)
		{
			ifind->second;
			key = ( *ifind).first;
			g_mainFrame->m_choices.Add( key,1);
		}
		if (! g_mainFrame->m_choices.empty() )
		{
			GetCaretPos( & m_point);
			wxPoint p;
			p.x = m_point.x;
			p.y = m_point.y;

			p.x = p.x + g_mainFrame->m_treesWidth;
			p.y = p.y + 22 + 16;

			g_mainFrame->m_listBox->SetPosition( p);
			g_mainFrame->m_listBox->Clear();
			g_mainFrame->m_listBox->Append( g_mainFrame->m_choices);
			g_mainFrame->m_listBox->SetSize( autoSize);
			g_mainFrame->m_listBox->Show();

			return;
		}
		else
		{
			g_mainFrame->m_listBox->Hide();
		}
	}
	else
	{
		g_mainFrame->m_listBox->Hide();
	}
	*/
}



void TextCtrl :: _onKeyDown( wxKeyEvent & p_event )
{
	switch ( p_event.GetKeyCode() )
	{
	case WXK_PAGEUP :
		p_event.Skip();
		PageUp();
		m_lines->PageUp();
		m_panel->PageUp();
		break;

	case WXK_PAGEDOWN :
		p_event.Skip();
		PageDown();
		m_lines->PageDown();
		m_panel->PageDown();
		break;

	case WXK_UP :
		p_event.Skip();
		LineUp();
		m_lines->LineUp();
		m_panel->LineUp();
		break;

	case WXK_DOWN :
		p_event.Skip();
		LineDown();
		m_lines->LineDown();
		m_panel->LineDown();
		break;

	case WXK_F1 :
	case WXK_F2 :
	case WXK_F3 :
	case WXK_F4 :
	case WXK_F5 :
	case WXK_F6 :
	case WXK_F7 :
	case WXK_F8 :
	case WXK_F9 :
	case WXK_F10 :
	case WXK_F11 :
	case WXK_F12 :
	case WXK_F13 :
	case WXK_F14 :
	case WXK_F15 :
	case WXK_F16 :
	case WXK_F17 :
	case WXK_F18 :
	case WXK_F19 :
	case WXK_F20 :
	case WXK_F21 :
	case WXK_F22 :
	case WXK_F23 :
	case WXK_F24 :
	case WXK_NUMLOCK :
	case WXK_SCROLL :
	case WXK_SHIFT:
	case WXK_ALT:
	case WXK_CONTROL:
	case WXK_MENU:
	case WXK_PAUSE:
	case WXK_CAPITAL:
	case WXK_END:
	case WXK_HOME:
	case WXK_LEFT:
	case WXK_RIGHT:
	case WXK_SELECT:
	case WXK_PRINT:
	case WXK_EXECUTE:
	case WXK_SNAPSHOT:
	case WXK_INSERT:
		p_event.Skip();
		break;

	case WXK_TAB:
		long l_begin, l_end;
		GetSelection( & l_begin, & l_end );

		if ( l_begin == l_end )
		{
			p_event.Skip();
		}
		else
		{
			long l_column, l_lineBegin, l_lineEnd;
			PositionToXY( l_begin, & l_column, & l_lineBegin );
			PositionToXY( l_end, & l_column, & l_lineEnd );
			wxString l_lineTxt;
			long l_position;
			bool l_beginModified = false;

			for ( long i = l_lineBegin ; i <= l_lineEnd ; i++ )
			{
				l_lineTxt = GetLineText( i );

				if ( ! p_event.ShiftDown() )
				{
					l_lineTxt = wxT( "\t" ) + l_lineTxt;
					l_end++;

					if ( ! l_beginModified )
					{
						l_begin++;
						l_beginModified = true;
					}
				}
				else
				{
					if ( l_lineTxt.at( 0 ) == '\t' )
					{
						l_lineTxt = l_lineTxt.Mid( 1 );
						l_end--;

						if ( ! l_beginModified )
						{
							l_begin--;
							l_beginModified = true;
						}
					}
				}

				l_position = XYToPosition( 0, i );
				Replace( l_position, l_position + GetLineLength( i ), l_lineTxt );
			}

			SetSelection( l_begin, l_end );
		}

		m_modified = true;
		break;

	default:
		p_event.Skip();
		m_modified = true;
	}
}



void TextCtrl :: _onKeyUp( wxKeyEvent & p_event )
{
	p_event.Skip();
	CompleteCode( p_event.GetKeyCode() );

	if ( m_linesNumber != GetNumberOfLines() )
	{
		SetSize( wxSize( GetSize().x, GetNumberOfLines() * GetCharHeight() ) );
		m_panel->UpdateSize();

		if ( m_linesNumber < GetNumberOfLines() )
		{
			m_lines->AddLines( GetNumberOfLines() - m_linesNumber );

			do
			{
				++m_linesNumber;
				m_panel->LineDown();
			}
			while ( m_linesNumber < GetNumberOfLines() );
		}
		else if ( m_linesNumber > GetNumberOfLines() )
		{
			do
			{
				--m_linesNumber;
				m_lines->RemoveLine();
			}
			while ( m_linesNumber > GetNumberOfLines() );
		}
	}
}



void TextCtrl :: _onScrollToTop( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

//	std::cout << "TextCtrl :: _onScrollToTop\n";
	m_lines->ScrollLines( -m_lines->GetScrollPos( wxVERTICAL ) );
	m_panel->ScrollLines( -m_panel->GetScrollPos( wxVERTICAL ) );
	m_precPosition = 0;
}



void TextCtrl :: _onScrollToBottom( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

//	std::cout << "TextCtrl :: _onScrollToBottom\n";
	m_lines->ScrollLines( m_lines->GetScrollRange( wxVERTICAL ) );
	m_panel->ScrollLines( m_panel->GetScrollRange( wxVERTICAL ) );
	m_precPosition = GetScrollRange( wxVERTICAL );
}



void TextCtrl :: _onScrollLineUp( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

//	std::cout << "TextCtrl :: _onScrollLineUp\n";
	m_lines->LineUp();
	m_precPosition = GetScrollPos( wxVERTICAL );
}



void TextCtrl :: _onScrollLineDown( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

//	std::cout << "TextCtrl :: _onScrollLineDown\n";
	m_lines->LineDown();
	m_precPosition = GetScrollPos( wxVERTICAL );
}



void TextCtrl :: _onScrollPageUp( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

//	std::cout << "TextCtrl :: _onScrollPageUp\n";
	m_lines->PageUp();
	m_precPosition = GetScrollPos( wxVERTICAL );
}



void TextCtrl :: _onScrollPageDown( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

//	std::cout << "TextCtrl :: _onScrollPageDown\n";
	m_lines->PageDown();
	m_precPosition = GetScrollPos( wxVERTICAL );
}



void TextCtrl :: _onScrollThumbTrack( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}

	int l_currentPosition = p_event.GetPosition();
	m_difference += ( l_currentPosition - m_precPosition );
	m_precPosition = l_currentPosition;
	float l_ratio = ( ( float )( GetScrollRange( wxVERTICAL ) - GetScrollThumb( wxVERTICAL ) ) ) / ( ( float )GetNumberOfLines() );

//	std::cout << "TextCtrl :: _onScrollThumbTrack - Difference : " << m_difference << "\n";
	while ( abs( m_difference ) >= l_ratio )
	{
		if ( m_difference < 0 )
		{
			m_lines->ScrollLines( -1 );
			m_difference += l_ratio;
		}
		else
		{
			m_lines->ScrollLines( 1 );
			m_difference -= l_ratio;
		}
	}
}



void TextCtrl :: _onScrollThumbRelease( wxScrollWinEvent & p_event )
{
	p_event.Skip();

	if ( m_scrollByWheel )
	{
		return;
	}
}


