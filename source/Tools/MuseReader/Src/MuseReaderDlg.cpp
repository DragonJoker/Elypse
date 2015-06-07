#include "stdafx.h"

#include "MuseReader.h"
#include "MuseReaderDlg.h"

#include <wx/button.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/stattext.h>

namespace
{
	enum eIDS
	{
		eBTNINFILE,
		eBTNOUTFOLDER,
		eBUTTON2,
	};

	const int OFFSET = 5;
	const int STATIC_HEIGHT = 20;
	const int EDIT_HEIGHT = 25;
	const wxSize BUTTON_SIZE( 75, 25 );
}

CMuseReaderDlg::CMuseReaderDlg( wxWindow * pParent )
	: wxDialog( pParent, wxID_ANY, _( "Muse file reader" ) )
	, m_dataStreamer( NULL )
{
	int l_width = 640;
	int l_height = 480;
	SetClientSize( wxSize( l_width, l_height ) );
	int l_effWidth = l_width - ( OFFSET * 3 );

	int l_leftListWidth = ( l_effWidth / 3 );
	int l_rightListWidth = ( 2 * l_effWidth / 3 );
	int l_listHeight = l_height - ( EDIT_HEIGHT * 2 ) - ( STATIC_HEIGHT * 3 ) - ( OFFSET * 3 );
	int l_left = l_leftListWidth + OFFSET * 2;
	int l_top = OFFSET;

	new wxStaticText( this, wxID_ANY, _( "Read files" ), wxPoint( OFFSET, l_top ), wxSize( l_leftListWidth, STATIC_HEIGHT ) );
	new wxStaticText( this, wxID_ANY, _( "Log" ), wxPoint( l_left, l_top ), wxSize( l_rightListWidth, STATIC_HEIGHT ) );
	l_top += STATIC_HEIGHT;
	m_output = new wxListBox( this, wxID_ANY, wxPoint( OFFSET, l_top ), wxSize( l_leftListWidth, l_listHeight ) );
	m_log = new wxListBox( this, wxID_ANY, wxPoint( l_left, l_top ), wxSize( l_rightListWidth, l_listHeight ) );

	l_left = l_rightListWidth + OFFSET * 2;
	l_top += l_listHeight + OFFSET;
	new wxStaticText( this, wxID_ANY, _( "File to read" ), wxPoint( OFFSET, l_top ), wxSize( l_rightListWidth, STATIC_HEIGHT ) );
	l_top += STATIC_HEIGHT;
	m_inFile = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( OFFSET, l_top ), wxSize( l_rightListWidth, EDIT_HEIGHT ) );
	new wxButton( this, eBTNINFILE, _( "Browse" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_top += EDIT_HEIGHT;
	new wxStaticText( this, wxID_ANY, _( "Output folder" ), wxPoint( OFFSET, l_top ), wxSize( l_rightListWidth, STATIC_HEIGHT ) );
	l_top += STATIC_HEIGHT;
	m_outFolder = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( OFFSET, l_top ), wxSize( l_rightListWidth, EDIT_HEIGHT ) );
	new wxButton( this, eBTNOUTFOLDER, _( "Browse" ), wxPoint( l_left, l_top ), BUTTON_SIZE );

	l_left = l_width - OFFSET - BUTTON_SIZE.x;
	l_top = l_height - ( BUTTON_SIZE.y + OFFSET ) * 2;
	new wxButton( this, eBUTTON2, _( "Read" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_top += BUTTON_SIZE.y + OFFSET;
	new wxButton( this, wxID_CANCEL, _( "Exit" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
}

BEGIN_EVENT_TABLE( CMuseReaderDlg, wxDialog )
	EVT_BUTTON( eBTNINFILE, CMuseReaderDlg::OnBnClickedBtninfile )
	EVT_BUTTON( eBTNOUTFOLDER, CMuseReaderDlg::OnBnClickedBtnoutfolder )
	EVT_BUTTON( eBUTTON2, CMuseReaderDlg::OnBnClickedBtnread )
END_EVENT_TABLE()

void CMuseReaderDlg::OnBnClickedBtninfile( wxCommandEvent & p_event )
{
	if ( !m_dataStreamer )
	{
		m_dataStreamer = std::make_shared< CDataStreamer >( m_log );
	}

	wxFileDialog l_dialog( this, _( "muse" ), wxEmptyString, wxEmptyString, _( "Muse files" ) + wxString( wxT( " (*.muse)|*.muse||" ) ), wxFD_OPEN | wxFD_FILE_MUST_EXIST );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		wxString l_log;
		wxString l_path = l_dialog.GetPath();

		if ( m_dataStreamer->SetFile( wxString( l_path.char_str().data(), wxConvLibc ) ) )
		{
			l_log.Format( _( "Successfully connected to %s" ), l_path );
			m_log->AppendString( l_log );
			m_inFileName = l_path;
			m_inFile->SetLabel( l_path );
		}
		else
		{
			l_log.Format( _( "Couldn't connect to %s" ), l_path );
			m_log->AppendString( l_log );
		}
	}
}

void CMuseReaderDlg::OnBnClickedBtnoutfolder( wxCommandEvent & p_event )
{
	if ( !m_dataStreamer )
	{
		m_dataStreamer = std::make_shared< CDataStreamer >( m_log );
	}

	wxDirDialog l_dialog( this, _( "Choose a folder" ) );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		m_outFolderName = l_dialog.GetPath();
		m_outFolder->SetLabel( m_outFolderName );
		m_dataStreamer->SetOutFolder( m_outFolderName );
	}
}

void CMuseReaderDlg::OnBnClickedBtnread( wxCommandEvent & p_event )
{
	m_log->Clear();

	if ( m_outFolderName.empty() )
	{
		m_log->AppendString( _( "Choose an output folder" ) );
	}
	else
	{
		if ( m_inFileName.empty() )
		{
			m_log->AppendString( _( "Choose a file to read" ) );
		}
		else
		{
			if ( m_dataStreamer )
			{
				if ( m_dataStreamer->ReadHeader() )
				{
					int i = 0;

					while ( m_dataStreamer->GetNextBlock() )
					{
						wxString l_name = m_dataStreamer->GetFileName( i++ );
						m_output->AppendString( l_name );
					}

					m_log->AppendString( _( "File read" ) );
					m_dataStreamer->Cleanup();

					m_inFile->SetLabel( wxString() );
					m_outFolder->SetLabel( wxString() );
				}
			}
		}
	}
}
