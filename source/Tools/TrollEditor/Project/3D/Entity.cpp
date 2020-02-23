#include "Entity.h"

#include "Mesh.h"
#include "SceneNode.h"
#include "Project/Media/VideoObject.h"
#include "GUI/Properties/Project/EntityProperties.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			TrollEntity::TrollEntity( wxString const & p_name, wxString const & p_fileName )
				: Troll3DObject( p_name, p_fileName, ttEntity )
				, m_mesh( nullptr )
				, m_ogreEntity( nullptr )
				, m_video( nullptr )
			{
			}

			TrollEntity::~TrollEntity()
			{
			}

			void TrollEntity::AttachTo( TrollSceneNode * p_node )
			{
				Troll3DObject::AttachTo( p_node );

				if ( m_ogreEntity->getParentSceneNode() )
				{
					m_ogreEntity->getParentSceneNode()->detachObject( m_ogreEntity );
				}

				if ( m_parent )
				{
					m_parent->AddObject( this );
					m_parent->GetOgreNode()->attachObject( m_ogreEntity );
				}
			}

			void TrollEntity::SetOgreEntity( Ogre::Entity * p_entity )
			{
				m_ogreEntity = p_entity;
			}

			void TrollEntity::SetMaterial( wxString const & p_name, bool p_updateOgre )
			{
				m_material = p_name;

				if ( p_updateOgre )
				{
					m_ogreEntity->setMaterialName( make_string( p_name ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollEntity::SetMesh( TrollMesh * p_mesh )
			{
				m_mesh = p_mesh;
				//m_ogreEntity->setMesh( make_string( p_mesh->GetName() ) );
			}

			void TrollEntity::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "object " ) + m_name + wxT( "\n{\n" ) );
				p_stream.WriteString( wxT( "\tmesh " ) + m_mesh->GetName() + wxT( "\n" ) );

				if ( m_parent != nullptr )
				{
					p_stream.WriteString( wxT( "\tattach_to " ) + m_parent->GetObjectName() + wxT( "\n" ) );
				}

				if ( m_visible = false )
				{
					p_stream.WriteString( wxT( "\thidden\n" ) );
				}

				if ( m_video != nullptr )
				{
					m_video->Write( p_stream );
				}

				p_stream.WriteString( wxT( "}\n\n" ) );
			}

			GUI::Properties::ObjectProperty * TrollEntity::DoCreateProperties()
			{
				return new GUI::Properties::EntityProperties( *this );
			}
		}
	}
}
