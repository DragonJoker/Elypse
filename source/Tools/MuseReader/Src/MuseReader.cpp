#include "stdafx.h"

#include "MuseReader.h"
#include "MuseReaderDlg.h"

IMPLEMENT_APP( CMuseReaderApp );

CMuseReaderApp::CMuseReaderApp()
{
}

bool CMuseReaderApp::OnInit()
{
	CMuseReaderDlg dlg( NULL );
	dlg.ShowModal();
	return false;
}

//********************************************************************************************************************