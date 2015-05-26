//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/TextPanel.h"
#include "GUI/TextLinesCtrl.h"
#include "GUI/MainFrame.h"
#include "Project/Scene.h"
#include "Project/Project.h"
//***********************************************************************************************************
using namespace Troll::GUI;
using namespace Troll;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( TextPanel, wxScrolledWindow )
	EVT_SIZE(	TextPanel::_onResize )
END_EVENT_TABLE()
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************

TextPanel :: TextPanel( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size,
						long style, TrollFile * p_file, bool p_showLines )
	:	wxScrolledWindow( parent, id, pos, size, style ),
		m_showLines( p_showLines ),
		m_file( p_file )
{
	m_container = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( size.x * 2, size.y ) );
	int x = m_container->GetClientSize().x - 40;
	int y = m_container->GetClientSize().y;

	if ( p_showLines )
	{
		m_text = new TextCtrl( m_container, wxID_ANY, wxT( "" ), wxPoint( 40, 0 ), wxSize( x, y ),
							   wxTE_MULTILINE | wxTE_RICH | wxTE_PROCESS_TAB, p_file );
	}
	else
	{
		m_text = new TextCtrl( m_container, wxID_ANY, wxT( "" ), wxPoint( 0, 0 ), wxSize( m_container->GetClientSize().x, y ),
							   wxTE_MULTILINE | wxTE_RICH | wxTE_PROCESS_TAB, p_file );
	}

	m_text->SetFont( wxFont( 10, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT( "Lucida Console" ) ) );
	wxString l_path = g_mainFrame->GetCurrentProject()->GetPath() + ( p_file->m_scene->IsMainScene() ? wxString() : wxString( p_file->m_scene->GetName() ) + wxT( "/" ) );
	m_text->LoadFile( l_path + p_file->FileName );
	m_text->SetLinesNumber( m_text->GetNumberOfLines() );

	if ( p_showLines )
	{
		m_text->SetSize( x * 2, ( m_text->GetNumberOfLines() ) * m_text->GetCharHeight() );
	}
	else
	{
		m_text->SetSize( m_container->GetClientSize().x * 2, ( m_text->GetNumberOfLines() ) * m_text->GetCharHeight() );
	}

	m_lines = new TextLinesCtrl( m_container, wxID_ANY, wxPoint( 0, 0 ), wxSize( 40, 0 ),
								 wxLC_LIST | wxLC_SINGLE_SEL | wxLC_ALIGN_LEFT );
//	m_lines->Enable( false);
	m_lines->AddLines( m_text->GetNumberOfLines() );

	for ( int i = 0 ; i <= m_text->GetNumberOfLines() ; i++ )
	{
		m_lines->LineUp();
	}

	if ( ! p_showLines )
	{
		m_lines->Hide();
	}

	m_text->SetLines( m_lines );
	UpdateSize();
}

//***********************************************************************************************************

void TextPanel :: WriteText( const wxString & p_text )
{
	m_text->WriteText( p_text );
}

//***********************************************************************************************************

void TextPanel :: SaveFile( const wxString & p_path )
{
	m_text->SaveFile( p_path );
}

//***********************************************************************************************************

void TextPanel :: UpdateSize()
{
	wxSize l_size( m_text->GetPosition().x + m_text->GetSize().x,
				   m_text->GetNumberOfLines() * m_text->GetCharHeight() );
	m_container->SetSize( l_size );
	UpdateScrollBars();
	/*
		int l_hScrollPos = GetScrollPos( wxHORIZONTAL);
		int l_vScrollPos = GetScrollPos( wxVERTICAL);
		Scroll( 0, 0);
		wxSize l_size( m_text->GetPosition().x + m_text->GetSize().x,
					  (m_text->GetNumberOfLines() + 2) * m_text->GetCharHeight());
		m_container->SetSize( l_size);
		SetScrollbars( 20, 20, l_size.x / 20, l_size.y / 20, 0, 0);
		Scroll( l_hScrollPos, l_vScrollPos);
	*/
}

//***********************************************************************************************************

void TextPanel :: UpdateScrollBars()
{
	int l_hScrollPos = GetScrollPos( wxHORIZONTAL );
	int l_vScrollPos = GetScrollPos( wxVERTICAL );
	int l_hRange = ( m_text->GetPosition().x + m_text->GetSize().x ) / m_text->GetCharWidth();
	int l_vRange = m_text->GetNumberOfLines();
	int l_hThumbSize = GetClientSize().x / m_text->GetCharWidth();
	int l_vThumbSize = GetClientSize().y / m_text->GetCharHeight();
	/*
		SetScrollbar( wxHORIZONTAL, l_hScrollPos, l_hThumbSize, l_hRange);
		SetScrollbar( wxVERTICAL, l_vScrollPos, l_vThumbSize, l_vRange);
		SetScrollRate( m_text->GetCharWidth(), m_text->GetCharHeight());
	*/
	SetScrollbars( m_text->GetCharWidth(), m_text->GetCharHeight(),
				   m_container->GetSize().x / m_text->GetCharWidth(), m_container->GetSize().y / m_text->GetCharHeight(),
				   GetScrollPos( wxHORIZONTAL ), GetScrollPos( wxVERTICAL ) );
//	std::cout << "TextPanel :: UpdateSize - Range : " << GetScrollRange( wxVERTICAL) << "\n";
}

//***********************************************************************************************************

bool TextPanel :: IsModified()
{
	return m_text->IsModified();
}

//***********************************************************************************************************

void TextPanel :: GoToLine( int p_line )
{
	ScrollLines( p_line - GetScrollPos( wxVERTICAL ) - 10 );
	m_lines->GoToLine( p_line );
	m_text->GoToLine( p_line );
}

//***********************************************************************************************************

bool TextPanel :: Search( const wxString & p_search )
{
	wxString l_txt = m_text->GetValue();
	m_search = p_search;
	m_currentSearchPosition = 0;
	int l_searchIndex = l_txt.Find( m_search );
	General::Utils::vector::deleteAll( m_searchPositions );

	if ( l_searchIndex != wxNOT_FOUND )
	{
//		std::cout << "TextPanel :: Search - " << l_index << "\n";
		int l_currentLine = 1;
		long l_currentPosition = 0;

		while ( l_currentPosition < l_searchIndex )
		{
//			std::cout << "TextPanel :: Search - " << (int)l_txt.at( l_currentPosition) << "{" << l_txt.at( l_currentPosition) << "}\n";
			if ( l_txt.at( l_currentPosition ) == '\n' )
			{
				l_currentLine++;
			}

			l_currentPosition++;
		}

		l_currentPosition += l_currentLine - 1;
		m_searchPositions.push_back( new SearchInfo( l_currentLine, l_currentPosition, l_searchIndex ) );
//		std::cout << "TextPanel :: Search - Index : " << m_searchIndex << " - Line : " << l_currentLine << " - Cursor : " << l_currentPosition << " To : " << l_currentPosition + p_search.size() << "\n";
		m_text->SetSelection( l_currentPosition, l_currentPosition + p_search.size() );
		m_text->SetFocus();
		ScrollLines( l_currentLine - GetScrollPos( wxVERTICAL ) - 10 );
		return true;
	}

	return false;
}

//***********************************************************************************************************

bool TextPanel :: SearchNext()
{
	if ( m_searchPositions.size() > 0 )
	{
		SearchInfo * l_info = m_searchPositions[m_currentSearchPosition];
		wxString l_txt = m_text->GetValue();
		l_txt = l_txt.Mid( l_info->m_index + m_search.size() );
		int l_index = l_txt.Find( m_search );
		l_txt = m_text->GetValue();

		if ( l_index != wxNOT_FOUND )
		{
			//		std::cout << "TextPanel :: Search - " << l_index << "\n";
			int l_currentLine = l_info->m_line;
			long l_currentPosition = l_info->m_position - l_currentLine + 1;
			int l_searchIndex = l_info->m_index + m_search.size() + l_index;

			while ( l_currentPosition < l_searchIndex )
			{
				//			std::cout << "TextPanel :: Search - " << (int)l_txt.at( l_currentPosition) << "{" << l_txt.at( l_currentPosition) << "}\n";
				if ( l_txt.at( l_currentPosition ) == '\n' )
				{
					l_currentLine++;
				}

				l_currentPosition++;
			}

			l_currentPosition += l_currentLine - 1;
			m_currentSearchPosition = m_searchPositions.size();
			m_searchPositions.push_back( new SearchInfo( l_currentLine, l_currentPosition, l_searchIndex ) );
			//		std::cout << "TextPanel :: Search - Index : " << m_searchIndex << " - Line : " << l_currentLine << " - Cursor : " << l_currentPosition << " To : " << l_currentPosition + p_search.size() << "\n";
			m_text->SetSelection( l_currentPosition, l_currentPosition + m_search.size() );
			m_text->SetFocus();
			ScrollLines( l_currentLine - GetScrollPos( wxVERTICAL ) - 10 );
			return true;
		}
	}

	return false;
}

//***********************************************************************************************************

bool TextPanel :: SearchPrevious()
{
	if ( m_searchPositions.size() > 0 && m_currentSearchPosition > 0 )
	{
		delete m_searchPositions[m_currentSearchPosition];
		General::Utils::vector::eraseValue( m_searchPositions, m_searchPositions[m_currentSearchPosition] );
		m_currentSearchPosition--;
		SearchInfo * l_info = m_searchPositions[m_currentSearchPosition];
		m_text->SetSelection( l_info->m_position, l_info->m_position + m_search.size() );
		m_text->SetFocus();
		ScrollLines( l_info->m_line - GetScrollPos( wxVERTICAL ) - 10 );
		return true;
	}

	return false;
}

//***********************************************************************************************************

void TextPanel :: Replace( const wxString & p_replacement )
{
	if ( m_searchPositions.size() == 0 )
	{
		return;
	}

	SearchInfo * l_info = m_searchPositions[m_currentSearchPosition];
	m_text->Replace( l_info->m_position, l_info->m_position + m_search.size(), p_replacement );
}

//***********************************************************************************************************

void TextPanel :: _onResize( wxSizeEvent & p_event )
{
	SetScrollbars( m_text->GetCharWidth(), m_text->GetCharHeight(),
				   m_container->GetSize().x / m_text->GetCharWidth(), m_container->GetSize().y / m_text->GetCharHeight(),
				   GetScrollPos( wxHORIZONTAL ), GetScrollPos( wxVERTICAL ) );
}

//***********************************************************************************************************

void TextPanel :: _onSetFocus( wxFocusEvent & p_event )
{
	m_text->SetFocus();
}

//***********************************************************************************************************
