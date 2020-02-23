#pragma once

#include <sdkddkver.h>

#undef _UNICODE
#undef UNICODE

#define _AFXDLL

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxmt.h>

#include <windows.h>
#include <winerror.h>

#include <gdiplus.h>

#include <afxdisp.h>
#include <afxinet.h>
#include <Mshtml.h>
#include <afxinet.h>
#include <Mshtml.h>

#include <ElypseController.h>
#include <ElypseFrameListener.h>
#include <ElypseInstance.h>
#include <ElypseLogs.h>
#include <ElypsePlugin.h>
#include <ScriptNode.h>
#include <VariableType.h>
#include <Version.h>

#include <CompInfo.h>
#include <Execute.h>
#include <File.h>
#include <MinMax.h>
#include <NeoCurl.h>
#include <Path.h>
#include <RegistryKey.h>
#include <RegistryManager.h>
#include <StringConverter.h>
#include <StringUtils.h>
#include <ThreadedQueue.h>

#include "resource.h"
