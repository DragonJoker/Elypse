
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/EntityProperties.h"

#include "Project/3D/Entity.h"
#include "Project/3D/Mesh.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

using namespace Troll::GUI;
using namespace Troll::Objects3D;

enum EntityPropsIDs
{
	entityName,
	entityParent,
	entityMesh,
	entityMeshSubMats,
};

extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( EntityProperties, wxPanel )
	EVT_TEXT_ENTER(	entityName,			EntityProperties::_onEditName )
	EVT_COMBOBOX(	entityParent,		EntityProperties::_onSelectParent )
	EVT_COMBOBOX(	entityMesh,			EntityProperties::_onSelectMesh )
	EVT_LIST_END_LABEL_EDIT(	entityMeshSubMats,	EntityProperties::_onEndEditLabel )
END_EVENT_TABLE()


EntityProperties::EntityProperties( TrollEntity * p_entity, wxWindow * parent, const wxPoint & pos,
									const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_entity( p_entity )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_entityName = BuildTextCtrl( this, l_position, entityName, wxT( "Name" ), m_entity->GetName() );
	TrollSceneNode * l_parent = m_entity->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
	l_nodesArray.Sort();
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	TrollMesh * l_mesh = m_entity->GetMesh();
	wxArrayString l_meshesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetMeshesNames( l_meshesArray );
	l_meshesArray.Sort();
	m_meshName = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityMesh, wxT( "Mesh" ), l_meshesArray, ( l_mesh ? l_mesh->GetName() : wxT( "" ) ) );

	if ( l_mesh != NULL )
	{
		unsigned int l_nbSubMat = l_mesh->GetNumSubMaterials();
		wxArrayString l_captions;
		wxArrayString l_values;

		for ( unsigned int i = 0 ; i < l_nbSubMat ; i++ )
		{
			l_captions.Add( wxT( "Submaterial " ) + ( wxString() << i ) );
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
	m_entityName->SetValue( m_entity->GetName() );
	TrollSceneNode * l_parent = m_entity->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
	l_nodesArray.Sort();
	RemoveChild( m_parentNode );
	delete m_parentNode;
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	TrollMesh * l_mesh = m_entity->GetMesh();
	wxArrayString l_meshesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetMeshesNames( l_meshesArray );
	l_meshesArray.Sort();
	RemoveChild( m_meshName );
	delete m_meshName;
	m_meshName = BuildComboBox( this, l_position += wxPoint( 0, 20 ), entityMesh, wxT( "Mesh" ), l_meshesArray, ( l_mesh ? l_mesh->GetName() : wxT( "" ) ) );

	if ( l_mesh != NULL )
	{
		unsigned int l_nbSubMat = l_mesh->GetNumSubMaterials();
		wxArrayString l_captions;
		wxArrayString l_values;

		for ( unsigned int i = 0 ; i < l_nbSubMat ; i++ )
		{
			l_captions.Add( wxT( "Submaterial " ) + ( wxString() << i ) );
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



void EntityProperties::_onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_entity->SetName( m_entityName->GetValue() );
	}
}



void EntityProperties::_onSelectParent( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		TrollSceneNode * l_node = g_mainFrame->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
		m_entity->AttachTo( l_node );
	}
}



void EntityProperties::_onSelectMesh( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		TrollMesh * l_mesh = g_mainFrame->GetCurrentProject()->GetMainScene()->GetMesh( m_meshName->GetValue() );
		m_entity->SetMesh( l_mesh );
	}
}



void EntityProperties::_onEndEditLabel( wxListEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		std::cout << "EntityProperties::_onEndEditLabel\n";
		long l_index = p_event.GetIndex();
		wxString l_value = p_event.GetText();
		TrollMesh * l_mesh = m_entity->GetMesh();

		if ( l_mesh )
		{
			l_mesh->SetSubMaterial( l_value, l_index );
		}
	}
}


