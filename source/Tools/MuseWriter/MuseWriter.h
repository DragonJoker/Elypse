/*
See LICENSE file in root folder
*/
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

DECLARE_APP( MuseWriterApp );
