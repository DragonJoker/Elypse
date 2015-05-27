
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/CameraProperties.h"

#include "Project/3D/Camera.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

using namespace Troll::GUI;
using namespace Troll::Objects3D;

enum CameraPropsIDs
{
	cameraName,
	cameraParent,
	cameraXPos,
	cameraYPos,
	cameraZPos,
	cameraXDir,
	cameraYDir,
	cameraZDir,
	cameraRoll,
	cameraPitch,
	cameraYaw,
	cameraPolygonMode,
	cameraYFOV
};

const long c_defaultTextEditStyle = wxBORDER_SIMPLE | wxTE_PROCESS_ENTER;
extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( CameraProperties, wxPanel )
	EVT_TEXT_ENTER(	cameraName,				CameraProperties::_onEditName )
	EVT_COMBOBOX(	cameraParent,			CameraProperties::_onSelectParent )
	EVT_TEXT_ENTER(	cameraXPos,				CameraProperties::_onEditXPos )
	EVT_TEXT_ENTER(	cameraYPos,				CameraProperties::_onEditYPos )
	EVT_TEXT_ENTER(	cameraZPos,				CameraProperties::_onEditZPos )
	EVT_TEXT_ENTER(	cameraXDir,				CameraProperties::_onEditXDir )
	EVT_TEXT_ENTER(	cameraYDir,				CameraProperties::_onEditYDir )
	EVT_TEXT_ENTER(	cameraZDir,				CameraProperties::_onEditZDir )
	EVT_TEXT_ENTER(	cameraRoll,				CameraProperties::_onEditRoll )
	EVT_TEXT_ENTER(	cameraPitch,			CameraProperties::_onEditPitch )
	EVT_TEXT_ENTER(	cameraYaw,				CameraProperties::_onEditYaw )
	EVT_COMBOBOX(	cameraPolygonMode,		CameraProperties::_onSelectPolygonMode )
	EVT_TEXT_ENTER(	cameraYFOV,				CameraProperties::_onEditYFOV )
END_EVENT_TABLE()


CameraProperties :: CameraProperties( TrollCamera * p_camera, wxWindow * parent, const wxPoint & pos,
									  const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_camera( p_camera )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_cameraName = BuildTextCtrl( this, l_position, cameraName, wxT( "Name" ), m_camera->GetName() );
	TrollSceneNode * l_parent = m_camera->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
	l_nodesArray.Sort();
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), cameraParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	Vector3 l_camPosition = m_camera->GetPosition();
	m_xPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraXPos, wxT( "X Position" ), l_camPosition.x );
	m_yPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYPos, wxT( "Y Position" ), l_camPosition.y );
	m_zPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraZPos, wxT( "Z Position" ), l_camPosition.z );
	Vector3 l_direction = m_camera->GetDirection();
	m_xDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraXDir, wxT( "X Position" ), l_direction.x );
	m_yDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYDir, wxT( "Y Position" ), l_direction.y );
	m_zDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraZDir, wxT( "Z Position" ), l_direction.z );
	Quaternion l_orientation = m_camera->GetOrientation();
	m_roll = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraRoll, wxT( "Roll" ), l_orientation.getRoll().valueDegrees() );
	m_pitch = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraPitch, wxT( "Pitch" ), l_orientation.getPitch().valueDegrees() );
	m_yaw = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYaw, wxT( "Yaw" ), l_orientation.getYaw().valueDegrees() );
	TrollCameraPolygonMode l_polygonMode = m_camera->GetPolygonMode();
	wxArrayString l_choices;
	l_choices.Add( wxT( "Point" ) );
	l_choices.Add( wxT( "Wireframe" ) );
	l_choices.Add( wxT( "Solid" ) );
	m_polygonMode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), cameraPolygonMode, wxT( "Polygon Mode" ), l_choices, l_choices[static_cast <unsigned int>( l_polygonMode - 1 )] );
	m_yFOV = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYFOV, wxT( "X Position" ), m_camera->GetYFOV() );
}



CameraProperties :: ~CameraProperties()
{
}



void CameraProperties :: UpdateProperties()
{
	m_updatingProperties = true;
	m_cameraName->SetValue( m_camera->GetName() );
	TrollSceneNode * l_parent = m_camera->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
	l_nodesArray.Sort();
	wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
	RemoveChild( m_parentNode );
	delete m_parentNode;
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), cameraParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	Vector3 l_camPosition = m_camera->GetPosition();
	wxString l_tmp;
	l_tmp << l_camPosition.x;
	m_xPosition->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_camPosition.y;
	m_yPosition->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_camPosition.z;
	m_zPosition->SetValue( l_tmp );
	l_tmp.clear();
	Vector3 l_direction = m_camera->GetDirection();
	l_tmp << l_direction.x;
	m_xDirection->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_direction.y;
	m_yDirection->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_direction.z;
	m_zDirection->SetValue( l_tmp );
	l_tmp.clear();
	Quaternion l_orientation = m_camera->GetOrientation();
	l_tmp << l_orientation.getRoll().valueDegrees();
	m_roll->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_orientation.getPitch().valueDegrees();
	m_pitch->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_orientation.getYaw().valueDegrees();
	m_yaw->SetValue( l_tmp );
	l_tmp.clear();
	TrollCameraPolygonMode l_polygonMode = m_camera->GetPolygonMode();
	wxArrayString l_choices;
	l_choices.Add( wxT( "Point" ) );
	l_choices.Add( wxT( "Wireframe" ) );
	l_choices.Add( wxT( "Solid" ) );
	l_tmp = l_choices[static_cast <unsigned int>( l_polygonMode - 1 )];
	m_polygonMode->SetValue( l_choices[static_cast <unsigned int>( l_polygonMode - 1 )] );
	l_tmp << m_camera->GetYFOV();
	m_yFOV->SetValue( l_tmp );
	l_tmp.clear();
	m_updatingProperties = false;
}



void CameraProperties :: _onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_camera->SetName( m_cameraName->GetValue() );
	}
}



void CameraProperties :: _onSelectParent( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		TrollSceneNode * l_node = g_mainFrame->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
		m_camera->AttachTo( l_node );
	}
}



void CameraProperties :: _onEditXPos( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_position = m_camera->GetPosition();
		wxString l_value = m_xPosition->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_position.x = l_res;
			m_camera->SetPosition( l_position );
		}
	}
}



void CameraProperties :: _onEditYPos( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_position = m_camera->GetPosition();
		wxString l_value = m_yPosition->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_position.y = l_res;
			m_camera->SetPosition( l_position );
		}
	}
}



void CameraProperties :: _onEditZPos( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_position = m_camera->GetPosition();
		wxString l_value = m_zPosition->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_position.z = l_res;
			m_camera->SetPosition( l_position );
		}
	}
}



void CameraProperties :: _onEditXDir( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_direction = m_camera->GetDirection();
		wxString l_value = m_xDirection->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_direction.x = l_res;
			m_camera->SetDirection( l_direction );
		}
	}
}



void CameraProperties :: _onEditYDir( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_direction = m_camera->GetDirection();
		wxString l_value = m_yDirection->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_direction.y = l_res;
			m_camera->SetDirection( l_direction );
		}
	}
}



void CameraProperties :: _onEditZDir( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_direction = m_camera->GetDirection();
		wxString l_value = m_zDirection->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_direction.z = l_res;
			m_camera->SetDirection( l_direction );
		}
	}
}



void CameraProperties :: _onEditRoll( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Quaternion l_orientation = m_camera->GetOrientation();
		wxString l_value = m_roll->GetValue();
		float l_angle = l_orientation.getRoll().valueDegrees();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_camera->GetOgreCamera()->roll( Degree( l_res - l_angle ) );
			m_camera->SetOrientation( m_camera->GetOgreCamera()->getOrientation() );
		}
	}
}



void CameraProperties :: _onEditPitch( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Quaternion l_orientation = m_camera->GetOrientation();
		wxString l_value = m_pitch->GetValue();
		float l_angle = l_orientation.getPitch().valueDegrees();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_camera->GetOgreCamera()->pitch( Degree( l_res - l_angle ) );
			m_camera->SetOrientation( m_camera->GetOgreCamera()->getOrientation() );
		}
	}
}



void CameraProperties :: _onEditYaw( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Quaternion l_orientation = m_camera->GetOrientation();
		wxString l_value = m_yaw->GetValue();
		float l_angle = l_orientation.getYaw().valueDegrees();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_camera->GetOgreCamera()->yaw( Degree( l_res - l_angle ) );
			m_camera->SetOrientation( m_camera->GetOgreCamera()->getOrientation() );
		}
	}
}



void CameraProperties :: _onSelectPolygonMode( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_camera->SetPolygonMode( static_cast <TrollCameraPolygonMode>( p_event.GetInt() + 1 ) );
	}
}



void CameraProperties :: _onEditYFOV( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_yFOV->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_camera->SetYFOV( l_res );
		}
	}
}


