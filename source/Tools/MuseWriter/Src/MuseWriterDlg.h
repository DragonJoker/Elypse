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
	void DoAddFolder( wxString const & p_file, EMuse::Download::DataBlockType p_type );
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

