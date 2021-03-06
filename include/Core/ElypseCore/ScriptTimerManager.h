/*
See LICENSE file in root folder
*/
#ifndef ___SCRIPT_TIMER_MANAGER_H___
#define ___SCRIPT_TIMER_MANAGER_H___

#include "Module_Script.h"

namespace Elypse::Script
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

		ScriptTimer * AddTimer( String const & p_timerName, Real p_baseTime, ScriptNode * p_code, ScriptTimerType p_type, ScriptNode * p_finalCode = nullptr );

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

#endif
