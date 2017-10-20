/*
See LICENSE file in root folder
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
