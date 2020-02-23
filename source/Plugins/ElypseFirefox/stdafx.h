// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined( _WIN32 )
#	include <sdkddkver.h>
#endif

#if VLD_AVAILABLE
#	define ____GENLIB_NO_MEMORY_DEBUG____
#endif

#define ___NO_NAMESPACE_OGRE___

#pragma warning( disable: 4251 )

#include <Thread.h>
#include <NeoCurl.h>
#include <StringUtils.h>
#include <StringConverter.h>

#include <ElypseFrameListener.h>
#include <ElypseInstance.h>
#include <ElypseController.h>
