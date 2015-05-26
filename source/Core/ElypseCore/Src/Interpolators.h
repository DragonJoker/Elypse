#ifndef ___SEQUENCE_INTERPOLATORS_H___
#define ___SEQUENCE_INTERPOLATORS_H___

#include "Module_Sequences.h"

namespace EMuse
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
