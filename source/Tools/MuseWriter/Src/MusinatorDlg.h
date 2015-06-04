#pragma once

#include <wx/dialog.h>
#include <wx/listbox.h>

#include "DataWriter.h"

class MusinatorDlg
	: public wxDialog
{
public:
	MusinatorDlg();

private:
	bool DoWrite();
	void DoUpdateFileList();
	void DoReadCFGFile();

protected:
	DECLARE_EVENT_TABLE()
	void OnBnClickedButtonaddfile( wxCommandEvent & p_event );
	void OnBnClickedBtnoutfile( wxCommandEvent & p_event );
	void OnBnClickedBtnok( wxCommandEvent & p_event );
	void OnEnChangeEditoutfile( wxCommandEvent & p_event );
	void OnBnClickedBtndel( wxCommandEvent & p_event );
	void OnBnClickedBtnfolder( wxCommandEvent & p_event );
	void OnBnClickedUp( wxCommandEvent & p_event );
	void OnBnClickedDown( wxCommandEvent & p_event );
	void OnBnClickedBtnoutfile2( wxCommandEvent & p_event );

private:
	wxListBox * m_files;
	wxListBox * m_log;
	wxTextCtrl * m_editLog;
	wxTextCtrl * m_editOut;
	DataWriter m_dataWriter;
	wxString m_outFileName;
	wxString m_cfgFileName;
	wxString m_folder;

};

