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

#include "VideoObject.h"

#include "Video.h"
#include "Project/Object.h"
#include "GUI/ObjectProperties/VideoObjectProperties.h"

using namespace Elypse::Media;

BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE
{
	TrollVideoObject::TrollVideoObject( const wxString & p_name, const wxString & p_fileName )
		: MediaObject( p_name, p_fileName, ttVideoObject )
		, m_video( NULL )
		, m_object( NULL )
		, m_attenuation( 0.0 )
	{
	}

	TrollVideoObject::~TrollVideoObject()
	{
	}

	int TrollVideoObject::BuildPanel( wxWindow * p_parent, int p_width )
	{
		int l_height = 20;
		wxSize l_returnSize( p_width, l_height );

		if ( m_propertiesPanel )
		{
			m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
			delete m_propertiesPanel;
		}

		m_propertiesPanel = new TROLL_GUI_PROPERTIES_NAMESPACE::VideoObjectProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
		return l_height;
	}

	//void TrollVideoObject::SetMuseMediaEntity( MediaEntity * p_me)
	//{
	//	m_museMediaEntity = p_me;
	//}

	void TrollVideoObject::SetAttenuation( float p_att )
	{
		m_attenuation = p_att;
	}

	void TrollVideoObject::SetMuseVideoObject( VideoObject * p_videoObject )
	{
		m_videoOverlay = NULL;
		m_videoObject = p_videoObject;
	}

	void TrollVideoObject::SetMuseVideoOverlay( VideoOverlay * p_videoOverlay )
	{
		m_videoOverlay = p_videoOverlay;
		m_videoObject = NULL;
	}

	void TrollVideoObject::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "\tuse_video " ) + m_video->GetObjectName() + wxT( "\n\t{\n" ) );
		wxString l_tmp;
		l_tmp.Printf( wxT( "%.2f" ), m_attenuation );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\t\tvolume " ) + l_tmp + wxT( "\n\t}\n" ) );
		l_tmp.clear();
	}
}
END_TROLL_PROJECT_MEDIA_NAMESPACE
