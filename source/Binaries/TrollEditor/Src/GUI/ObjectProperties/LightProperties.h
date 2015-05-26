//***********************************************************************************************************
#ifndef ___LightProperties___
#define ___LightProperties___
//***********************************************************************************************************
#include "../../Project/3D/Module_3D.h"
#include "ObjectProperties.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class LightProperties : public ObjectProperties
		{
		public:
			Objects3D::TrollLight * m_light;
			wxTextCtrl * m_lightName;
			wxComboBox * m_parentNode;
			wxTextCtrl * m_xDirection;
			wxTextCtrl * m_yDirection;
			wxTextCtrl * m_zDirection;
			wxTextCtrl * m_redDiffuse;
			wxTextCtrl * m_greenDiffuse;
			wxTextCtrl * m_blueDiffuse;
			wxTextCtrl * m_alphaDiffuse;
			wxTextCtrl * m_redSpecular;
			wxTextCtrl * m_greenSpecular;
			wxTextCtrl * m_blueSpecular;
			wxTextCtrl * m_alphaSpecular;
			wxTextCtrl * m_attenuationRange;
			wxTextCtrl * m_attenuationConstant;
			wxTextCtrl * m_attenuationLinear;
			wxTextCtrl * m_attenuationQuadric;
			wxTextCtrl * m_spotlightInnerAngle;
			wxTextCtrl * m_spotlightOuterAngle;
			wxTextCtrl * m_spotlightFalloff;

			LightProperties( Objects3D::TrollLight * p_light, wxWindow * parent,
							 const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							 long style = wxBORDER_NONE );
			~LightProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );
			void _onSelectParent( wxCommandEvent & p_event );
			void _onEditXDir( wxCommandEvent & p_event );
			void _onEditYDir( wxCommandEvent & p_event );
			void _onEditZDir( wxCommandEvent & p_event );
			void _onEditRedDiffuse( wxCommandEvent & p_event );
			void _onEditGreenDiffuse( wxCommandEvent & p_event );
			void _onEditBlueDiffuse( wxCommandEvent & p_event );
			void _onEditAlphaDiffuse( wxCommandEvent & p_event );
			void _onEditRedSpecular( wxCommandEvent & p_event );
			void _onEditGreenSpecular( wxCommandEvent & p_event );
			void _onEditBlueSpecular( wxCommandEvent & p_event );
			void _onEditAlphaSpecular( wxCommandEvent & p_event );
			void _onEditAttenuationRange( wxCommandEvent & p_event );
			void _onEditAttenuationConstant( wxCommandEvent & p_event );
			void _onEditAttenuationLinear( wxCommandEvent & p_event );
			void _onEditAttenuationQuadric( wxCommandEvent & p_event );
			void _onEditSpotlightInnerAngle( wxCommandEvent & p_event );
			void _onEditSpotlightOuterAngle( wxCommandEvent & p_event );
			void _onEditSpotlightFalloff( wxCommandEvent & p_event );

		public:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
