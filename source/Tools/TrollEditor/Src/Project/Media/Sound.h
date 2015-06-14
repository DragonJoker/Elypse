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
#ifndef _TrollSound_H_
#define _TrollSound_H_

#include "MediaObject.h"

#include "Project/3D/Module_3D.h"

#include <Module_Media.h>

BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE
{
	class TrollSound
		: public MediaObject
	{
	public:
		TrollSound( const wxString & p_name, const wxString & p_fileName );
		~TrollSound();

		TrollSoundObject * AddObject( TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * p_node );

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void SetMuseSound( Elypse::Media::SoundObject * p_sound );
		void SetUrl( const wxString & p_filePath );
		void SetSoundType( TrollSoundType p_type );
		void SetLocal();
		void SetGlobalAttenuation( float p_att );
		void SetGlobalLooped( bool p_looped );
		void SetGlobalTimer( float p_time );
		void SetGlobalFadeIn( float p_time );
		void SetGlobalFadeOut( float p_time );

		void Write( wxTextOutputStream * p_stream );

	public:
		inline wxString GetUrl()const
		{
			return m_filePath;
		}
		inline TrollSoundType GetSoundType()const
		{
			return m_soundType;
		}
		inline bool IsGlobal()const
		{
			return m_isGlobal;
		}
		inline SoundInstance * GetGlobal()const
		{
			return m_globalInstance;
		}

	private:
		wxString m_filePath;
		TrollSoundType m_soundType;
		bool m_isGlobal;
		TrollSoundObjectMap m_objects;
		Elypse::Media::SoundObject * m_museSound;

		//if global
		SoundInstance * m_globalInstance;
	};
}
END_TROLL_PROJECT_MEDIA_NAMESPACE

#endif

