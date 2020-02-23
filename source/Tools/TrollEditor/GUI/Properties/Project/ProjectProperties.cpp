#include "ProjectProperties.h"

#include "GUI/Properties/Common/AdditionalProperties.h"
#include "GUI/Properties/Common/SizeProperties.h"

#include "GUI/ScriptDialog.h"

#include "Project/Project.h"

namespace Troll
{
	using namespace ProjectComponents;

	namespace GUI
	{
		namespace Properties
		{
			ProjectProperties::ProjectProperties( ProjectComponents::Project & p_project )
				: ObjectProperty{ DataType::Project }
				, m_project{ p_project }
			{
			}

			ProjectProperties::~ProjectProperties()
			{
			}

			void ProjectProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_PROJECT = _( "Project: " );
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
				wxString const PROPERTY_PROJECT_SEPARABLE = _( "Separable scenes" );
				wxString const PROPERTY_PROJECT_STARTUP_SCRIPT = _( "Startup Script" );

				wxPGChoices l_bgTypes{ make_wxArrayString( PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS ), make_wxArrayInt( PROPERTY_PROJECT_BACKGROUND_TYPE_VALUES ) };
				wxString l_bgType{ PROPERTY_PROJECT_BACKGROUND_TYPE_TEXTS[m_project.GetBackgroundType()] };
				wxPGChoices l_aas{ make_wxArrayString( PROPERTY_PROJECT_ANTIALIASING_TEXTS ), make_wxArrayInt( PROPERTY_PROJECT_ANTIALIASING_VALUES ) };
				wxString l_aa{ PROPERTY_PROJECT_ANTIALIASING_TEXTS[m_project.GetFSAA()] };

				DoAddCategory( PROPERTY_CATEGORY_PROJECT + wxString( m_project.GetName() ) );
				DoAddProperty( PROPERTY_PROJECT_RESOLUTION, m_project.GetResolution(), [this]( wxPGProperty * p_property )
				{
					m_project.SetResolution( wxSizeRefFromVariant( p_property->GetValue() ) );
				} );
				DoAddProperty( PROPERTY_PROJECT_BACKGROUND_TYPE, l_bgTypes, l_bgType, [this]( wxPGProperty * p_property )
				{
					m_project.SetBackgroundType( ProjectComponents::BackgroundType( GetValue< int >( p_property->GetValue() ) ) );
				} );
				DoAddProperty( PROPERTY_PROJECT_BACKGROUND_SELECT, ButtonEventHandler( ProjectProperties::DoSelectBackground ), this, p_editor );
				DoAddProperty( PROPERTY_PROJECT_SHADOWS, m_project.GetShadows(), [this]( wxPGProperty * p_property )
				{
					m_project.SetShadows( p_property->GetValue() );
				} );
				DoAddProperty( PROPERTY_PROJECT_ANTIALIASING, l_aas, l_aa, [this]( wxPGProperty * p_property )
				{
					m_project.SetFSAA( ProjectComponents::AntiAliasing( GetValue< int >( p_property->GetValue() ) ) );
				} );
				DoAddProperty( PROPERTY_PROJECT_CONSOLE, m_project.GetShowDebug(), [this]( wxPGProperty * p_property )
				{
					m_project.SetShowDebug( p_property->GetValue() );
				} );
				DoAddProperty( PROPERTY_PROJECT_SHOW_FPS, m_project.GetShowFPS(), [this]( wxPGProperty * p_property )
				{
					m_project.SetShowFPS( p_property->GetValue() );
				} );
				DoAddProperty( PROPERTY_PROJECT_SEPARABLE, m_project.GetSeparable(), [this]( wxPGProperty * p_property )
				{
					m_project.SetSeparable( p_property->GetValue() );
				} );
				DoAddProperty( PROPERTY_PROJECT_STARTUP_SCRIPT, ButtonEventHandler( ProjectProperties::DoChangeStartupScript ), this, p_editor );
			}

			bool ProjectProperties::DoSelectBackground( wxPGProperty * p_property )
			{
				if ( m_project.GetBackgroundType() == ProjectComponents::bgColour )
				{
					wxColourDialog l_colourDialor;

					if ( l_colourDialor.ShowModal() )
					{
						wxColourData retData = l_colourDialor.GetColourData();
						m_project.SetBackgroundColour( retData.GetColour().GetAsString() );
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
						m_project.SetBackgroundImage( l_imagePath );
					}
				}

				return false;
			}

			bool ProjectProperties::DoChangeStartupScript( wxPGProperty * p_property )
			{
				wxString l_value{ m_project.GetStartupScript() };
				l_value.Replace( ';', ";\n" );
				ScriptDialog l_dialog{ nullptr, _( "Startup Script" ), _( "Modify the startup script" ), l_value };

				if ( l_dialog.ShowModal() == wxID_OK )
				{
					l_value = l_dialog.GetValue();
					l_value.Replace( ";\n", ";" );
					m_project.SetStartupScript( l_value );
				}

				return false;
			}
		}
	}
}
