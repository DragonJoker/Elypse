#include "CameraProperties.h"

#include "Project/3D/Camera.h"

#include "Project/3D/SceneNode.h"
#include "Project/Project.h"
#include "Project/Scene.h"

#include "GUI/ProjectFrame.h"
#include "GUI/Properties/Common/VectorProperties.h"

#include <wx/propgrid/advprops.h>
#include <numeric>

namespace Troll
{
	using namespace ProjectComponents;
	using namespace ProjectComponents::Objects3D;

	namespace GUI
	{
		namespace Properties
		{
			CameraProperties::CameraProperties( TrollCamera & p_camera )
				: ObjectProperty{ DataType::Camera }
				, m_camera{ p_camera }
			{
			}

			CameraProperties::~CameraProperties()
			{
			}

			void CameraProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_CAMERA = _( "Camera: " );
				wxString const PROPERTY_CAMERA_NAME = _( "Name" );
				wxString const PROPERTY_CAMERA_PARENT = _( "Parent Node" );
				wxString const PROPERTY_CAMERA_POSITION = _( "Position" );
				wxString const PROPERTY_CAMERA_DIRECTION = _( "Direction" );
				wxString const PROPERTY_CAMERA_ORIENTATION = _( "Orientation" );
				wxString const PROPERTY_CAMERA_POLYGON_MODE = _( "Polygon Mode" );
				wxString const PROPERTY_CAMERA_YFOV = _( "FovY" );
				std::array< wxString, 3 > const PROPERTY_CAMERA_POLYGON_MODE_TEXTS
				{
					_( "Point" ),
					_( "Wireframe" ),
					_( "Solid" )
				};
				std::array< int, 3 > const PROPERTY_CAMERA_POLYGON_MODE_VALUES
				{
					tcPoint,
					tcWireFrame,
					tcSolid
				};

				wxString l_mode{ PROPERTY_CAMERA_POLYGON_MODE_TEXTS[m_camera.GetPolygonMode() - 1] };
				m_nodesArray.clear();
				wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( m_nodesArray );
				m_nodesArray.Sort();
				wxArrayInt l_values( m_nodesArray.size() );
				std::iota( l_values.begin(), l_values.end(), 0 );
				wxPGChoices l_nodes{ m_nodesArray, l_values };
				TrollSceneNode * l_parent = m_camera.GetParent();

				DoAddCategory( PROPERTY_CATEGORY_CAMERA + wxString( m_camera.GetObjectName() ) );
				DoAddProperty( PROPERTY_CAMERA_NAME, m_camera.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_camera.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_camera.GetObjectName() );
				} );
				DoAddProperty( PROPERTY_CAMERA_PARENT, l_nodes, l_parent ? l_parent->GetObjectName() : wxT( "root node" ), [this]( wxPGProperty * p_property )
				{
					m_camera.AttachTo( wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetSceneNode( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_camera.GetParent()->GetObjectName() );
				} );
				DoAddProperty< Vector3Names::Point >( PROPERTY_CAMERA_POSITION, m_camera.GetPosition(), [this]( wxPGProperty * p_property )
				{
					m_camera.SetPosition( Vector3RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_camera.GetPosition() ) );
				} );
				DoAddProperty< Vector3Names::Point >( PROPERTY_CAMERA_DIRECTION, m_camera.GetDirection(), [this]( wxPGProperty * p_property )
				{
					m_camera.SetDirection( Vector3RefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_camera.GetDirection() ) );
				} );
				DoAddProperty( PROPERTY_CAMERA_ORIENTATION, m_camera.GetOrientation(), [this]( wxPGProperty * p_property )
				{
					m_camera.SetOrientation( QuaternionRefFromVariant( p_property->GetValue() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( WXVARIANT( m_camera.GetOrientation() ) );
				} );
				DoAddProperty( PROPERTY_CAMERA_POLYGON_MODE, l_mode, [this]( wxPGProperty * p_property )
				{
					m_camera.SetPolygonMode( TrollCameraPolygonMode( GetValue< int >( p_property->GetValue() ) + 1 ) );
				},
				[this, PROPERTY_CAMERA_POLYGON_MODE_TEXTS]( wxPGProperty * p_property )
				{
					p_property->SetValue( PROPERTY_CAMERA_POLYGON_MODE_TEXTS[m_camera.GetPolygonMode() - 1] );
				} );
				DoAddProperty( PROPERTY_CAMERA_YFOV, m_camera.GetYFOV(), [this]( wxPGProperty * p_property )
				{
					m_camera.SetYFOV( Real( double( p_property->GetValue() ) ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_camera.GetYFOV() );
				} );
			}
		}
	}
}
