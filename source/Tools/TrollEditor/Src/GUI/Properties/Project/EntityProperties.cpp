#include "EntityProperties.h"

#include "Project/3D/Entity.h"
#include "Project/3D/Mesh.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"

#include "GUI/ProjectFrame.h"

#include <OgreMaterial.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			EntityProperties::EntityProperties( ProjectComponents::Objects3D::TrollEntity & p_entity )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_ENTITY }
				, m_entity{ p_entity }
			{
			}

			EntityProperties::~EntityProperties()
			{
			}

			void EntityProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_ENTITY{ _( "Entity: " ) };
				wxString const PROPERTY_ENTITY_NAME{ _( "Name" ) };
				wxString const PROPERTY_ENTITY_PARENT_NODE{ _( "Parent Node" ) };
				wxString const PROPERTY_ENTITY_MESH{ _( "Mesh" ) };

				wxArrayString l_nodesArray;
				wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_nodesArray );
				l_nodesArray.Sort();
				auto l_parent = m_entity.GetParent();

				wxArrayString l_meshesArray;
				wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetMeshesNames( l_meshesArray );
				l_meshesArray.Sort();
				auto l_mesh = m_entity.GetMesh();

				DoAddCategory( PROPERTY_CATEGORY_ENTITY );
				DoAddProperty( PROPERTY_ENTITY_NAME, m_entity.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_entity.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_entity.GetObjectName() );
				} );
				DoAddProperty( PROPERTY_ENTITY_PARENT_NODE, l_nodesArray, l_parent ? l_parent->GetObjectName() : wxT( "root node" ), [this]( wxPGProperty * p_property )
				{
					m_entity.AttachTo( wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetSceneNode( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					auto l_parent = m_entity.GetParent();
					wxArrayString l_nodesArray;
					wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_nodesArray );
					p_property->SetChoices( wxPGChoices{ l_nodesArray } );
					p_property->SetValue( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) );
				} );
				DoAddProperty( PROPERTY_ENTITY_MESH, l_meshesArray, l_mesh ? l_mesh->GetName() : wxEmptyString, [this]( wxPGProperty * p_property )
				{
					m_entity.SetMesh( wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetMesh( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					auto l_mesh = m_entity.GetMesh();
					wxArrayString l_meshesArray;
					wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetMeshesNames( l_meshesArray );
					p_property->SetChoices( wxPGChoices{ l_meshesArray } );
					p_property->SetValue( l_mesh ? l_mesh->GetName() : wxEmptyString );
					DoRemoveMaterialsProperties();

					if ( l_mesh )
					{
						DoUpdateMeshMaterials();
					}
				} );

				if ( l_mesh )
				{
					DoUpdateMeshMaterials();
				}
			}

			void EntityProperties::DoUpdateMeshMaterials()
			{
				wxString const PROPERTY_ENTITY_SUBMATERIAL{ _( "Submaterial " ) };

				auto l_mesh = m_entity.GetMesh();
				auto l_nbSubMat = l_mesh->GetNumSubMaterials();
				wxArrayString l_values;

				auto materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

				while ( materialIterator.hasMoreElements() )
				{
					l_values.push_back( materialIterator.peekNextValue()->getName() );
					materialIterator.moveNext();
				}

				for ( uint32_t i = 0; i < l_nbSubMat; i++ )
				{
					DoAddProperty( PROPERTY_ENTITY_SUBMATERIAL + ( wxString() << i )
						, l_values
						, l_mesh->GetMaterial( i )
						, [this, l_mesh, i]( wxPGProperty * p_property )
						{
							l_mesh->SetSubMaterial( p_property->GetValue(), i );
						},
						[this, l_mesh, i]( wxPGProperty * p_property )
						{
							p_property->SetValue( l_mesh->GetMaterial( i ) );
						} );
				}
			}

			void EntityProperties::DoRemoveMaterialsProperties()
			{
				// TODO
			}
		}
	}
}
