#pragma once

#if !defined( __AFXCTL_H__ )
#	error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols

class CElypseOcxApp
	: public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

