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
#ifndef ___OverlayProperties___
#define ___OverlayProperties___

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class OverlayProperties
		: public ObjectProperties
	{
	public:
		OverlayProperties( TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * p_overlay, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
		~OverlayProperties();

		virtual void UpdateProperties();

	private:
		void _buildTextProperties( wxPoint & p_position );
		void _buildBorderProperties( wxPoint & p_position );

		void _updateTextProperties();
		void _updateBorderProperties();

		void _getVideosNames( wxArrayString & p_names );
		
		DECLARE_EVENT_TABLE()
		void OnEditName( wxCommandEvent & p_event );
		void OnEditBaseMaterialName( wxCommandEvent & p_event );
		void OnEditMouseOverMaterialName( wxCommandEvent & p_event );
		void OnEditClickedMaterialName( wxCommandEvent & p_event );
		void OnSelectVerticalAlign( wxCommandEvent & p_event );
		void OnSelectHorizontalAlign( wxCommandEvent & p_event );
		void OnEditTop( wxCommandEvent & p_event );
		void OnEditLeft( wxCommandEvent & p_event );
		void OnEditWidth( wxCommandEvent & p_event );
		void OnEditHeight( wxCommandEvent & p_event );
		void OnEditBorderMaterial( wxCommandEvent & p_event );
		void OnEditBorderTop( wxCommandEvent & p_event );
		void OnEditBorderBottom( wxCommandEvent & p_event );
		void OnEditBorderLeft( wxCommandEvent & p_event );
		void OnEditBorderRight( wxCommandEvent & p_event );
		void OnEditCaption( wxCommandEvent & p_event );
		void OnEditFontName( wxCommandEvent & p_event );
		void OnEditFontSize( wxCommandEvent & p_event );
		void OnEditTextRedValue( wxCommandEvent & p_event );
		void OnEditTextGreenValue( wxCommandEvent & p_event );
		void OnEditTextBlueValue( wxCommandEvent & p_event );
		void OnEditTextAlphaValue( wxCommandEvent & p_event );
		void OnSelectTextVerticalAlign( wxCommandEvent & p_event );
		void OnSelectTextHorizontalAlign( wxCommandEvent & p_event );
		void OnSelectVisibility( wxCommandEvent & p_event );
		void OnSelectVideo( wxCommandEvent & p_event );

	private:
		TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * m_overlay;
		wxStaticText * m_overlayName;
		wxTextCtrl * m_baseMaterialName;
		wxTextCtrl * m_mouseOverMaterialName;
		wxTextCtrl * m_clickedMaterialName;
		wxComboBox * m_verticalAlign;
		wxComboBox * m_horizontalAlign;
		wxTextCtrl * m_top;
		wxTextCtrl * m_left;
		wxTextCtrl * m_width;
		wxTextCtrl * m_height;
		wxComboBox * m_visibility;
		wxComboBox * m_video;

		//Border
		wxTextCtrl * m_borderMaterialName;
		wxTextCtrl * m_borderTop;
		wxTextCtrl * m_borderBottom;
		wxTextCtrl * m_borderLeft;
		wxTextCtrl * m_borderRight;

		//Text
		wxTextCtrl * m_caption;
		wxTextCtrl * m_fontName;
		wxTextCtrl * m_fontSize;
		wxComboBox * m_textVerticalAlign;
		wxComboBox * m_textHorizontalAlign;
		wxTextCtrl * m_redTextValue;
		wxTextCtrl * m_greenTextValue;
		wxTextCtrl * m_blueTextValue;
		wxTextCtrl * m_alphaTextValue;
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
