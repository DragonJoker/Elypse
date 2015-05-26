#include "ElypseCtrl.h"
#include "ElypseApp.h"
#include "ElypseFrame.h"
#include "ElypsePluginBin.h"

#include "../EMuseCore/Main/EMuseInstance.h"
#include "../EMuseCore/Main/EMuseController.h"
#include "../EMuseCore/EMuseLogs.h"


#include <General/StringConverter.h>
#include <General/StringUtils.h>
#include <General/ThreadId.h>
#include <General/File.h>

IMPLEMENT_APP( ElypseApp)

using namespace General::Utils;

bool ElypseApp :: OnInit()
{
	EMuseController::GetSingletonPtr()->ShowConsole();

	Path l_installPath;

#if ELYPSE_WINDOWS
	char l_buffer[256];
	GetModuleFileName( NULL, l_buffer, 256);
	l_installPath = l_buffer;
	l_installPath = string::replace( l_installPath, "/", "\\");
	genlib_assert( ! l_installPath.empty());
#elif
	l_installPath = "/home/fordev/.elypse/";
#endif

	l_installPath = l_installPath.GetPath();

	if ( ! DirectoryExists( l_installPath / "cfg") || ! FileExists( l_installPath / "cfg/core.zip"))
	{
		std::cerr << "Core files missing, try reinstalling the application.\nDetected install path : " << l_installPath << std::endl;
		return false;
	}

	String l_fileName;

	if (argc <= 1)
	{
		l_fileName = wxFileSelector( "Enregistrez sous ...", wxEmptyString, wxEmptyString, 0,
									 "Muse file (*.muse)|*.muse", wxFD_OPEN);
	}
	else
	{
		l_fileName = argv[1];
	}
		
	if ( ! FileExists( l_fileName))
	{
		std::cerr << "Usage :" << std::endl;
		std::cerr << "ElypseBinary [musefile path] or \n ElypseBinary [config file path]" << std::endl;
		return false;
	}

	m_plugin = new EMusePlugin_Binary;
	m_ogre = new EMuseInstance( l_installPath, m_plugin);

	m_ogre->SetMain( true);
//	m_ogre->SetInstanceName( "Main");
	m_ogre->UseDirectX( false);
	m_ogre->ShowFPS( false);
	m_ogre->SetAntialiasing( 0);
	m_ogre->SetDownloadFiles( true);
	m_width = 800;
	m_height = 600;

	if (FileExists( l_installPath / "defaults.cfg"))
	{
		_readConfigFile( l_installPath / "defaults.cfg");
	}

	if ( ! InitParams( l_fileName))
	{
		return false;
	}

	m_frame = new ElypseFrame( this, _T( "Elypse Player"), wxSize( m_width, m_height));
	m_control = new ElypseCtrl( this, m_frame, m_plugin, m_ogre);

	m_plugin->SetHandle( m_control);

	m_ogre->SetHandle( m_control->GetWHandle());
	m_ogre->SetCurrentDirectory( l_installPath);
	m_ogre->Init( m_width, m_height, "");

	SetTopWindow( m_frame);

	return true;
}

bool ElypseApp :: InitParams( const String & p_filepath)
{
	Path l_path = p_filepath;
	#if ELYPSE_WINDOWS
	l_path = string::replace( l_path, "/", "\\");
	#endif

	String l_filename = l_path.GetLeaf();
	size_t l_index = l_filename.find_last_of( '.');
	if (l_index != String::npos)
	{
		l_filename = l_filename.substr( l_index);
	}
	if (l_filename == ".muse")
	{
		m_ogre->SetFileUrl( "file://" + l_path);
		return true;
	}

	_readConfigFile( p_filepath);

	return true;
}

void ElypseApp :: _readConfigFile( const String & p_filepath)
{
	std::ifstream l_file;
	l_file.open( p_filepath.c_str());

	if ( ! l_file.is_open() || l_file.eof())
	{
		return;
	}

	while ( ! l_file.eof())
	{
		String l_line;
		std::getline( l_file, l_line);

//		std::cout << "l_line = " << l_line << std::endl;
		const StringArray & l_array = string::split( l_line, "=", 1);
		
		if (l_array.size() < 2)
		{
			continue;
		}
		String l_paramName = l_array[0];
		String l_paramValue = l_array[1];

		string::trim( l_paramValue);
		string::trim( l_paramName);
		string::toLowerCase( l_paramName);

//		std::cout << "param name : " << l_paramName << " // " << l_paramValue << std::endl;

		if (l_paramName == "renderer")
		{
			m_ogre->UseDirectX( l_paramValue == "DirectX");
		}
		else if (l_paramName == "downloadfiles")
		{
			m_ogre->SetDownloadFiles( l_paramValue != "false");
		}
		else if (l_paramName == "antialiasing")
		{
			m_ogre->SetAntialiasing( parseInt( l_paramValue));
		}
		else if (l_paramName == "startupscript")
		{
			m_ogre->SetStartupScript( l_paramValue);
		}
		else if (l_paramName == "filename")
		{
			Path l_url ( p_filepath);
			m_ogre->SetFileUrl( "file://" + l_url.GetPath() + d_path_slash + l_paramValue);
		}
		else if (l_paramName == "showfps")
		{
			m_ogre->ShowFPS( l_paramValue == "1");
		}
		else if (l_paramName == "resolution")
		{
			const StringArray & l_arraySize = string::split( l_paramValue, " ,;x\t");
			if (l_arraySize.size() == 2)
			{
				m_width = parseFloat( l_arraySize[0]);
				m_height = parseFloat( l_arraySize[1]);
			}
		}
	}
}

int ElypseApp :: OnExit()
{
	Close();

	if (EMuseController::GetSingletonPtr() != NULL && m_ogre->IsMain())
	{
		EMuseController::GetSingletonPtr()->WaitForThreadEnded();
		EMuseController::Destroy();
	}

	delete m_ogre;
	m_ogre = NULL;

	delete m_plugin;
	m_plugin = NULL;

//	Sleep( 500000);

	return 0;
}

void ElypseApp :: Close()
{
	m_ogre->WaitForDeletion();
}

