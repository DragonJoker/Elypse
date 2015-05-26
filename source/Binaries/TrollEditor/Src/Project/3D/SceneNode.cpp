//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "Project/3D/SceneNode.h"
#include "Project/Media/SoundObject.h"
#include "Project/3D/3DObject.h"
#include "GUI/ObjectProperties/SceneNodeProperties.h"
/*
#include <wx/listctrl.h>
#include <wx/txtstrm.h>
*/
//***********************************************************************************************************
using namespace Troll::Objects3D;
using namespace Troll::GUI;
//***********************************************************************************************************

TrollSceneNode :: TrollSceneNode( const wxString & p_name, const wxString & p_fileName, const Vector3 & p_position,
								  const Quaternion & p_orientation, const Vector3 & p_scale )
	:	Troll3DObject( p_name, p_fileName, ttNode ),
		m_position( p_position ),
		m_orientation( p_orientation ),
		m_scale( p_scale ),
		m_ogreNode( NULL ),
		m_sound( NULL ),
		m_inheritOrientation( false ),
		m_inheritScale( false )
{
}

//***********************************************************************************************************

TrollSceneNode :: ~TrollSceneNode()
{
	m_parent = NULL;
	m_childs.clear();
	m_objects.clear();
	/*
		if (m_childs.size() > 0)
		{
			TrollSceneNodeMap::iterator l_it = m_childs.begin();
			while (l_it != m_childs.end())
			{
				l_it->second->Detach();
				++l_it;
			}
		}
		if (m_objects.size() > 0)
		{
			Troll3DObjectMap::iterator l_it = m_objects.begin();
			while (l_it != m_objects.end())
			{
				l_it->second->Detach();
				++l_it;
			}
		}
	*/
}

//***********************************************************************************************************

void TrollSceneNode :: AttachTo( TrollSceneNode * p_node )
{
	Troll3DObject::AttachTo( p_node );

	if ( m_ogreNode->getParentSceneNode() )
	{
		m_ogreNode->getParentSceneNode()->removeChild( m_ogreNode );
	}

	if ( m_parent )
	{
		m_parent->AddChild( this );
		m_parent->GetOgreNode()->addChild( m_ogreNode );
	}
}

//***********************************************************************************************************

int TrollSceneNode :: BuildPanel( wxWindow * p_parent, int p_width )
{
	wxSize l_returnSize( p_width, 160 );

	if ( m_propertiesPanel )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new SceneNodeProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
	return l_returnSize.y;
}

//***********************************************************************************************************

bool TrollSceneNode :: AddObject( Troll3DObject * p_object )
{
	wxString l_name = p_object->GetName();

	if ( m_objects.find( l_name ) != m_objects.end() )
	{
		return false;
	}

	m_objects[l_name] = p_object;
	return true;
}

//***********************************************************************************************************

bool TrollSceneNode :: AddChild( TrollSceneNode * p_node )
{
	wxString l_name = p_node->GetName();

	if ( m_childs.find( l_name ) != m_childs.end() )
	{
		return false;
	}

	m_childs[l_name] = p_node;
	return true;
}

//***********************************************************************************************************

void TrollSceneNode :: SetOgreNode( Ogre::SceneNode * p_node )
{
	m_ogreNode = p_node;
	m_orientation = m_ogreNode->getOrientation();
	m_position = m_ogreNode->getPosition();
	m_scale = m_ogreNode->getScale();
	m_inheritOrientation = m_ogreNode->getInheritOrientation();
	m_inheritScale = m_ogreNode->getInheritScale();
}

//***********************************************************************************************************

void TrollSceneNode :: SetPosition( const Vector3 & p_position, bool p_updateOgre )
{
	m_position = p_position;

	if ( p_updateOgre )
	{
		m_ogreNode->setPosition( p_position );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->Update();
	}
}

//***********************************************************************************************************

void TrollSceneNode :: SetOrientation( const Quaternion & p_orientation, bool p_updateOgre )
{
	m_orientation = p_orientation;

	if ( p_updateOgre )
	{
		m_ogreNode->setOrientation( p_orientation );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->Update();
	}
}

//***********************************************************************************************************

void TrollSceneNode :: SetScale( const Vector3 & p_scale, bool p_updateOgre )
{
	m_scale = p_scale;

	if ( p_updateOgre )
	{
		m_ogreNode->setScale( p_scale );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->Update();
	}
}

//***********************************************************************************************************

void TrollSceneNode :: RemoveObject( const wxString & p_name )
{
	Troll3DObjectMap::iterator l_it = m_objects.find( p_name );

	if ( l_it == m_objects.end() )
	{
		return;
	}

	m_objects.erase( l_it );
}

//***********************************************************************************************************

void TrollSceneNode :: RemoveChild( const wxString & p_name )
{
	TrollSceneNodeMap::iterator l_it = m_childs.find( p_name );

	if ( l_it == m_childs.end() )
	{
		return;
	}

	m_childs.erase( l_it );
}

//***********************************************************************************************************

void TrollSceneNode :: Write( wxTextOutputStream * p_stream )
{
	if ( m_name == wxT( "root node" ) )
	{
		return;
	}

	p_stream->WriteString( wxT( "scene_node " ) + m_name + wxT( "\n{\n" ) );

	if ( m_parent != NULL )
	{
		p_stream->WriteString( wxT( "\tparent " ) + m_parent->GetName() + wxT( "\n" ) );
	}

	wxString l_tmp;

	if ( m_position != Vector3::ZERO )
	{
		l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_position.x, m_position.y, m_position.z );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tposition " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_orientation != Quaternion::IDENTITY )
	{
		Radian l_angle;
		Vector3 l_axis;
		m_orientation.ToAngleAxis( l_angle, l_axis );
		l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), l_axis.x, l_axis.y, l_axis.z, l_angle.valueDegrees() );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\torientation " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_scale != Vector3::UNIT_SCALE )
	{
		l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_scale.x, m_scale.y, m_scale.z );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tscale " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_inheritOrientation )
	{
		p_stream->WriteString( wxT( "\tinherit_orientation true\n" ) );
	}

	if ( m_inheritScale )
	{
		p_stream->WriteString( wxT( "\tinherit_scale true\n" ) );
	}

	if ( m_sound != NULL )
	{
		m_sound->Write( p_stream );
	}

	p_stream->WriteString( wxT( "}\n\n" ) );
	General::Utils::map::cycle( m_childs, & TrollSceneNode::Write, p_stream );
}

//***********************************************************************************************************
