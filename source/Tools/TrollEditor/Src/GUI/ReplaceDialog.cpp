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

#include "GUI/ReplaceDialog.h"

#include "GUI/StcTextEditor.h"

namespace Troll
{
	namespace GUI
	{
		enum ReplaceDialogIDs
		{
			rdTextToReplace,
			rdReplacementText,
			rdFind,
			rdReplace,
			rdReplaceAll
		};

		BEGIN_EVENT_TABLE( ReplaceDialog, wxFrame )
			EVT_BUTTON( rdFind, ReplaceDialog::OnFind )
			EVT_BUTTON( rdReplace, ReplaceDialog::OnReplace )
			EVT_BUTTON( rdReplaceAll, ReplaceDialog::OnReplaceAll )
		END_EVENT_TABLE()

		ReplaceDialog::ReplaceDialog( wxWindow * p_parent, wxStcTextEditor * p_textPanel )
			: wxFrame( p_parent, wxID_ANY, _( "Replace..." ), wxPoint( 100, 100 ), wxSize( 200, 160 ), wxDEFAULT_FRAME_STYLE | wxFRAME_FLOAT_ON_PARENT )
			, m_textPanel( p_textPanel )
			, m_first( true )
		{
			SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			wxPoint l_position( 10, 5 );
			wxSize l_size( GetClientSize().x - 20, 20 );
			wxPanel * l_panel = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), GetClientSize() );
			l_panel->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			l_panel->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			wxStaticText * l_stTxt = new wxStaticText( l_panel, wxID_ANY, _( "Find the text..." ), l_position );
			l_stTxt->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			l_stTxt->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			m_textToReplace = new wxTextCtrl( l_panel, rdTextToReplace, wxEmptyString, l_position += wxPoint( 0, 15 ), l_size, wxBORDER_SIMPLE );
			m_textToReplace->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			m_textToReplace->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			l_stTxt = new wxStaticText( l_panel, wxID_ANY, _( "Replace with..." ), l_position += wxPoint( 0, 25 ) );
			l_stTxt->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			l_stTxt->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			m_replacementText = new wxTextCtrl( l_panel, rdReplacementText, wxEmptyString, l_position += wxPoint( 0, 15 ), l_size, wxBORDER_SIMPLE );
			m_replacementText->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			m_replacementText->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			m_find = new wxButton( l_panel, rdFind, _( "Next" ), l_position += wxPoint( 0, 25 ), wxSize( 85, 20 ), wxBORDER_SIMPLE );
			m_find->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			m_find->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			m_replace = new wxButton( l_panel, rdReplace, _( "Replace" ), l_position += wxPoint( 95, 0 ), wxSize( 85, 20 ), wxBORDER_SIMPLE );
			m_replace->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			m_replace->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			m_replaceAll = new wxButton( l_panel, rdReplaceAll, _( "Replace All" ), l_position += wxPoint( 0, 25 ), wxSize( 85, 20 ), wxBORDER_SIMPLE );
			m_replaceAll->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			m_replaceAll->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
		}

		void ReplaceDialog::OnFind( wxCommandEvent & p_event )
		{
			wxString l_text = m_textToReplace->GetValue();

			if ( l_text.empty() )
			{
				return;
			}

			if ( l_text != m_text )
			{
				m_text = l_text;
			}

			m_textPanel->SearchNext( wxSTC_FIND_MATCHCASE, m_text );
		}

		void ReplaceDialog::OnReplace( wxCommandEvent & p_event )
		{
			if ( m_text.empty() )
			{
				return;
			}

			if ( m_first )
			{
				m_text = m_textToReplace->GetValue();
				m_textPanel->SearchNext( wxSTC_FIND_MATCHCASE, m_text );
				m_first = false;
			}
			else
			{
				//m_textPanel->Replace( m_replacementText->GetValue() );
				m_textPanel->SearchNext( wxSTC_FIND_MATCHCASE, m_text );
			}
		}

		void ReplaceDialog::OnReplaceAll( wxCommandEvent & p_event )
		{
			wxString l_text = m_textToReplace->GetValue();

			if ( l_text.empty() )
			{
				return;
			}

			wxString l_rtxt = m_replacementText->GetValue();

			if ( m_textPanel->SearchNext( wxSTC_FIND_MATCHCASE, l_text ) )
			{
				//m_textPanel->Replace( l_rtxt );

				while ( m_textPanel->SearchNext( wxSTC_FIND_MATCHCASE, l_text ) )
				{
					//m_textPanel->Replace( l_rtxt );
				}
			}
		}
	}
}
