/*
See LICENSE file in root folder
*/
#ifndef ___ANIMATED_OBJECT_GROUP_H___
#define ___ANIMATED_OBJECT_GROUP_H___

#include "Module_Animation.h"

namespace Elypse
{
	namespace Animation
	{
		class d_dll_export  AnimatedObjectGroup
			: public General::Theory::named
			, public General::Theory::owned_by<AnimationManager>
			, d_noncopyable
		{
		protected:
			AnimatedObjectMap m_objects;
			RealMap m_commonAnimationsTime;

		public:
			AnimatedObjectGroup( String const & p_name, AnimationManager & p_owner );
			~AnimatedObjectGroup();

		public:
			void Update( Real p_timeStep );
			AnimatedObject * AddEntity( Ogre::Entity * p_timeStep );

			void StartAnimation( String const & p_animationName );
			void StopAnimation( String const & p_animationName );
			void PauseAnimation( String const & p_animationName );
			void SetTimeAnimation( String const & p_animationName, Real p_timeIndex );
			void SetWeightAnimation( String const & p_animationName, Real p_weight );
			void SetAnimationTimeOut( String const & p_animationName, Real p_timeOut );
			void SetAnimationTimeIn( String const & p_animationName, Real p_timeIn );
			void SetAnimationLooped( String const & p_animationName, bool p_looped );
			void SetAnimationReversed( String const & p_animationName, bool p_reversed );

			void SetTimeScaleAnimation( String const & p_animationName, Real p_timescale );

			void ChainAnimation( String const & p_animName, String const & p_chainedAnimationName );

			void RemoveObject( String const & p_objectName );

			Real GetAnimationTime( String const & p_animationName )const;

			void StopAllAnimations();

			void ChainAllAnimations( bool p_looped );

			void Clear();

			void SetPauseAll( bool p_pause );

		public:
			inline size_t GetNumObjects()const
			{
				return m_objects.size();
			}
			inline AnimatedObject * GetObjectByName( String const & p_objectName )const
			{
				return General::Utils::map::findOrNull( m_objects, p_objectName );
			}
			inline bool HasAnimation( String const & p_name )
			{
				return General::Utils::map::has( m_commonAnimationsTime, p_name );
			}
		};
	}
}

#endif
