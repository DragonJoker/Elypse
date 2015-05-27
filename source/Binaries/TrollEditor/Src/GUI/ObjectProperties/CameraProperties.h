
#ifndef ___CameraProperties___
#define ___CameraProperties___

#include "../../Project/3D/Module_3D.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class CameraProperties : public ObjectProperties
		{
		public:
			Objects3D::TrollCamera * m_camera;
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

			CameraProperties( Objects3D::TrollCamera * p_camera, wxWindow * parent,
							  const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							  long style = wxBORDER_NONE );
			~CameraProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );
			void _onSelectParent( wxCommandEvent & p_event );
			void _onEditXPos( wxCommandEvent & p_event );
			void _onEditYPos( wxCommandEvent & p_event );
			void _onEditZPos( wxCommandEvent & p_event );
			void _onEditXDir( wxCommandEvent & p_event );
			void _onEditYDir( wxCommandEvent & p_event );
			void _onEditZDir( wxCommandEvent & p_event );
			void _onEditRoll( wxCommandEvent & p_event );
			void _onEditPitch( wxCommandEvent & p_event );
			void _onEditYaw( wxCommandEvent & p_event );
			void _onSelectPolygonMode( wxCommandEvent & p_event );
			void _onEditYFOV( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

