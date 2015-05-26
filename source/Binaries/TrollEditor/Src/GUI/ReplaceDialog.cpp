//***********************************************************************************************************
#include "PrecompiledHeader.h"
#include "GUI/ReplaceDialog.h"

#include "GUI/TextPanel.h"
//***********************************************************************************************************
using namespace Troll::GUI;
//***********************************************************************************************************
enum ReplaceDialogIDs
{
	rdTextToReplace,
	rdReplacementText,
	rdFind,
	rdReplace,
	rdReplaceAll
};
//***********************************************************************************************************
BEGIN_EVENT_TABLE( ReplaceDialog, wxFrame )
	EVT_BUTTON( rdFind,			ReplaceDialog::_onFind )
	EVT_BUTTON( rdReplace,		ReplaceDialog::_onReplace )
	EVT_BUTTON( rdReplaceAll,	ReplaceDialog::_onReplaceAll )
END_EVENT_TABLE()
//***********************************************************************************************************

ReplaceDialog :: ReplaceDialog( wxWindow * p_parent, TextPanel * p_textPanel )
	:	wxFrame( p_parent, wxID_ANY, wxT( "Remplacer..." ), wxPoint( 100, 100 ), wxSize( 200, 160 ),
				 wxDEFAULT_FRAME_STYLE | wxFRAME_FLOAT_ON_PARENT ),
	m_textPanel( p_textPanel ),
	m_first( true )
{
	wxPoint l_position( 10, 5 );
	wxSize l_size( GetClientSize().x - 20, 20 );
	wxPanel * l_panel = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), GetClientSize() );
	wxStaticText * l_stTxt = new wxStaticText( l_panel, wxID_ANY, wxT( "Trouver le texte..." ), l_position );
	m_textToReplace = new wxTextCtrl( l_panel, rdTextToReplace, wxEmptyString, l_position += wxPoint( 0, 15 ), l_size, wxBORDER_SIMPLE );
	l_stTxt = new wxStaticText( l_panel, wxID_ANY, wxT( "Remplacer par..." ), l_position += wxPoint( 0, 25 ) );
	m_replacementText = new wxTextCtrl( l_panel, rdReplacementText, wxEmptyString, l_position += wxPoint( 0, 15 ), l_size, wxBORDER_SIMPLE );
	m_find = new wxButton( l_panel, rdFind, wxT( "Suivant" ), l_position += wxPoint( 0, 25 ), wxSize( 85, 20 ), wxBORDER_SIMPLE );
	m_replace = new wxButton( l_panel, rdReplace, wxT( "Remplacer" ), l_position += wxPoint( 95, 0 ), wxSize( 85, 20 ), wxBORDER_SIMPLE );
	m_replaceAll = new wxButton( l_panel, rdReplaceAll, wxT( "Remplacer Tous" ), l_position += wxPoint( 0, 25 ), wxSize( 85, 20 ), wxBORDER_SIMPLE );
}

//***********************************************************************************************************

void ReplaceDialog :: _onFind( wxCommandEvent & p_event )
{
	wxString l_text = m_textToReplace->GetValue();

	if ( l_text.empty() )
	{
		return;
	}

	if ( m_first || l_text != m_text )
	{
		m_textPanel->Search( l_text );
		m_first = false;
		m_text = l_text;
	}
	else
	{
		m_textPanel->SearchNext();
	}
}

//***********************************************************************************************************

void ReplaceDialog :: _onReplace( wxCommandEvent & p_event )
{
	if ( m_text.empty() )
	{
		return;
	}

	if ( m_first )
	{
		m_text = m_textToReplace->GetValue();
		m_textPanel->Search( m_text );
		m_first = false;
	}
	else
	{
		m_textPanel->Replace( m_replacementText->GetValue() );
		m_textPanel->SearchNext();
	}
}

//***********************************************************************************************************

void ReplaceDialog :: _onReplaceAll( wxCommandEvent & p_event )
{
	wxString l_text = m_textToReplace->GetValue();

	if ( l_text.empty() )
	{
		return;
	}

	wxString l_rtxt = m_replacementText->GetValue();

	if ( m_textPanel->Search( l_text ) )
	{
		m_textPanel->Replace( l_rtxt );

		while ( m_textPanel->SearchNext() )
		{
			m_textPanel->Replace( l_rtxt );
		}
	}
}

//***********************************************************************************************************
