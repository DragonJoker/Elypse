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
#ifndef _TrollSoundObject_H_
#define _TrollSoundObject_H_

#include "MediaObject.h"

#include <Module_Media.h>

BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE
{
	class TrollSoundObject
		: public MediaObject
	{
	public:
		TrollSoundObject( const wxString & p_name, const wxString & p_fileName );
		~TrollSoundObject();

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void SetMuseInstance( Elypse::Media::SoundInstance * p_instance );
		void SetAttenuation( float p_att );
		void SetLooped( bool p_looped );
		void SetTimer( float p_time );
		void SetFadeIn( float p_time );
		void SetFadeOut( float p_time );

		void Write( wxTextOutputStream * p_stream );

		inline TrollSound * GetSound()const
		{
			return m_sound;
		}
		inline SoundInstance GetInstance()const
		{
			return m_instance;
		}
		inline void SetSound( TrollSound * p_sound )
		{
			m_sound = p_sound;
		}

	private:
		TrollSound * m_sound;
		Object * m_object;
		SoundInstance m_instance;
		Elypse::Media::SoundInstance * m_museInstance;
	};
}
END_TROLL_PROJECT_MEDIA_NAMESPACE

#endif
