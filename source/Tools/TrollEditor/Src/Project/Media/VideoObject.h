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
#ifndef _TrollVideoObject_H_
#define _TrollVideoObject_H_

#include "Module_Media.h"
#include "MediaObject.h"

#include <Module_Media.h>

BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE
{
	class TrollVideoObject
		: public MediaObject
	{
	public:
		TrollVideoObject( const wxString & p_name, const wxString & p_fileName );
		~TrollVideoObject();

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void SetAttenuation( float p_att );
		void Write( wxTextOutputStream * p_stream );
		void SetMuseVideoObject( Elypse::Media::VideoObject * p_videoObject );
		void SetMuseVideoOverlay( Elypse::Media::VideoOverlay * p_videoOverlay );

		inline void SetVideo( TrollVideo * p_video )
		{
			m_video = p_video;
		}
		inline TrollVideo * GetVideo()const
		{
			return m_video;
		}
		inline float GetAttenuation()const
		{
			return m_attenuation;
		}

	private:
		TrollVideo * m_video;
		Object * m_object;
		float m_attenuation;
		Elypse::Media::VideoObject * m_videoObject;
		Elypse::Media::VideoOverlay * m_videoOverlay;
	};
}
END_TROLL_PROJECT_MEDIA_NAMESPACE

#endif
