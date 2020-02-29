/*
See LICENSE file in root folder
*/
#ifndef ___EMUSE_SCRIPT_TIMER___
#define ___EMUSE_SCRIPT_TIMER___

#include "Module_Script.h"

namespace Elypse::Script
{
	class d_dll_export ScriptTimer : public named
	{
	public:
		ScriptNode * m_action;
		ScriptNode * m_finalAction;

		ScriptTimerType m_type;

		Real m_leftTime;
		Real m_baseTime;

		bool m_paused;

		uint32_t m_numExecs;

	public:
		ScriptTimer( String const & p_name, Real p_baseTime, ScriptNode * p_code, ScriptTimerType p_type, ScriptNode * p_finalAction = nullptr );
		~ScriptTimer();

	public:
		bool NeedExec( Real p_timeStep );

	public:
		inline void SetBaseTime( Real p_baseTime )
		{
			m_baseTime = p_baseTime;
		}
		inline void Reset()
		{
			m_leftTime = m_baseTime;
		}
		inline void Shift( Real p_time )
		{
			m_leftTime -= p_time;
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
