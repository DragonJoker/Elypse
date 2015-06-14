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

#include "SceneNodeProperties.h"

#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
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

	BEGIN_EVENT_TABLE( SceneNodeProperties, wxPanel )
		EVT_TEXT_ENTER( sceneNodeName, SceneNodeProperties::OnEditName )
		EVT_COMBOBOX( sceneNodeParent, SceneNodeProperties::OnSelectParent )
		EVT_TEXT_ENTER( sceneNodeXPos, SceneNodeProperties::OnEditXPos )
		EVT_TEXT_ENTER( sceneNodeYPos, SceneNodeProperties::OnEditYPos )
		EVT_TEXT_ENTER( sceneNodeZPos, SceneNodeProperties::OnEditZPos )
		EVT_TEXT_ENTER( sceneNodeXScale, SceneNodeProperties::OnEditXScale )
		EVT_TEXT_ENTER( sceneNodeYScale, SceneNodeProperties::OnEditYScale )
		EVT_TEXT_ENTER( sceneNodeZScale, SceneNodeProperties::OnEditZScale )
		EVT_TEXT_ENTER( sceneNodeRoll, SceneNodeProperties::OnEditRoll )
		EVT_TEXT_ENTER( sceneNodePitch, SceneNodeProperties::OnEditPitch )
		EVT_TEXT_ENTER( sceneNodeYaw, SceneNodeProperties::OnEditYaw )
	END_EVENT_TABLE()

	SceneNodeProperties::SceneNodeProperties( TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * p_sceneNode, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_sceneNode( p_sceneNode )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_nodeName = BuildTextCtrl( this, l_position, sceneNodeName, _( "Name" ), m_sceneNode->GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_sceneNode->GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray, m_sceneNode->GetObjectName() );
		l_nodesArray.Sort();
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), sceneNodeParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
		Vector3 l_nodePosition = m_sceneNode->GetPosition();
		m_xPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeXPos, _( "X Position" ), l_nodePosition.x );
		m_yPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeYPos, _( "Y Position" ), l_nodePosition.y );
		m_zPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeZPos, _( "Z Position" ), l_nodePosition.z );
		Vector3 l_scale = m_sceneNode->GetScale();
		m_xScale = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeXScale, _( "X Scale" ), l_scale.x );
		m_yScale = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeYScale, _( "Y Scale" ), l_scale.y );
		m_zScale = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeZScale, _( "Z Scale" ), l_scale.z );
		Quaternion l_orientation = m_sceneNode->GetOrientation();
		m_roll = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeRoll, _( "Roll" ), l_orientation.getRoll().valueDegrees() );
		m_pitch = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodePitch, _( "Pitch" ), l_orientation.getPitch().valueDegrees() );
		m_yaw = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), sceneNodeYaw, _( "Yaw" ), l_orientation.getYaw().valueDegrees() );
	}

	SceneNodeProperties::~SceneNodeProperties()
	{
	}

	void SceneNodeProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
		m_nodeName->SetValue( m_sceneNode->GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_sceneNode->GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray, m_sceneNode->GetObjectName() );
		l_nodesArray.Sort();
		RemoveChild( m_parentNode );
		delete m_parentNode;
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), sceneNodeParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
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

	void SceneNodeProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_sceneNode->SetObjectName( m_nodeName->GetValue() );
		}
	}

	void SceneNodeProperties::OnSelectParent( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_node = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() );
			m_sceneNode->AttachTo( l_node );
		}
	}

	void SceneNodeProperties::OnEditXPos( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditYPos( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditZPos( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditXScale( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditYScale( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditZScale( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditRoll( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditPitch( wxCommandEvent & p_event )
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

	void SceneNodeProperties::OnEditYaw( wxCommandEvent & p_event )
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
}
END_TROLL_GUI_PROPERTIES_NAMESPACE