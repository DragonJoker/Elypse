//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "Project/3D/3DObject.h"
#include "Project/3D/SceneNode.h"
#include "GUI/MainFrame.h"
//***********************************************************************************************************
using namespace Troll::Objects3D;
using namespace Troll::GUI;
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************

Troll3DObject :: Troll3DObject( const wxString & p_name, const wxString & p_fileName, Troll3DObjectType p_type )
	:	TrollObject( p_name, p_fileName, tt3DObject ),
		m_parent( NULL ),
		m_3DObjectType( p_type )
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
	g_mainFrame->LogDebugMessage( l_msg );
}

//***********************************************************************************************************

Troll3DObject :: ~Troll3DObject()
{
	if ( m_parent != NULL )
	{
		Detach();
	}
}

//***********************************************************************************************************

void Troll3DObject :: AttachTo( TrollSceneNode * p_node )
{
	if ( m_parent )
	{
		Detach();
	}

	if ( ! p_node )
	{
		return;
	}

	m_parent = p_node;
}

//***********************************************************************************************************

void Troll3DObject :: Detach()
{
	if ( m_parent == NULL )
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

	m_parent = NULL;
}

//***********************************************************************************************************
