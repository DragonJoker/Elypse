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
#ifndef ___SCRIPT_TIMER_MANAGER_H___
#define ___SCRIPT_TIMER_MANAGER_H___

#include "Module_Script.h"

namespace Elypse
{
	namespace Script
	{
		class d_dll_export ScriptTimerManager d_debug_check_noncopyable
		{
		private:
			ScriptEngine * m_engine;
			ScriptNode * m_timeLeft;
			ScriptNode * m_timeBase;
			ScriptNode * m_numExecutions;
			ScriptNode * m_timeElapsed;
			ScriptNode * m_self;
			ScriptTimer * m_currentTimer;

			TimerMap m_timers;

			bool m_paused;
			bool m_deleteCurrentTimer;

		public:
			ScriptTimerManager( ScriptEngine * p_scriptEngine );
			~ScriptTimerManager();

		public:
			void UpdateAll( Real p_timeStep );

			ScriptTimer * AddTimer( String const & p_timerName, Real p_baseTime, ScriptNode * p_code, ScriptTimerType p_type, ScriptNode * p_finalCode = NULL );

			void DestroyTimer( String const & p_timerName );

			void PauseAll();
			void PlayAll();
			void KillAll();

			inline bool			  HasTimer( String const & p_timerName )const
			{
				return General::Utils::map::has( m_timers, p_timerName );
			}
			inline ScriptTimer * GetByName( String const & p_timerName )const
			{
				return General::Utils::map::findOrNull( m_timers, p_timerName );
			}

			inline void Pause()
			{
				m_paused = true;
			}
			inline void Play()
			{
				m_paused = false;
			}
		};
	}
}

#endif
