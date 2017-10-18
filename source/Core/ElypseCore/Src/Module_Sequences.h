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
#ifndef ___MODULE_SEQUENCE_H___
#define ___MODULE_SEQUENCE_H___

#include "Elypse.h"

namespace Elypse
{
	namespace Sequences
	{
		class SequenceManager;
		class Sequence;

		enum class EventTargetType
		{
			SceneNode,
			ScriptNode,
			OverlayGroup,
			Overlay,
			OverlayElement,
			Sequence,
		};

		class EventTargetBase;
		template< EventTargetType Type >
		class EventTarget;

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

using namespace Elypse::Sequences;

#endif
