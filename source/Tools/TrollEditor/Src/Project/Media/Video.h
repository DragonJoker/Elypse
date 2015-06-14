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
#ifndef _TrollVideo_H_
#define _TrollVideo_H_

#include "MediaObject.h"

#include <Module_Media.h>

BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE
{
	class TrollVideo
		: public MediaObject
	{
	public:
		TrollVideo( const wxString & p_name, const wxString & p_fileName );
		~TrollVideo();

		TrollVideoObject * AddObject( Object * p_object );

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void SetMuseVideo( VideoObject * p_museVideo );
		void SetUrl( const wxString & p_url );
		void SetLooped( bool p_looped );
		void SetEnabled( bool p_enabled );

		void Write( wxTextOutputStream * p_stream );

	public:
		inline const wxString &	GetUrl()const
		{
			return m_filePath;
		}
		inline bool IsLooped()const
		{
			return m_looped;
		}
		inline bool IsEnabled()const
		{
			return m_enabled;
		}

	private:
		wxString m_filePath;
		TrollVideoObjectMap m_objects;
		bool m_looped;
		bool m_enabled;
		VideoObject * m_museVideo;
	};
}
END_TROLL_PROJECT_MEDIA_NAMESPACE

#endif

