
#ifndef ___Troll_Sequence___
#define ___Troll_Sequence___

#include "Module_Temporal.h"
#include "GUI/Module_GUI.h"
#include "TemporalObject.h"

#include <Sequence.h>
#include <Events.h>

namespace Troll
{
	namespace Temporal
	{
		class TrollSequence
			: public TemporalObject
			, public Sequence
		{
		private:
			GUI::LinePanel * m_panel;
			float m_begin;
			float m_end;
			float m_duration;
			TrollPonctualEventMap m_teponctualEvents;
			TrollContinuousEventMap m_tecontinuousEvents;

		public:
			TrollSequence( const wxString & p_name, const wxString & p_fileName );
			~TrollSequence();

			TrollPonctualEvent * AddPonctualEvent( BasePonctualEvent * p_event, const wxString & p_targetTypeName,
												   const wxString & p_targetName, float p_fireTime,
												   const wxString & p_eventAction, const wxString & p_name,
												   const wxString & p_fileName );
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

		private:
			void _rollback( Real p_time );
			void _update( Real p_time );
			void _computeTimes();

		public:
			inline TrollPonctualEventMap	GetPonctualEvents()const
			{
				return m_teponctualEvents;
			}
			inline TrollContinuousEventMap	GetContinuousEvents()const
			{
				return m_tecontinuousEvents;
			}
			inline float				GetTotalLength()const
			{
				return m_duration;
			}
			inline float				GetStartTime()const
			{
				return m_begin;
			}
			inline float				GetEndTime()const
			{
				return m_end;
			}
			inline GUI::LinePanel 	*	GetLinePanel()const
			{
				return m_panel;
			}

			inline void SetPanel( GUI::LinePanel * p_panel )
			{
				m_panel = p_panel;
			}
		};
	}
}

#endif

