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

#include "Light.h"

#include "SceneNode.h"
#include "GUI/ObjectProperties/LightProperties.h"

BEGIN_TROLL_PROJECT_3D_NAMESPACE
{
	TrollLight::TrollLight( const wxString & p_name, const wxString & p_fileName )
		: Troll3DObject( p_name, p_fileName, ttLight )
		, m_lightType( ltDirectional )
		, m_direction( Vector3::ZERO )
		, m_diffuse( ColourValue::White )
		, m_specular( ColourValue::White )
	{
	}

	TrollLight::~TrollLight()
	{
	}

	void TrollLight::AttachTo( TrollSceneNode * p_node )
	{
		Troll3DObject::AttachTo( p_node );

		if ( m_ogreLight->getParentSceneNode() )
		{
			m_ogreLight->getParentSceneNode()->detachObject( m_ogreLight );
		}

		if ( m_parent )
		{
			m_parent->AddObject( this );
			m_parent->GetOgreNode()->attachObject( m_ogreLight );
		}
	}

	int TrollLight::BuildPanel( wxWindow * p_parent, int p_width )
	{
		int l_height = 340 + ( m_lightType == ltSpotLight ? 60 : 0 );
		wxSize l_returnSize( p_width, l_height );

		if ( m_propertiesPanel )
		{
			m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
			delete m_propertiesPanel;
		}

		m_propertiesPanel = new TROLL_GUI_PROPERTIES_NAMESPACE::LightProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
		return l_height;
	}

	void TrollLight::SetOgreLight( Ogre::Light * p_light )
	{
		m_ogreLight = p_light;
		m_lightType = static_cast <TrollLightType>( m_ogreLight->getType() );
		m_direction = m_ogreLight->getDirection();
		m_diffuse = m_ogreLight->getDiffuseColour();
		m_specular = m_ogreLight->getSpecularColour();
		m_attenuation.x = m_ogreLight->getAttenuationRange();
		m_attenuation.y = m_ogreLight->getAttenuationConstant();
		m_attenuation.z = m_ogreLight->getAttenuationLinear();
		m_attenuation.w = m_ogreLight->getAttenuationQuadric();
		m_spotLightRange.x = m_ogreLight->getSpotlightInnerAngle().valueDegrees();
		m_spotLightRange.y = m_ogreLight->getSpotlightOuterAngle().valueDegrees();
		m_spotLightRange.z = m_ogreLight->getSpotlightFalloff();
	}

	void TrollLight::SetLightType( TrollLightType p_lt, bool p_updateOgre )
	{
		m_lightType = p_lt;

		if ( p_updateOgre )
		{
			m_ogreLight->setType( static_cast <Ogre::Light::LightTypes>( p_lt ) );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::SetDirection( const Vector3 & p_dir, bool p_updateOgre )
	{
		m_direction = p_dir;

		if ( p_updateOgre )
		{
			m_ogreLight->setDirection( p_dir );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::SetPosition( const Vector3 & p_pos, bool p_updateOgre )
	{
		m_position = p_pos;

		if ( p_updateOgre )
		{
			m_ogreLight->setPosition( p_pos );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::SetDiffuse( const ColourValue & p_l, bool p_updateOgre )
	{
		m_diffuse = p_l;

		if ( p_updateOgre )
		{
			m_ogreLight->setDiffuseColour( p_l );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::SetSpecular( const ColourValue & p_l, bool p_updateOgre )
	{
		m_specular = p_l;

		if ( p_updateOgre )
		{
			m_ogreLight->setSpecularColour( p_l );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::SetAttenuation( const Vector4 & p_att, bool p_updateOgre )
	{
		m_attenuation = p_att;

		if ( p_updateOgre )
		{
			m_ogreLight->setAttenuation( p_att.x, p_att.y, p_att.z, p_att.w );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::SetSpotLightRange( const Vector3 & p_splr, bool p_updateOgre )
	{
		m_spotLightRange = p_splr;

		if ( p_updateOgre )
		{
			m_ogreLight->setSpotlightRange( Degree( p_splr.x ), Degree( p_splr.y ), p_splr.z );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollLight::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "light " ) + m_name + wxT( "\n{\n" ) );
		wxString l_tmp;

		if ( m_lightType != ltDirectional )
		{
			if ( m_lightType == ltPoint )
			{
				p_stream->WriteString( wxT( "\ttype point\n" ) );
			}
			else if ( m_lightType == ltSpotLight )
			{
				p_stream->WriteString( wxT( "\ttype spotlight\n" ) );
				l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_direction.x, m_direction.y, m_direction.z );
				l_tmp.Replace( wxT( "," ), wxT( "." ) );
				p_stream->WriteString( wxT( "\tdirection " ) + l_tmp + wxT( "\n" ) );
				l_tmp.clear();
				l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_spotLightRange.x, m_spotLightRange.y, m_spotLightRange.z );
				l_tmp.Replace( wxT( "," ), wxT( "." ) );
				p_stream->WriteString( wxT( "\tspotlight_range " ) + l_tmp + wxT( "\n" ) );
				l_tmp.clear();
			}

			if ( m_parent != NULL )
			{
				p_stream->WriteString( wxT( "attach_to " ) + m_parent->GetObjectName() );
			}

			l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_position.x, m_position.y, m_position.z );
			l_tmp.Replace( wxT( "," ), wxT( "." ) );
			p_stream->WriteString( wxT( "\tposition " ) + l_tmp + wxT( "\n" ) );
			l_tmp.clear();
			l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_attenuation.x, m_attenuation.y, m_attenuation.z, m_attenuation.w );
			l_tmp.Replace( wxT( "," ), wxT( "." ) );
			p_stream->WriteString( wxT( "\tattenuation " ) + l_tmp + wxT( "\n" ) );
			l_tmp.clear();
		}
		else
		{
			p_stream->WriteString( wxT( "\ttype directional\n" ) );
			l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_direction.x, m_direction.y, m_direction.z );
			l_tmp.Replace( wxT( "," ), wxT( "." ) );
			p_stream->WriteString( wxT( "\tdirection " ) + l_tmp + wxT( "\n" ) );
			l_tmp.clear();
		}

		l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_diffuse.r, m_diffuse.g, m_diffuse.b, m_diffuse.a );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tdiffuse " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
		l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_specular.r, m_specular.g, m_specular.b, m_specular.a );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tspecular " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
		p_stream->WriteString( wxT( "}\n\n" ) );
	}
}
END_TROLL_PROJECT_3D_NAMESPACE
