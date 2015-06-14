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

#include "Video.h"

#include "VideoObject.h"
#include "GUI/ObjectProperties/VideoProperties.h"

BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE
{
	TrollVideo::TrollVideo( const wxString & p_name, const wxString & p_fileName )
		: MediaObject( p_name, p_fileName, ttVideo )
		, m_filePath( wxEmptyString )
		, m_looped( false )
		, m_museVideo( NULL )
	{
	}

	TrollVideo::~TrollVideo()
	{
		General::Utils::map::deleteAll( m_objects );
	}

	TrollVideoObject * TrollVideo::AddObject( Object * p_object )
	{
		if ( ! p_object )
		{
			return NULL;
		}

		wxString l_name = p_object->GetObjectName();
		TrollVideoObjectMap::iterator l_it = m_objects.find( l_name );

		if ( l_it != m_objects.end() )
		{
			return l_it->second;
		}

		TrollVideoObject * l_result = new TrollVideoObject( l_name, m_fileName );
		l_result->SetVideo( this );
		m_objects.insert( std::make_pair( l_name, l_result ) );
		return l_result;
	}

	int TrollVideo::BuildPanel( wxWindow * p_parent, int p_width )
	{
		int l_height = 80;
		wxSize l_returnSize( p_width, l_height );

		if ( m_propertiesPanel )
		{
			m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
			delete m_propertiesPanel;
		}

		m_propertiesPanel = new TROLL_GUI_PROPERTIES_NAMESPACE::VideoProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
		return l_height;
	}

	void TrollVideo::SetMuseVideo( VideoObject * p_museVideo )
	{
		m_museVideo = p_museVideo;
		m_filePath = make_wxString( m_museVideo->GetMediaUrl() );
		m_looped = m_museVideo->IsLooped();
		m_enabled = m_museVideo->IsAutoInitialised();
	}

	void TrollVideo::SetUrl( const wxString & p_url )
	{
		m_filePath = p_url;
		m_museVideo->SetUrl( make_string( p_url ) );
	}

	void TrollVideo::SetLooped( bool p_looped )
	{
		m_looped = p_looped;
		m_museVideo->SetLooped( p_looped );
	}

	void TrollVideo::SetEnabled( bool p_enabled )
	{
		m_enabled = p_enabled;
		m_museVideo->SetAutoInitialise( p_enabled );
	}

	void TrollVideo::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "video " ) + m_name + wxT( "\n{\n" ) );
		p_stream->WriteString( wxT( "\turl " ) + m_filePath + wxT( "\n" ) );
		p_stream->WriteString( wxT( "\tlooped " ) + ( m_looped ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n" ) );
		p_stream->WriteString( wxT( "\tenabled " ) + ( m_enabled ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n}\n\n" ) );
	}
}
END_TROLL_PROJECT_MEDIA_NAMESPACE

