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
#ifndef ___BASE_FRAME_H___
#define ___BASE_FRAME_H___

#include "Module_Sequences.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace Elypse
{
	namespace Sequences
	{
		struct BaseVector3Frame
		{
			Real time;
			Vector3 vector3;

			BaseVector3Frame( Real p_time, const Vector3 & p_vector3 )
				: time( p_time )
				, vector3( p_vector3 )
			{}
		};

		struct BaseQuaternionFrame
		{
			Real time;
			Quaternion quaternion;

			BaseQuaternionFrame( Real p_time, const Quaternion & p_quaternion )
				: time( p_time )
				, quaternion( p_quaternion )
			{}
		};

		struct BaseRealFrame
		{
			Real time;
			Real real;

			BaseRealFrame( Real p_time, Real p_real )
				: time( p_time )
				, real( p_real )
			{}
		};
	}
}

#endif
