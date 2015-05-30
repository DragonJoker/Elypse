
#include "PrecompiledHeader.h"

#include "Project/3D/Camera.h"
#include "Project/3D/SceneNode.h"
#include "GUI/ObjectProperties/CameraProperties.h"
/*
#include <OgreCamera.h>
#include <wx/txtstrm.h>
*/

using namespace Troll::Objects3D;
using namespace Troll::GUI;


TrollCamera::TrollCamera( const wxString & p_name, const wxString & p_fileName )
	:	Troll3DObject( p_name, p_fileName, ttCamera ),
		m_position( Vector3::ZERO ),
		m_direction( Vector3::ZERO ),
		m_orientation( Quaternion::IDENTITY ),
		m_polygonMode( tcSolid ),
		m_yFOV( 0.0 ),
		m_ogreCamera( NULL )
{
}



TrollCamera::~TrollCamera()
{
}



void TrollCamera::AttachTo( TrollSceneNode * p_node )
{
	Troll3DObject::AttachTo( p_node );

	if ( m_ogreCamera->getParentSceneNode() )
	{
		m_ogreCamera->getParentSceneNode()->detachObject( m_ogreCamera );
	}

	if ( m_parent )
	{
		m_parent->AddObject( this );
		m_parent->GetOgreNode()->attachObject( m_ogreCamera );
	}
}



int TrollCamera::BuildPanel( wxWindow * p_parent, int p_width )
{
	wxSize l_returnSize = wxSize( p_width, 260 );

	if ( m_propertiesPanel )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new CameraProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
	return l_returnSize.y;
}



void TrollCamera::SetOgreCamera( Ogre::Camera * p_camera )
{
	m_ogreCamera = p_camera;
	m_position = m_ogreCamera->getPosition();
	m_direction = m_ogreCamera->getDirection();
	m_polygonMode = static_cast <TrollCameraPolygonMode>( m_ogreCamera->getPolygonMode() );
	m_yFOV = m_ogreCamera->getFOVy().valueDegrees();
}



void TrollCamera::SetPosition( const Ogre::Vector3 & p_pos, bool p_updateOgre )
{
	m_position = p_pos;

	if ( p_updateOgre )
	{
		m_ogreCamera->setPosition( p_pos );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollCamera::SetDirection( const Ogre::Vector3 & p_dir, bool p_updateOgre )
{
	m_direction = p_dir;

	if ( p_updateOgre )
	{
		m_ogreCamera->setDirection( p_dir );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollCamera::SetOrientation( const Ogre::Quaternion & p_orientation, bool p_updateOgre )
{
	m_orientation = p_orientation;
}



void TrollCamera::SetPolygonMode( TrollCameraPolygonMode p_cpm, bool p_updateOgre )
{
	m_polygonMode = p_cpm;

	if ( p_updateOgre )
	{
		m_ogreCamera->setPolygonMode( static_cast <Ogre::PolygonMode>( p_cpm ) );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollCamera::SetYFOV( float p_fov, bool p_updateOgre )
{
	m_yFOV = p_fov;

	if ( p_updateOgre )
	{
		m_ogreCamera->setFOVy( Degree( p_fov ) );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollCamera::Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "camera " ) + m_name + wxT( "\n{\n" ) );
	wxString l_tmp;

	if ( m_position != Vector3::ZERO )
	{
		l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_position.x, m_position.y, m_position.z );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tposition " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_direction != Vector3::NEGATIVE_UNIT_Z )
	{
		l_tmp.Printf( wxT( "%.2f %.2f %.2f" ), m_direction.x, m_direction.y, m_direction.z );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tdirection " ) + l_tmp + wxT( "\n" ) );
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

	if ( m_polygonMode != tcSolid )
	{
		p_stream->WriteString( wxT( "\tpolygon_mode " ) + ( m_polygonMode == tcPoint ? make_wxString( "point" ) : make_wxString( "wireframe" ) ) + wxT( "\n" ) );
	}

	if ( m_yFOV != 45.0 )
	{
		l_tmp.Printf( wxT( "%.2f" ), m_yFOV );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\tfov " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_parent != NULL )
	{
		p_stream->WriteString( wxT( "\tattach_to " ) + m_parent->GetName() + wxT( "\n" ) );
	}

	p_stream->WriteString( wxT( "}\n\n" ) );
}


