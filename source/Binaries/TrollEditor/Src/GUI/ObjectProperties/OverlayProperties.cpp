
#include "PrecompiledHeader.h"

#include "GUI/ObjectProperties/OverlayProperties.h"
#include "Project/2D/Overlay.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"
#include "GUI/MainFrame.h"
/*
#include <Main/Context.h>
#include <VideoManager.h>
*/

using namespace Troll::GUI;
using namespace Troll::Objects2D;
using namespace Troll::Media;
using namespace EMuse::Media;

enum OverlayPropsIDs
{
	overlayName,
	overlayBaseMaterialName,
	overlayMouseOverMaterialName,
	overlayClickedMaterialName,
	overlayVerticalAlign,
	overlayHorizontalAlign,
	overlayTop,
	overlayLeft,
	overlayWidth,
	overlayHeight,
	overlayCaption,
	overlayFontName,
	overlayFontSize,
	overlayTextRedValue,
	overlayTextGreenValue,
	overlayTextBlueValue,
	overlayTextAlphaValue,
	overlayTextVerticalAlign,
	overlayTextHorizontalAlign,
	overlayVisibility,
	overlayBorderMaterial,
	overlayBorderTop,
	overlayBorderBottom,
	overlayBorderLeft,
	overlayBorderRight,
	overlayVideo
};

BEGIN_EVENT_TABLE( OverlayProperties, wxPanel )
//	EVT_TEXT_ENTER(	overlayName,					OverlayProperties::_onEditName)
	EVT_TEXT_ENTER(	overlayBaseMaterialName,		OverlayProperties::_onEditBaseMaterialName )
	EVT_TEXT_ENTER(	overlayMouseOverMaterialName,	OverlayProperties::_onEditMouseOverMaterialName )
	EVT_TEXT_ENTER(	overlayClickedMaterialName,		OverlayProperties::_onEditClickedMaterialName )
	EVT_COMBOBOX(	overlayVerticalAlign,			OverlayProperties::_onSelectVerticalAlign )
	EVT_COMBOBOX(	overlayHorizontalAlign,			OverlayProperties::_onSelectHorizontalAlign )
	EVT_TEXT_ENTER(	overlayTop,						OverlayProperties::_onEditTop )
	EVT_TEXT_ENTER(	overlayLeft,					OverlayProperties::_onEditLeft )
	EVT_TEXT_ENTER(	overlayWidth,					OverlayProperties::_onEditWidth )
	EVT_TEXT_ENTER(	overlayHeight,					OverlayProperties::_onEditHeight )

	EVT_TEXT_ENTER(	overlayBorderMaterial,			OverlayProperties::_onEditBorderMaterial )
	EVT_TEXT_ENTER(	overlayBorderTop,				OverlayProperties::_onEditBorderTop )
	EVT_TEXT_ENTER(	overlayBorderBottom,			OverlayProperties::_onEditBorderBottom )
	EVT_TEXT_ENTER(	overlayBorderLeft,				OverlayProperties::_onEditBorderLeft )
	EVT_TEXT_ENTER(	overlayBorderRight,				OverlayProperties::_onEditBorderRight )

	EVT_TEXT_ENTER(	overlayCaption,					OverlayProperties::_onEditCaption )
	EVT_TEXT_ENTER(	overlayFontName,				OverlayProperties::_onEditFontName )
	EVT_TEXT_ENTER(	overlayFontSize,				OverlayProperties::_onEditFontSize )
	EVT_TEXT_ENTER(	overlayTextRedValue,			OverlayProperties::_onEditTextRedValue )
	EVT_TEXT_ENTER(	overlayTextGreenValue,			OverlayProperties::_onEditTextGreenValue )
	EVT_TEXT_ENTER(	overlayTextBlueValue,			OverlayProperties::_onEditTextBlueValue )
	EVT_TEXT_ENTER(	overlayTextAlphaValue,			OverlayProperties::_onEditTextAlphaValue )
	EVT_COMBOBOX(	overlayVerticalAlign,			OverlayProperties::_onSelectTextVerticalAlign )
	EVT_COMBOBOX(	overlayHorizontalAlign,			OverlayProperties::_onSelectTextHorizontalAlign )
	EVT_COMBOBOX(	overlayVisibility,				OverlayProperties::_onSelectVisibility )
	EVT_COMBOBOX(	overlayVideo,					OverlayProperties::_onSelectVideo )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


OverlayProperties::OverlayProperties( TrollOverlay * p_overlay, wxWindow * parent, const wxPoint & pos,
									  const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_overlay( p_overlay )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_overlayName = new wxStaticText( this, overlayName, m_overlay->GetName(), l_position );
//	m_overlayName = BuildTextCtrl( this, l_position, overlayName, m_overlay->GetName());
	m_baseMaterialName = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayBaseMaterialName, wxT( "Base Material" ), m_overlay->GetBaseMaterial() );
	m_mouseOverMaterialName = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayMouseOverMaterialName, wxT( "MouseOver Material" ), m_overlay->GetMouseOverMaterial() );
	m_clickedMaterialName = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayClickedMaterialName, wxT( "Clicked Material" ), m_overlay->GetClickedMaterial() );
	int l_align = static_cast <int>( m_overlay->GetVerticalAlign() );
	wxArrayString l_vAligns;
	l_vAligns.Add( wxT( "None" ) );
	l_vAligns.Add( wxT( "Top" ) );
	l_vAligns.Add( wxT( "Center" ) );
	l_vAligns.Add( wxT( "Bottom" ) );
	m_verticalAlign = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayVerticalAlign, wxT( "Vertical Align" ), l_vAligns, l_vAligns[l_align + 1] );
	l_align = static_cast <int>( m_overlay->GetHorizontalAlign() );
	wxArrayString l_hAligns;
	l_hAligns.Add( wxT( "None" ) );
	l_hAligns.Add( wxT( "Left" ) );
	l_hAligns.Add( wxT( "Center" ) );
	l_hAligns.Add( wxT( "Right" ) );
	m_horizontalAlign = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayHorizontalAlign, wxT( "Horizontal Align" ), l_hAligns, l_hAligns[l_align + 1] );
	m_top = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayTop, wxT( "Top" ), m_overlay->GetTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	m_left = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayLeft, wxT( "Left" ), m_overlay->GetLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
	m_width = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayWidth, wxT( "Width" ), m_overlay->GetWidth() * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
	m_height = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayHeight, wxT( "Height" ), m_overlay->GetHeight() * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	_buildBorderProperties( l_position );
	_buildTextProperties( l_position );
	wxArrayString l_visibility;
	l_visibility.Add( wxT( "Visible" ) );
	l_visibility.Add( wxT( "Hidden" ) );
	m_visibility = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayVisibility, wxT( "Visibility" ), l_visibility, wxT( "Visible" ) );
	wxArrayString l_videos;
	l_videos.push_back( wxT( "Aucune" ) );
	_getVideosNames( l_videos );
	TrollVideoObject * l_video = m_overlay->GetVideo();
	wxString l_videoName = wxT( "Aucune" );

	if ( l_video != NULL )
	{
		l_videoName = l_video->GetVideo()->GetName();
	}

	m_video = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayVideo, wxT( "Vidéo" ), l_videos, l_videoName );
}



OverlayProperties::~OverlayProperties()
{
}



void OverlayProperties::UpdateProperties()
{
	m_updatingProperties = true;
//	m_overlayName->SetValue( m_overlay->GetName());
	m_overlayName->SetLabel( m_overlay->GetName() );
	m_baseMaterialName->SetValue( m_overlay->GetBaseMaterial() );
	m_mouseOverMaterialName->SetValue( m_overlay->GetMouseOverMaterial() );
	m_clickedMaterialName->SetValue( m_overlay->GetClickedMaterial() );
	int l_align = static_cast <int>( m_overlay->GetVerticalAlign() );
	wxArrayString l_vAligns;
	l_vAligns.Add( wxT( "None" ) );
	l_vAligns.Add( wxT( "Top" ) );
	l_vAligns.Add( wxT( "Center" ) );
	l_vAligns.Add( wxT( "Bottom" ) );
	m_verticalAlign->SetValue( l_vAligns[l_align + 1] );
	l_align = static_cast <int>( m_overlay->GetHorizontalAlign() );
	wxArrayString l_hAligns;
	l_hAligns.Add( wxT( "None" ) );
	l_hAligns.Add( wxT( "Left" ) );
	l_hAligns.Add( wxT( "Center" ) );
	l_hAligns.Add( wxT( "Right" ) );
	m_horizontalAlign->SetValue( l_hAligns[l_align + 1] );
	wxString l_tmp;
	l_tmp << m_overlay->GetTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight();
	m_top->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << m_overlay->GetLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth();
	m_left->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << m_overlay->GetWidth() * ScriptEngine::GetContext()->mainViewport->getActualWidth();
	m_width->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << m_overlay->GetHeight() * ScriptEngine::GetContext()->mainViewport->getActualHeight();
	m_height->SetValue( l_tmp );
	l_tmp.clear();
	_updateBorderProperties();
	_updateTextProperties();
	m_updatingProperties = false;
}



void OverlayProperties::_buildTextProperties( wxPoint & p_position )
{
	TextInfos * l_textInfos = m_overlay->GetTextInfos();

	if ( l_textInfos != NULL )
	{
		int l_align;
		wxArrayString l_vAligns;
		l_vAligns.Add( wxT( "None" ) );
		l_vAligns.Add( wxT( "Top" ) );
		l_vAligns.Add( wxT( "Center" ) );
		l_vAligns.Add( wxT( "Bottom" ) );
		wxArrayString l_hAligns;
		l_hAligns.Add( wxT( "None" ) );
		l_hAligns.Add( wxT( "Left" ) );
		l_hAligns.Add( wxT( "Center" ) );
		l_hAligns.Add( wxT( "Right" ) );
		m_caption = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayCaption, wxT( "Caption" ), l_textInfos->m_caption );
		m_fontName = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayFontName, wxT( "Font Name" ), l_textInfos->m_fontName );
		m_fontSize = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayFontSize, wxT( "Font Size" ), l_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		m_redTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextRedValue, wxT( "Red Value" ), l_textInfos->m_textColour.r );
		m_greenTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextGreenValue, wxT( "Green Value" ), l_textInfos->m_textColour.g );
		m_blueTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextBlueValue, wxT( "Blue Value" ), l_textInfos->m_textColour.b );
		m_alphaTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextAlphaValue, wxT( "Alpha Value" ), l_textInfos->m_textColour.a );
		l_align = static_cast <int>( l_textInfos->m_verticalAlign );
		m_verticalAlign = BuildComboBox( this, p_position += wxPoint( 0, 20 ), overlayTextVerticalAlign, wxT( "Text Vertical Align" ), l_vAligns, l_vAligns[l_align + 1] );
		l_align = static_cast <int>( l_textInfos->m_horizontalAlign );
		m_horizontalAlign = BuildComboBox( this, p_position += wxPoint( 0, 20 ), overlayTextHorizontalAlign, wxT( "Text Horizontal Align" ), l_hAligns, l_hAligns[l_align + 1] );
	}
}



void OverlayProperties::_buildBorderProperties( wxPoint & p_position )
{
	BorderInfos * l_borderInfos = m_overlay->GetBorderInfos();

	if ( l_borderInfos != NULL )
	{
		m_borderMaterialName = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderMaterial, wxT( "Border Material" ), l_borderInfos->m_material );
		m_borderTop = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderTop, wxT( "Top Border Size" ), l_borderInfos->m_top * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		m_borderBottom = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderBottom, wxT( "Bottom Border Size" ), l_borderInfos->m_bottom * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		m_borderLeft = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderLeft, wxT( "Left Border Size" ), l_borderInfos->m_left * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		m_borderRight = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderRight, wxT( "Right Border Size" ), l_borderInfos->m_right * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
	}
}



void OverlayProperties::_getVideosNames( wxArrayString & p_names )
{
	const VideoObjectMap & l_map = VideoManager::GetSingletonPtr()->GetVideoMap();
	VideoObjectMap::const_iterator l_it = l_map.begin();

	for ( ; l_it != l_map.end() ; ++ l_it )
	{
		p_names.push_back( make_wxString( l_it->first ) );
	}
}



void OverlayProperties::_updateTextProperties()
{
	TextInfos * l_textInfos = m_overlay->GetTextInfos();

	if ( l_textInfos != NULL )
	{
		wxString l_tmp;
		int l_align;
		wxArrayString l_vAligns;
		l_vAligns.Add( wxT( "None" ) );
		l_vAligns.Add( wxT( "Top" ) );
		l_vAligns.Add( wxT( "Center" ) );
		l_vAligns.Add( wxT( "Bottom" ) );
		wxArrayString l_hAligns;
		l_hAligns.Add( wxT( "None" ) );
		l_hAligns.Add( wxT( "Left" ) );
		l_hAligns.Add( wxT( "Center" ) );
		l_hAligns.Add( wxT( "Right" ) );
		m_caption->SetValue( l_textInfos->m_caption );
		m_fontName->SetValue( l_textInfos->m_fontName );
		l_tmp << l_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight();
		m_fontSize->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_textInfos->m_textColour.r;
		m_redTextValue->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_textInfos->m_textColour.g;
		m_greenTextValue->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_textInfos->m_textColour.b;
		m_blueTextValue->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_textInfos->m_textColour.a;
		m_alphaTextValue->SetValue( l_tmp );
		l_tmp.clear();
		l_align = static_cast <int>( l_textInfos->m_verticalAlign );
		m_verticalAlign->SetValue( l_vAligns[l_align + 1] );
		l_align = static_cast <int>( l_textInfos->m_horizontalAlign );
		m_horizontalAlign->SetValue( l_hAligns[l_align + 1] );
	}
}



void OverlayProperties::_updateBorderProperties()
{
	BorderInfos * l_borderInfos = m_overlay->GetBorderInfos();

	if ( l_borderInfos != NULL )
	{
		wxString l_tmp;
		m_borderMaterialName->SetValue( l_borderInfos->m_material );
		l_tmp << l_borderInfos->m_top * ScriptEngine::GetContext()->mainViewport->getActualHeight();
		m_borderTop->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_borderInfos->m_bottom * ScriptEngine::GetContext()->mainViewport->getActualHeight();
		m_borderBottom->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_borderInfos->m_left * ScriptEngine::GetContext()->mainViewport->getActualWidth();
		m_borderLeft->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_borderInfos->m_right * ScriptEngine::GetContext()->mainViewport->getActualWidth();
		m_borderRight->SetValue( l_tmp );
		l_tmp.clear();
	}
}



void OverlayProperties::_onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
//		m_overlay->SetName( m_overlayName->GetValue());
		m_overlay->SetName( m_overlayName->GetLabel() );
	}
}



void OverlayProperties::_onEditBaseMaterialName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_overlay->SetBaseMaterial( m_baseMaterialName->GetValue() );
	}
}



void OverlayProperties::_onEditMouseOverMaterialName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_overlay->SetMouseOverMaterial( m_mouseOverMaterialName->GetValue() );
	}
}



void OverlayProperties::_onEditClickedMaterialName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_overlay->SetClickedMaterial( m_clickedMaterialName->GetValue() );
	}
}



void OverlayProperties::_onSelectVerticalAlign( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = p_event.GetString();

		if ( l_value == wxT( "Top" ) )
		{
			m_overlay->SetVerticalAlign( vTop );
		}
		else if ( l_value == wxT( "Center" ) )
		{
			m_overlay->SetVerticalAlign( vCenter );
		}
		else if ( l_value == wxT( "Bottom" ) )
		{
			m_overlay->SetVerticalAlign( vBottom );
		}

		wxString l_tmp;
		l_tmp << m_overlay->GetTop();
		m_top->SetValue( l_tmp );
	}
}



void OverlayProperties::_onSelectHorizontalAlign( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = p_event.GetString();

		if ( l_value == wxT( "Left" ) )
		{
			m_overlay->SetHorizontalAlign( hLeft );
		}
		else if ( l_value == wxT( "Center" ) )
		{
			m_overlay->SetHorizontalAlign( hCenter );
		}
		else if ( l_value == wxT( "Right" ) )
		{
			m_overlay->SetHorizontalAlign( hRight );
		}

		wxString l_tmp;
		l_tmp << m_overlay->GetLeft();
		m_left->SetValue( l_tmp );
	}
}



void OverlayProperties::_onEditTop( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_top->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetTop( l_res / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		}
	}
}



void OverlayProperties::_onEditLeft( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_left->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetLeft( l_res / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		}
	}
}



void OverlayProperties::_onEditWidth( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_width->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetWidth( l_res / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		}
	}
}



void OverlayProperties::_onEditHeight( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_height->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetHeight( l_res / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		}
	}
}



void OverlayProperties::_onSelectVisibility( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = p_event.GetString();

		if ( l_value == wxT( "Visible" ) )
		{
			m_overlay->SetVisible( true );
		}
		else if ( l_value == wxT( "Hidden" ) )
		{
			m_overlay->SetVisible( false );
		}
	}
}



void OverlayProperties::_onSelectVideo( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		TrollVideo * l_video = g_mainFrame->GetCurrentProject()->GetMainScene()->GetVideo( p_event.GetString() );

		if ( l_video != NULL )
		{
			TrollVideoObject * l_videoObject = l_video->AddObject( m_overlay );

			if ( l_videoObject != NULL )
			{
				m_overlay->SetVideo( l_videoObject );
			}
		}
	}
}



void OverlayProperties::_onEditBorderMaterial( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_borderMaterialName->GetValue();
		m_overlay->SetBorderMaterial( l_value );
	}
}



void OverlayProperties::_onEditBorderTop( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_borderTop->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetBorderTop( l_res / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		}
	}
}



void OverlayProperties::_onEditBorderBottom( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_borderBottom->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetBorderBottom( l_res / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		}
	}
}



void OverlayProperties::_onEditBorderLeft( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_borderLeft->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetBorderLeft( l_res / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		}
	}
}



void OverlayProperties::_onEditBorderRight( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_borderRight->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetBorderRight( l_res / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		}
	}
}



void OverlayProperties::_onEditCaption( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_caption->GetValue();
		m_overlay->SetCaption( l_value );
	}
}



void OverlayProperties::_onEditFontName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_fontName->GetValue();
		m_overlay->SetFontName( l_value );
	}
}



void OverlayProperties::_onEditFontSize( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_fontSize->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_overlay->SetFontSize( l_res / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		}
	}
}



void OverlayProperties::_onEditTextRedValue( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		ColourValue l_colour = m_overlay->GetTextInfos()->m_textColour;
		wxString l_value = m_redTextValue->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_colour.r = l_res;
			m_overlay->SetTextColour( l_colour );
		}
	}
}



void OverlayProperties::_onEditTextGreenValue( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		ColourValue l_colour = m_overlay->GetTextInfos()->m_textColour;
		wxString l_value = m_greenTextValue->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_colour.g = l_res;
			m_overlay->SetTextColour( l_colour );
		}
	}
}



void OverlayProperties::_onEditTextBlueValue( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		ColourValue l_colour = m_overlay->GetTextInfos()->m_textColour;
		wxString l_value = m_blueTextValue->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_colour.b = l_res;
			m_overlay->SetTextColour( l_colour );
		}
	}
}



void OverlayProperties::_onEditTextAlphaValue( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		ColourValue l_colour = m_overlay->GetTextInfos()->m_textColour;
		wxString l_value = m_alphaTextValue->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_colour.a = l_res;
			m_overlay->SetTextColour( l_colour );
		}
	}
}



void OverlayProperties::_onSelectTextVerticalAlign( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = p_event.GetString();

		if ( l_value == wxT( "Top" ) )
		{
			m_overlay->SetTextVerticalAlign( vTop );
		}
		else if ( l_value == wxT( "Center" ) )
		{
			m_overlay->SetTextVerticalAlign( vCenter );
		}
		else if ( l_value == wxT( "Bottom" ) )
		{
			m_overlay->SetTextVerticalAlign( vBottom );
		}
	}
}



void OverlayProperties::_onSelectTextHorizontalAlign( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = p_event.GetString();

		if ( l_value == wxT( "Left" ) )
		{
			m_overlay->SetTextHorizontalAlign( hLeft );
		}
		else if ( l_value == wxT( "Center" ) )
		{
			m_overlay->SetTextHorizontalAlign( hCenter );
		}
		else if ( l_value == wxT( "Right" ) )
		{
			m_overlay->SetTextHorizontalAlign( hRight );
		}
	}
}


