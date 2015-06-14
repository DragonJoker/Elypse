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
#ifndef ___VIDEOMANAGER_H___
#define ___VIDEOMANAGER_H___

#include <Singleton.h>

#include "Module_Media.h"
#include "Module_Main.h"

#include <PreciseTimer.h>

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoManager : public General::Theory::Singleton <VideoManager>
		{
			friend class VideoImplementation;

		private:
			VideoObjectMap m_objectList;
			VideoFactoryMap m_factories;
			int m_volume;
			bool m_muted;

			PreciseTimer m_timer;

			VideoFactory * m_currentFactory;

		public:
			VideoManager();
			~VideoManager();

		public:
			VideoObject * CreateVideoObject( const String & p_name, ElypsePlugin * p_plugin );
			bool IsVideo( const String & p_entityName );
			void Cleanup();
			void StartAll();
			void StopAll();
			void PauseAll();
			void SetVolume( int p_volume );
			void Mute( bool p_muted );
			//void UpdateVolume( const Vector3 & p_position, Radian p_orientation);
			//void ReleaseAllZone( const String & p_zoneName);
			void DeleteVideoObject( const String & p_name );

			void RegisterFactory( VideoFactory * p_factory );
			void SetCurrentFactory( const String & p_name );

		public:
			void Update();

		public:
			inline bool						IsMuted()const
			{
				return m_muted;
			}
			inline Real						GetVolume()const
			{
				return Real( m_volume );
			}
			inline VideoObject 	*		GetVideoObject( const String & p_name )const
			{
				return General::Utils::map::findOrNull( m_objectList, p_name );
			}
			inline VideoFactory 	*		GetCurrentFactory()const
			{
				return m_currentFactory;
			}
			inline const VideoObjectMap &	GetVideoMap()const
			{
				return m_objectList;
			}
		};
	}
}

#endif
