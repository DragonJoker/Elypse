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
#ifndef ___VIDEO_IMPLEMENTATION_H___
#define ___VIDEO_IMPLEMENTATION_H___

#include "Module_Media.h"
#include <Url.h>

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoImplementation
			: public owned_by< VideoObject >
		{
		public:
			VideoImplementation( VideoObject & p_owner )
				: owned_by< VideoObject >( p_owner )
			{
			}
			virtual ~VideoImplementation()
			{
			}

		public:
			virtual bool Start() = 0;
			virtual bool Pause() = 0;
			virtual bool Stop() = 0;
			virtual void Reset() = 0;
			virtual void Initialise( Url const & p_mediaUrl ) = 0;
			virtual bool Update( Real p_time ) = 0;
			virtual void SetMaxVolume( int p_maxVolume ) = 0;	//  0 < p_maxVolume < 100
			virtual void SetVolumePercent( Real p_percent ) = 0;
			virtual void Mute( bool p_muted ) = 0;
		};
	}
}

#endif
