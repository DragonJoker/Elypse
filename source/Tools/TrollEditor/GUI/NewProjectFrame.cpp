#include "NewProjectFrame.h"

#include "Properties/Common/AdditionalProperties.h"
#include "Properties/Common/SizeProperties.h"
#include "ScriptDialog.h"
#include "MainFrame.h"

#include "Project/Project.h"

#include "TrollEditorApplication.h"

namespace Troll
{
	using namespace ProjectComponents;

	namespace GUI
	{
		using namespace Properties;

		namespace
		{
			const wxString DEFAULT_PROJECT_NAME = _( "Project name" );
			const wxString DEFAULT_SCENE_NAME = _( "Scene name" );
			const wxString DEFAULT_FOLDER = _( "Path" );
		}

		NewProjectProperties::NewProjectProperties( ProjectComponents::ProjectSPtr project )
			: ProjectProperties{ *project }
			, m_project{ project }
			, m_name{ DEFAULT_PROJECT_NAME }
			, m_sceneName{ DEFAULT_SCENE_NAME }
			, m_path{ DEFAULT_FOLDER }
		{
		}

		NewProjectProperties::~NewProjectProperties()
		{
		}

		bool NewProjectProperties::OnValidate( wxWindow * p_parent )
		{
			if ( m_name.empty() || m_name == DEFAULT_PROJECT_NAME )
			{
				wxMessageBox( _( "Enter a project name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, p_parent );
				return false;
			}

			if ( m_sceneName.empty() || m_sceneName == DEFAULT_SCENE_NAME )
			{
				wxMessageBox( _( "Enter the main scene name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, p_parent );
				return false;
			}

			if ( m_path.empty() || m_path == DEFAULT_FOLDER )
			{
				wxMessageBox( _( "Choose the project's folder" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, p_parent );
				return false;
			}

			if ( m_path[m_path.size() - 1] != wxT( '\\' ) && m_path[m_path.size() - 1] != wxT( '/' ) )
			{
				m_path.Append( wxFileName::GetPathSeparator() );
			}

			if ( !DirectoryExists( make_string( m_path ) ) )
			{
				DirectoryCreate( make_string( m_path ) );
			}

			if ( m_bgColour.empty() && m_bgImage.empty() )
			{
				wxMessageBox( _( "Choose a background colour or image" ), TROLL_EDITOR_ERROR, wxOK | wxICON_EXCLAMATION, p_parent );
				return false;
			}

			if ( m_backgroundType == ProjectComponents::bgColour )
			{
				wxGetApp().GetMainFrame()->SetProject( std::make_shared< ProjectComponents::Project >( m_name
					, m_sceneName
					, m_path
					, m_backgroundType
					, m_bgColour
					, m_shadows
					, m_antiAliasing
					, m_resolution ) );
			}
			else
			{
				wxGetApp().GetMainFrame()->SetProject( std::make_shared< ProjectComponents::Project >( m_name
					, m_sceneName
					, m_path
					, m_backgroundType
					, m_bgImage
					, m_shadows
					, m_antiAliasing
					, m_resolution ) );
			}

			return true;
		}

		void NewProjectProperties::DoCreateProperties( wxPGEditor * editor )
		{
			wxString const PROPERTY_PROJECT_NAME = _( "Project Name" );
			wxString const PROPERTY_PROJECT_FOLDER = _( "Project Folder" );
			wxString const PROPERTY_PROJECT_MAIN_SCENE_NAME = _( "Main Scene Name" );
			wxString const PROPERTY_PROJECT_SEPARABLE = _( "Separable scenes" );
			wxString const PROPERTY_PROJECT_RESOLUTION = _( "Resolution" );
			wxString const PROPERTY_PROJECT_BACKGROUND_TYPE = _( "Background type" );
			std::array< wxString, BackgroundTypeCount > const PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS = { _( "Colour" ), _( "Image" ) };
			std::array< int, BackgroundTypeCount > const PROPERTY_PROJECT_BACKGROUND_TYPE_VALUES = { bgColour, bgImage };
			wxString const PROPERTY_PROJECT_BACKGROUND_SELECT = _( "Choose" );
			wxString const PROPERTY_PROJECT_SHADOWS = _( "Shadows" );
			wxString const PROPERTY_PROJECT_ANTIALIASING = _( "Anti Aliasing" );
			std::array< wxString, AntialiasingCount > const PROPERTY_PROJECT_ANTIALIASING_TEXTS = { _( "None" ), _( "2x" ), _( "4x" ) };
			std::array< int, AntialiasingCount > const PROPERTY_PROJECT_ANTIALIASING_VALUES = { aa0, aa2x, aa4x };
			wxString const PROPERTY_PROJECT_CONSOLE = _( "Console" );
			wxString const PROPERTY_PROJECT_SHOW_FPS = _( "Show FPS" );
			wxString const PROPERTY_PROJECT_STARTUP_SCRIPT = _( "Startup Script" );

			wxPGChoices bgTypes{ make_wxArrayString( PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS ), make_wxArrayInt( PROPERTY_PROJECT_BACKGROUND_TYPE_VALUES ) };
			wxString bgType{ PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS[m_backgroundType] };
			wxPGChoices aas{ make_wxArrayString( PROPERTY_PROJECT_ANTIALIASING_TEXTS ), make_wxArrayInt( PROPERTY_PROJECT_ANTIALIASING_VALUES ) };
			wxString aa{ PROPERTY_PROJECT_ANTIALIASING_TEXTS[m_antiAliasing] };

			DoAddProperty( PROPERTY_PROJECT_NAME, m_name, [this]( wxPGProperty * property )
				{
					m_name = property->GetValue().GetString();
				} );
			DoAddProperty( PROPERTY_PROJECT_FOLDER, ButtonEventHandler( NewProjectProperties::DoSelectFolder ), this, editor );
			DoAddProperty( PROPERTY_PROJECT_MAIN_SCENE_NAME, m_sceneName, [this]( wxPGProperty * property )
				{
					m_sceneName = property->GetValue().GetString();
				} );
			DoAddProperty( PROPERTY_PROJECT_SEPARABLE, m_project->GetSeparable(), [this]( wxPGProperty * property )
				{
					m_separable = property->GetValue();
				} );
			DoAddProperty( PROPERTY_PROJECT_RESOLUTION, m_project->GetResolution(), [this]( wxPGProperty * property )
				{
					m_resolution = wxSizeRefFromVariant( property->GetValue() );
				} );
			DoAddProperty( PROPERTY_PROJECT_BACKGROUND_TYPE, bgTypes, bgType, [this]( wxPGProperty * property )
				{
					m_backgroundType = ProjectComponents::BackgroundType( GetValue< int >( property->GetValue() ) );
				} );
			DoAddProperty( PROPERTY_PROJECT_BACKGROUND_SELECT, ButtonEventHandler( NewProjectProperties::DoSelectBackground ), this, editor );
			DoAddProperty( PROPERTY_PROJECT_SHADOWS, m_project->GetShadows(), [this]( wxPGProperty * property )
				{
					m_shadows = property->GetValue();
				} );
			DoAddProperty( PROPERTY_PROJECT_ANTIALIASING, aas, aa, [this]( wxPGProperty * property )
				{
					m_antiAliasing = ProjectComponents::AntiAliasing( GetValue< int >( property->GetValue() ) );
				} );
			DoAddProperty( PROPERTY_PROJECT_CONSOLE, m_project->GetShowDebug(), [this]( wxPGProperty * property )
				{
					m_showDebug = property->GetValue();
				} );
			DoAddProperty( PROPERTY_PROJECT_SHOW_FPS, m_project->GetShowFPS(), [this]( wxPGProperty * property )
				{
					m_showFps = property->GetValue();
				} );
			DoAddProperty( PROPERTY_PROJECT_STARTUP_SCRIPT, ButtonEventHandler( NewProjectProperties::DoChangeStartupScript ), this, editor );
		}

		bool NewProjectProperties::DoSelectFolder( wxPGProperty * p_property )
		{
			wxDirDialog l_dialog( nullptr, _( "Choose a directory" ) );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				m_path = l_dialog.GetPath();
			}

			return false;
		}

		bool NewProjectProperties::DoSelectBackground( wxPGProperty * p_property )
		{
			if ( m_backgroundType == ProjectComponents::bgColour )
			{
				wxColourDialog l_colourDialor;

				if ( l_colourDialor.ShowModal() )
				{
					wxColourData retData = l_colourDialor.GetColourData();
					m_bgColour = retData.GetColour().GetAsString();
				}
			}
			else
			{
				wxString l_imagePath = wxFileSelector( _( "Choose an image" )
					, wxEmptyString
					, wxEmptyString
					, wxEmptyString
					, wxString() << _( "BMP files" ) << wxT( " (*.bmp)|*.bmp|" ) << _( "PNG files" ) << wxT( " (*.png)|*.png|" ) << _( "JPEG files" ) << wxT( " (*.jpg)|*.jpg" ) );

				if ( !l_imagePath.empty() )
				{
					m_bgImage = l_imagePath;
				}
			}

			return false;
		}

		bool NewProjectProperties::DoChangeStartupScript( wxPGProperty * p_property )
		{
			wxString value{ m_startupScript };
			value.Replace( ';', ";\n" );
			ScriptDialog dialog{ nullptr, _( "Startup Script" ), _( "Modify the startup script" ), value };

			if ( dialog.ShowModal() == wxID_OK )
			{
				value = dialog.GetValue();
				value.Replace( ";\n", ";" );
				m_startupScript = value;
			}

			return false;
		}
	}
}
