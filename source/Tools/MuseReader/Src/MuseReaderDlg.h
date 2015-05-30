#ifndef ___MUSE_READER_DLG_H___
#define ___MUSE_READER_DLG_H___

#include <memory>

#include <wx/dialog.h>
#include <wx/textctrl.h>

#include "DataStreamer.h"

class CMuseReaderDlg
	: public wxDialog
{
public:
	CMuseReaderDlg( wxWindow * pParent );

public:
	DECLARE_EVENT_TABLE();
	void OnBnClickedBtninfile( wxCommandEvent & p_event );
	void OnBnClickedBtnoutfolder( wxCommandEvent & p_event );
	void OnBnClickedBtnread( wxCommandEvent & p_event );

private:
	std::shared_ptr< CDataStreamer > m_dataStreamer;
	wxListBox * m_log;
	wxListBox * m_output;
	wxTextCtrl * m_inFile;
	wxTextCtrl * m_outFolder;
	wxString m_outFolderName;
	wxString m_inFileName;
};

#endif
