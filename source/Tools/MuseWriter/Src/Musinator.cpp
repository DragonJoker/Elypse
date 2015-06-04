#include "stdafx.h"

#include "Musinator.h"
#include "MusinatorDlg.h"

wxIMPLEMENT_APP( MusinatorApp );

MusinatorApp::MusinatorApp()
{
}

bool MusinatorApp::OnInit()
{
	MusinatorDlg * dlg = new MusinatorDlg;
	dlg->ShowModal();
	return false;
}


