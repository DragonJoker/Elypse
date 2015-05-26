//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "Project/3D/Entity.h"
#include "Project/3D/Mesh.h"
#include "Project/3D/SceneNode.h"
#include "Project/Media/VideoObject.h"
#include "GUI/ObjectProperties/EntityProperties.h"
#include "GUI/MainFrame.h"
/*
#include <OgreEntity.h>
#include <wx/txtstrm.h>
*/
//***********************************************************************************************************
using namespace Troll::Objects3D;
using namespace Troll::GUI;
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************

TrollEntity :: TrollEntity( const wxString & p_name, const wxString & p_fileName )
	:	Troll3DObject( p_name, p_fileName, ttEntity ),
		m_mesh( NULL ),
		m_ogreEntity( NULL ),
		m_video( NULL )
{
}

//***********************************************************************************************************

TrollEntity :: ~TrollEntity()
{
}

//***********************************************************************************************************

void TrollEntity :: AttachTo( TrollSceneNode * p_node )
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

//***********************************************************************************************************

int TrollEntity :: BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 120 + ( m_mesh ? 30 * m_mesh->GetNumSubMaterials() : 0 );
	wxSize l_returnSize( p_width, l_height );

	if ( m_propertiesPanel )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new EntityProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
	return l_height;
}

//***********************************************************************************************************

void TrollEntity :: SetOgreEntity( Ogre::Entity * p_entity )
{
	m_ogreEntity = p_entity;
}

//***********************************************************************************************************

void TrollEntity :: SetMaterial( const wxString & p_name, bool p_updateOgre )
{
	m_material = p_name;

	if ( p_updateOgre )
	{
		m_ogreEntity->setMaterialName( p_name.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}

//***********************************************************************************************************

void TrollEntity :: SetMesh( TrollMesh * p_mesh )
{
	m_mesh = p_mesh;
//	m_ogreEntity->setMesh( p_mesh->GetName().char_str().data());
}

//***********************************************************************************************************

void TrollEntity :: Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "object " ) + m_name + wxT( "\n{\n" ) );
	p_stream->WriteString( wxT( "\tmesh " ) + m_mesh->GetName() + wxT( "\n" ) );

	if ( m_parent != NULL )
	{
		p_stream->WriteString( wxT( "\tattach_to " ) + m_parent->GetName() + wxT( "\n" ) );
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

//***********************************************************************************************************
