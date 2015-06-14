/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "TrollEditorPch.h"

#include "OverlayProperties.h"

#include "Project/2D/Overlay.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"
#include "GUI/MainFrame.h"

using namespace Elypse::Media;

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
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
		EVT_TEXT_ENTER( overlayBaseMaterialName, OverlayProperties::OnEditBaseMaterialName )
		EVT_TEXT_ENTER( overlayMouseOverMaterialName, OverlayProperties::OnEditMouseOverMaterialName )
		EVT_TEXT_ENTER( overlayClickedMaterialName, OverlayProperties::OnEditClickedMaterialName )
		EVT_COMBOBOX( overlayVerticalAlign, OverlayProperties::OnSelectVerticalAlign )
		EVT_COMBOBOX( overlayHorizontalAlign, OverlayProperties::OnSelectHorizontalAlign )
		EVT_TEXT_ENTER( overlayTop, OverlayProperties::OnEditTop )
		EVT_TEXT_ENTER( overlayLeft, OverlayProperties::OnEditLeft )
		EVT_TEXT_ENTER( overlayWidth, OverlayProperties::OnEditWidth )
		EVT_TEXT_ENTER( overlayHeight, OverlayProperties::OnEditHeight )

		EVT_TEXT_ENTER( overlayBorderMaterial, OverlayProperties::OnEditBorderMaterial )
		EVT_TEXT_ENTER( overlayBorderTop, OverlayProperties::OnEditBorderTop )
		EVT_TEXT_ENTER( overlayBorderBottom, OverlayProperties::OnEditBorderBottom )
		EVT_TEXT_ENTER( overlayBorderLeft, OverlayProperties::OnEditBorderLeft )
		EVT_TEXT_ENTER( overlayBorderRight, OverlayProperties::OnEditBorderRight )

		EVT_TEXT_ENTER( overlayCaption, OverlayProperties::OnEditCaption )
		EVT_TEXT_ENTER( overlayFontName, OverlayProperties::OnEditFontName )
		EVT_TEXT_ENTER( overlayFontSize, OverlayProperties::OnEditFontSize )
		EVT_TEXT_ENTER( overlayTextRedValue, OverlayProperties::OnEditTextRedValue )
		EVT_TEXT_ENTER( overlayTextGreenValue, OverlayProperties::OnEditTextGreenValue )
		EVT_TEXT_ENTER( overlayTextBlueValue, OverlayProperties::OnEditTextBlueValue )
		EVT_TEXT_ENTER( overlayTextAlphaValue, OverlayProperties::OnEditTextAlphaValue )
		EVT_COMBOBOX( overlayVerticalAlign, OverlayProperties::OnSelectTextVerticalAlign )
		EVT_COMBOBOX( overlayHorizontalAlign, OverlayProperties::OnSelectTextHorizontalAlign )
		EVT_COMBOBOX( overlayVisibility, OverlayProperties::OnSelectVisibility )
		EVT_COMBOBOX( overlayVideo, OverlayProperties::OnSelectVideo )
	END_EVENT_TABLE()

	OverlayProperties::OverlayProperties( TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * p_overlay, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_overlay( p_overlay )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_overlayName = new wxStaticText( this, overlayName, m_overlay->GetObjectName(), l_position );
		//m_overlayName = BuildTextCtrl( this, l_position, overlayName, m_overlay->GetName());
		m_baseMaterialName = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayBaseMaterialName, _( "Base Material" ), m_overlay->GetBaseMaterial() );
		m_mouseOverMaterialName = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayMouseOverMaterialName, _( "MouseOver Material" ), m_overlay->GetMouseOverMaterial() );
		m_clickedMaterialName = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayClickedMaterialName, _( "Clicked Material" ), m_overlay->GetClickedMaterial() );
		int l_align = static_cast< int >( m_overlay->GetVerticalAlign() );
		wxArrayString l_vAligns;
		l_vAligns.Add( _( "None" ) );
		l_vAligns.Add( _( "Top" ) );
		l_vAligns.Add( _( "Center" ) );
		l_vAligns.Add( _( "Bottom" ) );
		m_verticalAlign = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayVerticalAlign, _( "Vertical Align" ), l_vAligns, l_vAligns[l_align + 1] );
		l_align = static_cast< int >( m_overlay->GetHorizontalAlign() );
		wxArrayString l_hAligns;
		l_hAligns.Add( _( "None" ) );
		l_hAligns.Add( _( "Left" ) );
		l_hAligns.Add( _( "Center" ) );
		l_hAligns.Add( _( "Right" ) );
		m_horizontalAlign = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayHorizontalAlign, _( "Horizontal Align" ), l_hAligns, l_hAligns[l_align + 1] );
		m_top = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayTop, _( "Top" ), m_overlay->GetTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		m_left = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayLeft, _( "Left" ), m_overlay->GetLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		m_width = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayWidth, _( "Width" ), m_overlay->GetWidth() * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		m_height = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), overlayHeight, _( "Height" ), m_overlay->GetHeight() * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		_buildBorderProperties( l_position );
		_buildTextProperties( l_position );
		wxArrayString l_visibility;
		l_visibility.Add( _( "Visible" ) );
		l_visibility.Add( _( "Hidden" ) );
		m_visibility = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayVisibility, _( "Visibility" ), l_visibility, _( "Visible" ) );
		wxArrayString l_videos;
		l_videos.push_back( _( "None" ) );
		_getVideosNames( l_videos );
		TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * l_video = m_overlay->GetVideo();
		wxString l_videoName = _( "None" );

		if ( l_video != NULL )
		{
			l_videoName = l_video->GetVideo()->GetObjectName();
		}

		m_video = BuildComboBox( this, l_position += wxPoint( 0, 20 ), overlayVideo, _( "Video" ), l_videos, l_videoName );
	}

	OverlayProperties::~OverlayProperties()
	{
	}

	void OverlayProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_overlayName->SetLabel( m_overlay->GetObjectName() );
		m_baseMaterialName->SetValue( m_overlay->GetBaseMaterial() );
		m_mouseOverMaterialName->SetValue( m_overlay->GetMouseOverMaterial() );
		m_clickedMaterialName->SetValue( m_overlay->GetClickedMaterial() );
		int l_align = static_cast< int >( m_overlay->GetVerticalAlign() );
		wxArrayString l_vAligns;
		l_vAligns.Add( _( "None" ) );
		l_vAligns.Add( _( "Top" ) );
		l_vAligns.Add( _( "Center" ) );
		l_vAligns.Add( _( "Bottom" ) );
		m_verticalAlign->SetValue( l_vAligns[l_align + 1] );
		l_align = static_cast< int >( m_overlay->GetHorizontalAlign() );
		wxArrayString l_hAligns;
		l_hAligns.Add( _( "None" ) );
		l_hAligns.Add( _( "Left" ) );
		l_hAligns.Add( _( "Center" ) );
		l_hAligns.Add( _( "Right" ) );
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
		TROLL_PROJECT_2D_NAMESPACE::TextInfos * l_textInfos = m_overlay->GetTextInfos();

		if ( l_textInfos != NULL )
		{
			int l_align;
			wxArrayString l_vAligns;
			l_vAligns.Add( _( "None" ) );
			l_vAligns.Add( _( "Top" ) );
			l_vAligns.Add( _( "Center" ) );
			l_vAligns.Add( _( "Bottom" ) );
			wxArrayString l_hAligns;
			l_hAligns.Add( _( "None" ) );
			l_hAligns.Add( _( "Left" ) );
			l_hAligns.Add( _( "Center" ) );
			l_hAligns.Add( _( "Right" ) );
			m_caption = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayCaption, _( "Caption" ), l_textInfos->m_caption );
			m_fontName = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayFontName, _( "Font Name" ), l_textInfos->m_fontName );
			m_fontSize = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayFontSize, _( "Font Size" ), l_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			m_redTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextRedValue, _( "Red Value" ), l_textInfos->m_textColour.r );
			m_greenTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextGreenValue, _( "Green Value" ), l_textInfos->m_textColour.g );
			m_blueTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextBlueValue, _( "Blue Value" ), l_textInfos->m_textColour.b );
			m_alphaTextValue = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayTextAlphaValue, _( "Alpha Value" ), l_textInfos->m_textColour.a );
			l_align = static_cast< int >( l_textInfos->m_verticalAlign );
			m_verticalAlign = BuildComboBox( this, p_position += wxPoint( 0, 20 ), overlayTextVerticalAlign, _( "Text Vertical Align" ), l_vAligns, l_vAligns[l_align + 1] );
			l_align = static_cast< int >( l_textInfos->m_horizontalAlign );
			m_horizontalAlign = BuildComboBox( this, p_position += wxPoint( 0, 20 ), overlayTextHorizontalAlign, _( "Text Horizontal Align" ), l_hAligns, l_hAligns[l_align + 1] );
		}
	}

	void OverlayProperties::_buildBorderProperties( wxPoint & p_position )
	{
		TROLL_PROJECT_2D_NAMESPACE::BorderInfos * l_borderInfos = m_overlay->GetBorderInfos();

		if ( l_borderInfos != NULL )
		{
			m_borderMaterialName = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderMaterial, _( "Border Material" ), l_borderInfos->m_material );
			m_borderTop = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderTop, _( "Top Border Size" ), l_borderInfos->m_top * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			m_borderBottom = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderBottom, _( "Bottom Border Size" ), l_borderInfos->m_bottom * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			m_borderLeft = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderLeft, _( "Left Border Size" ), l_borderInfos->m_left * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
			m_borderRight = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), overlayBorderRight, _( "Right Border Size" ), l_borderInfos->m_right * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
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
		TROLL_PROJECT_2D_NAMESPACE::TextInfos * l_textInfos = m_overlay->GetTextInfos();

		if ( l_textInfos != NULL )
		{
			wxString l_tmp;
			int l_align;
			wxArrayString l_vAligns;
			l_vAligns.Add( _( "None" ) );
			l_vAligns.Add( _( "Top" ) );
			l_vAligns.Add( _( "Center" ) );
			l_vAligns.Add( _( "Bottom" ) );
			wxArrayString l_hAligns;
			l_hAligns.Add( _( "None" ) );
			l_hAligns.Add( _( "Left" ) );
			l_hAligns.Add( _( "Center" ) );
			l_hAligns.Add( _( "Right" ) );
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
			l_align = static_cast< int >( l_textInfos->m_verticalAlign );
			m_verticalAlign->SetValue( l_vAligns[l_align + 1] );
			l_align = static_cast< int >( l_textInfos->m_horizontalAlign );
			m_horizontalAlign->SetValue( l_hAligns[l_align + 1] );
		}
	}

	void OverlayProperties::_updateBorderProperties()
	{
		TROLL_PROJECT_2D_NAMESPACE::BorderInfos * l_borderInfos = m_overlay->GetBorderInfos();

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

	void OverlayProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_overlay->SetObjectName( m_overlayName->GetLabel() );
		}
	}

	void OverlayProperties::OnEditBaseMaterialName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_overlay->SetBaseMaterial( m_baseMaterialName->GetValue() );
		}
	}

	void OverlayProperties::OnEditMouseOverMaterialName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_overlay->SetMouseOverMaterial( m_mouseOverMaterialName->GetValue() );
		}
	}

	void OverlayProperties::OnEditClickedMaterialName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_overlay->SetClickedMaterial( m_clickedMaterialName->GetValue() );
		}
	}

	void OverlayProperties::OnSelectVerticalAlign( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = p_event.GetString();

			if ( l_value == _( "Top" ) )
			{
				m_overlay->SetVerticalAlign( TROLL_PROJECT_2D_NAMESPACE::vTop );
			}
			else if ( l_value == _( "Center" ) )
			{
				m_overlay->SetVerticalAlign( TROLL_PROJECT_2D_NAMESPACE::vCenter );
			}
			else if ( l_value == _( "Bottom" ) )
			{
				m_overlay->SetVerticalAlign( TROLL_PROJECT_2D_NAMESPACE::vBottom );
			}

			wxString l_tmp;
			l_tmp << m_overlay->GetTop();
			m_top->SetValue( l_tmp );
		}
	}

	void OverlayProperties::OnSelectHorizontalAlign( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = p_event.GetString();

			if ( l_value == _( "Left" ) )
			{
				m_overlay->SetHorizontalAlign( TROLL_PROJECT_2D_NAMESPACE::hLeft );
			}
			else if ( l_value == _( "Center" ) )
			{
				m_overlay->SetHorizontalAlign( TROLL_PROJECT_2D_NAMESPACE::hCenter );
			}
			else if ( l_value == _( "Right" ) )
			{
				m_overlay->SetHorizontalAlign( TROLL_PROJECT_2D_NAMESPACE::hRight );
			}

			wxString l_tmp;
			l_tmp << m_overlay->GetLeft();
			m_left->SetValue( l_tmp );
		}
	}

	void OverlayProperties::OnEditTop( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditLeft( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditWidth( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditHeight( wxCommandEvent & p_event )
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

	void OverlayProperties::OnSelectVisibility( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = p_event.GetString();

			if ( l_value == _( "Visible" ) )
			{
				m_overlay->SetVisible( true );
			}
			else if ( l_value == _( "Hidden" ) )
			{
				m_overlay->SetVisible( false );
			}
		}
	}

	void OverlayProperties::OnSelectVideo( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideo * l_video = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetVideo( p_event.GetString() );

			if ( l_video != NULL )
			{
				TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * l_videoObject = l_video->AddObject( m_overlay );

				if ( l_videoObject != NULL )
				{
					m_overlay->SetVideo( l_videoObject );
				}
			}
		}
	}

	void OverlayProperties::OnEditBorderMaterial( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_borderMaterialName->GetValue();
			m_overlay->SetBorderMaterial( l_value );
		}
	}

	void OverlayProperties::OnEditBorderTop( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditBorderBottom( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditBorderLeft( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditBorderRight( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditCaption( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_caption->GetValue();
			m_overlay->SetCaption( l_value );
		}
	}

	void OverlayProperties::OnEditFontName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_fontName->GetValue();
			m_overlay->SetFontName( l_value );
		}
	}

	void OverlayProperties::OnEditFontSize( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditTextRedValue( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditTextGreenValue( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditTextBlueValue( wxCommandEvent & p_event )
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

	void OverlayProperties::OnEditTextAlphaValue( wxCommandEvent & p_event )
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

	void OverlayProperties::OnSelectTextVerticalAlign( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = p_event.GetString();

			if ( l_value == _( "Top" ) )
			{
				m_overlay->SetTextVerticalAlign( TROLL_PROJECT_2D_NAMESPACE::vTop );
			}
			else if ( l_value == _( "Center" ) )
			{
				m_overlay->SetTextVerticalAlign( TROLL_PROJECT_2D_NAMESPACE::vCenter );
			}
			else if ( l_value == _( "Bottom" ) )
			{
				m_overlay->SetTextVerticalAlign( TROLL_PROJECT_2D_NAMESPACE::vBottom );
			}
		}
	}

	void OverlayProperties::OnSelectTextHorizontalAlign( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = p_event.GetString();

			if ( l_value == _( "Left" ) )
			{
				m_overlay->SetTextHorizontalAlign( TROLL_PROJECT_2D_NAMESPACE::hLeft );
			}
			else if ( l_value == _( "Center" ) )
			{
				m_overlay->SetTextHorizontalAlign( TROLL_PROJECT_2D_NAMESPACE::hCenter );
			}
			else if ( l_value == _( "Right" ) )
			{
				m_overlay->SetTextHorizontalAlign( TROLL_PROJECT_2D_NAMESPACE::hRight );
			}
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
