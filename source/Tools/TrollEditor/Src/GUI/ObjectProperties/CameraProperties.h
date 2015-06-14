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
#ifndef ___CameraProperties___
#define ___CameraProperties___

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class CameraProperties
		: public ObjectProperties
	{
	public:
		CameraProperties( TROLL_PROJECT_3D_NAMESPACE::TrollCamera & p_camera, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
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
		TROLL_PROJECT_3D_NAMESPACE::TrollCamera & m_camera;
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
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
