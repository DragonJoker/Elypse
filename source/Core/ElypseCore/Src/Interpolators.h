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
#ifndef ___SEQUENCE_INTERPOLATORS_H___
#define ___SEQUENCE_INTERPOLATORS_H___

#include "Module_Sequences.h"

namespace Elypse
{
	namespace Sequences
	{
		namespace Interpolators
		{
			bool _getFramesV3( const BaseVector3FrameMap & p_frames, const BaseVector3Frame *& p_start, const BaseVector3Frame *& p_end, Real & p_time );
			bool _getFramesQ( const BaseQuaternionFrameMap & p_frames, const BaseQuaternionFrame *& p_start, const BaseQuaternionFrame *& p_end, Real & p_time );
			bool _getFramesR( const BaseRealFrameMap & p_frames, const BaseRealFrame *& p_start, const BaseRealFrame *& p_end, Real & p_time );

			Vector3 V3_Linear( const BaseVector3FrameMap & p_frames, Real p_time );
			Vector3 V3_Sinus( const BaseVector3FrameMap & p_frames, Real p_time );
			Vector3 V3_Squared( const BaseVector3FrameMap & p_frames, Real p_time );

			Vector3 V3_Bezier3( const BaseVector3FrameMap & p_frames, Real p_time );
			Vector3 V3_Bezier4( const BaseVector3FrameMap & p_frames, Real p_time );
			Vector3 V3_Bezier5( const BaseVector3FrameMap & p_frames, Real p_time );
			Vector3 V3_BSpline( const BaseVector3FrameMap & p_frames, Real p_time );

			Quaternion Q_Linear( const BaseQuaternionFrameMap & p_frames, Real p_time );

			Real R_Linear( const BaseRealFrameMap & p_frames, Real p_time );
		}
	}
}

#endif
