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
#ifndef ___SEQUENCE_EVENT_H___
#define ___SEQUENCE_EVENT_H___

#include "Module_Sequences.h"

#include "Interpolators.h"
#include "Module_Script.h"

#include "BaseEvent.h"

namespace Elypse
{
	namespace Sequences
	{
		class SceneNode_SetPosition : public BasePonctualEvent
		{
			Vector3 m_position;
			Vector3 m_previousPosition;
		public:
			SceneNode_SetPosition( const Vector3 & p_position )
				: m_position( p_position )
			{}
			virtual ~SceneNode_SetPosition() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class SceneNode_SetScale : public BasePonctualEvent
		{
			Vector3 m_scale;
			Vector3 m_previousScale;
		public:
			SceneNode_SetScale( const Vector3 & p_scale )
				: m_scale( p_scale )
			{}
			virtual ~SceneNode_SetScale() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class Overlay_Hide : public BasePonctualEvent
		{
		public:
			Overlay_Hide() {}
			virtual ~Overlay_Hide() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class Overlay_Show : public BasePonctualEvent
		{
		public:
			Overlay_Show() {}
			virtual ~Overlay_Show() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class ScriptNode_Execute : public BasePonctualEvent
		{
		public:
			ScriptNode_Execute() {}
			virtual ~ScriptNode_Execute() {}
			virtual void Apply();
			virtual void Rollback() {}
		};

		class Sequence_Start : public BasePonctualEvent
		{
		public:
			Sequence_Start() {}
			virtual ~Sequence_Start() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class Sequence_Pause : public BasePonctualEvent
		{
		public:
			Sequence_Pause() {}
			virtual ~Sequence_Pause() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class Sequence_Unpause : public BasePonctualEvent
		{
		public:
			Sequence_Unpause() {}
			virtual ~Sequence_Unpause() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class Sequence_Stop : public BasePonctualEvent
		{
		public:
			Sequence_Stop() {}
			virtual ~Sequence_Stop() {}
			virtual void Apply();
			virtual void Rollback();
		};

		class SceneNode_Translate : public BaseContinuousV3Event
		{
		public:
			SceneNode_Translate() {}
			virtual ~SceneNode_Translate() {}
			virtual void Apply( Real p_time );
		};

		class SceneNode_Scale : public BaseContinuousV3Event
		{
		public:
			SceneNode_Scale() {}
			virtual ~SceneNode_Scale() {}
			virtual void Apply( Real p_time );
		};

		class SceneNode_Rotate : public BaseContinuousQEvent
		{
		public:
			SceneNode_Rotate() {}
			virtual ~SceneNode_Rotate() {}
			virtual void Apply( Real p_time );
		};

		class Overlay_Translate : public BaseContinuousV3Event
		{
		public:
			Overlay_Translate() {}
			virtual ~Overlay_Translate() {}
			virtual void Apply( Real p_time );
		};

		class Overlay_Rotate : public BaseContinuousREvent
		{
		public:
			Overlay_Rotate() {}
			virtual ~Overlay_Rotate() {}
			virtual void Apply( Real p_time );
		};

		class Overlay_Resize : public BaseContinuousV3Event
		{
		public:
			Overlay_Resize() {}
			virtual ~Overlay_Resize() {}
			virtual void Apply( Real p_time );
		};

		class Overlay_TranslateRelative : public BaseContinuousV3Event
		{
		public:
			Overlay_TranslateRelative() {}
			virtual ~Overlay_TranslateRelative() {}
			virtual void Apply( Real p_time );
		};

		class Overlay_ResizeRelative : public BaseContinuousV3Event
		{
		public:
			Overlay_ResizeRelative() {}
			virtual ~Overlay_ResizeRelative() {}
			virtual void Apply( Real p_time );
		};
	}
}

#endif
