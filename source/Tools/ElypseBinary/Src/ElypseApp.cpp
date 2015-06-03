#include "ElypseCtrl.h"
#include "ElypseApp.h"
#include "ElypseFrame.h"
#include "ElypsePluginBin.h"

#include <EMuseInstance.h>
#include <EMuseController.h>
#include <EMuseLogs.h>

#include <StringConverter.h>
#include <StringUtils.h>
#include <ThreadId.h>
#include <File.h>

#include <wx/stdpaths.h>

IMPLEMENT_APP( ElypseApp )

using namespace General::Utils;

bool ElypseApp :: OnInit()
{
	EMuseController::GetSingletonPtr()->ShowConsole();
	wxStandardPathsBase & l_paths = wxStandardPaths::Get();
	Path l_installPath = l_paths.GetExecutablePath().char_str().data();
	Path l_userPath = l_paths.GetUserDataDir().char_str().data();
	l_installPath = l_installPath.GetPath().GetPath() / "share" / "ElypseBinary";

	if ( !DirectoryExists( l_installPath / "cfg" ) )
	{
		std::cerr << "Core files missing, try reinstalling the application.\nDetected install path : " << l_installPath << std::endl;
		return false;
	}

	if ( !FileExists( l_installPath / "cfg" / "core.zip" ) )
	{
		std::cerr << "Core files missing, try reinstalling the application.\nDetected install path : " << l_installPath << std::endl;
		return false;
	}

	if ( !DirectoryExists( l_userPath / "cfg" ) )
	{
		DirectoryCreate( l_userPath / "cfg" );
	}

	FileCopy( l_installPath / "cfg" / "core.zip", l_userPath / "cfg" / "core.zip" );

	String l_fileName;

	if ( argc <= 1 )
	{
		l_fileName = wxFileSelector( wxT( "Enregistrez sous ..." ), wxEmptyString, wxEmptyString, wxEmptyString,
									 wxT( "Muse file (*.muse)|*.muse" ), wxFD_OPEN ).char_str();
	}
	else
	{
		l_fileName = wxString( argv[1] ).char_str();
	}

	if ( !FileExists( l_fileName ) )
	{
		std::cerr << "Usage :" << std::endl;
		std::cerr << "ElypseBinary [musefile path] or \n ElypseBinary [config file path]" << std::endl;
		return false;
	}

	m_plugin = new EMusePlugin_Binary;
	m_ogre = new EMuseInstance( l_userPath, m_plugin );

	m_ogre->SetMain( true );
//	m_ogre->SetInstanceName( "Main");
	m_ogre->UseDirectX( false );
	m_ogre->ShowFPS( false );
	m_ogre->SetAntialiasing( 0 );
	m_ogre->SetDownloadFiles( true );
	m_width = 800;
	m_height = 600;

	if ( FileExists( l_userPath / "defaults.cfg" ) )
	{
		_readConfigFile( l_userPath / "defaults.cfg" );
	}

	if ( !InitParams( l_fileName ) )
	{
		return false;
	}

	m_frame = new ElypseFrame( this, _T( "Elypse Player" ), wxSize( m_width, m_height ) );
	m_control = new ElypseCtrl( this, m_frame, m_plugin, m_ogre );

	m_plugin->SetHandle( m_control );

	m_ogre->SetHandle( m_control->GetWHandle() );
	m_ogre->SetCurrentDirectory( l_userPath );
	m_ogre->Init( m_width, m_height, "" );

	SetTopWindow( m_frame );

	return true;
}

bool ElypseApp :: InitParams( const String & p_filepath )
{
	Path l_path = p_filepath;
	String l_filename = l_path.GetLeaf();
	size_t l_index = l_filename.find_last_of( '.' );

	if ( l_index != String::npos )
	{
		l_filename = l_filename.substr( l_index );
	}

	if ( l_filename == ".muse" )
	{
		m_ogre->SetFileUrl( "file://" + l_path );
		return true;
	}

	_readConfigFile( p_filepath );

	return true;
}

void ElypseApp :: _readConfigFile( const String & p_filepath )
{
	std::ifstream l_file;
	l_file.open( p_filepath.c_str() );

	if ( ! l_file.is_open() || l_file.eof() )
	{
		return;
	}

	while ( ! l_file.eof() )
	{
		String l_line;
		std::getline( l_file, l_line );

//		std::cout << "l_line = " << l_line << std::endl;
		const StringArray & l_array = string::split( l_line, std::string( "=" ), 1 );

		if ( l_array.size() < 2 )
		{
			continue;
		}

		String l_paramName = l_array[0];
		String l_paramValue = l_array[1];

		string::trim( l_paramValue );
		string::trim( l_paramName );
		string::toLowerCase( l_paramName );

//		std::cout << "param name : " << l_paramName << " // " << l_paramValue << std::endl;

		if ( l_paramName == "renderer" )
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
		else if ( l_paramName == "startupscript" )
		{
			m_ogre->SetStartupScript( l_paramValue );
		}
		else if ( l_paramName == "filename" )
		{
			Path l_url( p_filepath );
			m_ogre->SetFileUrl( "file://" + l_url.GetPath() + d_path_slash + l_paramValue );
		}
		else if ( l_paramName == "showfps" )
		{
			m_ogre->ShowFPS( l_paramValue == "1" );
		}
		else if ( l_paramName == "resolution" )
		{
			const StringArray & l_arraySize = string::split( l_paramValue, String( " ,;x\t" ) );

			if ( l_arraySize.size() == 2 )
			{
				m_width = parseFloat( l_arraySize[0] );
				m_height = parseFloat( l_arraySize[1] );
			}
		}
	}
}

int ElypseApp :: OnExit()
{
	Close();

	if ( EMuseController::GetSingletonPtr() != NULL && m_ogre->IsMain() )
	{
		EMuseController::GetSingletonPtr()->WaitForThreadEnded();
		EMuseController::Destroy();
	}

	delete m_ogre;
	m_ogre = NULL;

	delete m_plugin;
	m_plugin = NULL;

	return 0;
}

void ElypseApp :: Close()
{
	m_ogre->WaitForDeletion();
}

