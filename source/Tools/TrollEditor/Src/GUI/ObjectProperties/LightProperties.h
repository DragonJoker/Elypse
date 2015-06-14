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
#ifndef ___LightProperties___
#define ___LightProperties___

#include "ObjectProperties.h"

#include "Project/3D/Module_3D.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class LightProperties
		: public ObjectProperties
	{
	public:
		LightProperties( TROLL_PROJECT_3D_NAMESPACE::TrollLight * p_light, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
		~LightProperties();

		virtual void UpdateProperties();

	private:
		DECLARE_EVENT_TABLE()
		void OnEditName( wxCommandEvent & p_event );
		void OnSelectParent( wxCommandEvent & p_event );
		void OnEditXDir( wxCommandEvent & p_event );
		void OnEditYDir( wxCommandEvent & p_event );
		void OnEditZDir( wxCommandEvent & p_event );
		void OnEditRedDiffuse( wxCommandEvent & p_event );
		void OnEditGreenDiffuse( wxCommandEvent & p_event );
		void OnEditBlueDiffuse( wxCommandEvent & p_event );
		void OnEditAlphaDiffuse( wxCommandEvent & p_event );
		void OnEditRedSpecular( wxCommandEvent & p_event );
		void OnEditGreenSpecular( wxCommandEvent & p_event );
		void OnEditBlueSpecular( wxCommandEvent & p_event );
		void OnEditAlphaSpecular( wxCommandEvent & p_event );
		void OnEditAttenuationRange( wxCommandEvent & p_event );
		void OnEditAttenuationConstant( wxCommandEvent & p_event );
		void OnEditAttenuationLinear( wxCommandEvent & p_event );
		void OnEditAttenuationQuadric( wxCommandEvent & p_event );
		void OnEditSpotlightInnerAngle( wxCommandEvent & p_event );
		void OnEditSpotlightOuterAngle( wxCommandEvent & p_event );
		void OnEditSpotlightFalloff( wxCommandEvent & p_event );

	private:
		TROLL_PROJECT_3D_NAMESPACE::TrollLight * m_light;
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
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
