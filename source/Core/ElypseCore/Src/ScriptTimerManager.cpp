#include "PrecompiledHeader.h"

#include "ScriptTimerManager.h"

#include "ScriptEngine.h"
#include "ScriptTimer.h"
#include "ScriptNode.h"

ScriptTimerManager::ScriptTimerManager( ScriptEngine * p_scriptEngine )
	:	m_engine( p_scriptEngine ),
		m_currentTimer( NULL ),
		m_paused( false ),
		m_deleteCurrentTimer( false )
{
	genlib_assert( m_engine != NULL );
	m_timeLeft = m_engine->GetVariable( "CURRENTTIMER_TIME_LEFT" );
	m_timeBase = m_engine->GetVariable( "CURRENTTIMER_TIME_BASE" );
	m_timeElapsed = m_engine->GetVariable( "CURRENTTIMER_TIME_ELAPSED" );
	m_numExecutions = m_engine->GetVariable( "CURRENTTIMER_NUMEXECS" );
	m_self = m_engine->GetVariable( "CURRENTTIMER_SELF" );
	genlib_assert( m_timeLeft != NULL );
	genlib_assert( m_timeBase != NULL );
	genlib_assert( m_timeElapsed != NULL );
	genlib_assert( m_numExecutions != NULL );
	genlib_assert( m_self != NULL );
}

ScriptTimerManager::~ScriptTimerManager()
{
	General::Utils::map::deleteAll( m_timers );
}

void ScriptTimerManager::UpdateAll( Real p_time )
{
	if ( m_paused )
	{
		return;
	}

	TimerMap::iterator i = m_timers.begin();

	while ( i != m_timers.end() )
	{
		ScriptTimer * l_timer = i->second;

		if ( l_timer->NeedExec( p_time ) )
		{
			m_numExecutions->set <int> ( static_cast< int >( l_timer->m_numExecs ) );
			m_timeBase->set <Real> ( l_timer->m_baseTime );
			m_self->get <ScriptTimer *> () = l_timer;

			if ( l_timer->m_type == EMTT_PERMANENT )
			{
				m_timeLeft->get <Real> () = p_time;
				m_timeElapsed->get <Real> () = l_timer->m_leftTime;
			}
			else
			{
				m_timeLeft->get <Real> () = l_timer->m_leftTime;
				m_timeElapsed->get <Real> () = l_timer->m_baseTime - l_timer->m_leftTime;
			}

			m_deleteCurrentTimer = false;
			m_currentTimer = l_timer;
			m_engine->Execute( l_timer->m_action );

			if ( m_deleteCurrentTimer )
			{
				delete l_timer;
				TimerMap::iterator j = i;
				++j;
				m_timers.erase( i );
				i = j;
				continue;
			}

			m_currentTimer = NULL;

			if ( l_timer->m_type == EMTT_ONCE )
			{
				delete l_timer;
				TimerMap::iterator j = i ;
				++j;
				m_timers.erase( i );
				i = j;
				continue;
			}

			if ( l_timer->m_type == EMTT_REPEAT )
			{
				l_timer->m_leftTime += l_timer->m_baseTime;
			}
			else if ( l_timer->m_type == EMTT_CONTINUOUS && l_timer->m_leftTime <= 0.0 )
			{
				if ( l_timer->m_finalAction )
				{
					m_engine->Execute( l_timer->m_finalAction );
				}

				delete l_timer;
				TimerMap::iterator j = i;
				++j;
				m_timers.erase( i );
				i = j;
				continue;
			}
		}

		++ i;
	}
}

ScriptTimer * ScriptTimerManager::AddTimer( const String & p_timerName, Real p_timerBaseTime, ScriptNode * p_timerCode, ScriptTimerType p_type, ScriptNode * p_finalTimer )
{
	genlib_assert( ! p_timerName.empty() );

	if ( p_timerCode == NULL )
	{
		return NULL;
	}

	ScriptTimer * l_timer = GetByName( p_timerName );

	if ( l_timer != NULL )
	{
		return l_timer;
	}

	l_timer = new ScriptTimer( p_timerName, p_timerBaseTime, p_timerCode, p_type, p_finalTimer );
	p_timerCode->Use();

	if ( p_finalTimer != NULL )
	{
		p_finalTimer->Use();
	}

	m_timers.insert( TimerMap::value_type( p_timerName, l_timer ) );
	return l_timer;
}

void ScriptTimerManager::DestroyTimer( const String & p_timerName )
{
	ScriptTimer * l_timer = General::Utils::map::findOrNull( m_timers, p_timerName );

	if ( m_currentTimer != NULL && l_timer == m_currentTimer )
	{
		m_deleteCurrentTimer = true;
		return;
	}

	General::Utils::map::deleteValue( m_timers, p_timerName );
}

void ScriptTimerManager::PauseAll()
{
	General::Utils::map::cycle( m_timers, & ScriptTimer::Pause );
}

void ScriptTimerManager::PlayAll()
{
	General::Utils::map::cycle( m_timers, & ScriptTimer::Play );
}

void ScriptTimerManager::KillAll()
{
	General::Utils::map::deleteAll( m_timers );
}
