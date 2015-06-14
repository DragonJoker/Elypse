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
#include "stdafx.h"

#include "MuseWriter.h"
#include "MuseWriterDlg.h"

#include <iostream>
#include <cstdio>

#include <wx/button.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/stattext.h>

namespace
{
	enum eIDC
	{
		eIDC_BUTTONADD,
		eIDC_FILESLIST,
		eIDC_EDITOUTFILE,
		eIDC_BTNOUTFILE,
		eIDC_BTNOK,
		eIDC_LISTLOG,
		eIDC_BTNDEL,
		eIDC_BUTTON1,
		eIDC_BTNFOLDER,
		eIDC_BUTTONUP,
		eIDC_BUTTONDOWN,
		eIDC_EDITOUTFILE2,
		eIDC_EDITCFGFILE,
		eIDC_BTNCFGFILE,
	};

	enum Mode
	{
		Undefined,
		Files,
		Folders,
		Out,
	};

	Mode GetMode( Mode p_mode, wxString const & p_line )
	{
		Mode l_return = p_mode;

		if ( p_line == wxT( "[infiles]" ) )
		{
			l_return = Files;
		}
		else if ( p_line == wxT( "[infolders]" ) )
		{
			l_return = Folders;
		}
		else if ( p_line == wxT( "[outfile]" ) )
		{
			l_return = Out;
		}

		return l_return;
	}

	wxString make_wxString( char const * p_string )
	{
		return wxString( p_string, wxConvLibc );
	}

	wxString make_wxString( std::string const & p_string )
	{
		return make_wxString( p_string.c_str() );
	}

	std::string make_string( wxString const & p_string )
	{
		return p_string.char_str().data();
	}

	const int OFFSET = 5;
	const int STATIC_HEIGHT = 20;
	const int EDIT_HEIGHT = 25;
	const wxSize BUTTON_SIZE( 75, 25 );
}

MuseWriterDlg::MuseWriterDlg()
	: wxDialog( NULL, wxID_ANY, _( "Muse file generator" ) )
{
	int l_width = 640;
	int l_height = 480;
	SetClientSize( wxSize( l_width, l_height ) );
	int l_effWidth = l_width - ( OFFSET * 3 );

	int l_leftListWidth = l_effWidth / 3;
	int l_rightListWidth = 2 * l_effWidth / 3;;
	int l_listHeight = l_height - ( EDIT_HEIGHT * 2 ) - ( STATIC_HEIGHT * 3 ) - ( OFFSET * 4 ) - BUTTON_SIZE.y;
	int l_left = l_leftListWidth + OFFSET * 2;
	int l_top = OFFSET;

	wxSize l_size = GetClientSize();
	new wxStaticText( this, wxID_ANY, _( "Files" ), wxPoint( OFFSET, l_top ), wxSize( l_leftListWidth, STATIC_HEIGHT ) );
	new wxStaticText( this, wxID_ANY, _( "Log" ), wxPoint( l_left, l_top ), wxSize( l_rightListWidth, STATIC_HEIGHT ) );
	l_top += STATIC_HEIGHT;
	m_files = new wxListBox( this, eIDC_FILESLIST, wxPoint( OFFSET, l_top ), wxSize( l_leftListWidth, l_listHeight ) );
	m_log = new wxListBox( this, eIDC_LISTLOG, wxPoint( l_left, l_top ), wxSize( l_rightListWidth, l_listHeight ) );
	l_top += OFFSET + l_listHeight;
	l_left = OFFSET;
	new wxButton( this, eIDC_BUTTONUP, _( "Up" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_left += OFFSET + BUTTON_SIZE.x;
	new wxButton( this, eIDC_BUTTONDOWN, _( "Down" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_left += OFFSET + BUTTON_SIZE.x;
	new wxButton( this, eIDC_BTNDEL, _( "Delete" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_left += OFFSET + BUTTON_SIZE.x;
	new wxButton( this, eIDC_BUTTONADD, _( "Add file(s)" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_left += OFFSET + BUTTON_SIZE.x;
	new wxButton( this, eIDC_BTNFOLDER, _( "Add Folder" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_left = OFFSET;
	l_top += OFFSET + BUTTON_SIZE.y;
	new wxStaticText( this, wxID_ANY, _( "Config file:" ), wxPoint( l_left, l_top ), wxSize( l_rightListWidth, STATIC_HEIGHT ) );
	l_top += STATIC_HEIGHT;
	m_editCfg = new wxTextCtrl( this, eIDC_EDITCFGFILE, wxEmptyString, wxPoint( l_left, l_top ), wxSize( l_rightListWidth, BUTTON_SIZE.y ) );
	l_left += l_rightListWidth;
	new wxButton( this, eIDC_BTNCFGFILE, _( "Browse" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_left = OFFSET;
	l_top += BUTTON_SIZE.y;
	new wxStaticText( this, wxID_ANY, _( "Output file:" ), wxPoint( l_left, l_top ), wxSize( l_rightListWidth, STATIC_HEIGHT ) );
	l_top += STATIC_HEIGHT;
	m_editOut = new wxTextCtrl( this, eIDC_EDITOUTFILE, wxEmptyString, wxPoint( l_left, l_top ), wxSize( l_rightListWidth, BUTTON_SIZE.y ) );
	l_left += l_rightListWidth;
	new wxButton( this, eIDC_BTNOUTFILE, _( "Browse" ), wxPoint( l_left, l_top ), BUTTON_SIZE );

	l_left = l_width - OFFSET - BUTTON_SIZE.x;
	l_top = l_height - ( OFFSET + BUTTON_SIZE.y ) * 2;
	new wxButton( this, eIDC_BTNOK, _( "Generate" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
	l_top += OFFSET + BUTTON_SIZE.y;
	new wxButton( this, wxID_CANCEL, _( "Exit" ), wxPoint( l_left, l_top ), BUTTON_SIZE );
}

MuseWriterDlg::~MuseWriterDlg()
{
}

void MuseWriterDlg::SetOutputFileName( wxString const & p_file )
{
	m_outFileName = p_file;
	m_editOut->SetLabel( m_outFileName );
}

void MuseWriterDlg::AddFile( wxString const & p_file )
{
	wxString l_log;

	if ( m_dataWriter.AddFile( make_string( p_file ) ) )
	{
		m_log->AppendString( _( "Added " ) + p_file );
		m_files->AppendString( p_file.substr( p_file.find_last_of( wxFileName::GetPathSeparator() ) + 1 ) );
	}
	else
	{
		m_log->AppendString( _( "Couldn't add " ) + p_file );
	}
}

void MuseWriterDlg::AddGfxFolder( wxString const & p_file )
{
	DoAddFolder( p_file, Elypse::Download::EM_BLOCK_ZIPDATA );
}

void MuseWriterDlg::AddSndFolder( wxString const & p_file )
{
	DoAddFolder( p_file, Elypse::Download::EM_BLOCK_SNDDATA );
}

void MuseWriterDlg::DoAddFolder( wxString const & p_file, Elypse::Download::DataBlockType p_type )
{
	wxString l_log;

	if ( m_dataWriter.AddFolder( make_string( p_file ), p_type ) )
	{
		m_log->AppendString( _( "Added " ) + p_file );
		m_files->AppendString( p_file.substr( p_file.find_last_of( wxFileName::GetPathSeparator() ) + 1 ) );
	}
	else
	{
		m_log->AppendString( _( "Couldn't add " ) + p_file );
	}
}

bool MuseWriterDlg::DoWrite()
{
	bool l_return = false;

	if ( !m_outFileName.empty() )
	{
		l_return = m_dataWriter.Write( make_string( m_outFileName ) );
	}

	return l_return;
}

void MuseWriterDlg::DoUpdateFileList()
{
	m_files->Clear();

	for ( auto && l_file : m_dataWriter.GetFileList() )
	{
		m_files->AppendString( make_wxString( l_file.substr( l_file.find_last_of( d_path_slash ) + 1 ) ) );
	}
}

void MuseWriterDlg::DoReadCFGFile()
{
	wxString l_line;
	wxTextFile l_cfgFile( m_cfgFileName );

	l_line = l_cfgFile.GetFirstLine();
	std::cout << l_line << std::endl;

	Mode l_mode = GetMode( Undefined, l_line );

	if ( l_mode != Undefined )
	{
		l_cfgFile.GetNextLine();

		for ( ; l_line != wxT( "[end]" ) && !l_cfgFile.Eof(); l_line = l_cfgFile.GetNextLine() )
		{
			std::cout << l_line << std::endl;
			l_mode = GetMode( l_mode, l_line );

			switch ( l_mode )
			{
			case Files:
				AddFile( l_line );
				break;

			case Folders:
				AddGfxFolder( l_line );
				break;

			case Out:
				SetOutputFileName( l_line );
				break;
			}
		}
	}
}

BEGIN_EVENT_TABLE( MuseWriterDlg, wxDialog )
	EVT_BUTTON( eIDC_BUTTONADD, MuseWriterDlg::OnBnClickedBtnAddFile )
	EVT_BUTTON( eIDC_BTNFOLDER, MuseWriterDlg::OnBnClickedBtnFolder )
	EVT_BUTTON( eIDC_BTNDEL, MuseWriterDlg::OnBnClickedBtnDel )
	EVT_BUTTON( eIDC_BTNOUTFILE, MuseWriterDlg::OnBnClickedBtnOutFile )
	EVT_BUTTON( eIDC_BTNOK, MuseWriterDlg::OnBnClickedBtnOk )
	EVT_BUTTON( eIDC_BUTTONUP, MuseWriterDlg::OnBnClickedBtnUp )
	EVT_BUTTON( eIDC_BUTTONDOWN, MuseWriterDlg::OnBnClickedBtnDown )
	EVT_BUTTON( eIDC_BTNCFGFILE, MuseWriterDlg::OnBnClickedBtnCfgFile )
END_EVENT_TABLE()

void MuseWriterDlg::OnBnClickedBtnAddFile( wxCommandEvent & p_event )
{
	wxFileDialog l_dialog( this, _( "Select the file to add" ),
						   wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr,
						   wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST | wxFD_OPEN );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		wxArrayString l_files;
		l_dialog.GetPaths( l_files );
		wxString l_log;

		for ( auto && l_file : l_files )
		{
			AddFile( l_file );
		}
	}
}

void MuseWriterDlg::OnBnClickedBtnFolder( wxCommandEvent & p_event )
{
	wxDirDialog l_dialog( this, _( "Choose a folder" ) );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		AddGfxFolder( l_dialog.GetPath() );
	}
}

void MuseWriterDlg::OnBnClickedBtnDel( wxCommandEvent & p_event )
{
	auto l_index = m_files->GetSelection();

	if ( m_dataWriter.DelFile( uint32_t( l_index ) ) )
	{
		m_files->Delete( l_index );
	}
	else
	{
		m_log->AppendString( _( "Couldn't delete the file" ) );
	}
}

void MuseWriterDlg::OnBnClickedBtnUp( wxCommandEvent & p_event )
{
	auto l_index = m_files->GetSelection();

	if ( m_dataWriter.UpFile( uint32_t( l_index ) ) )
	{
		DoUpdateFileList();
		m_files->SetSelection( l_index - 1 );
	}
	else
	{
		m_log->AppendString( _( "Couldn't move the file up" ) );
	}
}

void MuseWriterDlg::OnBnClickedBtnDown( wxCommandEvent & p_event )
{
	auto l_index = m_files->GetSelection();

	if ( m_dataWriter.DownFile( uint32_t( l_index ) ) )
	{
		DoUpdateFileList();
		m_files->SetSelection( l_index + 1 );
	}
	else
	{
		m_log->AppendString( _( "Couldn't move the file down" ) );
	}
}

void MuseWriterDlg::OnBnClickedBtnOutFile( wxCommandEvent & p_event )
{
	wxFileDialog l_dialog( this, _( "Select the output file name" ),
						   wxEmptyString, wxEmptyString, _( "Muse Files" ) + wxString( wxT( " (*.muse)|*.muse||" ) ),
						   wxFD_SAVE );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		SetOutputFileName( l_dialog.GetPath() );
	}
}

void MuseWriterDlg::OnBnClickedBtnOk( wxCommandEvent & p_event )
{
	if ( m_outFileName.empty() )
	{
		m_log->AppendString( _( "Select an output file" ) );
	}
	else if ( DoWrite() )
	{
		m_log->AppendString( _( "File successfully created" ) );
	}
}

void MuseWriterDlg::OnBnClickedBtnCfgFile( wxCommandEvent & p_event )
{
	wxFileDialog l_dialog( this, _( "Select the configuration file" ),
						   wxEmptyString, wxEmptyString, _( "Configuration file" ) + wxString( wxT( " (*.cfg)|*.cfg||" ) ),
						   wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST | wxFD_OPEN );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		m_cfgFileName = l_dialog.GetPath();
		m_editCfg->SetLabel( m_cfgFileName );
		DoReadCFGFile();
	}
}
