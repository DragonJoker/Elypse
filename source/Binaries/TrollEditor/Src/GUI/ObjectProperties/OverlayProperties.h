
#ifndef ___OverlayProperties___
#define ___OverlayProperties___

#include "../../Project/2D/Module_2D.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class OverlayProperties : public ObjectProperties
		{
		public:
			Objects2D::TrollOverlay * m_overlay;
			wxStaticText * m_overlayName;
//		wxTextCtrl * m_overlayName;
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

			OverlayProperties( Objects2D::TrollOverlay * p_overlay, wxWindow * parent,
							   const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							   long style = wxBORDER_NONE );
			~OverlayProperties();

			virtual void UpdateProperties();

		private:
			void _buildTextProperties( wxPoint & p_position );
			void _buildBorderProperties( wxPoint & p_position );

			void _updateTextProperties();
			void _updateBorderProperties();

			void _getVideosNames( wxArrayString & p_names );

			void _onEditName( wxCommandEvent & p_event );
			void _onEditBaseMaterialName( wxCommandEvent & p_event );
			void _onEditMouseOverMaterialName( wxCommandEvent & p_event );
			void _onEditClickedMaterialName( wxCommandEvent & p_event );
			void _onSelectVerticalAlign( wxCommandEvent & p_event );
			void _onSelectHorizontalAlign( wxCommandEvent & p_event );
			void _onEditTop( wxCommandEvent & p_event );
			void _onEditLeft( wxCommandEvent & p_event );
			void _onEditWidth( wxCommandEvent & p_event );
			void _onEditHeight( wxCommandEvent & p_event );

			void _onEditBorderMaterial( wxCommandEvent & p_event );
			void _onEditBorderTop( wxCommandEvent & p_event );
			void _onEditBorderBottom( wxCommandEvent & p_event );
			void _onEditBorderLeft( wxCommandEvent & p_event );
			void _onEditBorderRight( wxCommandEvent & p_event );

			void _onEditCaption( wxCommandEvent & p_event );
			void _onEditFontName( wxCommandEvent & p_event );
			void _onEditFontSize( wxCommandEvent & p_event );
			void _onEditTextRedValue( wxCommandEvent & p_event );
			void _onEditTextGreenValue( wxCommandEvent & p_event );
			void _onEditTextBlueValue( wxCommandEvent & p_event );
			void _onEditTextAlphaValue( wxCommandEvent & p_event );

			void _onSelectTextVerticalAlign( wxCommandEvent & p_event );
			void _onSelectTextHorizontalAlign( wxCommandEvent & p_event );
			void _onSelectVisibility( wxCommandEvent & p_event );

			void _onSelectVideo( wxCommandEvent & p_event );

		public:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

