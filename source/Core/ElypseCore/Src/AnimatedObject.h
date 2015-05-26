/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___ANIMATED_OBJECT_H___
#define ___ANIMATED_OBJECT_H___

#include "Module_Animation.h"

namespace EMuse
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
