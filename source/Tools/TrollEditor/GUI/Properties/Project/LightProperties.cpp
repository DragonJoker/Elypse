#include "LightProperties.h"

#include "Project/3D/Light.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"

#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			LightProperties::LightProperties( ProjectComponents::Objects3D::TrollLight & p_light )
				: ObjectProperty{ DataType::Light }
				, m_light{ p_light }
			{
			}

			LightProperties::~LightProperties()
			{
			}

			void LightProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_LIGHT{ _( "Light: " ) };
				wxString const PROPERTY_LIGHT_NAME{ _( "Name" ) };
				wxString const PROPERTY_LIGHT_PARENT_NODE{ _( "Parent Node" ) };
				wxString const PROPERTY_LIGHT_DIRECTION{ _( "Direction" ) };
				wxString const PROPERTY_LIGHT_DIFFUSE{ _( "Diffuse" ) };
				wxString const PROPERTY_LIGHT_SPECULAR{ _( "Specular" ) };
				wxString const PROPERTY_LIGHT_ATTENUATION{ _( "Attenuation" ) };
				wxString const PROPERTY_LIGHT_SPOTLIGHT{ _( "Spotlight" ) };

				wxArrayString l_nodesArray;
				wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_nodesArray );
				l_nodesArray.Sort();
				auto l_parent = m_light.GetParent();

				DoAddCategory( PROPERTY_CATEGORY_LIGHT );
				DoAddProperty( PROPERTY_LIGHT_NAME, m_light.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_light.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_light.GetObjectName() );
				} );
				DoAddProperty( PROPERTY_LIGHT_PARENT_NODE, l_nodesArray, l_parent ? l_parent->GetObjectName() : wxT( "root node" ), [this]( wxPGProperty * p_property )
				{
					m_light.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					wxArrayString l_nodesArray;
					wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_nodesArray );
					l_nodesArray.Sort();
					auto l_parent = m_light.GetParent();
					p_property->SetChoices( wxPGChoices{ l_nodesArray } );
					p_property->SetValue( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) );
				} );
				DoAddProperty< Vector3Names::Point >( PROPERTY_LIGHT_DIRECTION, m_light.GetDirection(), [this]( wxPGProperty * p_property )
				{
					m_light.SetDirection( Vector3RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_light.GetDirection() ) );
				} );
				DoAddProperty( PROPERTY_LIGHT_DIFFUSE, make_wxColour( m_light.GetDiffuse() ), [this]( wxPGProperty * p_property )
				{
					wxColour l_colour;
					l_colour << p_property->GetValue();
					m_light.SetDiffuse( make_ColourValue( l_colour ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_light.GetDiffuse() ) );
				} );
				DoAddProperty( PROPERTY_LIGHT_SPECULAR, make_wxColour( m_light.GetSpecular() ), [this]( wxPGProperty * p_property )
				{
					wxColour l_colour;
					l_colour << p_property->GetValue();
					m_light.SetSpecular( make_ColourValue( l_colour ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_light.GetSpecular() ) );
				} );
				DoAddProperty< Vector4Names::Attenuation >( PROPERTY_LIGHT_ATTENUATION, m_light.GetAttenuation(), [this]( wxPGProperty * p_property )
				{
					m_light.SetAttenuation( Vector4RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_light.GetAttenuation() ) );
				} );

				if ( m_light.GetLightType() == ProjectComponents::Objects3D::ltSpotLight )
				{
					DoAddProperty< Vector3Names::Spotlight >( PROPERTY_LIGHT_SPOTLIGHT, m_light.GetSpotlightRange(), [this]( wxPGProperty * p_property )
					{
						m_light.SetSpotLightRange( Vector3RefFromVariant( p_property->GetValue() ) );
					},
					[this]( wxPGProperty * p_property )
					{
						p_property->SetValue( WXVARIANT( m_light.GetSpotlightRange() ) );
					} );
				}
			}
		}
	}
}
