#pragma once

#include <wx/app.h>

class MuseWriterApp
	: public wxApp
{
public:
	MuseWriterApp();

public:
	virtual bool OnInit();
};

wxDECLARE_APP( MuseWriterApp );
