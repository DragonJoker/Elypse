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
#ifndef ___Troll_PonctualEvent___
#define ___Troll_PonctualEvent___

#include "TemporalObject.h"

#include "GUI/TimeSequence/Module_Time.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	class TrollPonctualEvent
		: public TemporalObject
	{
	private:
		TrollSequence * m_sequence;
		BasePonctualEvent * m_museEvent;
		float m_fireTime;
		wxString m_targetTypeName;
		wxString m_targetName;
		wxString m_actionArguments;
		TROLL_GUI_TIME_NAMESPACE::PonctualEventPanel * m_panel;

	public:
		TrollPonctualEvent( TrollSequence * p_sequence, BasePonctualEvent * p_event, const wxString & p_targetTypeName, const wxString & p_targetName, float p_fireTime, const wxString & p_eventAction, const wxString & p_name, const wxString & p_fileName );
		~TrollPonctualEvent();

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void Write( wxTextOutputStream * p_stream );

		void SetFireTime( float p_time );
		void SetActionName( const wxString & p_name );
		void SetTarget( const wxString & p_type, const wxString & p_target );
		void SetActionArgs( const wxString & p_args );

		inline float GetFireTime()const
		{
			return m_fireTime;
		}
		inline wxString GetTargetType()const
		{
			return m_targetTypeName;
		}
		inline wxString GetTarget()const
		{
			return m_targetName;
		}
		inline BasePonctualEvent * GetMuseEvent()const
		{
			return m_museEvent;
		}
		inline wxString GetActionArgs()const
		{
			return m_actionArguments;
		}
		inline TrollSequence * GetSequence()const
		{
			return m_sequence;
		}
		inline TROLL_GUI_TIME_NAMESPACE::PonctualEventPanel * GetEventPanel()const
		{
			return m_panel;
		}

		inline void SetPanel( TROLL_GUI_TIME_NAMESPACE::PonctualEventPanel * p_panel )
		{
			m_panel = p_panel;
		}
		inline void	SetMuseEvent( BasePonctualEvent * p_event )
		{
			m_museEvent = p_event;
		}
	};
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE

#endif

