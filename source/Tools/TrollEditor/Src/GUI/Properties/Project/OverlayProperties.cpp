#include "OverlayProperties.h"

#include "GUI/Properties/Common/VectorProperties.h"

#include "Project/2D/Overlay.h"

#include "GUI/ProjectFrame.h"

#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"

#include <wx/propgrid/advprops.h>

using namespace Elypse::Media;

namespace Troll
{
	using namespace ProjectComponents;
	using namespace ProjectComponents::Objects2D;

	namespace GUI
	{
		namespace Properties
		{
			OverlayProperties::OverlayProperties( TrollOverlay & p_overlay )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_OVERLAY }
				, m_overlay( p_overlay )
			{
			}

			OverlayProperties::~OverlayProperties()
			{
			}

			void OverlayProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_OVERLAY = _( "Overlay: " );
				wxString const PROPERTY_OVERLAY_BASE_MATERIAL_NAME = _( "Base Material" );
				wxString const PROPERTY_OVERLAY_MOUSE_OVER_MATERIAL_NAME = _( "MouseOver Material" );
				wxString const PROPERTY_OVERLAY_CLICKED_MATERIAL_NAME = _( "Clicked Material" );
				wxString const PROPERTY_OVERLAY_VERTICAL_ALIGN = _( "Vertical Align" );
				std::array< wxString, 4 > const PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS = { _( "None" ), _( "Top" ), _( "Center" ), _( "Bottom" ) };
				std::array< int, 4 > const PROPERTY_OVERLAY_VERTICAL_ALIGN_VALUES = { vNone + 1, vTop + 1, vCenter + 1, vBottom + 1 };
				wxString const PROPERTY_OVERLAY_HORIZONTAL_ALIGN = _( "Horizontal Align" );
				std::array< wxString, 4 > const PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS = { _( "None" ), _( "Left" ), _( "Center" ), _( "Right" ) };
				std::array< int, 4 > const PROPERTY_OVERLAY_HORIZONTAL_ALIGN_VALUES = { hNone + 1, hLeft + 1, hCenter + 1, hRight + 1 };
				wxString const PROPERTY_OVERLAY_POSITION = _( "Position" );
				wxString const PROPERTY_OVERLAY_SIZE = _( "Size" );
				wxString const PROPERTY_OVERLAY_VISIBILITY = _( "Visibility" );
				wxString const PROPERTY_OVERLAY_VIDEO = _( "Video" );

				wxPGChoices l_vAligns{ make_wxArrayString( PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS ), make_wxArrayInt( PROPERTY_OVERLAY_VERTICAL_ALIGN_VALUES ) };
				wxString l_vAlign{ PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS[m_overlay.GetVerticalAlign()] };
				wxPGChoices l_hAligns{ make_wxArrayString( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS ), make_wxArrayInt( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_VALUES ) };
				wxString l_hAlign{ PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS[m_overlay.GetHorizontalAlign()] };
				wxArrayString l_videos = DoGetVideosNames();
				auto l_video = m_overlay.GetVideo();
				wxString l_videoName = _( "None" );

				if ( l_video )
				{
					l_videoName = l_video->GetVideo()->GetObjectName();
				}

				DoAddCategory( PROPERTY_CATEGORY_OVERLAY + wxString( m_overlay.GetMuseOverlay()->GetName() ) );
				DoAddProperty( PROPERTY_OVERLAY_BASE_MATERIAL_NAME, m_overlay.GetBaseMaterial(), [this]( wxPGProperty * p_property )
				{
					m_overlay.SetBaseMaterial( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_overlay.GetBaseMaterial() );
				} );
				DoAddProperty( PROPERTY_OVERLAY_MOUSE_OVER_MATERIAL_NAME, m_overlay.GetMouseOverMaterial(), [this]( wxPGProperty * p_property )
				{
					m_overlay.SetMouseOverMaterial( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_overlay.GetMouseOverMaterial() );
				} );
				DoAddProperty( PROPERTY_OVERLAY_CLICKED_MATERIAL_NAME, m_overlay.GetClickedMaterial(), [this]( wxPGProperty * p_property )
				{
					m_overlay.SetClickedMaterial( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_overlay.GetClickedMaterial() );
				} );
				DoAddProperty( PROPERTY_OVERLAY_VERTICAL_ALIGN, l_vAligns, l_vAlign, [this]( wxPGProperty * p_property )
				{
					m_overlay.SetVerticalAlign( TEVerticAlign( GetValue< int >( p_property->GetValue() ) - 1 ) );
				},
				[this, PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS]( wxPGProperty * p_property )
				{
					p_property->SetValue( PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS[m_overlay.GetVerticalAlign()] );
				} );
				DoAddProperty( PROPERTY_OVERLAY_HORIZONTAL_ALIGN, l_hAligns, l_hAlign, [this]( wxPGProperty * p_property )
				{
					m_overlay.SetHorizontalAlign( TEHorizAlign( GetValue< int >( p_property->GetValue() ) - 1 ) );
				},
				[this, PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS]( wxPGProperty * p_property )
				{
					p_property->SetValue( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS[m_overlay.GetHorizontalAlign()] );
				} );
				DoAddProperty< Vector2Names::Point >( PROPERTY_OVERLAY_POSITION, m_overlay.GetPosition(), [this]( wxPGProperty * p_property )
				{
					m_overlay.SetPosition( Vector2RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_overlay.GetPosition() ) );
				} );
				DoAddProperty< Vector2Names::Size >( PROPERTY_OVERLAY_SIZE, m_overlay.GetSize(), [this]( wxPGProperty * p_property )
				{
					m_overlay.SetSize( Vector2RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_overlay.GetSize() ) );
				} );
				DoAddProperty( PROPERTY_OVERLAY_VISIBILITY, m_overlay.IsVisible(), [this]( wxPGProperty * p_property )
				{
					m_overlay.SetVisible( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_overlay.IsVisible() );
				} );
				DoAddProperty( PROPERTY_OVERLAY_VIDEO, l_videos, l_videoName, [this]( wxPGProperty * p_property )
				{
					auto l_video = wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetVideo( p_property->GetValue() );

					if ( l_video )
					{
						auto l_videoObject = l_video->AddObject( &m_overlay );

						if ( l_videoObject )
						{
							m_overlay.SetVideo( l_videoObject );
						}
					}
				},
				[this]( wxPGProperty * p_property )
				{
					auto l_video = m_overlay.GetVideo();
					wxString l_videoName = _( "None" );

					if ( l_video )
					{
						l_videoName = l_video->GetVideo()->GetObjectName();
					}

					p_property->SetValue( l_videoName );
				} );

				DoCreateTextProperties( p_editor );
				DoCreateBorderProperties( p_editor );
			}

			void OverlayProperties::DoCreateTextProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_OVERLAY_TEXT_CAPTION = _( "Text Caption" );
				wxString const PROPERTY_OVERLAY_TEXT_FONT_NAME = _( "Text Font Name" );
				wxString const PROPERTY_OVERLAY_TEXT_FONT_SIZE = _( "Text Font Size" );
				wxString const PROPERTY_OVERLAY_TEXT_COLOUR = _( "Text Colour" );
				wxString const PROPERTY_OVERLAY_TEXT_OPACITY = _( "Text Opacity" );
				wxString const PROPERTY_OVERLAY_TEXT_VERTICAL_ALIGN = _( "Text Vertical Align" );
				std::array< wxString, 4 > const PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS = { _( "None" ), _( "Top" ), _( "Center" ), _( "Bottom" ) };
				std::array< int, 4 > const PROPERTY_OVERLAY_VERTICAL_ALIGN_VALUES = { vNone + 1, vTop + 1, vCenter + 1, vBottom + 1 };
				wxString const PROPERTY_OVERLAY_TEXT_HORIZONTAL_ALIGN = _( "Text Horizontal Align" );
				std::array< wxString, 4 > const PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS = { _( "None" ), _( "Left" ), _( "Center" ), _( "Right" ) };
				std::array< int, 4 > const PROPERTY_OVERLAY_HORIZONTAL_ALIGN_VALUES = { hNone + 1, hLeft + 1, hCenter + 1, hRight + 1 };

				wxPGChoices l_vAligns{ make_wxArrayString( PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS ), make_wxArrayInt( PROPERTY_OVERLAY_VERTICAL_ALIGN_VALUES ) };
				wxString l_vAlign{ PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS[m_overlay.GetVerticalAlign() + 1] };
				wxPGChoices l_hAligns{ make_wxArrayString( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS ), make_wxArrayInt( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_VALUES ) };
				wxString l_hAlign{ PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS[m_overlay.GetHorizontalAlign() + 1] };

				auto l_textInfos = m_overlay.GetTextInfos();

				if ( l_textInfos )
				{
					wxPGChoices l_vAligns{ make_wxArrayString( PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS ), make_wxArrayInt( PROPERTY_OVERLAY_VERTICAL_ALIGN_VALUES ) };
					wxString l_vAlign{ PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS[l_textInfos->m_verticalAlign + 1] };
					wxPGChoices l_hAligns{ make_wxArrayString( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS ), make_wxArrayInt( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_VALUES ) };
					wxString l_hAlign{ PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS[l_textInfos->m_horizontalAlign + 1] };

					DoAddProperty( PROPERTY_OVERLAY_TEXT_CAPTION, l_textInfos->m_caption, [this]( wxPGProperty * p_property )
					{
						m_overlay.SetCaption( p_property->GetValue() );
					},
					[l_textInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_textInfos->m_caption );
					} );
					DoAddProperty( PROPERTY_OVERLAY_TEXT_FONT_NAME, l_textInfos->m_fontName, [this]( wxPGProperty * p_property )
					{
						m_overlay.SetFontName( p_property->GetValue() );
					},
					[l_textInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_textInfos->m_fontName );
					} );
					DoAddProperty( PROPERTY_OVERLAY_TEXT_FONT_SIZE, l_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight(), [this]( wxPGProperty * p_property )
					{
						m_overlay.SetFontSize( Real( double( p_property->GetValue() ) / ScriptEngine::GetContext()->mainViewport->getActualHeight() ) );
					},
					[l_textInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
					} );
					DoAddProperty( PROPERTY_OVERLAY_TEXT_COLOUR, make_wxColour( l_textInfos->m_textColour ), [this]( wxPGProperty * p_property )
					{
						wxColour l_colour;
						m_overlay.SetTextColour( make_ColourValue( l_colour << p_property->GetValue() ) );
					},
					[l_textInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( WXVARIANT( make_wxColour( l_textInfos->m_textColour ) ) );
					} );
					DoAddProperty( PROPERTY_OVERLAY_TEXT_OPACITY, l_textInfos->m_textColour.a, [this]( wxPGProperty * p_property )
					{
						ColourValue l_colour = m_overlay.GetTextInfos()->m_textColour;
						l_colour.a = Real( double( p_property->GetValue() ) );
						m_overlay.SetTextColour( l_colour );
					},
					[l_textInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_textInfos->m_textColour.a );
					} );
					DoAddProperty( PROPERTY_OVERLAY_TEXT_VERTICAL_ALIGN, l_vAligns, l_vAlign, [this]( wxPGProperty * p_property )
					{
						m_overlay.SetTextVerticalAlign( TEVerticAlign( GetValue< int >( p_property->GetValue() ) - 1 ) );
					},
					[l_textInfos, PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS]( wxPGProperty * p_property )
					{
						p_property->SetValue( PROPERTY_OVERLAY_VERTICAL_ALIGN_TEXTS[l_textInfos->m_verticalAlign + 1] );
					} );
					DoAddProperty( PROPERTY_OVERLAY_TEXT_HORIZONTAL_ALIGN, l_hAligns, l_hAlign, [this]( wxPGProperty * p_property )
					{
						m_overlay.SetTextHorizontalAlign( TEHorizAlign( GetValue< int >( p_property->GetValue() ) - 1 ) );
					},
					[l_textInfos, PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS]( wxPGProperty * p_property )
					{
						p_property->SetValue( PROPERTY_OVERLAY_HORIZONTAL_ALIGN_TEXTS[l_textInfos->m_horizontalAlign + 1] );
					} );
				}
			}

			void OverlayProperties::DoCreateBorderProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_OVERLAY_BORDER_MATERIAL = _( "Border Material" );
				wxString const PROPERTY_OVERLAY_BORDER_TOP = _( "Border Top Size" );
				wxString const PROPERTY_OVERLAY_BORDER_BOTTOM = _( "Border Bottom Size" );
				wxString const PROPERTY_OVERLAY_BORDER_LEFT = _( "Border Left Size" );
				wxString const PROPERTY_OVERLAY_BORDER_RIGHT = _( "Border Right Size" );

				auto l_borderInfos = m_overlay.GetBorderInfos();

				if ( l_borderInfos )
				{
					DoAddProperty( PROPERTY_OVERLAY_BORDER_MATERIAL, l_borderInfos->m_material, [this]( wxPGProperty * p_property )
					{
						m_overlay.SetBorderMaterial( p_property->GetValue() );
					},
					[l_borderInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_borderInfos->m_material );
					} );
					DoAddProperty( PROPERTY_OVERLAY_BORDER_TOP, l_borderInfos->m_top * ScriptEngine::GetContext()->mainViewport->getActualHeight(), [this]( wxPGProperty * p_property )
					{
						m_overlay.SetBorderTop( Real( double( p_property->GetValue() ) / ScriptEngine::GetContext()->mainViewport->getActualWidth() ) );
					},
					[l_borderInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_borderInfos->m_top * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
					} );
					DoAddProperty( PROPERTY_OVERLAY_BORDER_BOTTOM, l_borderInfos->m_bottom * ScriptEngine::GetContext()->mainViewport->getActualHeight(), [this]( wxPGProperty * p_property )
					{
						m_overlay.SetBorderBottom( Real( double( p_property->GetValue() ) / ScriptEngine::GetContext()->mainViewport->getActualWidth() ) );
					},
					[l_borderInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_borderInfos->m_bottom * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
					} );
					DoAddProperty( PROPERTY_OVERLAY_BORDER_LEFT, l_borderInfos->m_left * ScriptEngine::GetContext()->mainViewport->getActualWidth(), [this]( wxPGProperty * p_property )
					{
						m_overlay.SetBorderLeft( Real( double( p_property->GetValue() ) / ScriptEngine::GetContext()->mainViewport->getActualWidth() ) );
					},
					[l_borderInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_borderInfos->m_left * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
					} );
					DoAddProperty( PROPERTY_OVERLAY_BORDER_RIGHT, l_borderInfos->m_right * ScriptEngine::GetContext()->mainViewport->getActualWidth(), [this]( wxPGProperty * p_property )
					{
						m_overlay.SetBorderRight( Real( double( p_property->GetValue() ) / ScriptEngine::GetContext()->mainViewport->getActualWidth() ) );
					},
					[l_borderInfos]( wxPGProperty * p_property )
					{
						p_property->SetValue( l_borderInfos->m_right * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
					} );
				}
			}

			wxArrayString OverlayProperties::DoGetVideosNames()
			{
				wxArrayString l_return;
				l_return.push_back( _( "None" ) );

				for ( auto l_it : VideoManager::GetSingletonPtr()->GetVideoMap() )
				{
					l_return.push_back( make_wxString( l_it.first ) );
				}

				return l_return;
			}
		}
	}
}
