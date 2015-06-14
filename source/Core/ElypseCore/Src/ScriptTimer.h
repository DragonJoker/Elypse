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
#ifndef ___EMUSE_SCRIPT_TIMER___
#define ___EMUSE_SCRIPT_TIMER___

#include "Module_Script.h"

namespace Elypse
{
	namespace Script
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

			unsigned int m_numExecs;

		public:
			ScriptTimer( const String & p_name, Real p_baseTime, ScriptNode * p_code, ScriptTimerType p_type, ScriptNode * p_finalAction = NULL );
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
}

#endif
