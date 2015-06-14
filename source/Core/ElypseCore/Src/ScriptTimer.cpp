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
#include "PrecompiledHeader.h"

#include "ScriptTimer.h"
#include "ScriptNode.h"

ScriptTimer::ScriptTimer( const String & p_name, Real p_baseTime, ScriptNode * p_code, ScriptTimerType p_type, ScriptNode * p_finalAction )
	: named( p_name ),
		m_action( p_code ),
		m_finalAction( p_finalAction ),
		m_type( p_type ),
		m_leftTime( p_baseTime ),
		m_baseTime( p_baseTime ),
		m_paused( false ),
		m_numExecs( 0 )
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
