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

#include "CameraProperties.h"

#include "Project/3D/Camera.h"
#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
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

	BEGIN_EVENT_TABLE( CameraProperties, wxPanel )
		EVT_TEXT_ENTER( cameraName, CameraProperties::OnEditName )
		EVT_COMBOBOX( cameraParent, CameraProperties::OnSelectParent )
		EVT_TEXT_ENTER( cameraXPos, CameraProperties::OnEditXPos )
		EVT_TEXT_ENTER( cameraYPos, CameraProperties::OnEditYPos )
		EVT_TEXT_ENTER( cameraZPos, CameraProperties::OnEditZPos )
		EVT_TEXT_ENTER( cameraXDir, CameraProperties::OnEditXDir )
		EVT_TEXT_ENTER( cameraYDir, CameraProperties::OnEditYDir )
		EVT_TEXT_ENTER( cameraZDir, CameraProperties::OnEditZDir )
		EVT_TEXT_ENTER( cameraRoll, CameraProperties::OnEditRoll )
		EVT_TEXT_ENTER( cameraPitch, CameraProperties::OnEditPitch )
		EVT_TEXT_ENTER( cameraYaw, CameraProperties::OnEditYaw )
		EVT_COMBOBOX( cameraPolygonMode, CameraProperties::OnSelectPolygonMode )
		EVT_TEXT_ENTER( cameraYFOV, CameraProperties::OnEditYFOV )
	END_EVENT_TABLE()

	CameraProperties::CameraProperties( TROLL_PROJECT_3D_NAMESPACE::TrollCamera & p_camera, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_camera( p_camera )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_cameraName = BuildTextCtrl( this, l_position, cameraName, _( "Name" ), m_camera.GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_camera.GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
		l_nodesArray.Sort();
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), cameraParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );
		Vector3 l_camPosition = m_camera.GetPosition();
		m_xPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraXPos, _( "X Position" ), l_camPosition.x );
		m_yPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYPos, _( "Y Position" ), l_camPosition.y );
		m_zPosition = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraZPos, _( "Z Position" ), l_camPosition.z );
		Vector3 l_direction = m_camera.GetDirection();
		m_xDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraXDir, _( "X Position" ), l_direction.x );
		m_yDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYDir, _( "Y Position" ), l_direction.y );
		m_zDirection = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraZDir, _( "Z Position" ), l_direction.z );
		Quaternion l_orientation = m_camera.GetOrientation();
		m_roll = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraRoll, _( "Roll" ), l_orientation.getRoll().valueDegrees() );
		m_pitch = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraPitch, _( "Pitch" ), l_orientation.getPitch().valueDegrees() );
		m_yaw = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYaw, _( "Yaw" ), l_orientation.getYaw().valueDegrees() );
		TROLL_PROJECT_3D_NAMESPACE::TrollCameraPolygonMode l_polygonMode = m_camera.GetPolygonMode();
		wxArrayString l_choices;
		l_choices.Add( _( "Point" ) );
		l_choices.Add( _( "Wireframe" ) );
		l_choices.Add( _( "Solid" ) );
		m_polygonMode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), cameraPolygonMode, _( "Polygon Mode" ), l_choices, l_choices[static_cast <unsigned int>( l_polygonMode - 1 )] );
		m_yFOV = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), cameraYFOV, _( "Field of view Y" ), m_camera.GetYFOV() );
	}

	CameraProperties::~CameraProperties()
	{
	}

	void CameraProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_cameraName->SetValue( m_camera.GetObjectName() );
		TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = m_camera.GetParent();
		wxArrayString l_nodesArray;
		GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_nodesArray );
		l_nodesArray.Sort();
		wxPoint l_position = wxPoint( GetClientSize().x / 2, 0 );
		RemoveChild( m_parentNode );
		delete m_parentNode;
		m_parentNode = BuildComboBox( this, l_position += wxPoint( 0, 20 ), cameraParent, _( "Parent Node" ), l_nodesArray, ( l_parent ? l_parent->GetObjectName() : wxT( "root node" ) ) );

		Vector3 l_camPosition = m_camera.GetPosition();
		m_xPosition->SetValue( wxString() << l_camPosition.x );
		m_yPosition->SetValue( wxString() << l_camPosition.y );
		m_zPosition->SetValue( wxString() << l_camPosition.z );

		Vector3 l_direction = m_camera.GetDirection();
		m_xDirection->SetValue( wxString() << l_direction.x );
		m_yDirection->SetValue( wxString() << l_direction.y );
		m_zDirection->SetValue( wxString() << l_direction.z );

		Quaternion l_orientation = m_camera.GetOrientation();
		m_roll->SetValue( wxString() << l_orientation.getRoll().valueDegrees() );
		m_pitch->SetValue( wxString() << l_orientation.getPitch().valueDegrees() );
		m_yaw->SetValue( wxString() << l_orientation.getYaw().valueDegrees() );

		TROLL_PROJECT_3D_NAMESPACE::TrollCameraPolygonMode l_polygonMode = m_camera.GetPolygonMode();
		wxArrayString l_choices;
		l_choices.Add( _( "Point" ) );
		l_choices.Add( _( "Wireframe" ) );
		l_choices.Add( _( "Solid" ) );
		m_polygonMode->SetValue( l_choices[static_cast< unsigned int >( l_polygonMode - 1 )] );
		m_yFOV->SetValue( wxString() << m_camera.GetYFOV() );
		m_updatingProperties = false;
	}

	void CameraProperties::OnEditRealValue( wxCommandEvent & p_event, std::function< void( float ) > p_function )
	{
		if ( !m_updatingProperties )
		{
			float l_res;

			if ( GetReal( p_event.GetString(), l_res ) )
			{
				p_function( l_res );
			}
		}
	}

	void CameraProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( !m_updatingProperties )
		{
			m_camera.SetObjectName( m_cameraName->GetValue() );
		}
	}

	void CameraProperties::OnSelectParent( wxCommandEvent & p_event )
	{
		if ( !m_updatingProperties )
		{
			m_camera.AttachTo( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( m_parentNode->GetValue() ) );
		}
	}

	void CameraProperties::OnEditXPos( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Vector3 l_position = m_camera.GetPosition();
			l_position.x = p_value;
			m_camera.SetPosition( l_position );
		} );
	}

	void CameraProperties::OnEditYPos( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Vector3 l_position = m_camera.GetPosition();
			l_position.y = p_value;
			m_camera.SetPosition( l_position );
		} );
	}

	void CameraProperties::OnEditZPos( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Vector3 l_position = m_camera.GetPosition();
			l_position.z = p_value;
			m_camera.SetPosition( l_position );
		} );
	}

	void CameraProperties::OnEditXDir( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Vector3 l_direction = m_camera.GetDirection();
			l_direction.x = p_value;
			m_camera.SetDirection( l_direction );
		} );
	}

	void CameraProperties::OnEditYDir( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Vector3 l_direction = m_camera.GetDirection();
			l_direction.y = p_value;
			m_camera.SetDirection( l_direction );
		} );
	}

	void CameraProperties::OnEditZDir( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Vector3 l_direction = m_camera.GetDirection();
			l_direction.z = p_value;
			m_camera.SetDirection( l_direction );
		} );
	}

	void CameraProperties::OnEditRoll( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Quaternion l_orientation = m_camera.GetOrientation();
			m_camera.GetOgreCamera()->roll( Degree( p_value - l_orientation.getRoll().valueDegrees() ) );
			m_camera.SetOrientation( m_camera.GetOgreCamera()->getOrientation() );
		} );
	}

	void CameraProperties::OnEditPitch( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Quaternion l_orientation = m_camera.GetOrientation();
			m_camera.GetOgreCamera()->pitch( Degree( p_value - l_orientation.getPitch().valueDegrees() ) );
			m_camera.SetOrientation( m_camera.GetOgreCamera()->getOrientation() );
		} );
	}

	void CameraProperties::OnEditYaw( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			Ogre::Quaternion l_orientation = m_camera.GetOrientation();
			m_camera.GetOgreCamera()->yaw( Degree( p_value - l_orientation.getYaw().valueDegrees() ) );
			m_camera.SetOrientation( m_camera.GetOgreCamera()->getOrientation() );
		} );
	}

	void CameraProperties::OnSelectPolygonMode( wxCommandEvent & p_event )
	{
		if ( !m_updatingProperties )
		{
			m_camera.SetPolygonMode( static_cast< TROLL_PROJECT_3D_NAMESPACE::TrollCameraPolygonMode >( p_event.GetInt() + 1 ) );
		}
	}

	void CameraProperties::OnEditYFOV( wxCommandEvent & p_event )
	{
		OnEditRealValue( p_event, [this]( float p_value )
		{
			m_camera.SetYFOV( p_value );
		} );
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
