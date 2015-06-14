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
#ifndef ___Troll_Sequence___
#define ___Troll_Sequence___

#include "TemporalObject.h"

#include "GUI/TimeSequence/Module_Time.h"

#include <Sequence.h>
#include <Events.h>

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	class TrollSequence
		: public TemporalObject
		, public Sequence
	{
	public:
		TrollSequence( const wxString & p_name, const wxString & p_fileName );
		~TrollSequence();

		TrollPonctualEvent * AddPonctualEvent( BasePonctualEvent * p_event, const wxString & p_targetTypeName, const wxString & p_targetName, float p_fireTime, const wxString & p_eventAction, const wxString & p_name, const wxString & p_fileName );
		TrollPonctualEvent * AddPonctualEvent( TrollPonctualEvent * p_event );
		TrollContinuousEvent * AddContinuousEvent( TrollContinuousEvent * p_event );

		void RemovePonctualEvent( TrollPonctualEvent * p_event );
		void RemoveContinuousEvent( TrollContinuousEvent * p_event );
		virtual int BuildPanel( wxWindow * p_parent, int p_width );
		void Write( wxTextOutputStream * p_stream );
		void ChangeContinuousEventTime( TrollContinuousEvent * p_event );
		void ChangePonctualEventTime( TrollPonctualEvent * p_event );
		void UpdatePonctualEvent( TrollPonctualEvent * p_event );
		void Rollback( Real p_time );
		virtual void Update( Real p_time );

		inline TrollPonctualEventMap GetPonctualEvents()const
		{
			return m_teponctualEvents;
		}
		inline TrollContinuousEventMap GetContinuousEvents()const
		{
			return m_tecontinuousEvents;
		}
		inline float GetTotalLength()const
		{
			return m_duration;
		}
		inline float GetStartTime()const
		{
			return m_begin;
		}
		inline float GetEndTime()const
		{
			return m_end;
		}
		inline TROLL_GUI_TIME_NAMESPACE::LinePanel * GetLinePanel()const
		{
			return m_panel;
		}
		inline void SetPanel( TROLL_GUI_TIME_NAMESPACE::LinePanel * p_panel )
		{
			m_panel = p_panel;
		}

	private:
		void _rollback( Real p_time );
		void _update( Real p_time );
		void _computeTimes();

	private:
		TROLL_GUI_TIME_NAMESPACE::LinePanel * m_panel;
		float m_begin;
		float m_end;
		float m_duration;
		TrollPonctualEventMap m_teponctualEvents;
		TrollContinuousEventMap m_tecontinuousEvents;
	};
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE

#endif
