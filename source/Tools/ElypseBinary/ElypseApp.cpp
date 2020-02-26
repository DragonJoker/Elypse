/*
See LICENSE file in root folder
*/
#include "ElypseBinaryPch.h"

#include "ElypseCtrl.h"
#include "ElypseApp.h"
#include "ElypseFrame.h"

#include <GuiElypsePlugin.h>

#include <ElypseInstance.h>
#include <ElypseController.h>
#include <ElypseLogs.h>

#include <StringConverter.h>
#include <StringUtils.h>
#include <File.h>

#include <wx/stdpaths.h>

IMPLEMENT_APP( ElypseBinary::ElypseApp )

using namespace General::Utils;

namespace ElypseBinary
{
	namespace
	{
		const wxString CONFIG_FOLDER_MISSING = _( "Configuration folder missing, try reinstalling the application." );
		const wxString CORE_FILES_MISSING = _( "Core file missing, try reinstalling the application." );
		const wxString DETECTED_INSTALL_PATH = _( "Detected install path : " );
		const wxString CHOOSE_MUSE_FILE = _( "Choose a MUSE file to open" );
		const wxString MUSE_FILES_NAME = _( "MUSE files" );
		const wxString MUSE_FILES_WILDCARD = wxT( " (*.muse)|*.muse" );
		const wxString ELYPSE_BINARY_USAGE = _( "Usage :" );
		const wxString ELYPSE_BINARY_MUSEFILE = _( "ElypseBinary [musefile path] or" );
		const wxString ELYPSE_BINARY_CONFIGFILE = _( "ElypseBinary [config file path]" );
		const wxString ELYPSE_PLAYER = wxT( "Elypse Player" );

		const String PATH_SHARE_FOLDER = "share";
		const String PATH_ELYPSE_FOLDER = "Elypse";
		const String PATH_CFG_FOLDER = "cfg";
		const String PATH_CORE_ZIP_FILE = "core.zip";
		const String PATH_DEFAULTS_CFG_FILE = "defaults.cfg";

		static constexpr std::string_view CFG_BOOLEAN_FALSE = "false";
		static constexpr std::string_view CFG_RENDERER = "renderer";
		static constexpr std::string_view CFG_RENDERER_DX = "DirectX";
		static constexpr std::string_view CFG_DOWNLOAD_FILES = "downloadfiles";
		static constexpr std::string_view CFG_ANTI_ALIASING = "antialiasing";
		static constexpr std::string_view CFG_STARTUP_SCRIPT = "startupscript";
		static constexpr std::string_view CFG_FILE_NAME = "filename";
		static constexpr std::string_view CFG_SHOW_FPS = "showfps";
		static constexpr std::string_view CFG_RESOLUTION = "resolution";
	}

	bool ElypseApp::OnInit()
	{
		wxStandardPathsBase & l_paths = wxStandardPaths::Get();
		Path l_installPath = make_string( l_paths.GetExecutablePath() );
		Path l_userPath = make_string( l_paths.GetUserDataDir() );

		if ( l_userPath.find( "ElypseBinary" ) != Path::npos )
		{
			l_userPath = l_userPath.GetPath();
		}

		l_installPath = l_installPath.GetPath().GetPath() / PATH_SHARE_FOLDER / "ElypseCore";
		bool l_return = false;

		if ( !DirectoryExists( l_installPath / PATH_CFG_FOLDER ) )
		{
			std::cerr << CONFIG_FOLDER_MISSING << std::endl;
			std::cerr << DETECTED_INSTALL_PATH << l_installPath << std::endl;
		}
		else if ( !FileExists( l_installPath / PATH_CFG_FOLDER / PATH_CORE_ZIP_FILE ) )
		{
			std::cerr << CORE_FILES_MISSING << std::endl;
			std::cerr << DETECTED_INSTALL_PATH << l_installPath << std::endl;
		}
		else
		{
			l_userPath /= "DragonJoker";

			if ( !DirectoryExists( l_userPath ) )
			{
				DirectoryCreate( l_userPath );
			}

			l_userPath /= PATH_ELYPSE_FOLDER;

			if ( !DirectoryExists( l_userPath ) )
			{
				DirectoryCreate( l_userPath );
			}

			if ( !DirectoryExists( l_userPath / PATH_CFG_FOLDER ) )
			{
				DirectoryCreate( l_userPath / PATH_CFG_FOLDER );
			}

			FileCopy( l_installPath / PATH_CFG_FOLDER / PATH_CORE_ZIP_FILE
				, l_userPath / PATH_CFG_FOLDER / PATH_CORE_ZIP_FILE );

			String l_fileName;

			if ( argc <= 1 )
			{
				l_fileName = make_string( wxFileSelector( CHOOSE_MUSE_FILE
					, wxEmptyString
					, wxEmptyString
					, wxEmptyString
					, MUSE_FILES_NAME + MUSE_FILES_WILDCARD
					, wxFD_OPEN ) );
			}
			else
			{
				l_fileName = GuiCommon::make_string( argv[1] );
			}

			m_plugin = std::make_shared< GuiCommon::wxElypsePlugin >();
			m_elypse = std::make_shared< ElypseInstance >( l_userPath, m_plugin.get() );

			m_elypse->SetMain( true );
			//m_elypse->SetInstanceName( "Main");
			m_elypse->UseDirectX( false );
			m_elypse->ShowFPS( false );
			m_elypse->SetAntialiasing( 0 );
			m_elypse->SetDownloadFiles( true );
			m_width = 800;
			m_height = 600;

			if ( FileExists( l_userPath / PATH_DEFAULTS_CFG_FILE ) )
			{
				_readConfigFile( l_userPath / PATH_DEFAULTS_CFG_FILE );
			}

			if ( InitParams( l_fileName ) )
			{
				m_frame = new ElypseFrame( ELYPSE_PLAYER, wxSize( m_width, m_height ) );
				m_control = new ElypseCtrl( *this, m_frame, m_plugin, m_elypse );

				m_plugin->SetHandle( m_control );

				m_elypse->SetHandle( m_control->GetWHandle() );
				m_elypse->SetCurrentDirectory( l_userPath );
				m_elypse->Init( m_width, m_height, "" );

				SetTopWindow( m_frame );
				l_return = true;
			}
		}

		return l_return;
	}

	bool ElypseApp::InitParams( String const & p_filepath )
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
			if ( FileExists( l_path ) )
			{
				m_elypse->SetFileUrl( Url{ "file://" } / l_path );
			}
			else
			{
				Url l_url{ l_path };

				if ( l_url.GetProtocol() != UNKNOWN )
				{
					m_elypse->SetFileUrl( l_url );
				}
				else
				{
					std::cerr << ELYPSE_BINARY_USAGE << std::endl;
					std::cerr << ELYPSE_BINARY_MUSEFILE << std::endl;
					std::cerr << ELYPSE_BINARY_CONFIGFILE << std::endl;
					return false;
				}
			}

			return true;
		}

		_readConfigFile( p_filepath );

		return true;
	}

	void ElypseApp::_readConfigFile( String const & p_filepath )
	{
		std::ifstream l_file;
		l_file.open( p_filepath.c_str() );

		if ( l_file.is_open() )
		{
			while ( !l_file.eof() )
			{
				String l_line;
				std::getline( l_file, l_line );
				const StringArray & l_array = string::split( l_line, std::string( "=" ), 1 );

				if ( l_array.size() >= 2 )
				{
					String l_paramName = l_array[0];
					String l_paramValue = l_array[1];

					string::trim( l_paramValue );
					string::trim( l_paramName );
					string::toLowerCase( l_paramName );

					if ( l_paramName == CFG_RENDERER )
					{
						m_elypse->UseDirectX( l_paramValue == CFG_RENDERER_DX );
					}
					else if ( l_paramName == CFG_DOWNLOAD_FILES )
					{
						m_elypse->SetDownloadFiles( l_paramValue != CFG_BOOLEAN_FALSE );
					}
					else if ( l_paramName == CFG_ANTI_ALIASING )
					{
						m_elypse->SetAntialiasing( parseInt( l_paramValue ) );
					}
					else if ( l_paramName == CFG_STARTUP_SCRIPT )
					{
						m_elypse->SetStartupScript( l_paramValue );
					}
					else if ( l_paramName == CFG_FILE_NAME )
					{
						Path l_url( p_filepath );
						m_elypse->SetFileUrl( "file://" + l_url.GetPath() + d_path_slash + l_paramValue );
					}
					else if ( l_paramName == CFG_SHOW_FPS )
					{
						m_elypse->ShowFPS( l_paramValue != CFG_BOOLEAN_FALSE );
					}
					else if ( l_paramName == CFG_RESOLUTION )
					{
						const StringArray & l_arraySize = string::split( l_paramValue, String( " ,;x\t" ) );

						if ( l_arraySize.size() == 2 )
						{
							m_width = int( parseFloat( l_arraySize[0] ) );
							m_height = int( parseFloat( l_arraySize[1] ) );
						}
					}
				}
			}
		}
	}

	int ElypseApp::OnExit()
	{
		Close();

		if ( ElypseController::GetSingletonPtr() != nullptr && m_elypse->IsMain() )
		{
			ElypseController::GetSingletonPtr()->WaitForThreadEnded();
			ElypseController::Destroy();
		}

		m_elypse.reset();
		m_plugin.reset();

		return 0;
	}

	void ElypseApp::Close()
	{
		m_elypse->WaitForDeletion();
	}
}
