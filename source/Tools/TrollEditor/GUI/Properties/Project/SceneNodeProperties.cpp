#include "SceneNodeProperties.h"

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
			SceneNodeProperties::SceneNodeProperties( ProjectComponents::Objects3D::TrollSceneNode & p_sceneNode )
				: ObjectProperty{ DataType::SceneNode }
				, m_sceneNode{ p_sceneNode }
			{
			}

			SceneNodeProperties::~SceneNodeProperties()
			{
			}

			void SceneNodeProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY{ _( "SceneNode: " ) };
				wxString const PROPERTY_NAME{ _( "Name" ) };
				wxString const PROPERTY_PARENT{ _( "Parent Node" ) };
				wxString const PROPERTY_TRANSLATE{ _( "Position" ) };
				wxString const PROPERTY_SCALE{ _( "Scale" ) };
				wxString const PROPERTY_ROTATE{ _( "Orientation" ) };

				wxArrayString l_nodesArray;
				wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_nodesArray );
				l_nodesArray.Sort();
				auto l_parent = m_sceneNode.GetParent();

				DoAddCategory( PROPERTY_CATEGORY );
				DoAddProperty( PROPERTY_NAME, m_sceneNode.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_sceneNode.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_sceneNode.GetObjectName() );
				} );
				DoAddProperty( PROPERTY_PARENT, l_nodesArray, l_parent ? l_parent->GetObjectName() : wxT( "root node" ), [this]( wxPGProperty * p_property )
				{
					m_sceneNode.AttachTo( wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetSceneNode( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					auto l_parent = m_sceneNode.GetParent();
					wxArrayString l_nodesArray;
					wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_nodesArray );
					p_property->SetChoices( wxPGChoices{ l_nodesArray } );
					p_property->SetValue( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) );
				} );
				DoAddProperty< Vector3Names::Point >( PROPERTY_TRANSLATE, m_sceneNode.GetPosition(), [this]( wxPGProperty * p_property )
				{
					m_sceneNode.SetPosition( Vector3RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_sceneNode.GetPosition() ) );
				} );
				DoAddProperty< Vector3Names::Point >( PROPERTY_SCALE, m_sceneNode.GetScale(), [this]( wxPGProperty * p_property )
				{
					m_sceneNode.SetScale( Vector3RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_sceneNode.GetScale() ) );
				} );
				DoAddProperty( PROPERTY_ROTATE, m_sceneNode.GetOrientation(), [this]( wxPGProperty * p_property )
				{
					m_sceneNode.SetOrientation( QuaternionRefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_sceneNode.GetOrientation() ) );
				} );
			}
		}
	}
}
