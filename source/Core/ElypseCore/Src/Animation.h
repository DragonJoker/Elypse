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
#ifndef ___ANIMATION_H___
#define ___ANIMATION_H___

#include "Module_Animation.h"

/**
 * Describes one animation, for an object.
 * Can be vertex animation ( morph ), or bone animation ( skeleton )
**/
namespace Elypse
{
	namespace Animation
	{
		class d_dll_export  EMAnimation : public General::Theory::named,
			General::Theory::owned_by<AnimatedObject>,
			d_noncopyable
		{
		protected:
			//The actual link to the Ogre animationState, which controls the bone/vertices needed by said animation.
			Ogre::AnimationState * m_animation;
			EMAnimation * m_nextAnimation;

			//Current status of the animation, Paused, Stopped, Started,Timeout, etc...
			AnimationStatus m_status;

			//Current time of the animation
			Real m_time;

			//Length of the animation
			Real m_length;

			Real m_timescale;

			//How long does the time out lasts
			Real m_maxTimeOut;

			//How long does the time int last
			Real m_maxTimeIn;

			//Current time index in regard to the timeout, permanently between 0 and m_maxTimeOut.
			Real m_timeOut;

			//Current weight of the animation. 1 during normal play, 0 when stopped, ranges from 0 to 1 during timeout, if applicable.
			Real m_weight;

			//Is this animation looped.
			bool m_looped;

			//Is this animation looped.
			bool m_reversed;

			//Does this animation time out
			bool m_useTimeOut;

			//Does this animation time in
			bool m_useTimeIn;

		public:
			EMAnimation( AnimatedObject * p_owner, Ogre::AnimationState * p_ogreAnimationState );
			~EMAnimation();

		private:
			void _stop( bool p_continueChain );
			void _setTime( Real p_time );

		public:
			void Play();
			void Stop();
			void Pause();
			void SetPause( bool p_paused );

			void SetTime( Real p_currentTimeIndex );
			void SetLooped( bool p_looped );

			void Update( Real p_timeStep );

			void SetWeight( Real p_weight );

		public: //Inlines
			inline			void				SetNextAnimation( EMAnimation * p_anim )
			{
				m_nextAnimation = p_anim;
			}
			inline			void				SetTimescale( Real p_timescale )
			{
				m_timescale = p_timescale;
			}
			inline			AnimationStatus		GetStatus()const
			{
				return m_status;
			}
			inline			Real				GetLength()const
			{
				return m_length;
			}
			inline const Ogre::AnimationState * GetAnimationState()const
			{
				return m_animation;
			}
			inline			void				SetTimeOut( Real p_maxTimeOut )
			{
				m_maxTimeOut = p_maxTimeOut;
				m_useTimeOut = ( p_maxTimeOut > 0.0 );
			}
			inline			void				SetTimeIn( Real p_maxTimeIn )
			{
				m_maxTimeIn = p_maxTimeIn;
				m_useTimeIn = ( p_maxTimeIn > 0.0 );
			}
			inline			bool				IsLooped()const
			{
				return m_looped;
			}
			inline			Real				GetTimeIn()const
			{
				return m_maxTimeIn;
			}
			inline			Real				GetTimeOut()const
			{
				return m_maxTimeOut;
			}
			inline			void				SetReversed( bool p_reversed )
			{
				m_reversed = p_reversed;
			}
			inline			bool				IsReversed()const
			{
				return m_reversed;
			}
		};
	}
}

#endif
