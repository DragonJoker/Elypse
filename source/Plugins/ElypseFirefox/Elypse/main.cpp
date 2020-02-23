#include "stdafx.h"

#define  ___NO_NAMESPACE_OGRE___

#include "main.h"

#include <ElypseFrameListener.h>
#include <ElypseInstance.h>
#include <ElypseController.h>
#include <ElypseLogs.h>
#include <Version.h>

#if ELYPSE_WINDOWS
#	include <CompInfo.h>
#	define M_SUBCLASS_WINDOW( p_windowHandle, p_function) ((WNDPROC) SetWindowLongPtr( p_windowHandle, GWLP_WNDPROC, (LPARAM)(WNDPROC)(p_function)))
#	include <RegistryManager.h>
#	include <RegistryKey.h>
#	undef SetCurrentDirectory
#endif

#include <File.h>
#include <Execute.h>
#include <StringUtils.h>

//#include "Firefox/np_class.h"

using namespace General::Computer;
using namespace General::Utils;

ElypsePluginInstance * g_main;

PluginInstanceMap g_mainMap;


#define VERBOSE( X) EMUSE_CONSOLE_MESSAGE_DEBUG(X)

namespace
{
	const char * c_updaterName = "ElypsePlayer_Updater.exe";
	const char * c_installerName = "ElypsePlayer_Installer.exe";
	const char * c_updaterParams = "-update";
}

/*
Initialisation Sequence :

1) NS_PluginInitialize
2) NS_NewPluginInstance
	|- Cconstructor
	|- InitParams
3) init

--------------------------

Destruction Sequence :

1) shut
2) NS_DestroyPluginInstance
	|-	Destructor
3) NS_PluginShutdown
*/



NPError NS_PluginInitialize()
{
#if ELYPSE_WINDOWS
#ifdef ____MUSE_DEBUG____
	AllocConsole();
	freopen( "CONOUT$" , "w+" , stdout );
	std::cout << "gn�" << std::endl;
//	m_useConsole = true;
#endif
	VERBOSE( "Global::NS_PluginInitialize" );
#endif

	return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
	VERBOSE( "Global::NS_PluginShutdown" );
//	Sleep( 5000);
}

NPBool ElypsePluginInstance :: isInitialized()
{
//	VERBOSE( "ElypsePluginInstance :: isInitialized");

	return m_initialized;
}

const char * ElypsePluginInstance :: getVersion()
{
	VERBOSE( "ElypsePluginInstance :: getVersion" );

	return NPN_UserAgent( m_instance );
}

nsPluginInstanceBase * NS_NewPluginInstance( nsPluginCreateData * aCreateDataStruct )
{

	VERBOSE( "Global::NS_NewPluginInstance" );

	if ( ! aCreateDataStruct )
	{
		return NULL;
	}

	ElypsePluginInstance * l_plugin = new ElypsePluginInstance( aCreateDataStruct->instance );

	l_plugin->InitParams( aCreateDataStruct );

	return l_plugin;
}

void NS_DestroyPluginInstance( nsPluginInstanceBase * p_pluginInstance )
{

	VERBOSE( "Global::NS_DestroyPluginInstance" );

	if ( p_pluginInstance != NULL )
	{
		delete( ElypsePluginInstance * )p_pluginInstance;
	}
}

ElypsePluginInstance :: ~ElypsePluginInstance()
{
	VERBOSE( "ElypsePluginInstance :: ~ElypsePluginInstance" );

//	if (g_main == this)
	{
		g_main = NULL;
	}
}

String ElypsePluginInstance :: _urlDecode( const String & p_url )
{
	String l_string;

	char l_buf[10];

	unsigned long l_ulong;

	for ( unsigned int i = 0 ; i < p_url.length() ; i ++ )
	{
		if ( p_url[i] == '%' )
		{
			l_buf[0] = p_url[i + 1];
			l_buf[1] = p_url[i + 2];
			l_buf[2] = 0;

			l_ulong = strtoul( l_buf, NULL, 16 );

			i += 2;

			l_string.push_back( ( char )l_ulong );
		}
		else
		{
			l_string.push_back( p_url[i] );
		}
	}

	return l_string;
}

void ElypsePluginInstance :: _checkInstall()
{
#if ELYPSE_WINDOWS
	ComputerInfo l_info;

	if ( l_info.GetOSMajorVersion() > 5 || l_info.GetOSMajorVersion() == 5 && l_info.GetOSMinorVersion() > 1 )
	{
		char * l_userProfile = std::getenv( "userprofile" );
		m_installPath = l_userProfile;

		if ( m_installPath.empty() )
		{
			return;
		}

		m_installPath = m_installPath / "AppData" / "Roaming" / "FDSSoftMedia" / "Elypse";
	}
	else
	{
		RegistryManager<String> l_registry;
		RegistryKey * l_key = l_registry.GetKey( "SOFTWARE\\MICROSOFT\\WINDOWS\\CURRENTVERSION" );

		if ( l_key == NULL )
		{
			return;
		}

		m_installPath = l_key->GetStringValue( "ProgramFilesDir" );

		if ( m_installPath.empty() )
		{
			return;
		}

		m_installPath = m_installPath / "FDSSoftMedia" / "Elypse";
	}

#endif
}

String ElypsePluginInstance :: _getUserAgent()
{
	NPObject * l_window;
	NPVariant l_vdoc;
	NPVariant l_vurl;

	String l_ua;

	NPError l_error = NPN_GetValue( m_instance, NPNVWindowNPObject, & l_window );

	if ( l_error == NPERR_NO_ERROR )
	{
		if ( NPN_GetProperty( m_instance, l_window, NPN_GetStringIdentifier( "navigator" ), & l_vdoc ) )
		{
			if ( NPN_GetProperty( m_instance, l_vdoc.value.objectValue, NPN_GetStringIdentifier( "userAgent" ), & l_vurl ) )
			{
				l_ua = std::string( l_vurl.value.stringValue.UTF8Characters );
				NPN_ReleaseVariantValue( & l_vurl );
			}

			NPN_ReleaseVariantValue( & l_vdoc );
		}
	}

	return l_ua;
}

void ElypsePluginInstance :: _getClientSite()
{
	NPObject * l_window;
	NPVariant l_vdoc;
	NPVariant l_vurl;

	NPN_GetValue( m_instance, NPNVWindowNPObject, & l_window );
	NPN_GetProperty( m_instance, l_window, NPN_GetStringIdentifier( "document" ), & l_vdoc );
	NPN_GetProperty( m_instance, l_vdoc.value.objectValue, NPN_GetStringIdentifier( "URL" ), & l_vurl );

	m_url = std::string( l_vurl.value.stringValue.UTF8Characters );
	m_url = m_url.substr( 0, m_url.find_last_of( '/' ) + 1 );
	m_url = _urlDecode( m_url );

	if ( m_url.find( "file://localhost/" ) != String::npos )
	{
		m_url = "file://" + string::replace( m_url.substr( strlen( "file://localhost/" ) ), "/", "\\" );
	}
	else if ( m_url.find( "file:///" ) != String::npos )
	{
		m_url = "file://" + string::replace( m_url.substr( strlen( "file:///" ) ), "/", "\\" );
	}
	else if ( m_url.find( "file://" ) != String::npos )
	{
		m_url = "file://" + string::replace( m_url.substr( strlen( "file://" ) ), "/", "\\" );
	}

	NPN_ReleaseVariantValue( & l_vdoc );
	NPN_ReleaseVariantValue( & l_vurl );
}

ElypsePluginInstance :: ElypsePluginInstance( NPP p_instance )
	: nsPluginInstanceBase()
	, m_instance( p_instance )
	, m_initialized( FALSE )
	, m_rightButton( false )
	, m_leftButton( false )
	, m_middleButton( false )
	, m_useConsole( false )
	, m_focus( false )
	, m_firefox( false )
	, m_noInit( false )
	, m_ogre( NULL )
	, m_plugin( NULL )
	, m_width( 0.0f )
	, m_height( 0.0f )
#if ELYPSE_WINDOWS
	, m_hook( NULL )
	, m_windowHandle( NULL )
	, m_oldProc( NULL )
#endif
{
	g_main = this;
	VERBOSE( "ElypsePluginInstance :: ElypsePluginInstance : begin" );

	m_plugin = new ElypsePlugin_Firefox;
	m_plugin->SetNPPInstance( m_instance );
	m_instance->pdata = this;
	m_plugin->InitGDI();

	_checkInstall();

	if ( m_installPath.empty() )
	{
		m_plugin->SetGraphicalStatus( StatusErrorDirectories );
//		m_plugin->MajorError( "Probleme : ne peut trouver les repertoires necessaires\nVeuillez reinstaller l'application.", "Erreur majeure");
		return;
	}

	m_plugin->SetInstallPath( m_installPath );
	_getClientSite();

	m_plugin->SetImagePath( m_installPath + "\\cfg\\" );

	String l_tempString;

	if ( ! DirectoryExists( m_installPath / "rsc" ) )
	{
		DirectoryCreate( m_installPath / "rsc" );
	}

	if ( ! DirectoryExists( m_installPath / "cfg" ) )
	{
		DirectoryCreate( m_installPath / "cfg" );
	}

	try
	{
		m_ogre = new ElypseInstance( m_installPath, m_plugin );
	}
	catch ( ... )
	{
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}

	m_plugin->SetBaseUrl( m_url );
	m_ogre->SetCurrentDirectory( m_url );

	VERBOSE( "ElypsePluginInstance :: ElypsePluginInstance : end" );

}

void ElypsePluginInstance :: InitParams( nsPluginCreateData * p_data )
{
	VERBOSE( "ElypsePluginInstance :: InitParams : begin" );

//	std::cout << "InitParams : " << std::endl;

	if ( m_ogre == NULL )
	{
		return;
	}

//	std::cout << "InitParams2 : " << std::endl;

	for ( int i = 0 ; i < p_data->argc ; i ++ )
	{
		String l_paramValue = p_data->argv[i];
		String l_paramName = p_data->argn[i];
		string :: toLowerCase( l_paramName );

		VERBOSE( "ElypsePluginInstance :: InitParams -> param found : " + l_paramName + " = " + l_paramValue );

		if ( l_paramName == "showdebug" && l_paramValue == "1" )
		{
			if ( ! m_useConsole )
			{
				m_useConsole = ElypseController::GetSingletonPtr()->ShowConsole();
//				std::cout << "yargh ZOOOB? " << std::endl;
			}
		}
		else if ( l_paramName == "file" )
		{

			Url l_url( l_paramValue );

			if ( l_url.GetProtocol() == HTTP )
			{
				m_ogre->SetFileUrl( l_url );
			}
			else
			{
				m_ogre->SetFileUrl( Url( m_url + l_paramValue ) );
			}
		}
		/*
		else if (l_paramName == "instancename")
		{
			m_ogre->SetInstanceName( l_paramValue);
		}
		*/
		else if ( l_paramName == "startupscript" )
		{
			m_ogre->SetStartupScript( l_paramValue );
		}
		else if ( l_paramName == "requiredversion" )
		{
			int l_version = parseInt( l_paramValue );

//			std::cout << "needed version : " << l_version << " // " << ELYPSE_VERSION << std::endl;
//			std::cout << "param value : " << l_paramValue << std::endl;
			if ( ! CHECK_ELYPSE_VERSION( l_version ) )
			{
				_updatePlayer();
				m_plugin->SetGraphicalStatus( StatusErrorUpdating );
				m_noInit = true;
			}
		}
		else if ( l_paramName == "renderer" )
		{
			m_ogre->UseDirectX( l_paramValue == "DirectX" );
		}
		else if ( l_paramName == "downloadfiles" )
		{
			m_ogre->SetDownloadFiles( l_paramValue != "false" );
		}
		else if ( l_paramName == "antialiasing" )
		{
			m_ogre->SetAntialiasing( parseInt( l_paramValue ) );
		}
		else
		{
			VERBOSE( "ElypsePluginInstance :: InitParams -> unknown param : " + l_paramName );
		}
	}

	VERBOSE( "ElypsePluginInstance :: InitParams : end" );
}

NPBool ElypsePluginInstance :: init( NPWindow * p_window )
{
	VERBOSE( "ElypsePluginInstance :: init : begin" );

	if ( p_window == NULL )
	{
		return TRUE;
	}


	m_initialized = TRUE;

	m_width = float( p_window->width );
	m_height = float( p_window->height );


	m_plugin->SetSize( p_window->width, p_window->height );


#if ELYPSE_WINDOWS
	m_windowHandle = ( HWND ) p_window->window;
	m_oldProc = M_SUBCLASS_WINDOW( m_windowHandle, static_cast <WNDPROC>( ProcessMessage ) );
	m_plugin->SetHandle( m_windowHandle );
	g_mainMap.insert( PluginInstanceMap::value_type( m_windowHandle, this ) );

	if ( m_windowHandle == NULL || m_ogre == NULL )
	{
		return TRUE;
	}

	m_hook = ::SetWindowsHookEx( WH_GETMESSAGE, GetMessageProc, NULL, GetCurrentThreadId() );
#endif

//	std::cout << "User Agent : " << _getUserAgent() << std::endl;
	m_firefox = ( _getUserAgent().find( "Firefox" ) != String::npos );


	m_ogre->SetHandle( ToString( reinterpret_cast<intptr_t>( m_windowHandle ) ) );

//	m_ogre->SetFilePath( m_url);

	m_ogre->SetMain( true );

	if ( ! m_noInit )
	{
		m_ogre->Init( p_window->width, p_window->height, String() );
	}

//	NPN_SetValue( m_instance, NPPVpluginScriptableNPObject, this);

//	NPN_CreateObject(
	VERBOSE( "ElypsePluginInstance :: init : end" );

	return TRUE;
}

NPError	ElypsePluginInstance :: GetValue( NPPVariable aVariable, void * aValue )
{
	NPError rv = NPERR_NO_ERROR;

//	if (aVariable == NPPVpluginScriptableInstance)
//	{
//		// addref happens in getter, so we don't addref here
//		np_elypsePlugin * scriptablePeer = getScriptablePeer();
//		if (scriptablePeer != NULL)
//		{
//			* (nsISupports **) aValue = scriptablePeer;
//		}
//		else
//		{
//			rv = NPERR_OUT_OF_MEMORY_ERROR;
//		}
//	}
//	else if (aVariable == NPPVpluginScriptableIID)
//	{
//		static nsIID scriptableIID = NP_ELYPSEPLUGIN_IID;
//		nsIID * ptr = (nsIID *) NPN_MemAlloc( sizeof( nsIID));
//		if (ptr != NULL)
//		{
//			* ptr = scriptableIID;
//			* (nsIID **) aValue = ptr;
//		}
//		else
//		{
//			rv = NPERR_OUT_OF_MEMORY_ERROR;
//		}
//	}
//	else if (aVariable == NPPVpluginScriptableNPObject)
//	{
//		//workaround needed. using the scriptable Object does litteraly nothing in Firefox, as of right now ( 14/04/2009)
//		if ( ! m_firefox)
//		{
//			*(NPObject **)(aValue) = Private_ScriptableObject();
//		}
//
//		return NPERR_NO_ERROR;
//	}
//	else
//	{
////		std::cout << aVariable << std::endl;
//	}

	return rv;
}

//nsScriptablePeer * ElypsePluginInstance :: getScriptablePeer()
//{
//	if ( ! mScriptablePeer)
//	{
//		mScriptablePeer = new nsScriptablePeer( m_plugin);
//		if ( ! mScriptablePeer)
//		{
//			return NULL;
//		}
//
//		NS_ADDREF( mScriptablePeer);
//	}
//
//  // add reference for the caller requesting the object
//  NS_ADDREF( mScriptablePeer);
//
//  return mScriptablePeer;
//}

void ElypsePluginInstance :: shut()
{
	VERBOSE( "ElypsePluginInstance :: shut : begin" );

	m_plugin->CloseGDI();

	const PluginInstanceMap::iterator & iter = g_mainMap.find( m_windowHandle );

	if ( iter != g_mainMap.end() )
	{
		VERBOSE( "ElypsePluginInstance :: shut : instance erased from map" );
		g_mainMap.erase( iter );
	}
	else
	{
		VERBOSE( "ElypsePluginInstance :: shut : instance NOT erased from map" );
	}

#ifndef ____MUSE_DEBUG____

	if ( g_mainMap.empty() )
	{
//		FreeConsole();
	}

#endif

	g_main = NULL;

	if ( m_ogre != NULL )
	{
		m_initialized = FALSE;

		m_ogre->WaitForDeletion();

		if ( ElypseController::GetSingletonPtr() != NULL && m_ogre->IsMain() )
		{
			ElypseController::GetSingletonPtr()->WaitForThreadEnded();
//			ElypseController::GetSingletonPtr()->DeleteOgre();
			ElypseController::Destroy();
		}

		delete m_ogre;
		m_ogre = NULL;
	}

	delete m_plugin;
	m_plugin = NULL;

#if ELYPSE_WINDOWS

	if ( m_oldProc != NULL )
	{
		M_SUBCLASS_WINDOW( m_windowHandle, m_oldProc );
	}

	if ( m_hook != NULL )
	{
		UnhookWindowsHookEx( m_hook );
	}

#endif

	m_windowHandle = NULL;

	VERBOSE( "ElypsePluginInstance :: shut : end" );
}

#if ELYPSE_WINDOWS

LRESULT CALLBACK ElypsePluginInstance :: ProcessMessage( HWND p_windowHandle, UINT p_message, WPARAM p_wParam, LPARAM p_lParam )
{
	VERBOSE( "message : " << p_message );

	switch ( p_message )
	{
	case WM_SETFOCUS:
	{
		const PluginInstanceMap::iterator & ifind = g_mainMap.find( GetFocus() );

		if ( ifind != g_mainMap.end() )
		{
			g_main = ifind->second;
			g_main->OnSetFocus();
		}

		break;
	}

	case WM_KILLFOCUS:
	{
		const PluginInstanceMap::iterator & ifind = g_mainMap.find( p_windowHandle );

		if ( ifind != g_mainMap.end() )
		{
			g_main = ifind->second;
			g_main->OnKillFocus();
		}

		break;
	}

	case WM_MOUSEMOVE:
	{
		if ( g_main != NULL )
		{
			g_main->OnMouseMove( MAKEPOINTS( p_lParam ) );
		}

		break;
	}

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	{
		const PluginInstanceMap::iterator & ifind = g_mainMap.find( p_windowHandle );

		if ( ifind != g_mainMap.end() )
		{
			g_main = ifind->second;
			g_main->OnLButtonDown( MAKEPOINTS( p_lParam ) );
		}

		break;
	}


	case WM_LBUTTONUP:
	{
		if ( g_main != NULL )
		{
			g_main->OnLButtonUp( MAKEPOINTS( p_lParam ) );
		}

		break;
	}

	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	{
		const PluginInstanceMap::iterator & ifind = g_mainMap.find( p_windowHandle );

		if ( ifind != g_mainMap.end() )
		{
			g_main = ifind->second;
			g_main->OnRButtonDown( MAKEPOINTS( p_lParam ) );
		}

		break;
	}

	case WM_RBUTTONUP:
	{
		if ( g_main != NULL )
		{
			g_main->OnRButtonUp( MAKEPOINTS( p_lParam ) );
		}

		break;
	}


	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	{
		const PluginInstanceMap::iterator & ifind = g_mainMap.find( p_windowHandle );

		if ( ifind != g_mainMap.end() )
		{
			g_main = ifind->second;
			g_main->OnMButtonDown( MAKEPOINTS( p_lParam ) );
		}

		break;
	}

	case WM_MBUTTONUP:
	{
		if ( g_main != NULL )
		{
			g_main->OnMButtonUp( MAKEPOINTS( p_lParam ) );
		}

		break;
	}


	case WM_MOUSEWHEEL:
	{
		short l_delta = GET_WHEEL_DELTA_WPARAM( p_wParam );

		if ( g_main != NULL )
		{
			g_main->OnMouseWheel( l_delta, MAKEPOINTS( p_lParam ) );
		}

		break;
	}

	case WM_SYSKEYUP:
	{
		if ( g_main != NULL )
		{
			g_main->OnSysKeyUp( UINT( p_wParam ) );
		}

		break;
	}

	case WM_SYSKEYDOWN:
	{
		if ( g_main != NULL )
		{
			g_main->OnSysKeyDown( UINT( p_wParam ), LOWORD( p_lParam ), HIWORD( p_lParam ) );
		}

		break;
	}

	case WM_KEYDOWN:
	{
		if ( g_main != NULL )
		{
			g_main->OnKeyDown( UINT( p_wParam ), LOWORD( p_lParam ), HIWORD( p_lParam ) );
		}

		break;
	}

	case WM_KEYUP:
	{
		if ( g_main != NULL )
		{
			g_main->OnKeyUp( UINT( p_wParam ) );
		}

		break;
	}

	case WM_PAINT:
	{
		if ( g_main != NULL )
		{
			g_main->m_plugin->Paint();
		}

		//Note : do not remove that -_-.
		return DefWindowProc( p_windowHandle, p_message, p_wParam, p_lParam );
	}

	default:
	{
//			VERBOSE( "message end by default : " << p_message);
		LRESULT l_res = DefWindowProc( p_windowHandle, p_message, p_wParam, p_lParam );
//			VERBOSE( "message endby default with result : " << l_res);
		return l_res;
	}
	}

//	VERBOSE( "message end: " << p_message);

	return 0;
}


LRESULT CALLBACK ElypsePluginInstance :: GetMessageProc( int p_code, WPARAM p_wParam, LPARAM p_lParam )
{
	LPMSG l_msg = ( LPMSG ) p_lParam;

	if ( g_main != NULL
			&&	g_main->m_focus
			&&	p_code >= 0
			&&	p_wParam == PM_REMOVE
			&&	l_msg->message >= WM_KEYFIRST
			&&	l_msg->message <= WM_KEYLAST )
	{
		if ( l_msg->message == WM_KEYDOWN
				&&	l_msg->wParam != 116
				&&	l_msg->wParam != 122 )
		{
			g_main->OnKeyDown( UINT( l_msg->wParam ), HIWORD( l_msg->lParam ), LOWORD( l_msg->lParam ) );

			l_msg->message = WM_NULL;
			l_msg->lParam = 0L;
			l_msg->wParam = 0;
		}

		if ( l_msg->message == WM_KEYUP
				&&	l_msg->wParam != 116
				&&	l_msg->wParam != 122 )
		{
			g_main->OnKeyUp( UINT( l_msg->wParam ) );

			l_msg->message = WM_NULL;
			l_msg->lParam = 0L;
			l_msg->wParam = 0;
		}

		if ( l_msg->message == WM_SYSKEYDOWN
				&&	l_msg->wParam != 116
				&& l_msg->wParam != 122 )
		{
			g_main->OnSysKeyDown( UINT( l_msg->wParam ), HIWORD( l_msg->lParam ), LOWORD( l_msg->lParam ) );

			l_msg->message = WM_NULL;
			l_msg->lParam = 0L;
			l_msg->wParam = 0;
		}

		if ( l_msg->message == WM_SYSKEYUP )
		{
			g_main->OnSysKeyUp( UINT( l_msg->wParam ) );

			l_msg->message = WM_NULL;
			l_msg->lParam = 0L;
			l_msg->wParam = 0;
		}
	}


	return ::CallNextHookEx( NULL, p_code, p_wParam, p_lParam );
}

#endif

//TODO : finir �a -> lier vers la fonction ExecuteScript, reorganiser le bidule NPObject de merde pour le multi instance.
extern NPClass Private_Class;

NPObject * ElypsePluginInstance :: Private_ScriptableObject()
{
	static NPObject * obj = NULL;

	if ( obj == NULL )
	{
		obj = NPN_CreateObject( m_instance, & Private_Class );
	}

	if ( obj != NULL )
	{
		NPN_RetainObject( obj );
	}

	return obj;
}

void ElypsePluginInstance :: _updatePlayer()
{
	if ( FileExists( m_installPath / c_installerName ) )
	{
		FileDelete( m_installPath / c_updaterName );
		FileCopy( m_installPath / c_installerName, m_installPath / c_updaterName );
		Execute( c_updaterName, m_installPath + "\\", c_updaterParams, ES_SHELL );
	}
}
