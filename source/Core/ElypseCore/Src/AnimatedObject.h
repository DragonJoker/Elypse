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
#ifndef ___ANIMATED_OBJECT_H___
#define ___ANIMATED_OBJECT_H___

#include "Module_Animation.h"

namespace Elypse
{
	namespace Animation
	{
		class d_dll_export AnimatedObject : General::Theory::owned_by<AnimatedObjectGroup>, d_noncopyable
		{
		protected:
			Ogre::Entity * m_entity;
			AnimationMap m_animations;

		public:
			AnimatedObject( AnimatedObjectGroup * p_group, Ogre::Entity * p_ogreEntity );
			~AnimatedObject();

		protected:
			void _createAnimationMap();
			void _addAnimation( AnimationState * p_ogreAnimationState );

		public:
			void Update( Real p_timeStep );

			void StopAllAnimations();

			void SetPauseAllAnimations( bool p_pause );

			EMAnimation * GetAnimation( const String & p_animationName )const
			{
				return General::Utils::map::findOrNull( m_animations, p_animationName );
			}

			RealMap GetAllAnimationsTimes() const;

			unsigned int GetNumPlayingAnimations();
			const String & GetPlayingAnimationName( unsigned int p_index );

		public:
			inline Ogre::Entity * GetEntity()const
			{
				return m_entity;
			}

		};
	}
}

#endif
