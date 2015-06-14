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

#include "Entity.h"

#include "Mesh.h"
#include "SceneNode.h"
#include "Project/Media/VideoObject.h"
#include "GUI/ObjectProperties/EntityProperties.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_PROJECT_3D_NAMESPACE
{
	TrollEntity::TrollEntity( const wxString & p_name, const wxString & p_fileName )
		: Troll3DObject( p_name, p_fileName, ttEntity )
		, m_mesh( NULL )
		, m_ogreEntity( NULL )
		, m_video( NULL )
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

	int TrollEntity::BuildPanel( wxWindow * p_parent, int p_width )
	{
		int l_height = 120 + ( m_mesh ? 30 * m_mesh->GetNumSubMaterials() : 0 );
		wxSize l_returnSize( p_width, l_height );

		if ( m_propertiesPanel )
		{
			m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
			delete m_propertiesPanel;
		}

		m_propertiesPanel = new TROLL_GUI_PROPERTIES_NAMESPACE::EntityProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
		return l_height;
	}

	void TrollEntity::SetOgreEntity( Ogre::Entity * p_entity )
	{
		m_ogreEntity = p_entity;
	}

	void TrollEntity::SetMaterial( const wxString & p_name, bool p_updateOgre )
	{
		m_material = p_name;

		if ( p_updateOgre )
		{
			m_ogreEntity->setMaterialName( make_string( p_name ) );
		}
		else if ( m_propertiesPanel != NULL )
		{
			m_propertiesPanel->UpdateProperties();
		}
	}

	void TrollEntity::SetMesh( TrollMesh * p_mesh )
	{
		m_mesh = p_mesh;
		//m_ogreEntity->setMesh( make_string( p_mesh->GetName() ) );
	}

	void TrollEntity::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "object " ) + m_name + wxT( "\n{\n" ) );
		p_stream->WriteString( wxT( "\tmesh " ) + m_mesh->GetName() + wxT( "\n" ) );

		if ( m_parent != NULL )
		{
			p_stream->WriteString( wxT( "\tattach_to " ) + m_parent->GetObjectName() + wxT( "\n" ) );
		}

		if ( m_visible = false )
		{
			p_stream->WriteString( wxT( "\thidden\n" ) );
		}

		if ( m_video != NULL )
		{
			m_video->Write( p_stream );
		}

		p_stream->WriteString( wxT( "}\n\n" ) );
	}
}
END_TROLL_PROJECT_3D_NAMESPACE
