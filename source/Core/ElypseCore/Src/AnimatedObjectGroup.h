/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___ANIMATED_OBJECT_GROUP_H___
#define ___ANIMATED_OBJECT_GROUP_H___

#include "Module_Animation.h"

namespace EMuse
{
	namespace Animation
	{
		class d_dll_export  AnimatedObjectGroup : public General::Theory::named,
			public General::Theory::owned_by<AnimationManager>,
			d_noncopyable
		{
		protected:
			AnimatedObjectMap m_objects;
			RealMap m_commonAnimationsTime;

		public:
			AnimatedObjectGroup( const String & p_name, AnimationManager * p_owner );
			~AnimatedObjectGroup();

		public:
			void Update( Real p_timeStep );
			AnimatedObject * AddEntity( Ogre::Entity * p_timeStep );

			void StartAnimation( const String & p_animationName );
			void StopAnimation( const String & p_animationName );
			void PauseAnimation( const String & p_animationName );
			void SetTimeAnimation( const String & p_animationName, Real p_timeIndex );
			void SetWeightAnimation( const String & p_animationName, Real p_weight );
			void SetAnimationTimeOut( const String & p_animationName, Real p_timeOut );
			void SetAnimationTimeIn( const String & p_animationName, Real p_timeIn );
			void SetAnimationLooped( const String & p_animationName, bool p_looped );
			void SetAnimationReversed( const String & p_animationName, bool p_reversed );

			void SetTimeScaleAnimation( const String & p_animationName, Real p_timescale );

			void ChainAnimation( const String & p_animName, const String & p_chainedAnimationName );

			void RemoveObject( const String & p_objectName );

			Real GetAnimationTime( const String & p_animationName )const;

			void StopAllAnimations();

			void ChainAllAnimations( bool p_looped );

			void Clear();

			void SetPauseAll( bool p_pause );

		public:
			inline size_t GetNumObjects()const
			{
				return m_objects.size();
			}
			inline AnimatedObject * GetObjectByName( const String & p_objectName )const
			{
				return General::Utils::map::findOrNull( m_objects, p_objectName );
			}
			inline bool HasAnimation( const String & p_name )
			{
				return General::Utils::map::has( m_commonAnimationsTime, p_name );
			}
		};
	}
}

#endif
