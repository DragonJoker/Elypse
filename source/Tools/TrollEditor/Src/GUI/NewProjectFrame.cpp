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
			wxString PROPERTY_CATEGORY_PROJECT;
			wxString PROPERTY_PROJECT_RESOLUTION;
			wxString PROPERTY_PROJECT_BACKGROUND_TYPE;
			std::array< wxString, BackgroundTypeCount > PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS;
			std::array< int, BackgroundTypeCount > PROPERTY_PROJECT_BACKGROUND_TYPE_VALUES;
			wxString PROPERTY_PROJECT_BACKGROUND_SELECT;
			wxString PROPERTY_PROJECT_SHADOWS;
			wxString PROPERTY_PROJECT_ANTIALIASING;
			std::array< wxString, AntialiasingCount > PROPERTY_PROJECT_ANTIALIASING_TEXTS;
			std::array< int, AntialiasingCount > PROPERTY_PROJECT_ANTIALIASING_VALUES;
			wxString PROPERTY_PROJECT_CONSOLE;
			wxString PROPERTY_PROJECT_SHOW_FPS;
			wxString PROPERTY_PROJECT_STARTUP_SCRIPT;
			wxString PROPERTY_PROJECT_NAME;
			wxString PROPERTY_PROJECT_MAIN_SCENE_NAME;
			wxString PROPERTY_PROJECT_FOLDER;

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
			PROPERTY_CATEGORY_PROJECT = _( "New Project" );
			PROPERTY_PROJECT_RESOLUTION = _( "Resolution" );
			PROPERTY_PROJECT_BACKGROUND_TYPE = _( "Background type" );
			PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS = { _( "Colour" ), _( "Image" ) };
			PROPERTY_PROJECT_BACKGROUND_TYPE_VALUES = { bgColour, bgImage };
			PROPERTY_PROJECT_BACKGROUND_SELECT = _( "Choose" );
			PROPERTY_PROJECT_SHADOWS = _( "Shadows" );
			PROPERTY_PROJECT_ANTIALIASING = _( "Anti Aliasing" );
			PROPERTY_PROJECT_ANTIALIASING_TEXTS = { _( "None" ), _( "2x" ), _( "4x" ) };
			PROPERTY_PROJECT_ANTIALIASING_VALUES = { aa0, aa2x, aa4x };
			PROPERTY_PROJECT_CONSOLE = _( "Console" );
			PROPERTY_PROJECT_SHOW_FPS = _( "Show FPS" );
			PROPERTY_PROJECT_STARTUP_SCRIPT = _( "Startup Script" );
			PROPERTY_PROJECT_NAME = _( "Project Name" );
			PROPERTY_PROJECT_MAIN_SCENE_NAME = _( "Main Scene Name" );
			PROPERTY_PROJECT_FOLDER = _( "Project Folder" );
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
				wxGetApp().GetMainFrame()->SetProject( std::make_shared< ProjectComponents::Project >( m_name, m_sceneName, m_path, m_backgroundType, m_bgColour, m_shadows, m_antiAliasing, m_resolution ) );
			}
			else
			{
				wxGetApp().GetMainFrame()->SetProject( std::make_shared< ProjectComponents::Project >( m_name, m_sceneName, m_path, m_backgroundType, m_bgImage, m_shadows, m_antiAliasing, m_resolution ) );
			}

			return true;
		}

		void NewProjectProperties::DoCreateProperties( wxPGEditor * p_editor, wxPropertyGrid * p_grid )
		{
			wxPGChoices l_bgTypes{ make_wxArrayString( PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS ), make_wxArrayInt( PROPERTY_PROJECT_BACKGROUND_TYPE_VALUES ) };
			wxString l_bgType{ PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS[m_backgroundType] };
			wxPGChoices l_aas{ make_wxArrayString( PROPERTY_PROJECT_ANTIALIASING_TEXTS ), make_wxArrayInt( PROPERTY_PROJECT_ANTIALIASING_VALUES ) };
			wxString l_aa{ PROPERTY_PROJECT_ANTIALIASING_TEXTS[m_antiAliasing] };

			p_grid->Append( new wxPropertyCategory( PROPERTY_CATEGORY_PROJECT ) );
			p_grid->Append( new wxStringProperty( PROPERTY_PROJECT_NAME, PROPERTY_PROJECT_NAME ) )->SetValue( m_name );
			p_grid->Append( new wxStringProperty( PROPERTY_PROJECT_MAIN_SCENE_NAME, PROPERTY_PROJECT_MAIN_SCENE_NAME ) )->SetValue( m_sceneName );
			p_grid->Append( CreateProperty( PROPERTY_PROJECT_FOLDER, PROPERTY_PROJECT_FOLDER, static_cast< ButtonEventMethod >( &NewProjectProperties::DoSelectFolder ), this, p_editor ) );
			p_grid->Append( new wxSizeProperty( PROPERTY_PROJECT_RESOLUTION, PROPERTY_PROJECT_RESOLUTION, m_resolution ) );
			p_grid->Append( new wxEnumProperty( PROPERTY_PROJECT_BACKGROUND_TYPE, PROPERTY_PROJECT_BACKGROUND_TYPE, l_bgTypes ) )->SetValue( l_bgType );
			p_grid->Append( CreateProperty( PROPERTY_PROJECT_BACKGROUND_SELECT, PROPERTY_PROJECT_BACKGROUND_SELECT, static_cast< ButtonEventMethod >( &NewProjectProperties::DoSelectBackground ), this, p_editor ) );
			p_grid->Append( new wxBoolProperty( PROPERTY_PROJECT_SHADOWS, PROPERTY_PROJECT_SHADOWS ) )->SetValue( m_shadows );
			p_grid->Append( new wxEnumProperty( PROPERTY_PROJECT_ANTIALIASING, PROPERTY_PROJECT_ANTIALIASING, l_aas ) )->SetValue( l_aa );
			p_grid->Append( new wxBoolProperty( PROPERTY_PROJECT_CONSOLE, PROPERTY_PROJECT_CONSOLE ) )->SetValue( m_showDebug );
			p_grid->Append( new wxBoolProperty( PROPERTY_PROJECT_SHOW_FPS, PROPERTY_PROJECT_SHOW_FPS ) )->SetValue( m_showFps );
			p_grid->Append( CreateProperty( PROPERTY_PROJECT_STARTUP_SCRIPT, PROPERTY_PROJECT_STARTUP_SCRIPT, static_cast< ButtonEventMethod >( &NewProjectProperties::DoChangeStartupScript ), this, p_editor ) );
		}

		void NewProjectProperties::DoPropertyChange( wxPropertyGridEvent & p_event )
		{
			wxPGProperty * l_property = p_event.GetProperty();

			if ( l_property )
			{
				if ( l_property->GetName() == PROPERTY_PROJECT_NAME )
				{
					m_name = l_property->GetValueAsString();
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_MAIN_SCENE_NAME )
				{
					m_sceneName = l_property->GetValueAsString();
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_RESOLUTION )
				{
					m_resolution = wxSizeRefFromVariant( l_property->GetValue() );
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_BACKGROUND_TYPE )
				{
					m_backgroundType = ProjectComponents::BackgroundType( GetValue< int >( l_property->GetValue() ) );
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_SHADOWS )
				{
					m_shadows = l_property->GetValue();
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_ANTIALIASING )
				{
					m_antiAliasing = ProjectComponents::AntiAliasing( GetValue< int >( l_property->GetValue() ) );
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_CONSOLE )
				{
					m_showDebug = l_property->GetValue();
				}
				else if ( l_property->GetName() == PROPERTY_PROJECT_SHOW_FPS )
				{
					m_showFps = l_property->GetValue();
				}
			}
		}

		bool NewProjectProperties::DoSelectFolder( wxPGProperty * p_property )
		{
			wxDirDialog l_dialog( NULL, _( "Choose a directory" ) );

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
			wxString l_value{ m_startupScript };
			l_value.Replace( ';', ";\n" );
			ScriptDialog l_dialog{ nullptr, _( "Startup Script" ), _( "Modify the startup script" ), l_value };

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_value = l_dialog.GetValue();
				l_value.Replace( ";\n", ";" );
				m_startupScript = l_value;
			}

			return false;
		}
	}
}
