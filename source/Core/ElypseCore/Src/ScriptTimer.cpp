/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ScriptTimer.h"
#include "ScriptNode.h"

ScriptTimer::ScriptTimer( String const & p_name, Real p_baseTime, ScriptNode * p_code, ScriptTimerType p_type, ScriptNode * p_finalAction )
	: named( p_name )
	, m_action( p_code )
	, m_finalAction( p_finalAction )
	, m_type( p_type )
	, m_leftTime( p_baseTime )
	, m_baseTime( p_baseTime )
	, m_paused( false )
	, m_numExecs( 0 )
{
}

ScriptTimer::~ScriptTimer()
{
	if ( m_action != NULL )
	{
		m_action->Delete();
		m_action = NULL;
	}

	if ( m_finalAction != NULL )
	{
		m_finalAction->Delete();
		m_finalAction = NULL;
	}
}

bool ScriptTimer::NeedExec( Real p_timeStep )
{
	if ( m_paused )
	{
		return false;
	}

	if ( m_type == EMTT_PERMANENT )
	{
		m_leftTime += p_timeStep;
		m_numExecs ++;
		return true;
	}

	m_leftTime -= p_timeStep;

	if ( m_leftTime <= 0.0 || m_type == EMTT_CONTINUOUS )
	{
		if ( m_leftTime <= 0.0 && m_type == EMTT_CONTINUOUS )
		{
			m_leftTime = 0.0;
		}

		m_numExecs ++;
		return true;
	}

	return false;
}
