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
