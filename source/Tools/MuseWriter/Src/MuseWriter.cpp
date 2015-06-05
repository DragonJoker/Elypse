#include "stdafx.h"

#include "MuseWriter.h"
#include "MuseWriterDlg.h"

wxIMPLEMENT_APP( MuseWriterApp );

MuseWriterApp::MuseWriterApp()
{
}

bool MuseWriterApp::OnInit()
{
	MuseWriterDlg * dlg = new MuseWriterDlg;
	dlg->ShowModal();
	return false;
}


