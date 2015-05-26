//***********************************************************************************************************
#ifndef ___Troll_ContinuousEvent___
#define ___Troll_ContinuousEvent___
//***********************************************************************************************************
#include "Module_Temporal.h"
#include "../../GUI/Module_GUI.h"
#include "TemporalObject.h"
//***********************************************************************************************************
namespace Troll
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

		class TrollContinuousEvent;

		class TrollKeyFrame : public TemporalObject
		{
		private:
			float m_fireTime;
			wxString m_action;
			ContinuousEventArgs m_argsType;
			TrollContinuousEvent * m_event;

		public:
			TrollKeyFrame( TrollContinuousEvent * p_event, float p_time, const wxString & p_action, const wxString & p_fileName, ContinuousEventArgs p_argsType )
				:	TemporalObject( wxEmptyString, p_fileName, ttKeyFrame ),
					m_fireTime( p_time ),
					m_action( p_action ),
					m_argsType( p_argsType ),
					m_event( p_event )
			{}
			~TrollKeyFrame() {}

			virtual int BuildPanel( wxWindow * p_parent, int p_width );
			void SetFireTime( float p_time, bool p_updateEvent = false );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline void SetAction( const wxString & p_action )
			{
				m_action = p_action;
			}
			inline void SetArgsType( ContinuousEventArgs p_type )
			{
				m_argsType = p_type;
			}

			inline float				GetFireTime()const
			{
				return m_fireTime;
			}
			inline wxString				GetAction()const
			{
				return m_action;
			}
			inline ContinuousEventArgs	GetArgsType()const
			{
				return m_argsType;
			}
		};

		class TrollContinuousEvent : public TemporalObject
		{
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
			GUI::ContinuousEventPanel * m_panel;

		public:
			TrollContinuousEvent( TrollSequence * p_sequence, BaseContinuousEvent * p_event, const wxString & p_name, const wxString & p_fileName );
			~TrollContinuousEvent();

			TrollKeyFrame * AddKeyFrame( float p_time, const wxString & p_action );
			void GetKeyFrames( wxArrayString & p_list );

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void UpdateKeyFrame( TrollKeyFrame * p_keyFrame );

			void UpdateTime( float p_newStartTime );

			void Write( wxTextOutputStream * p_stream );

		private:
			void _computeTimes();

		public:
			inline float						GetStartTime()const
			{
				return m_startTime;
			}
			inline float						GetLength()const
			{
				return m_duration;
			}
			inline wxString						GetTargetType()const
			{
				return m_targetTypeName;
			}
			inline wxString						GetTarget()const
			{
				return m_targetName;
			}
			inline wxString						GetInterpolation()const
			{
				return m_interpolationType;
			}
			inline TrollKeyFrame 		*			GetKeyFrame( float p_timeIndex )
			{
				return ( m_keyFrames.find( p_timeIndex ) != m_keyFrames.end() ? m_keyFrames[p_timeIndex] : NULL );
			}
			inline BaseContinuousEvent 	*	GetMuseEvent()const
			{
				return m_museEvent;
			}
			inline TrollSequence 		*			GetSequence()const
			{
				return m_sequence;
			}
			inline GUI::ContinuousEventPanel *	GetEventPanel()const
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
			inline void SetPanel( GUI::ContinuousEventPanel * p_panel )
			{
				m_panel = p_panel;
			}
			inline void	SetMuseEvent( BaseContinuousEvent * p_event )
			{
				m_museEvent = p_event;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************