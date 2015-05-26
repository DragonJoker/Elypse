#ifndef ___MODULE_SEQUENCE_H___
#define ___MODULE_SEQUENCE_H___

#include "EMuse.h"

namespace EMuse
{
	namespace Sequences
	{
		class SequenceManager;
		class Sequence;

		class BasePonctualEvent;
		class BaseContinuousEvent;

		class BaseContinuousV3Event;
		class BaseContinuousQEvent;
		class BaseContinuousREvent;

		class BasePonctualEventFactory;
		class BaseContinuousEventFactory;

		struct BaseVector3Frame;
		struct BaseQuaternionFrame;
		struct BaseRealFrame;

		enum SequenceInstanceStatus
		{
			STOPPED,
			PLAYING,
			PAUSED
		};

		enum SequenceTrackType
		{
			VECTOR3,
			QUATERNION,
			REAL
		};

		typedef std::multimap <Real, BasePonctualEvent *> PonctualEventMap;
		typedef std::multimap <Real, BaseContinuousEvent *> ContinuousEventMap;

		typedef std::set <BaseContinuousEvent *> ContinuousEventSet;

		typedef std::map <Real, BaseVector3Frame> BaseVector3FrameMap;
		typedef std::map <Real, BaseQuaternionFrame> BaseQuaternionFrameMap;
		typedef std::map <Real, BaseRealFrame> BaseRealFrameMap;

		typedef std::map <String, BasePonctualEventFactory *> BasePonctualEventFactoryMap;
		typedef std::map <String, BaseContinuousEventFactory *> BaseContinuousEventFactoryMap;

		typedef Vector3( Vector3_Interpolator )( const BaseVector3FrameMap & p_frames, Real p_time );
		typedef Quaternion( Quaternion_Interpolator )( const BaseQuaternionFrameMap & p_frames, Real p_time );
		typedef Real( Real_Interpolator )( const BaseRealFrameMap & p_frames, Real p_time );

		typedef std::map <String, Vector3_Interpolator *> V3InterpolatorMap;
		typedef std::map <String, Quaternion_Interpolator *> QInterpolatorMap;
		typedef std::map <String, Real_Interpolator *> RInterpolatorMap;
	}
}

using namespace EMuse::Sequences;

#endif
