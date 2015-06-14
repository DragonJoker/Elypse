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
#ifndef ___Troll_ContinuousEvent___
#define ___Troll_ContinuousEvent___

#include "TemporalObject.h"

#include "GUI/TimeSequence/Module_Time.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	enum ContinuousEventArgs
	{
		ceReal,
		ceVector3,
		ceVector2,
		ceQuaternion,
	};

	class TrollKeyFrame
		: public TemporalObject
	{
	public:
		TrollKeyFrame( TrollContinuousEvent * p_event, float p_time, const wxString & p_action, const wxString & p_fileName, ContinuousEventArgs p_argsType )
			: TemporalObject( wxEmptyString, p_fileName, ttKeyFrame )
			, m_fireTime( p_time )
			, m_action( p_action )
			, m_argsType( p_argsType )
			, m_event( p_event )
		{
		}
		~TrollKeyFrame()
		{
		}

		virtual int BuildPanel( wxWindow * p_parent, int p_width );
		void SetFireTime( float p_time, bool p_updateEvent = false );

		void Write( wxTextOutputStream * p_stream );

		inline void SetAction( const wxString & p_action )
		{
			m_action = p_action;
		}
		inline void SetArgsType( ContinuousEventArgs p_type )
		{
			m_argsType = p_type;
		}
		inline float GetFireTime()const
		{
			return m_fireTime;
		}
		inline wxString GetAction()const
		{
			return m_action;
		}
		inline ContinuousEventArgs GetArgsType()const
		{
			return m_argsType;
		}

	private:
		float m_fireTime;
		wxString m_action;
		ContinuousEventArgs m_argsType;
		TrollContinuousEvent * m_event;
	};

	class TrollContinuousEvent
		: public TemporalObject
	{
	public:
		TrollContinuousEvent( TrollSequence * p_sequence, BaseContinuousEvent * p_event, const wxString & p_name, const wxString & p_fileName );
		~TrollContinuousEvent();

		TrollKeyFrame * AddKeyFrame( float p_time, const wxString & p_action );
		void GetKeyFrames( wxArrayString & p_list );

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void UpdateKeyFrame( TrollKeyFrame * p_keyFrame );
		void UpdateTime( float p_newStartTime );
		void Write( wxTextOutputStream * p_stream );

		inline float GetStartTime()const
		{
			return m_startTime;
		}
		inline float GetLength()const
		{
			return m_duration;
		}
		inline wxString GetTargetType()const
		{
			return m_targetTypeName;
		}
		inline wxString GetTarget()const
		{
			return m_targetName;
		}
		inline wxString GetInterpolation()const
		{
			return m_interpolationType;
		}
		inline TrollKeyFrame * GetKeyFrame( float p_timeIndex )
		{
			return ( m_keyFrames.find( p_timeIndex ) != m_keyFrames.end() ? m_keyFrames[p_timeIndex] : NULL );
		}
		inline BaseContinuousEvent 	*	GetMuseEvent()const
		{
			return m_museEvent;
		}
		inline TrollSequence * GetSequence()const
		{
			return m_sequence;
		}
		inline TROLL_GUI_TIME_NAMESPACE::ContinuousEventPanel * GetEventPanel()const
		{
			return m_panel;
		}
		inline void SetTarget( const wxString & p_targetType, const wxString & p_target )
		{
			m_targetTypeName = p_targetType;
			m_targetName = p_target;
		}
		inline void SetInterpolation( const wxString & p_type )
		{
			m_interpolationType = p_type;
		}
		inline void SetTargetType( const wxString & p_type )
		{
			m_targetTypeName = p_type;
		}
		inline void SetTarget( const wxString & p_name )
		{
			m_targetName = p_name;
		}
		inline void SetActionName( const wxString & p_name )
		{
			m_name = p_name;
		}
		inline void SetPanel( TROLL_GUI_TIME_NAMESPACE::ContinuousEventPanel * p_panel )
		{
			m_panel = p_panel;
		}
		inline void	SetMuseEvent( BaseContinuousEvent * p_event )
		{
			m_museEvent = p_event;
		}

	private:
		void _computeTimes();

	private:
		TrollSequence * m_sequence;
		BaseContinuousEvent * m_museEvent;
		float m_startTime;
		float m_endTime;
		float m_duration;
		wxString m_targetTypeName;
		wxString m_targetName;
		wxString m_interpolationType;
		TrollKeyFrameMap m_keyFrames;
		TROLL_GUI_TIME_NAMESPACE::ContinuousEventPanel * m_panel;
	};
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE

#endif
