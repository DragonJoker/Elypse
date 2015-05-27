
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/LightProperties.h"

#include "Project/3D/Light.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

using namespace Troll::GUI;
using namespace Troll::Objects3D;

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
extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( LightProperties, wxPanel )
	EVT_TEXT_ENTER(	lightName,					LightProperties::_onEditName )
	EVT_COMBOBOX(	lightParent,				LightProperties::_onSelectParent )
	EVT_TEXT_ENTER(	lightXDir,					LightProperties::_onEditXDir )
	EVT_TEXT_ENTER(	lightYDir,					LightProperties::_onEditYDir )
	EVT_TEXT_ENTER(	lightZDir,					LightProperties::_onEditZDir )
	EVT_TEXT_ENTER(	lightRedDiffuse,			LightProperties::_onEditRedDiffuse )
	EVT_TEXT_ENTER(	lightGreenDiffuse,			LightProperties::_onEditGreenDiffuse )
	EVT_TEXT_ENTER(	lightBlueDiffuse,			LightProperties::_onEditBlueDiffuse )
	EVT_TEXT_ENTER(	lightRedSpecular,			LightProperties::_onEditRedSpecular )
	EVT_TEXT_ENTER(	lightGreenSpecular,			LightProperties::_onEditGreenSpecular )
	EVT_TEXT_ENTER(	lightBlueSpecular,			LightProperties::_onEditBlueSpecular )
	EVT_TEXT_ENTER(	lightAttenuationRange,		LightProperties::_onEditAttenuationRange )
	EVT_TEXT_ENTER(	lightAttenuationConstant,	LightProperties::_onEditAttenuationConstant )
	EVT_TEXT_ENTER(	lightAttenuationLinear,		LightProperties::_onEditAttenuationLinear )
	EVT_TEXT_ENTER(	lightAttenuationQuadric,	LightProperties::_onEditAttenuationQuadric )
	EVT_TEXT_ENTER(	lightSpotlightInnerAngle,	LightProperties::_onEditSpotlightInnerAngle )
	EVT_TEXT_ENTER(	lightSpotlightOuterAngle,	LightProperties::_onEditSpotlightOuterAngle )
	EVT_TEXT_ENTER(	lightSpotlightFalloff,		LightProperties::_onEditSpotlightFalloff )
END_EVENT_TABLE()


LightProperties :: LightProperties( TrollLight * p_light, wxWindow * parent, const wxPoint & pos,
									const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_light( p_light )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_lightName = BuildTextCtrl( this, l_position, lightName, wxT( "Name" ), m_light->GetName() );
	TrollSceneNode * l_parent = m_light->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
	l_nodesArray.Sort();
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), lightParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	Vector3 l_direction = m_light->GetDirection();
	m_xDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightXDir, wxT( "X Direction" ), l_direction.x );
	m_yDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightYDir, wxT( "Y Direction" ), l_direction.y );
	m_zDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightZDir, wxT( "Z Direction" ), l_direction.z );
	ColourValue l_diffuse = m_light->GetDiffuse();
	m_redDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightRedDiffuse, wxT( "Red Diffuse" ), l_diffuse.r );
	m_greenDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightGreenDiffuse, wxT( "Green Diffuse" ), l_diffuse.g );
	m_blueDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightBlueDiffuse, wxT( "Blue Diffuse" ), l_diffuse.b );
	m_alphaDiffuse = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAlphaDiffuse, wxT( "Alpha Diffuse" ), l_diffuse.a );
	ColourValue l_specular = m_light->GetSpecular();
	m_redSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightRedSpecular, wxT( "Red Specular" ), l_specular.r );
	m_greenSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightGreenSpecular, wxT( "Green Specular" ), l_specular.g );
	m_blueSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightBlueSpecular, wxT( "Blue Specular" ), l_specular.b );
	m_alphaSpecular = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAlphaSpecular, wxT( "Alpha Specular" ), l_specular.a );
	Vector4 l_attenuation = m_light->GetAttenuation();
	m_attenuationRange = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationRange, wxT( "Attenuation Range" ), l_attenuation.x );
	m_attenuationConstant = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationConstant, wxT( "Constant Att. Value" ), l_attenuation.y );
	m_attenuationLinear = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationLinear, wxT( "Linear Att. Value" ), l_attenuation.z );
	m_attenuationQuadric = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightAttenuationQuadric, wxT( "Quadric Att. Value" ), l_attenuation.w );

	if ( m_light->GetLightType() != ltSpotLight )
	{
		return;
	}

	Vector3 l_spotlightRange = m_light->GetSpotlightRange();
	m_spotlightInnerAngle = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightSpotlightInnerAngle, wxT( "Spotlight Inner Angle" ), l_spotlightRange.x );
	m_spotlightOuterAngle = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightSpotlightOuterAngle, wxT( "Spotlight Outer Angle" ), l_spotlightRange.y );
	m_spotlightFalloff = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), lightSpotlightFalloff, wxT( "Spotlight Falloff" ), l_spotlightRange.z );
}



LightProperties :: ~LightProperties()
{
}



void LightProperties :: UpdateProperties()
{
	m_updatingProperties = true;
	wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
	m_lightName->SetValue( m_light->GetName() );
	TrollSceneNode * l_parent = m_light->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
	l_nodesArray.Sort();
	RemoveChild( m_parentNode );
	delete m_parentNode;
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), lightParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
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

	if ( m_light->GetLightType() != ltSpotLight )
	{
		return;
	}

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



void LightProperties :: _onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_light->SetName( m_lightName->GetValue() );
	}
}



void LightProperties :: _onSelectParent( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		TrollSceneNode * l_node = g_mainFrame->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
		m_light->AttachTo( l_node );
	}
}



void LightProperties :: _onEditXDir( wxCommandEvent & p_event )
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



void LightProperties :: _onEditYDir( wxCommandEvent & p_event )
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



void LightProperties :: _onEditZDir( wxCommandEvent & p_event )
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



void LightProperties :: _onEditRedDiffuse( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
		wxString l_value = m_redDiffuse->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_diffuse.r = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditGreenDiffuse( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
		wxString l_value = m_greenDiffuse->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_diffuse.g = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditBlueDiffuse( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
		wxString l_value = m_blueDiffuse->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_diffuse.b = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditAlphaDiffuse( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_diffuse = m_light->GetDiffuse();
		wxString l_value = m_alphaDiffuse->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_diffuse.a = l_res;
				m_light->SetDiffuse( l_diffuse );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditRedSpecular( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_specular = m_light->GetSpecular();
		wxString l_value = m_redSpecular->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_specular.r = l_res;
				m_light->SetSpecular( l_specular );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditGreenSpecular( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_specular = m_light->GetSpecular();
		wxString l_value = m_greenSpecular->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_specular.g = l_res;
				m_light->SetSpecular( l_specular );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditBlueSpecular( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_specular = m_light->GetSpecular();
		wxString l_value = m_blueSpecular->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_specular.b = l_res;
				m_light->SetSpecular( l_specular );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditAlphaSpecular( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::ColourValue l_specular = m_light->GetSpecular();
		wxString l_value = m_alphaSpecular->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				l_specular.a = l_res;
				m_light->SetSpecular( l_specular );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0 et 1" ), wxT( "Erreur" ) );
			}
		}
	}
}



void LightProperties :: _onEditAttenuationRange( wxCommandEvent & p_event )
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



void LightProperties :: _onEditAttenuationConstant( wxCommandEvent & p_event )
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



void LightProperties :: _onEditAttenuationLinear( wxCommandEvent & p_event )
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



void LightProperties :: _onEditAttenuationQuadric( wxCommandEvent & p_event )
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



void LightProperties :: _onEditSpotlightInnerAngle( wxCommandEvent & p_event )
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



void LightProperties :: _onEditSpotlightOuterAngle( wxCommandEvent & p_event )
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



void LightProperties :: _onEditSpotlightFalloff( wxCommandEvent & p_event )
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


