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
#ifndef ___VIDEOOVERLAY_H___
#define ___VIDEOOVERLAY_H___

#include "Module_Media.h"

#include "ZoneObject.h"
#include "EMOverlay.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoOverlay : public ZoneObject, owned_by <VideoObject>
		{
		private:
			EMOverlay * m_EMOverlay;
			String m_oldMaterialName;
			bool m_delayLoad;

		public:
			VideoOverlay( VideoObject * p_owner, EMOverlay * p_EMOverlay )
				: owned_by <VideoObject>	( p_owner ),
					 m_EMOverlay( p_EMOverlay ),
					 m_delayLoad( false )
			{
				genlib_assert( m_EMOverlay != NULL );
				genlib_assert( m_owner != NULL );
			}
			~VideoOverlay()	{}
			void Initialise( const String & p_videoTextureName );
			void Reset();

		public:
			inline EMOverlay *	GetEMOverlay()const
			{
				return m_EMOverlay;
			}
			inline bool			IsLoadDelayed()const
			{
				return m_delayLoad;
			}

			inline void		DelayLoad()
			{
				m_delayLoad = true;
			}

		public:
			virtual void AddToZone( Zone * p_zone );
			virtual void RemoveFromZone( Zone * p_zone );
		};
	}
}

#endif
