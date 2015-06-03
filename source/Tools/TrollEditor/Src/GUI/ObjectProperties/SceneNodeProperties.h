
#ifndef ___SceneProperties___
#define ___SceneProperties___

#include "Project/3D/Module_3D.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class SceneNodeProperties
			: public ObjectProperties
		{
		public:
			Objects3D::TrollSceneNode * m_sceneNode;
			wxTextCtrl * m_nodeName;
			wxComboBox * m_parentNode;
			wxTextCtrl * m_xPosition;
			wxTextCtrl * m_yPosition;
			wxTextCtrl * m_zPosition;
			wxTextCtrl * m_xScale;
			wxTextCtrl * m_yScale;
			wxTextCtrl * m_zScale;
			wxTextCtrl * m_roll;
			wxTextCtrl * m_pitch;
			wxTextCtrl * m_yaw;

			SceneNodeProperties( Objects3D::TrollSceneNode * p_node, wxWindow * parent,
								 const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
								 long style = wxBORDER_NONE );
			~SceneNodeProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );
			void _onSelectParent( wxCommandEvent & p_event );
			void _onEditXPos( wxCommandEvent & p_event );
			void _onEditYPos( wxCommandEvent & p_event );
			void _onEditZPos( wxCommandEvent & p_event );
			void _onEditXScale( wxCommandEvent & p_event );
			void _onEditYScale( wxCommandEvent & p_event );
			void _onEditZScale( wxCommandEvent & p_event );
			void _onEditRoll( wxCommandEvent & p_event );
			void _onEditPitch( wxCommandEvent & p_event );
			void _onEditYaw( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

