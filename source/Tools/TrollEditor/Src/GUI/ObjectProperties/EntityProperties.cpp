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
#include "GUI/ObjectProperties/EntityProperties.h"

#include "Project/3D/Entity.h"
#include "Project/3D/Mesh.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum EntityPropsIDs
	{
		entityName,
		entityParent,
		entityMesh,
		entityMeshSubMats,
	};

	BEGIN_EVENT_TABLE( EntityProperties, wxPanel )
		EVT_TEXT_ENTER( entityName, EntityProperties::OnEditName )
		EVT_COMBOBOX( entityParent, EntityProperties::OnSelectParent )
		EVT_COMBOBOX( entityMesh, EntityProperties::OnSelectMesh )
		EVT_LIST_END_LABEL_EDIT( entityMeshSubMats, EntityProperties::OnEndEditLabel )
	END_EVENT_TABLE()

	EntityProperties::EntityProperties( TROLL_PROJECT_3D_NAMESPACE::TrollEntity * p_entity, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_entity( p_entity )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_entityName = BuildTextCtrl( this, l_position, entityName, _( "Name" ), m_entity->GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_entity->GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
		l_nodesArray.Sort();
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
		TROLL_PROJECT_3D_NAMESPACE::TrollMesh * l_mesh = m_entity->GetMesh();
		wxArrayString l_meshesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetMeshesNames( l_meshesArray );
		l_meshesArray.Sort();
		m_meshName = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityMesh, _( "Mesh" ), l_meshesArray, ( l_mesh ? l_mesh->GetName() : wxT( "" ) ) );

		if ( l_mesh != NULL )
		{
			unsigned int l_nbSubMat = l_mesh->GetNumSubMaterials();
			wxArrayString l_captions;
			wxArrayString l_values;

			for ( unsigned int i = 0 ; i < l_nbSubMat ; i++ )
			{
				l_captions.Add( _( "Submaterial " ) + ( wxString() << i ) );
				l_values.Add( l_mesh->GetMaterial( i ) );
			}

			m_submatValues = BuildListBox( this, l_position += wxPoint( 0, 20 ), entityMeshSubMats, l_captions, l_values );
		}
	}

	EntityProperties::~EntityProperties()
	{
	}

	void EntityProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
		m_entityName->SetValue( m_entity->GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_entity->GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
		l_nodesArray.Sort();
		RemoveChild( m_parentNode );
		delete m_parentNode;
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
		TROLL_PROJECT_3D_NAMESPACE::TrollMesh * l_mesh = m_entity->GetMesh();
		wxArrayString l_meshesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetMeshesNames( l_meshesArray );
		l_meshesArray.Sort();
		RemoveChild( m_meshName );
		delete m_meshName;
		m_meshName = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityMesh, _( "Mesh" ), l_meshesArray, ( l_mesh ? l_mesh->GetName() : wxT( "" ) ) );

		if ( l_mesh != NULL )
		{
			unsigned int l_nbSubMat = l_mesh->GetNumSubMaterials();
			wxArrayString l_captions;
			wxArrayString l_values;

			for ( unsigned int i = 0 ; i < l_nbSubMat ; i++ )
			{
				l_captions.Add( _( "Submaterial " ) + ( wxString() << i ) );
				l_values.Add( l_mesh->GetMaterial( i ) );
			}

			if ( m_submatValues != NULL )
			{
				RemoveChild( m_submatValues );
				delete m_submatValues;
			}

			m_submatValues = BuildListBox( this, l_position += wxPoint( 0, 20 ), entityMeshSubMats, l_captions, l_values );
		}

		m_updatingProperties = false;
	}

	void EntityProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_entity->SetObjectName( m_entityName->GetValue() );
		}
	}

	void EntityProperties::OnSelectParent( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_node = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
			m_entity->AttachTo( l_node );
		}
	}

	void EntityProperties::OnSelectMesh( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			TROLL_PROJECT_3D_NAMESPACE::TrollMesh * l_mesh = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetMesh( m_meshName->GetValue() );
			m_entity->SetMesh( l_mesh );
		}
	}

	void EntityProperties::OnEndEditLabel( wxListEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			std::cout << "EntityProperties::OnEndEditLabel\n";
			long l_index = p_event.GetIndex();
			wxString l_value = p_event.GetText();
			TROLL_PROJECT_3D_NAMESPACE::TrollMesh * l_mesh = m_entity->GetMesh();

			if ( l_mesh )
			{
				l_mesh->SetSubMaterial( l_value, l_index );
			}
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
