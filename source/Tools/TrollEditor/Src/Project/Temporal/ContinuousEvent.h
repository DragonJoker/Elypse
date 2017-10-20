/*
See LICENSE file in root folder
*/
#ifndef ___Troll_ContinuousEvent___
#define ___Troll_ContinuousEvent___

#include "TemporalObject.h"

#include "GUI/TimeSequence/Module_Time.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
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
				TrollKeyFrame( TrollContinuousEvent * p_event, float p_time, wxString const & p_action, wxString const & p_fileName, ContinuousEventArgs p_argsType )
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

				void SetFireTime( float p_time, bool p_updateEvent = false );

				void Write( wxTextOutputStream & p_stream );

				inline void SetAction( wxString const & p_action )
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
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

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
				TrollContinuousEvent( TrollSequence * p_sequence, BaseContinuousEvent * p_event, wxString const & p_name, wxString const & p_fileName );
				~TrollContinuousEvent();

				TrollKeyFrame * AddKeyFrame( float p_time, wxString const & p_action );
				void GetKeyFrames( wxArrayString & p_list );

				void UpdateKeyFrame( TrollKeyFrame * p_keyFrame );
				void UpdateTime( float p_newStartTime );
				void Write( wxTextOutputStream & p_stream );

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
				inline GUI::Time::ContinuousEventPanel * GetEventPanel()const
				{
					return m_panel;
				}
				inline void SetTarget( wxString const & p_targetType, wxString const & p_target )
				{
					m_targetTypeName = p_targetType;
					m_targetName = p_target;
				}
				inline void SetInterpolation( wxString const & p_type )
				{
					m_interpolationType = p_type;
				}
				inline void SetTargetType( wxString const & p_type )
				{
					m_targetTypeName = p_type;
				}
				inline void SetTarget( wxString const & p_name )
				{
					m_targetName = p_name;
				}
				inline void SetActionName( wxString const & p_name )
				{
					m_name = p_name;
				}
				inline void SetPanel( GUI::Time::ContinuousEventPanel * p_panel )
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
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

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
				GUI::Time::ContinuousEventPanel * m_panel;
			};
		}
	}
}

#endif
