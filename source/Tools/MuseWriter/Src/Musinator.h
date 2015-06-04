#pragma once

#include <wx/app.h>

class MusinatorApp
	: public wxApp
{
public:
	MusinatorApp();

public:
	virtual bool OnInit();
};

wxDECLARE_APP( MusinatorApp );
