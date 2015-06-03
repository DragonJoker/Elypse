
#ifndef ___CameraProperties___
#define ___CameraProperties___

#include "Project/3D/Module_3D.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class CameraProperties
			: public ObjectProperties
		{
		public:
			CameraProperties( Objects3D::TrollCamera & p_camera, wxWindow * parent,
							  const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							  long style = wxBORDER_NONE );
			~CameraProperties();

			virtual void UpdateProperties();

		private:
			void OnEditRealValue( wxCommandEvent & p_event, std::function< void( float ) > p_function );
			void OnEditName( wxCommandEvent & p_event );
			void OnSelectParent( wxCommandEvent & p_event );
			void OnEditXPos( wxCommandEvent & p_event );
			void OnEditYPos( wxCommandEvent & p_event );
			void OnEditZPos( wxCommandEvent & p_event );
			void OnEditXDir( wxCommandEvent & p_event );
			void OnEditYDir( wxCommandEvent & p_event );
			void OnEditZDir( wxCommandEvent & p_event );
			void OnEditRoll( wxCommandEvent & p_event );
			void OnEditPitch( wxCommandEvent & p_event );
			void OnEditYaw( wxCommandEvent & p_event );
			void OnSelectPolygonMode( wxCommandEvent & p_event );
			void OnEditYFOV( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()

		private:
			Objects3D::TrollCamera & m_camera;
			wxTextCtrl * m_cameraName;
			wxComboBox * m_parentNode;
			wxTextCtrl * m_xPosition;
			wxTextCtrl * m_yPosition;
			wxTextCtrl * m_zPosition;
			wxTextCtrl * m_xDirection;
			wxTextCtrl * m_yDirection;
			wxTextCtrl * m_zDirection;
			wxTextCtrl * m_roll;
			wxTextCtrl * m_pitch;
			wxTextCtrl * m_yaw;
			wxComboBox * m_polygonMode;
			wxTextCtrl * m_yFOV;
		};
	}
}

#endif

