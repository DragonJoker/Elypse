#include "stdafx.h"

#include "Musinator.h"
#include "MusinatorDlg.h"

#include <iostream>
#include <cstdio>

#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>

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
		eIDC_BTNOUTFILE2,
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
}

MusinatorDlg::MusinatorDlg()
	: wxDialog( NULL, wxID_ANY, _( "Muse file generator" ), wxDefaultPosition, wxSize( 418, 292 ) )
{
	m_dataWriter.SetLog( m_editLog );
}

bool MusinatorDlg::DoWrite()
{
	bool l_return = false;

	if ( !m_outFileName.empty() )
	{
		l_return = m_dataWriter.Write( m_outFileName );
	}

	return l_return;
}

void MusinatorDlg::DoUpdateFileList()
{
	m_dataWriter.UpdateFileList( m_files );
}

void MusinatorDlg::DoReadCFGFile()
{
	wxString l_line;
	wxTextFile l_cfgFile( m_cfgFileName );

	l_line = l_cfgFile.GetFirstLine();
	std::cout << l_line << std::endl;

	Mode l_mode = GetMode( Undefined, l_line );

	if ( l_mode != Undefined )
	{
		l_cfgFile.GetNextLine();
		wxString l_log;

		for (;l_line != wxT( "[end]" ) && !l_cfgFile.Eof(); l_line = l_cfgFile.GetNextLine() )
		{
			std::cout << l_line << std::endl;
			l_mode = GetMode( l_mode, l_line );

			switch ( l_mode )
			{
			case Files:
				if ( m_dataWriter.AddFile( l_line ) )
				{
					l_log.Format( _( "Entry for %s added" ), l_line );
					m_log->AppendString( l_log );
					m_files->AppendString( l_line );
				}
				break;

			case Folders:
				if ( m_dataWriter.AddFolder( l_line, EMuse::Download::EM_BLOCK_ZIPDATA ) )
				{
					l_log.Format( _( "Entry for %s added" ), l_line );
					m_log->AppendString( l_log );
					m_files->AppendString( l_line );
				}

			case Out:
				{
					m_outFileName = l_line;
					m_editOut->SetLabel( m_outFileName );
				}
				break;
			}
		}
	}
}

BEGIN_EVENT_TABLE( MusinatorDlg, wxDialog )
	EVT_BUTTON( eIDC_BUTTONADD, MusinatorDlg::OnBnClickedButtonaddfile )
	EVT_BUTTON( eIDC_BTNOUTFILE, MusinatorDlg::OnBnClickedBtnoutfile )
	EVT_BUTTON( eIDC_BTNOK, MusinatorDlg::OnBnClickedBtnok )
	EVT_TEXT( eIDC_EDITOUTFILE, MusinatorDlg::OnEnChangeEditoutfile )
	EVT_BUTTON( eIDC_BTNDEL, MusinatorDlg::OnBnClickedBtndel )
	EVT_BUTTON( eIDC_BTNFOLDER, MusinatorDlg::OnBnClickedBtnfolder )
	EVT_BUTTON( eIDC_BUTTONUP, MusinatorDlg::OnBnClickedUp )
	EVT_BUTTON( eIDC_BUTTONDOWN, MusinatorDlg::OnBnClickedDown )
	EVT_BUTTON( eIDC_BTNOUTFILE2, MusinatorDlg::OnBnClickedBtnoutfile2 )
END_EVENT_TABLE()

void MusinatorDlg::OnBnClickedButtonaddfile( wxCommandEvent & p_event )
{
	wxFileDialog l_dialog( this, _( "Select the file to add" ),
		wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, 
		wxFD_MULTIPLE|wxFD_FILE_MUST_EXIST|wxFD_OPEN );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		wxArrayString l_files;
		l_dialog.GetPaths( l_files );
		wxString l_log;

		for ( auto && l_file : l_files )
		{
			if ( m_dataWriter.AddFile( l_file ) )
			{
				l_log.Format( _( "Entry for %s added" ), l_file );
				m_log->AppendString( l_log );
				m_files->AppendString( l_file );
			}
			else
			{
				l_log.Format( _( "Entry for %s not added" ), l_file );
				m_log->AppendString( l_log );
			}
		}
	}
}

void MusinatorDlg::OnBnClickedBtnfolder( wxCommandEvent & p_event )
{
	wxDirDialog l_dialog( this, _( "Choose a folder" ) );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		wxString l_log;
		m_folder = l_dialog.GetPath();
		m_log->AppendString( m_folder );

		if ( m_dataWriter.AddFolder( m_folder, EMuse::Download::EM_BLOCK_ZIPDATA ) )
		{
			l_log.Format( _( "Entry for %s added" ), m_folder );
			m_log->AppendString(l_log);
			m_files->AppendString( m_folder );
		}
		else
		{
			l_log.Format( _( "Entry for %s not added" ), m_folder );
			m_log->AppendString( l_log );
		}
	}
}

void MusinatorDlg::OnBnClickedBtndel( wxCommandEvent & p_event )
{
	unsigned int l_nbElems = m_files->GetCount();
	wxString l_log;

	for ( unsigned int i = 0; i < l_nbElems; i++ )
	{
		if ( m_files->IsSelected( i ) )
		{
			wxString l_path = m_files->GetString( i );

			if ( m_dataWriter.DelFile( l_path ) )
			{
				m_files->Delete( i );
			}
			else
			{
				l_log.Format( _( "Entry for %s not deleted" ), l_path );
				m_log->AppendString( l_log );
			}
		}
	}
}

void MusinatorDlg::OnBnClickedUp( wxCommandEvent & p_event )
{
	unsigned int l_nbElems = m_files->GetCount();
	wxString l_log;
	
	for ( unsigned int i = 0; i < l_nbElems; i++ )
	{
		if (l_listbox->GetSel(i) > 0)
		{
			char * l_path = new char[256];
			if (l_listbox->GetText(i, l_path) != LB_ERR && m_dataWriter.UpFile(l_path))
			{
				UpdateFileList();
				l_listbox->SetCurSel(i-1);
			}
			else
			{
				char * l_log = new char[256];
				sprintf_s(l_log, 256, "Can't up %s\0", l_path);
				m_log->AddString(l_log);
				delete [] l_log;
			}
		}
	}
}

void MusinatorDlg::OnBnClickedDown( wxCommandEvent & p_event )
{
	unsigned int l_nbElems = m_files->GetCount();
	wxString l_log;
	
	for ( unsigned int i = 0; i < l_nbElems; i++ )
	{
		if (l_listbox->GetSel(i) > 0)
		{
			char * l_path = new char[256];
			if (l_listbox->GetText(i, l_path) != LB_ERR && m_dataWriter.DownFile(l_path))
			{
				UpdateFileList();
				l_listbox->SetCurSel(i+1);
			}
			else
			{
				char * l_log = new char[256];
				sprintf_s(l_log, 256, "Can't down %s\0", l_path);
				m_log->AddString(l_log);
				delete [] l_log;
			}
		}
	}
}

void MusinatorDlg::OnBnClickedBtnoutfile( wxCommandEvent & p_event )
{
	CFileDialog FileSaveDialog(FALSE, "muse", NULL, OFN_HIDEREADONLY, "Fichiers (*.muse)|*.muse||", AfxGetMainWnd());

	if (FileSaveDialog.DoModal() == IDOK)
	{
		CString l_path = FileSaveDialog.GetPathName();
		int l_size = l_path.GetLength();
		m_outFileName = new char[l_size+1];
		for (int i = 0;i < l_size;i++)
			m_outFileName[i] = l_path[i];
		m_outFileName[l_size] = '\0';
		CEdit * l_edit = (CEdit *)GetDlgItem(IDC_EDITOUTFILE);
		l_edit->SetWindowText(m_outFileName);
	}
}

void MusinatorDlg::OnEnChangeEditoutfile( wxCommandEvent & p_event )
{
	CEdit * l_edit = (CEdit *)GetDlgItem(IDC_EDITOUTFILE);
	CString l_path;
	l_edit->GetWindowText(l_path);
	int l_size = l_path.GetLength();
	m_outFileName = new char[l_size+1];
	for (int i = 0;i < l_size;i++)
		m_outFileName[i] = l_path[i];
	m_outFileName[l_size] = '\0';
}

void MusinatorDlg::OnBnClickedBtnok( wxCommandEvent & p_event )
{
	if (m_outFileName == NULL)
	{
		m_log->AddString("Choisissez un fichier de sortie");
		return;
	}
		
	if (m_dataWriter == NULL)
	{
		m_log->AddString("Rien à écrire");
		return;
	}

	if (Write())
	{
		m_log->AddString("Fichier créé avec succès");
		return;
	}
}

void MusinatorDlg::OnBnClickedBtnoutfile2( wxCommandEvent & p_event )
{
	CFileDialog FileOpenDialog(TRUE, NULL, NULL, 
							   OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST, 
							   "Config File (*.cfg)|*.cfg||", AfxGetMainWnd());

	if (FileOpenDialog.DoModal() == IDOK)
	{
		CString l_fileName = FileOpenDialog.GetPathName();
		int l_size = l_fileName.GetLength();
		m_cfgFileName = new char[l_size+1];
		for (int i = 0;i < l_size;i++)
			m_cfgFileName[i] = l_fileName[i];
		m_cfgFileName[l_size] = '\0';
		CEdit * l_edit = (CEdit *)GetDlgItem(IDC_EDITCFGFILE);
		l_edit->SetWindowText(m_cfgFileName);
		ReadCFGFile();
	}
}
