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

#include "VideoObjectProperties.h"

#include "Project/Media/VideoObject.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum VideoObjectPropsIDs
	{
		videoAttenuation,
	};

	BEGIN_EVENT_TABLE( VideoObjectProperties, wxPanel )
		EVT_TEXT_ENTER( videoAttenuation, VideoObjectProperties::OnEditAttenuation )
	END_EVENT_TABLE()

	VideoObjectProperties::VideoObjectProperties( TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * p_videoObject, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_videoObject( p_videoObject )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_attenuation = BuildTextCtrl( this, l_position, videoAttenuation, _( "Attenuation" ), m_videoObject->GetAttenuation() );
	}

	VideoObjectProperties::~VideoObjectProperties()
	{
	}

	void VideoObjectProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_updatingProperties = false;
	}

	void VideoObjectProperties::OnEditAttenuation( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			wxString l_value = m_attenuation->GetValue();
			float l_res;

			if ( GetReal( l_value, 0.0f, 1.0f, l_res ) )
			{
				m_videoObject->SetAttenuation( l_res );
			}
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
