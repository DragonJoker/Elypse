//***********************************************************************************************************
#ifndef ___Troll_PonctualEvent___
#define ___Troll_PonctualEvent___
//***********************************************************************************************************
#include "Module_Temporal.h"
#include "TemporalObject.h"
#include "../../GUI/Module_GUI.h"
//***********************************************************************************************************
namespace Troll
{
	namespace Temporal
	{
		class TrollPonctualEvent : public TemporalObject
		{
		private:
			TrollSequence * m_sequence;
			BasePonctualEvent * m_museEvent;
			float m_fireTime;
			wxString m_targetTypeName;
			wxString m_targetName;
			wxString m_actionArguments;
			GUI::PonctualEventPanel * m_panel;

		public:
			TrollPonctualEvent( TrollSequence * p_sequence, BasePonctualEvent * p_event,
								const wxString & p_targetTypeName, const wxString & p_targetName, float p_fireTime,
								const wxString & p_eventAction, const wxString & p_name, const wxString & p_fileName );
			~TrollPonctualEvent();

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void Write( wxTextOutputStream * p_stream );

			void SetFireTime( float p_time );
			void SetActionName( const wxString & p_name );
			void SetTarget( const wxString & p_type, const wxString & p_target );
			void SetActionArgs( const wxString & p_args );

		public:
			inline float						GetFireTime()const
			{
				return m_fireTime;
			}
			inline wxString						GetTargetType()const
			{
				return m_targetTypeName;
			}
			inline wxString						GetTarget()const
			{
				return m_targetName;
			}
			inline BasePonctualEvent 	*		GetMuseEvent()const
			{
				return m_museEvent;
			}
			inline wxString						GetActionArgs()const
			{
				return m_actionArguments;
			}
			inline TrollSequence 		*			GetSequence()const
			{
				return m_sequence;
			}
			inline GUI::PonctualEventPanel *	GetEventPanel()const
			{
				return m_panel;
			}

			inline void SetPanel( GUI::PonctualEventPanel * p_panel )
			{
				m_panel = p_panel;
			}
			inline void	SetMuseEvent( BasePonctualEvent * p_event )
			{
				m_museEvent = p_event;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************