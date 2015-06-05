#pragma once

#include <vector>
#include <iostream>
#include <cstdio>

#include <wx/app.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/dir.h>
#include <wx/dirdlg.h>
#include <wx/listbox.h>
#include <wx/filename.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>

#include <zlib.h>

#include <File.h>
#include <MD5.h>
#include <STLMacros.h>

#include <Module_Download.h>

#if defined( _MSC_VER )
#	if defined _M_IX86
#		pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#	elif defined _M_IA64
#		pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#	elif defined _M_X64
#		pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#	else
#		pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#	endif
#endif
