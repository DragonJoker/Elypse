/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "3DObject.h"

#include "SceneNode.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			Troll3DObject::Troll3DObject( wxString const & p_name, wxString const & p_fileName, Troll3DObjectType p_type )
				: Object( p_name, p_fileName, tt3DObject )
				, m_parent( nullptr )
				, m_3DObjectType( p_type )
			{
				wxString l_msg = wxT( "Creating 3D object : " );

				if ( p_type == ttNode )
				{
					l_msg += wxT( "TrollSceneNode - " );
				}
				else if ( p_type == ttEntity )
				{
					l_msg += wxT( "Entity - " );
				}
				else if ( p_type == ttLight )
				{
					l_msg += wxT( "Light - " );
				}
				else if ( p_type == ttCamera )
				{
					l_msg += wxT( "Camera - " );
				}

				l_msg += wxT( "Name : " ) + p_name;
				LogMessage( l_msg );
			}

			Troll3DObject::~Troll3DObject()
			{
				if ( m_parent != nullptr )
				{
					Detach();
				}
			}

			void Troll3DObject::AttachTo( TrollSceneNode * p_node )
			{
				if ( m_parent )
				{
					Detach();
				}

				if ( !p_node )
				{
					return;
				}

				m_parent = p_node;
			}

			void Troll3DObject::Detach()
			{
				if ( m_parent == nullptr )
				{
					return;
				}

				if ( m_3DObjectType != ttNode )
				{
					m_parent->RemoveObject( m_name );
				}
				else
				{
					m_parent->RemoveChild( m_name );
				}

				m_parent = nullptr;
			}
		}
	}
}
