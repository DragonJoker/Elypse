/*
See LICENSE file in root folder
*/
#ifndef ___Troll_Sequence___
#define ___Troll_Sequence___

#include "TemporalObject.h"

#include "GUI/TimeSequence/Module_Time.h"

#include <Sequence.h>
#include <Events.h>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TrollSequence
				: public TemporalObject
				, public Sequence
			{
			public:
				TrollSequence( wxString const & p_name, wxString const & p_fileName );
				~TrollSequence();

				TrollPonctualEvent * AddPonctualEvent( BasePonctualEvent * p_event, wxString const & p_targetTypeName, wxString const & p_targetName, float p_fireTime, wxString const & p_eventAction, wxString const & p_name, wxString const & p_fileName );
				TrollPonctualEvent * AddPonctualEvent( TrollPonctualEvent * p_event );
				TrollContinuousEvent * AddContinuousEvent( TrollContinuousEvent * p_event );

				void RemovePonctualEvent( TrollPonctualEvent * p_event );
				void RemoveContinuousEvent( TrollContinuousEvent * p_event );
				void Write( wxTextOutputStream & p_stream );
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
				inline GUI::Time::LinePanel * GetLinePanel()const
				{
					return m_panel;
				}
				inline void SetPanel( GUI::Time::LinePanel * p_panel )
				{
					m_panel = p_panel;
				}

			private:
				void _rollback( Real p_time );
				void _update( Real p_time );
				void _computeTimes();

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				GUI::Time::LinePanel * m_panel;
				float m_begin;
				float m_end;
				float m_duration;
				TrollPonctualEventMap m_teponctualEvents;
				TrollContinuousEventMap m_tecontinuousEvents;
			};
		}
	}
}

#endif
