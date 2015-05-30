#ifndef ___MUSE_READER_H___
#define ___MUSE_READER_H___

#include "resource.h"
#include <wx/app.h>

class CMuseReaderApp
	: public wxApp
{
public:
	CMuseReaderApp();

public:
	virtual bool OnInit();
};

DECLARE_APP( CMuseReaderApp );

#endif
