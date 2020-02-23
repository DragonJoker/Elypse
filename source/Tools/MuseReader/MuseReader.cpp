/*
See LICENSE file in root folder
*/
#include "stdafx.h"

#include "MuseReader.h"
#include "MuseReaderDlg.h"

IMPLEMENT_APP( CMuseReaderApp );

CMuseReaderApp::CMuseReaderApp()
{
}

bool CMuseReaderApp::OnInit()
{
	CMuseReaderDlg dlg( nullptr );
	dlg.ShowModal();
	return false;
}

//********************************************************************************************************************