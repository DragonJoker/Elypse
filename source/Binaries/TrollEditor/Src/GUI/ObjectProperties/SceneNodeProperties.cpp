
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/SceneNodeProperties.h"

#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

using namespace Troll::GUI;
using namespace Troll::Objects3D;

enum SceneNodePropsIDs
{
	sceneNodeName,
	sceneNodeParent,
	sceneNodeXPos,
	sceneNodeYPos,
	sceneNodeZPos,
	sceneNodeXScale,
	sceneNodeYScale,
	sceneNodeZScale,
	sceneNodeRoll,
	sceneNodePitch,
	sceneNodeYaw
};

const long c_defaultTextEditStyle = wxBORDER_SIMPLE | wxTE_PROCESS_ENTER;
extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( SceneNodeProperties, wxPanel )
	EVT_TEXT_ENTER(	sceneNodeName,				SceneNodeProperties::_onEditName )
	EVT_COMBOBOX(	sceneNodeParent,			SceneNodeProperties::_onSelectParent )
	EVT_TEXT_ENTER(	sceneNodeXPos,				SceneNodeProperties::_onEditXPos )
	EVT_TEXT_ENTER(	sceneNodeYPos,				SceneNodeProperties::_onEditYPos )
	EVT_TEXT_ENTER(	sceneNodeZPos,				SceneNodeProperties::_onEditZPos )
	EVT_TEXT_ENTER(	sceneNodeXScale,			SceneNodeProperties::_onEditXScale )
	EVT_TEXT_ENTER(	sceneNodeYScale,			SceneNodeProperties::_onEditYScale )
	EVT_TEXT_ENTER(	sceneNodeZScale,			SceneNodeProperties::_onEditZScale )
	EVT_TEXT_ENTER(	sceneNodeRoll,				SceneNodeProperties::_onEditRoll )
	EVT_TEXT_ENTER(	sceneNodePitch,				SceneNodeProperties::_onEditPitch )
	EVT_TEXT_ENTER(	sceneNodeYaw,				SceneNodeProperties::_onEditYaw )
END_EVENT_TABLE()


SceneNodeProperties::SceneNodeProperties( TrollSceneNode * p_sceneNode, wxWindow * parent, const wxPoint & pos,
		const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_sceneNode( p_sceneNode )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_nodeName = BuildTextCtrl( this, l_position, sceneNodeName, wxT( "Name" ), m_sceneNode->GetName() );
	TrollSceneNode * l_parent = m_sceneNode->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray, m_sceneNode->GetName() );
	l_nodesArray.Sort();
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), sceneNodeParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	Vector3 l_nodePosition = m_sceneNode->GetPosition();
	m_xPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeXPos, wxT( "X Position" ), l_nodePosition.x );
	m_yPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeYPos, wxT( "Y Position" ), l_nodePosition.y );
	m_zPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeZPos, wxT( "Z Position" ), l_nodePosition.z );
	Vector3 l_scale = m_sceneNode->GetScale();
	m_xScale = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeXScale, wxT( "X Scale" ), l_scale.x );
	m_yScale = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeYScale, wxT( "Y Scale" ), l_scale.y );
	m_zScale = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeZScale, wxT( "Z Scale" ), l_scale.z );
	Quaternion l_orientation = m_sceneNode->GetOrientation();
	m_roll = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeRoll, wxT( "Roll" ), l_orientation.getRoll().valueDegrees() );
	m_pitch = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodePitch, wxT( "Pitch" ), l_orientation.getPitch().valueDegrees() );
	m_yaw = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeYaw, wxT( "Yaw" ), l_orientation.getYaw().valueDegrees() );
}



SceneNodeProperties::~SceneNodeProperties()
{
}



void SceneNodeProperties::UpdateProperties()
{
	m_updatingProperties = true;
	wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
	m_nodeName->SetValue( m_sceneNode->GetName() );
	TrollSceneNode * l_parent = m_sceneNode->GetParent();
	wxArrayString l_nodesArray;
	g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray, m_sceneNode->GetName() );
	l_nodesArray.Sort();
	RemoveChild( m_parentNode );
	delete m_parentNode;
	m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), sceneNodeParent, wxT( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetName() : wxT( "root node" ) ) );
	Vector3 l_nodePosition = m_sceneNode->GetPosition();
	wxString l_tmp;
	l_tmp << l_nodePosition.x;
	m_xPosition->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_nodePosition.y;
	m_yPosition->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_nodePosition.z;
	m_zPosition->SetValue( l_tmp );
	l_tmp.clear();
	Vector3 l_scale = m_sceneNode->GetScale();
	l_tmp << l_scale.x;
	m_xScale->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_scale.y;
	m_yScale->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_scale.z;
	m_zScale->SetValue( l_tmp );
	l_tmp.clear();
	Quaternion l_orientation = m_sceneNode->GetOrientation();
	l_tmp << l_orientation.getRoll().valueDegrees();
	m_roll->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_orientation.getPitch().valueDegrees();
	m_pitch->SetValue( l_tmp );
	l_tmp.clear();
	l_tmp << l_orientation.getYaw().valueDegrees();
	m_yaw->SetValue( l_tmp );
	l_tmp.clear();
	m_updatingProperties = false;
}



void SceneNodeProperties::_onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_sceneNode->SetName( m_nodeName->GetValue() );
	}
}



void SceneNodeProperties::_onSelectParent( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		TrollSceneNode * l_node = g_mainFrame->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
		m_sceneNode->AttachTo( l_node );
	}
}



void SceneNodeProperties::_onEditXPos( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_position = m_sceneNode->GetPosition();
		wxString l_value = m_xPosition->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_position.x = l_res;
			m_sceneNode->SetPosition( l_position );
		}
	}
}



void SceneNodeProperties::_onEditYPos( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_position = m_sceneNode->GetPosition();
		wxString l_value = m_yPosition->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_position.y = l_res;
			m_sceneNode->SetPosition( l_position );
		}
	}
}



void SceneNodeProperties::_onEditZPos( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_position = m_sceneNode->GetPosition();
		wxString l_value = m_zPosition->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_position.z = l_res;
			m_sceneNode->SetPosition( l_position );
		}
	}
}



void SceneNodeProperties::_onEditXScale( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_scale = m_sceneNode->GetScale();
		wxString l_value = m_xScale->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_scale.x = l_res;
			m_sceneNode->SetScale( l_scale );
		}
	}
}



void SceneNodeProperties::_onEditYScale( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_scale = m_sceneNode->GetScale();
		wxString l_value = m_yScale->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_scale.y = l_res;
			m_sceneNode->SetScale( l_scale );
		}
	}
}



void SceneNodeProperties::_onEditZScale( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Vector3 l_scale = m_sceneNode->GetScale();
		wxString l_value = m_zScale->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			l_scale.z = l_res;
			m_sceneNode->SetScale( l_scale );
		}
	}
}



void SceneNodeProperties::_onEditRoll( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Quaternion l_orientation = m_sceneNode->GetOrientation();
		wxString l_value = m_roll->GetValue();
		float l_angle = l_orientation.getRoll().valueDegrees();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_sceneNode->GetOgreNode()->roll( Degree( l_res - l_angle ) );
			m_sceneNode->SetOrientation( m_sceneNode->GetOgreNode()->getOrientation() );
		}
	}
}



void SceneNodeProperties::_onEditPitch( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Quaternion l_orientation = m_sceneNode->GetOrientation();
		wxString l_value = m_pitch->GetValue();
		float l_angle = l_orientation.getPitch().valueDegrees();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_sceneNode->GetOgreNode()->pitch( Degree( l_res - l_angle ) );
			m_sceneNode->SetOrientation( m_sceneNode->GetOgreNode()->getOrientation() );
		}
	}
}



void SceneNodeProperties::_onEditYaw( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		Ogre::Quaternion l_orientation = m_sceneNode->GetOrientation();
		wxString l_value = m_yaw->GetValue();
		float l_angle = l_orientation.getYaw().valueDegrees();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_sceneNode->GetOgreNode()->yaw( Degree( l_res - l_angle ) );
			m_sceneNode->SetOrientation( m_sceneNode->GetOgreNode()->getOrientation() );
		}
	}
}


