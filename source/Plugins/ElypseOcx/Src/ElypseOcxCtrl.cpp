// ElypseOcxCtrl.cpp : Implementation of the CElypseOcxCtrl ActiveX Control class.

#include "ElypseOcxPch.h"

#include "ElypseOcx.h"
#include "ElypseOcxPropPage.h"
#include "ElypseOcxCtrl.h"

#include <ElypseController.h>
#include <ElypseInstance.h>
#include <ElypseFrameListener.h>
#include <ElypseLogs.h>
#include <Version.h>

#include <File.h>
#include <CompInfo.h>
#include <RegistryManager.h>
#include <RegistryKey.h>
#include <Execute.h>

namespace
{
	const char * c_updaterName = "ElypsePlayer_Updater.exe";
	const char * c_installerName = "ElypsePlayer_Installer.exe";
	const char * c_updaterParams = "-update";
}

#undef SetCurrentDirectory

GENLIB_INIT_SINGLETON( ElypseOcxCtrl );
/*
BEGIN_MESSAGE_MAP(ElypseOcxCtrl, COleControl)

END_MESSAGE_MAP()
*/
using namespace General::Computer;

void ElypseOcxCtrl::DoCheckInstall()
{
	ComputerInfo l_info;

	if ( l_info.GetOSMajorVersion() > 5 || l_info.GetOSMajorVersion() == 5 && l_info.GetOSMinorVersion() > 1 )
	{
		char * l_userProfile = std::getenv( "userprofile" );
		m_installPath = l_userProfile;

		if ( m_installPath.empty() )
		{
			return;
		}

		m_installPath = m_installPath / "AppData" / "Roaming" / "DragonJoker" / "Elypse";
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

		m_installPath = m_installPath / "DragonJoker" / "Elypse";
	}
}

void ElypseOcxCtrl::DoGetClientSite()
{
	IOleClientSite * l_clientSite = GetClientSite();

	if ( l_clientSite == NULL )
	{
		return;
	}

	IMoniker * l_moniker = NULL;

	m_plugin->SetClientSite( l_clientSite );

	if ( FAILED( l_clientSite->GetMoniker( OLEGETMONIKER_TEMPFORUSER, OLEWHICHMK_CONTAINER, & l_moniker ) ) )
	{
		l_clientSite->Release();
		return;
	}

	LPOLESTR l_displayName;

	if ( FAILED( l_moniker->GetDisplayName( NULL, NULL, & l_displayName ) ) )
	{
		l_clientSite->Release();
		return;
	}

	l_moniker->Release();

	char n1[256];
	WideCharToMultiByte( CP_ACP, 0, l_displayName, -1, n1, 255, NULL, NULL );
	size_t i = strnlen( n1, 255 );

	while ( i > 0 && n1[i] != '/' && n1[i] != '\\' )
	{
		i --;
	}

	n1[i] = '\0';

	m_baseDirectory = String( n1 );

	if ( m_baseDirectory.substr( 0, 8 ) == "file:///" )
	{
		m_baseDirectory.erase( m_baseDirectory.begin() + 7, m_baseDirectory.begin() + 8 );
	}

	if ( m_baseDirectory.substr( 0, 4 ) == "file" )
	{
		m_baseDirectory.push_back( '\\' );
	}
	else
	{
		m_baseDirectory.push_back( '/' );
	}

	std::cout << "base directory : " << m_baseDirectory << std::endl;
	m_plugin->SetBaseUrl( m_baseDirectory );
	m_ogre->SetCurrentDirectory( m_baseDirectory );
	l_clientSite->Release();
}

ElypseOcxCtrl::ElypseOcxCtrl()
	:	m_focused( false ),
		m_destroyed( false ),
		m_deleteAll( false ),
		m_oneTimePostSetFocus( false ),
		m_ogreActive( false ),
		m_plugin( NULL ),
		m_secondProp( false ),
		m_leftButton( false ),
		m_rightButton( false ),
		m_middleButton( false ),
		m_noInit( false ),
		m_ogre( NULL )
{

#ifdef ___EMUSE_DEBUG___
	_activateDebugConsole();
#endif
//	std::cout << "CREATE Elypse CTRL ? O.o" << std::endl;
}

void ElypseOcxCtrl::DoInitialise()
{
	SetSingleton( this );

	InitializeIIDs( &IID_ElypsePlayerDispatch, &IID_ElypsePlayerEvent );

	// Initialize GDI+.

	m_plugin = new Elypse::OCX::ElypseOcxPlugin;
	m_plugin->InitGDI();

	DoCheckInstall();

	if ( m_installPath.empty() )
	{
		m_plugin->SetGraphicalStatus( StatusErrorDirectories );
		return;
	}

	m_plugin->SetInstallPath( m_installPath );

	if ( ! DirectoryExists( m_installPath ) )
	{
		DirectoryCreate( m_installPath );
	}

	if ( ! DirectoryExists( m_installPath  / "rsc" ) )
	{
		DirectoryCreate( m_installPath  / "rsc" );
	}

	if ( ! DirectoryExists( m_installPath  / "cfg" ) )
	{
		DirectoryCreate( m_installPath  / "cfg" );
	}

	try
	{
		m_ogre = new ElypseInstance( m_installPath, m_plugin );
	}
	catch ( ... )
	{
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}

	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::CElypseOcxCtrl out" );
}

int ElypseOcxCtrl::OnCreate( LPCREATESTRUCT p_createStruct )
{
	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::OnCreate" );

	int x, y;
	this->GetControlSize( & x, & y );
	m_plugin->SetSize( x, y );
	m_plugin->SetHandle( m_hWnd );

	if ( m_ogre == NULL )
	{
		return 0;
	}

	m_ogre->SetHandle( ToString( reinterpret_cast< intptr_t >( m_hWnd ) ) );
	m_hook = ::SetWindowsHookEx( WH_GETMESSAGE, ElypseOcxCtrl::MessageProc, AfxGetInstanceHandle(), GetCurrentThreadId() );

	m_height = Real( y );
	m_width = Real( x );

	EMUSE_MESSAGE_DEBUG( "Screen Size : " + ToString( x ) + " by " + StringConverter::toString( y ) );

//	m_plugin->SetGraphicalStatus( StatusErrorDirectories);

	if ( ! m_noInit )
	{
		m_primary = m_ogre->Init( static_cast<unsigned int>( m_width ), static_cast<unsigned int>( m_height ) , m_linkedTo );
	}

	SetFocus();
	CWnd::OnPaint();

	ShowCursor( true );

	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::OnCreate out" );

	return 0;
}

String ElypseOcxCtrl::DoGetProperty( const String & p_propertyName, CPropExchange * p_properties )
{
	CString l_cString;
	PX_String( p_properties, p_propertyName.c_str(), l_cString );

	return String( l_cString );
}

void ElypseOcxCtrl::DoPropExchange( CPropExchange * p_properties )
{
	COleControl::DoPropExchange( p_properties );

	DoInitialise();

	if ( m_ogre == NULL )
	{
		return;
	}

	if ( m_secondProp )
	{
		return;
	}

	m_secondProp = true;

	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::DoPropExchange in" );

	ExchangeVersion( p_properties, MAKELONG( _wVerMinor, _wVerMajor ) );

	String l_temp2 = DoGetProperty( "ShowDebug",  p_properties );

	if ( l_temp2 == String( "1" ) )
	{
		DoActivateDebugConsole();
	}

	/*
	String l_temp = DoGetProperty( "InstanceName",  p_properties);
	if ( ! l_temp.empty())
	{
		m_ogre->SetInstanceName( l_temp);
	}
	*/

	String l_temp = DoGetProperty( "LinkTo",  p_properties );

	if ( ! l_temp.empty() )
	{
		m_linkedTo = l_temp;
	}

	l_temp = DoGetProperty( "StartupScript",  p_properties );

	if ( ! l_temp.empty() )
	{
		m_ogre->SetStartupScript( l_temp );
	}

	l_temp = DoGetProperty( "Renderer",  p_properties );

	if ( ! l_temp.empty() )
	{
		m_ogre->UseDirectX( "DirectX" == l_temp );
	}

	l_temp = DoGetProperty( "DownloadFiles",  p_properties );

	if ( ! l_temp.empty() )
	{
		m_ogre->SetDownloadFiles( "false" != l_temp );
	}

	l_temp = DoGetProperty( "Antialiasing",  p_properties );

	if ( ! l_temp.empty() )
	{
		m_ogre->SetAntialiasing( parseInt( l_temp ) );
	}

	l_temp = DoGetProperty( "ShowFPS",  p_properties );

	if ( ! l_temp.empty() )
	{
		m_ogre->ShowFPS( "true" == l_temp );
	}

	l_temp = DoGetProperty( "RequiredVersion",  p_properties );

	if ( ! l_temp.empty() )
	{
		int l_version = parseInt( l_temp );

//		std::cout << "needed version : " << l_version << " // " << ELYPSE_VERSION << std::endl;
//		std::cout << "param value : " << l_temp << std::endl;
		if ( ! CHECK_ELYPSE_VERSION( l_version ) )
		{
			DoUpdatePlayer();
		}
	}

	DoGetClientSite();

	Url l_url( DoGetProperty( "File",  p_properties ) );
//	std::cout << "GetProperty File : " << l_url << std::endl;

	if ( l_url.GetProtocol() == HTTP )
	{
		m_ogre->SetFileUrl( l_url );
	}
	else
	{
//		std::cout << "Base directory : " << m_baseDirectory << std::endl;
		m_ogre->SetFileUrl( Url( m_baseDirectory + l_url ) );
	}

	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::DoPropExchange out" );
}

void ElypseOcxCtrl::OnDestroy()
{
//	std::cout << "OnDestroy Elypse CTRL ? O.o" << std::endl;
	if ( GetSingletonPtr() == this )
	{
		SetSingleton( NULL );
	}

	EMUSE_CONSOLE_MESSAGE_NORMAL( "CElypseOcxCtrl::OnDestroy : \n" );

	if ( m_ogre != NULL )
	{
		m_ogre->WaitForDeletion();
		EMUSE_CONSOLE_MESSAGE_NORMAL( "EMuse Instance " + m_ogre->GetName() + " deleted\n" );
	}

}

ElypseOcxCtrl::~ElypseOcxCtrl()
{
//	std::cout << "Delete Elypse CTRL ? O.o" << std::endl;
	if ( m_ogre != NULL )
	{
		EMUSE_CONSOLE_MESSAGE_NORMAL( "delete instance : " + m_ogre->GetName() + "\n" );
	}

	if ( m_plugin != NULL )
	{
		m_plugin->CloseGDI();
	}

	if ( m_ogre != NULL )
	{
		if ( m_ogre->IsMain() )
		{
			ElypseController::GetSingletonPtr()->WaitForThreadEnded();
		}

		if ( ElypseController::GetSingletonPtr() != NULL && m_ogre->IsMain() )
		{
			EMUSE_MESSAGE_NORMAL( "delete controller instance\n" );
//			EMuseController::GetSingletonPtr()->DeleteOgre();
			ElypseController::Destroy();
		}
		else
		{
			EMUSE_MESSAGE_NORMAL( "controller instance remains\n" );
		}

		EMUSE_MESSAGE_NORMAL( "EMuse Instance " + m_ogre->GetName() + " exited\n" );

		::UnhookWindowsHookEx( m_hook );

		EMUSE_MESSAGE_NORMAL( "UNHHook the window\n" );

		delete m_ogre;
	}

	delete m_plugin;

//	std::cout << "Ending ? O.O" << std::endl;
	EMUSE_CONSOLE_MESSAGE_NORMAL( "The End\n" );
}

void ElypseOcxCtrl::DoActivateDebugConsole()
{
	ElypseController::GetSingletonPtr()->ShowConsole();

//	std::cout << "m_ogre : " << m_ogre << "\n";
}

LRESULT ElypseOcxCtrl::OnExecJS( WPARAM, LPARAM )
{
	m_plugin->ExecJS();
	return TRUE;
}

HRESULT ElypseOcxCtrl::DoExecuteScript( char * p_text )
{
	if ( m_ogre->IsInitialised() )
	{
		m_ogre->GetFL()->AddToScriptQueue( p_text );
	}

	return S_OK;
}

void ElypseOcxCtrl::DoUpdatePlayer()
{
	m_noInit = true;

	if ( FileExists( m_installPath / c_installerName ) )
	{
		FileDelete( m_installPath / c_updaterName );
		FileCopy( m_installPath / c_installerName, m_installPath / c_updaterName );
		Execute( c_updaterName, m_installPath + "\\", c_updaterParams, ES_SHELL );
		m_plugin->SetGraphicalStatus( StatusErrorUpdating );
	}
	else
	{
		m_plugin->SetGraphicalStatus( StatusErrorDirectories );
	}
}
