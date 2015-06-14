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

#include "LightProperties.h"

#include "Project/3D/Light.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum LightPropsIDs
	{
		lightName,
		lightParent,
		lightXDir,
		lightYDir,
		lightZDir,
		lightRedDiffuse,
		lightGreenDiffuse,
		lightBlueDiffuse,
		lightAlphaDiffuse,
		lightRedSpecular,
		lightGreenSpecular,
		lightBlueSpecular,
		lightAlphaSpecular,
		lightAttenuationRange,
		lightAttenuationConstant,
		lightAttenuationLinear,
		lightAttenuationQuadric,
		lightSpotlightInnerAngle,
		lightSpotlightOuterAngle,
		lightSpotlightFalloff
	};

	const long c_defaultTextEditStyle = wxBORDER_SIMPLE | wxTE_PROCESS_ENTER;

	BEGIN_EVENT_TABLE( LightProperties, wxPanel )
		EVT_TEXT_ENTER( lightName, LightProperties::OnEditName )
		EVT_COMBOBOX( lightParent, LightProperties::OnSelectParent )
		EVT_TEXT_ENTER( lightXDir, LightProperties::OnEditXDir )
		EVT_TEXT_ENTER( lightYDir, LightProperties::OnEditYDir )
		EVT_TEXT_ENTER( lightZDir, LightProperties::OnEditZDir )
		EVT_TEXT_ENTER( lightRedDiffuse, LightProperties::OnEditRedDiffuse )
		EVT_TEXT_ENTER( lightGreenDiffuse, LightProperties::OnEditGreenDiffuse )
		EVT_TEXT_ENTER( lightBlueDiffuse, LightProperties::OnEditBlueDiffuse )
		EVT_TEXT_ENTER( lightRedSpecular, LightProperties::OnEditRedSpecular )
		EVT_TEXT_ENTER( lightGreenSpecular, LightProperties::OnEditGreenSpecular )
		EVT_TEXT_ENTER( lightBlueSpecular, LightProperties::OnEditBlueSpecular )
		EVT_TEXT_ENTER( lightAttenuationRange, LightProperties::OnEditAttenuationRange )
		EVT_TEXT_ENTER( lightAttenuationConstant, LightProperties::OnEditAttenuationConstant )
		EVT_TEXT_ENTER( lightAttenuationLinear, LightProperties::OnEditAttenuationLinear )
		EVT_TEXT_ENTER( lightAttenuationQuadric, LightProperties::OnEditAttenuationQuadric )
		EVT_TEXT_ENTER( lightSpotlightInnerAngle, LightProperties::OnEditSpotlightInnerAngle )
		EVT_TEXT_ENTER( lightSpotlightOuterAngle, LightProperties::OnEditSpotlightOuterAngle )
		EVT_TEXT_ENTER( lightSpotlightFalloff, LightProperties::OnEditSpotlightFalloff )
	END_EVENT_TABLE()


	LightProperties::LightProperties( TROLL_PROJECT_3D_NAMESPACE::TrollLight * p_light, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_light( p_light )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_lightName = BuildTextCtrl( this, l_position, lightName, _( "Name" ), m_light->GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_light->GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
		l_nodesArray.Sort();
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), lightParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
		Vector3 l_direction = m_light->GetDirection();
		m_xDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightXDir, _( "X Direction" ), l_direction.x );
		m_yDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightYDir, _( "Y Direction" ), l_direction.y );
		m_zDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightZDir, _( "Z Direction" ), l_direction.z );
		ColourValue l_diffuse = m_light->GetDiffuse();
		m_redDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightRedDiffuse, _( "Red Diffuse" ), l_diffuse.r );
		m_greenDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightGreenDiffuse, _( "Green Diffuse" ), l_diffuse.g );
		m_blueDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightBlueDiffuse, _( "Blue Diffuse" ), l_diffuse.b );
		m_alphaDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAlphaDiffuse, _( "Alpha Diffuse" ), l_diffuse.a );
		ColourValue l_specular = m_light->GetSpecular();
		m_redSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightRedSpecular, _( "Red Specular" ), l_specular.r );
		m_greenSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightGreenSpecular, _( "Green Specular" ), l_specular.g );
		m_blueSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightBlueSpecular, _( "Blue Specular" ), l_specular.b );
		m_alphaSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAlphaSpecular, _( "Alpha Specular" ), l_specular.a );
		Vector4 l_attenuation = m_light->GetAttenuation();
		m_attenuationRange = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationRange, _( "Attenuation Range" ), l_attenuation.x );
		m_attenuationConstant = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationConstant, _( "Constant Att. Value" ), l_attenuation.y );
		m_attenuationLinear = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationLinear, _( "Linear Att. Value" ), l_attenuation.z );
		m_attenuationQuadric = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationQuadric, _( "Quadric Att. Value" ), l_attenuation.w );

		if ( m_light->GetLightType() == TROLL_PROJECT_3D_NAMESPACE::ltSpotLight )
		{
			Vector3 l_spotlightRange = m_light->GetSpotlightRange();
			m_spotlightInnerAngle = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightSpotlightInnerAngle, _( "Spotlight Inner Angle" ), l_spotlightRange.x );
			m_spotlightOuterAngle = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightSpotlightOuterAngle, _( "Spotlight Outer Angle" ), l_spotlightRange.y );
			m_spotlightFalloff = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightSpotlightFalloff, _( "Spotlight Falloff" ), l_spotlightRange.z );
		}
	}

	LightProperties::~LightProperties()
	{
	}

	void LightProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
		m_lightName->SetValue( m_light->GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_light->GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
		l_nodesArray.Sort();
		RemoveChild( m_parentNode );
		delete m_parentNode;
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), lightParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
		Vector3 l_direction = m_light->GetDirection();
		wxString l_tmp;
		l_tmp << l_direction.x;
		m_xDirection->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_direction.y;
		m_yDirection->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_direction.z;
		m_zDirection->SetValue( l_tmp );
		l_tmp.clear();
		ColourValue l_diffuse = m_light->GetDiffuse();
		l_tmp << l_diffuse.r;
		m_redDiffuse->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_diffuse.g;
		m_greenDiffuse->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_diffuse.b;
		m_blueDiffuse->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_diffuse.a;
		m_alphaDiffuse->SetValue( l_tmp );
		l_tmp.clear();
		ColourValue l_specular = m_light->GetSpecular();
		l_tmp << l_specular.r;
		m_redSpecular->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_specular.g;
		m_greenSpecular->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_specular.b;
		m_blueSpecular->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_specular.a;
		m_alphaSpecular->SetValue( l_tmp );
		l_tmp.clear();
		Vector4 l_attenuation = m_light->GetAttenuation();
		l_tmp << l_attenuation.x;
		m_attenuationRange->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_attenuation.y;
		m_attenuationConstant->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_attenuation.z;
		m_attenuationLinear->SetValue( l_tmp );
		l_tmp.clear();
		l_tmp << l_attenuation.w;
		m_attenuationQuadric->SetValue( l_tmp );
		l_tmp.clear();

		if ( m_light->GetLightType() == TROLL_PROJECT_3D_NAMESPACE::ltSpotLight )
		{
			Vector3 l_spotlightRange = m_light->GetSpotlightRange();
			l_tmp << l_spotlightRange.x;
			m_spotlightInnerAngle->SetValue( l_tmp );
			l_tmp.clear();
			l_tmp << l_spotlightRange.y;
			m_spotlightOuterAngle->SetValue( l_tmp );
			l_tmp.clear();
			l_tmp << l_spotlightRange.z;
			m_spotlightFalloff->SetValue( l_tmp );
			l_tmp.clear();
			m_updatingProperties = false;
		}
	}

	void LightProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_light->SetObjectName( m_lightName->GetValue() );
		}
	}

	void LightProperties::OnSelectParent( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_node = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
			m_light->AttachTo( l_node );
		}
	}

	void LightProperties::OnEditXDir( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector3 l_direction = m_light->GetDirection();
			wxString l_value = m_xDirection->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_direction.x = l_res;
				m_light->SetDirection( l_direction );
			}
		}
	}

	void LightProperties::OnEditYDir( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector3 l_direction = m_light->GetDirection();
			wxString l_value = m_yDirection->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_direction.y = l_res;
				m_light->SetDirection( l_direction );
			}
		}
	}

	void LightProperties::OnEditZDir( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector3 l_direction = m_light->GetDirection();
			wxString l_value = m_zDirection->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_direction.z = l_res;
				m_light->SetDirection( l_direction );
			}
		}
	}

	void LightProperties::OnEditRedDiffuse( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
			wxString l_value = m_redDiffuse->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_diffuse.r = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
		}
	}

	void LightProperties::OnEditGreenDiffuse( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
			wxString l_value = m_greenDiffuse->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_diffuse.g = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
		}
	}

	void LightProperties::OnEditBlueDiffuse( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
			wxString l_value = m_blueDiffuse->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_diffuse.b = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
		}
	}

	void LightProperties::OnEditAlphaDiffuse( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
			wxString l_value = m_alphaDiffuse->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_diffuse.a = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
		}
	}

	void LightProperties::OnEditRedSpecular( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_specular = m_light->GetSpecular();
			wxString l_value = m_redSpecular->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_specular.r = l_res;
				m_light->SetSpecular( l_specular );
			}
		}
	}

	void LightProperties::OnEditGreenSpecular( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_specular = m_light->GetSpecular();
			wxString l_value = m_greenSpecular->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_specular.g = l_res;
				m_light->SetSpecular( l_specular );
			}
		}
	}

	void LightProperties::OnEditBlueSpecular( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_specular = m_light->GetSpecular();
			wxString l_value = m_blueSpecular->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_specular.b = l_res;
				m_light->SetSpecular( l_specular );
			}
		}
	}

	void LightProperties::OnEditAlphaSpecular( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::ColourValue l_specular = m_light->GetSpecular();
			wxString l_value = m_alphaSpecular->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				l_specular.a = l_res;
				m_light->SetSpecular( l_specular );
			}
		}
	}

	void LightProperties::OnEditAttenuationRange( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector4 l_attenuation = m_light->GetAttenuation();
			wxString l_value = m_attenuationRange->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_attenuation.x = l_res;
				m_light->SetAttenuation( l_attenuation );
			}
		}
	}

	void LightProperties::OnEditAttenuationConstant( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector4 l_attenuation = m_light->GetAttenuation();
			wxString l_value = m_attenuationConstant->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_attenuation.y = l_res;
				m_light->SetAttenuation( l_attenuation );
			}
		}
	}

	void LightProperties::OnEditAttenuationLinear( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector4 l_attenuation = m_light->GetAttenuation();
			wxString l_value = m_attenuationLinear->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_attenuation.z = l_res;
				m_light->SetAttenuation( l_attenuation );
			}
		}
	}

	void LightProperties::OnEditAttenuationQuadric( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector4 l_attenuation = m_light->GetAttenuation();
			wxString l_value = m_attenuationQuadric->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_attenuation.w = l_res;
				m_light->SetAttenuation( l_attenuation );
			}
		}
	}

	void LightProperties::OnEditSpotlightInnerAngle( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector3 l_spotlightRange = m_light->GetSpotlightRange();
			wxString l_value = m_spotlightInnerAngle->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_spotlightRange.x = l_res;
				m_light->SetSpotLightRange( l_spotlightRange );
			}
		}
	}

	void LightProperties::OnEditSpotlightOuterAngle( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector3 l_spotlightRange = m_light->GetSpotlightRange();
			wxString l_value = m_spotlightOuterAngle->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_spotlightRange.y = l_res;
				m_light->SetSpotLightRange( l_spotlightRange );
			}
		}
	}

	void LightProperties::OnEditSpotlightFalloff( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			Ogre::Vector3 l_spotlightRange = m_light->GetSpotlightRange();
			wxString l_value = m_spotlightFalloff->GetValue();
			float l_res;

			if ( GetReal( l_value, l_res ) )
			{
				l_spotlightRange.z = l_res;
				m_light->SetSpotLightRange( l_spotlightRange );
			}
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
