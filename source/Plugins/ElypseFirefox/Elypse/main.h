/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *

 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.

#undef _UNICODE
#undef UNICODE

#define ____NO_ODE_INT_TYPES____

#define ____ELYPSE_NO_MEMORY_DEBUG____
#include "ElypsePlugin_Firefox.h"

#include "NpApi/npapi.h"

#include <Module_Main.h>

#include <Path.h>

class ElypsePluginInstance;

#if ELYPSE_WINDOWS
typedef std::map< HWND, ElypsePluginInstance *> PluginInstanceMap;
#endif

#include "Firefox/pluginbase.h"

class ElypsePluginInstance
	: public nsPluginInstanceBase
{
#if ELYPSE_WINDOWS
	HWND m_windowHandle;
	WNDPROC m_oldProc;
public:
	HHOOK m_hook;
public:
	static LRESULT CALLBACK ProcessMessage( HWND p_windowHandle, UINT p_message, WPARAM p_wParam, LPARAM p_lParam );
	static LRESULT CALLBACK GetMessageProc( int p_code, WPARAM p_wParam, LPARAM p_lParam );
#endif

private:
	NPP m_instance;
	NPBool m_initialized;

	String m_url;

	bool m_rightButton;
	bool m_leftButton;
	bool m_middleButton;
	bool m_useConsole;
	bool m_focus;
	bool m_noInit;

	ElypseInstance * m_ogre;

	ElypsePlugin_Firefox * m_plugin;

	Path m_installPath;

	float m_width;
	float m_height;

	bool m_firefox;

	//nsScriptablePeer * mScriptablePeer;

public:
	ElypsePluginInstance( NPP p_instance );
	virtual ~ElypsePluginInstance();

public:
	virtual NPBool init( NPWindow * p_window );
	virtual NPBool isInitialized();

	virtual void shut();

public:
	void InitParams( nsPluginCreateData * p_data );
	const char * getVersion();

	NPObject  * Private_ScriptableObject();

private:
	void OnSetFocus();
	void OnKillFocus();
#if ELYPSE_WINDOWS
	void OnMouseMove( const POINTS & p_point );
	void OnMButtonDown( const POINTS & p_point );
	void OnMButtonUp( const POINTS & p_point );
	void OnLButtonDown( const POINTS & p_point );
	void OnLButtonUp( const POINTS & p_point );
	void OnRButtonDown( const POINTS & p_point );
	void OnRButtonUp( const POINTS & p_point );

	void OnSysKeyUp( UINT p_key );
	void OnKeyUp( UINT p_key );
	void OnSysKeyDown( UINT p_key, UINT p_flags, UINT p_repeat );
	void OnKeyDown( UINT p_key, UINT p_flags, UINT p_repeat );

	void OnMouseWheel( short zdelta, const POINTS & p_point );
#endif

	void _setFocus();
	String _urlDecode( const String & p_url );
	void _getClientSite();
	void _checkInstall();
	String _getUserAgent();
	void _execJavascript();

	void _updatePlayer();

	NPError	GetValue( NPPVariable aVariable, void * aValue );
	//nsScriptablePeer * getScriptablePeer();

public:
	inline bool _contains( const POINTS & p_point )const
	{
		return p_point.x >= 0 && p_point.y >= 0 || p_point.x < m_width || p_point.y < m_height;
	}

	inline ElypseInstance * GetInstance()const
	{
		return m_ogre;
	}
};

#endif // __PLUGIN_H__
