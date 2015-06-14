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
#pragma once

#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>

#include <DataWriter.h>

class MuseWriterDlg
	: public wxDialog
{
public:
	MuseWriterDlg();
	virtual ~MuseWriterDlg();

	void SetOutputFileName( wxString const & p_file );
	void AddFile( wxString const & p_file );
	void AddGfxFolder( wxString const & p_file );
	void AddSndFolder( wxString const & p_file );

private:
	void DoAddFolder( wxString const & p_file, Elypse::Download::DataBlockType p_type );
	bool DoWrite();
	void DoUpdateFileList();
	void DoReadCFGFile();

protected:
	DECLARE_EVENT_TABLE()
	void OnBnClickedBtnAddFile( wxCommandEvent & p_event );
	void OnBnClickedBtnOutFile( wxCommandEvent & p_event );
	void OnBnClickedBtnOk( wxCommandEvent & p_event );
	void OnBnClickedBtnDel( wxCommandEvent & p_event );
	void OnBnClickedBtnFolder( wxCommandEvent & p_event );
	void OnBnClickedBtnUp( wxCommandEvent & p_event );
	void OnBnClickedBtnDown( wxCommandEvent & p_event );
	void OnBnClickedBtnCfgFile( wxCommandEvent & p_event );

private:
	wxListBox * m_files;
	wxListBox * m_log;
	wxTextCtrl * m_editOut;
	wxTextCtrl * m_editCfg;
	DataWriter m_dataWriter;
	wxString m_outFileName;
	wxString m_cfgFileName;
	wxString m_folder;

};

