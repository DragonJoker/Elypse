#include "Sequence.h"

#include "ContinuousEvent.h"
#include "PonctualEvent.h"
#include "GUI/Properties/Project/SequenceProperties.h"
#include "GUI/TimeSequence/LinePanel.h"
#include "GUI/TimeSequence/SequencePanel.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TrollSequence::TrollSequence( wxString const & p_name, wxString const & p_fileName )
				: TemporalObject( p_name, p_fileName, ttSequence )
				, Sequence( make_string( p_name ) )
				, m_panel( nullptr )
				, m_begin( 0.0 )
				, m_end( 0.0 )
				, m_duration( 0.0 )
			{
			}

			TrollSequence::~TrollSequence()
			{
				multimap::deleteAll( m_teponctualEvents );
				multimap::deleteAll( m_tecontinuousEvents );
				m_ponctualEvents.clear();
				m_continuousEvents.clear();
			}

			TrollPonctualEvent * TrollSequence::AddPonctualEvent( BasePonctualEvent * p_event, wxString const & p_targetTypeName,
					wxString const & p_targetName, float p_fireTime,
					wxString const & p_eventAction, wxString const & p_name,
					wxString const & p_fileName )
			{
				if ( m_ponctualEvents.find( p_fireTime ) != m_ponctualEvents.end() )
				{
					return m_teponctualEvents.find( p_fireTime )->second;
				}

				Sequence::AddPonctualEvent( p_event, p_fireTime );
				TrollPonctualEvent * l_event = new TrollPonctualEvent( this, p_event, p_targetTypeName, p_targetName, p_fireTime, p_eventAction, p_name, p_fileName );
				m_teponctualEvents.insert( std::make_pair( p_fireTime, l_event ) );
				m_end = max <float>( m_end, p_fireTime );
				m_duration = m_end - m_begin;

				if ( m_ponctualEvents.size() == 1 )
				{
					m_ponctualIterator = m_ponctualEvents.begin();
				}

				return l_event;
			}

			TrollPonctualEvent * TrollSequence::AddPonctualEvent( TrollPonctualEvent * p_event )
			{
				float l_fireTime = p_event->GetFireTime();
				Sequence::AddPonctualEvent( p_event->GetMuseEvent(), l_fireTime );
				m_teponctualEvents.insert( std::make_pair( l_fireTime, p_event ) );
				m_end = max <float>( m_end, l_fireTime );
				m_duration = m_end - m_begin;

				if ( m_ponctualEvents.size() == 1 )
				{
					m_ponctualIterator = m_ponctualEvents.begin();
				}

				return p_event;
			}

			TrollContinuousEvent * TrollSequence::AddContinuousEvent( TrollContinuousEvent * p_event )
			{
				float l_fireTime = p_event->GetStartTime();
				Sequence::AddContinuousEvent( p_event->GetMuseEvent() );
				m_tecontinuousEvents.insert( std::make_pair( l_fireTime, p_event ) );
				m_end = max <float>( m_end, l_fireTime + p_event->GetLength() );
				m_duration = m_end - m_begin;

				if ( m_continuousEvents.size() == 1 )
				{
					m_continuousIterator = m_continuousEvents.begin();
				}

				return p_event;
			}

			void TrollSequence::RemovePonctualEvent( TrollPonctualEvent * p_event )
			{
				Sequence::RemovePonctualEvent( p_event->GetMuseEvent() );
				bool l_found = false;
				TrollPonctualEventMap::iterator l_it = m_teponctualEvents.begin();

				while ( l_it != m_teponctualEvents.end() && !l_found )
				{
					if ( l_it->second == p_event )
					{
						reinterpret_cast< GUI::Time::SequencePanel * >( m_panel->GetParent() )->RemovePonctualEvent( p_event );
						m_teponctualEvents.erase( l_it );
						delete p_event;
						l_found = true;
					}
					else
					{
						++l_it;
					}
				}
			}

			void TrollSequence::RemoveContinuousEvent( TrollContinuousEvent * p_event )
			{
				Sequence::RemoveContinuousEvent( p_event->GetMuseEvent() );
				bool l_found = false;
				TrollContinuousEventMap::iterator l_it = m_tecontinuousEvents.begin();

				while ( l_it != m_tecontinuousEvents.end() && !l_found )
				{
					if ( l_it->second == p_event )
					{
						reinterpret_cast< GUI::Time::SequencePanel * >( m_panel->GetParent() )->RemoveContinuousEvent( p_event );
						m_tecontinuousEvents.erase( l_it );
						delete p_event;
						l_found = true;
					}
					else
					{
						++l_it;
					}
				}
			}

			void TrollSequence::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "sequence " ) + Object::m_name + wxT( "\n{\n" ) );
				TrollPonctualEventMap::iterator l_it = m_teponctualEvents.begin();

				while ( l_it != m_teponctualEvents.end() )
				{
					l_it->second->Write( p_stream );
					++l_it;
				}

				TrollContinuousEventMap::iterator l_it2 = m_tecontinuousEvents.begin();

				while ( l_it2 != m_tecontinuousEvents.end() )
				{
					l_it2->second->Write( p_stream );
					++l_it2;
				}

				p_stream.WriteString( wxT( "}\n\n" ) );
			}

			void TrollSequence::ChangeContinuousEventTime( TrollContinuousEvent * p_event )
			{
				bool l_found = false;
				TrollContinuousEventMap::iterator l_it = m_tecontinuousEvents.begin();

				while ( l_it != m_tecontinuousEvents.end() && !l_found )
				{
					if ( l_it->second == p_event )
					{
						l_found = true;
						m_tecontinuousEvents.erase( l_it );
						m_tecontinuousEvents.insert( std::make_pair( p_event->GetStartTime(), p_event ) );
						Sequence::ChangeContinuousEventTime( p_event->GetMuseEvent(), p_event->GetStartTime() );
					}
					else
					{
						++l_it;
					}
				}

				_computeTimes();

				if ( m_panel != nullptr )
				{
					m_panel->Replace();
				}
			}

			void TrollSequence::ChangePonctualEventTime( TrollPonctualEvent * p_event )
			{
				bool l_found = false;
				TrollPonctualEventMap::iterator l_it = m_teponctualEvents.begin();

				while ( l_it != m_teponctualEvents.end() && !l_found )
				{
					if ( l_it->second == p_event )
					{
						l_found = true;
						m_teponctualEvents.erase( l_it );
						m_teponctualEvents.insert( std::make_pair( p_event->GetFireTime(), p_event ) );
						Sequence::ChangePonctualEventTime( p_event->GetMuseEvent(), p_event->GetFireTime() );
					}
					else
					{
						++l_it;
					}
				}

				_computeTimes();

				if ( m_panel != nullptr )
				{
					m_panel->Replace();
				}
			}

			void TrollSequence::_computeTimes()
			{
				m_begin = 0.0;
				m_end = 0.0;
				m_duration = 0.0;
				TrollPonctualEventMap::iterator l_it = m_teponctualEvents.begin();

				while ( l_it != m_teponctualEvents.end() )
				{
					m_end = max <float>( m_end, l_it->first );
					++l_it;
				}

				TrollContinuousEventMap::iterator l_it2 = m_tecontinuousEvents.begin();

				while ( l_it2 != m_tecontinuousEvents.end() )
				{
					m_end = max <float>( m_end, l_it2->first + l_it2->second->GetLength() );
					++l_it2;
				}

				m_duration = m_end - m_begin;
			}

			void TrollSequence::UpdatePonctualEvent( TrollPonctualEvent * p_event )
			{
				bool l_found = false;
				TrollPonctualEventMap::iterator l_it = m_teponctualEvents.begin();

				while ( l_it != m_teponctualEvents.end() && !l_found )
				{
					if ( l_it->second == p_event )
					{
						l_found = true;
						m_teponctualEvents.erase( l_it );
						m_teponctualEvents.insert( std::make_pair( p_event->GetFireTime(), p_event ) );
						Sequence::RemovePonctualEvent( p_event->GetMuseEvent() );
						p_event->SetMuseEvent( nullptr );
						Ogre::StringVector l_vector = Ogre::StringUtil::split( make_string( p_event->GetActionArgs() ), ", \t" );
						BasePonctualEvent * l_museEvent = ScriptEngine::GetContext()->sequenceManager->CreatePonctualEvent( make_string( p_event->GetObjectName() ), StringArray( l_vector.begin(), l_vector.end() ) );
						p_event->SetMuseEvent( l_museEvent );
						Sequence::AddPonctualEvent( l_museEvent, p_event->GetFireTime() );
					}
					else
					{
						++l_it;
					}
				}
			}

			void TrollSequence::Update( Real p_time )
			{
				m_currentTime += p_time;

				//std::cout << "TrollSequence::Update - dt : " << p_time << " - current time : " << m_currentTime << "\n";
				if ( p_time < 0 )
				{
					_rollback( p_time );
				}
				else
				{
					_update( p_time );
				}
			}

			void TrollSequence::_rollback( Real p_time )
			{
				if ( m_ponctualIterator == m_ponctualEvents.end() )
				{
					--m_ponctualIterator;
				}

				while ( m_ponctualIterator != m_ponctualEvents.begin() && m_ponctualIterator->first > m_currentTime )
				{
					//std::cout << "Sequence::_rollback - Rollback ponctual event " << m_ponctualIterator->first << "\n";
					m_ponctualIterator->second->Rollback();
					--m_ponctualIterator;
				}

				if ( m_ponctualIterator == m_ponctualEvents.begin() && m_ponctualIterator->first > m_currentTime )
				{
					//std::cout << "Sequence::_rollback - Rollback first ponctual event " << m_ponctualIterator->first << "\n";
					m_ponctualIterator->second->Rollback();
				}

				ContinuousEventSet::iterator i = m_currentEvents.begin();
				const ContinuousEventSet::iterator & iend = m_currentEvents.end();

				for ( ; m_currentEvents.size() > 0; )
				{
					( *i )->Reset();
					General::Utils::set::eraseIterator( m_currentEvents, i );
					i = m_currentEvents.begin();
				}

				m_continuousIterator = m_continuousEvents.begin();

				while ( m_continuousIterator != m_continuousEvents.end() && m_continuousIterator->first <= m_currentTime )
				{
					//std::cout << "Sequence::_rollback - Begin continuous event " << m_continuousIterator->first << "\n";
					m_currentEvents.insert( m_continuousIterator->second );
					++m_continuousIterator;
				}

				i = m_currentEvents.begin();

				for ( ; i != iend; )
				{
					( *i )->Apply( m_currentTime - ( *i )->GetStartTime() );
					++i;
				}
			}

			void TrollSequence::_update( Real p_time )
			{
				while ( m_ponctualIterator != m_ponctualEvents.end() && m_ponctualIterator->first <= m_currentTime )
				{
					//std::cout << "Sequence::_update - Apply ponctual event - " << m_ponctualIterator->first << "\n";
					m_ponctualIterator->second->Apply();
					++m_ponctualIterator;
				}

				while ( m_continuousIterator != m_continuousEvents.end() && m_continuousIterator->first <= m_currentTime )
				{
					//std::cout << "Sequence::_update - Begin continuous event " << m_continuousIterator->first << "\n";
					m_currentEvents.insert( m_continuousIterator->second );
					++m_continuousIterator;
				}

				ContinuousEventSet::iterator i = m_currentEvents.begin();
				const ContinuousEventSet::iterator & iend = m_currentEvents.end();

				for ( ; i != iend; )
				{
					( *i )->Apply( p_time );

					if ( ( *i )->IsFinished() )
					{
						//std::cout << "Sequence::_update - Event finished\n";
						General::Utils::set::eraseIterator( m_currentEvents, i );
					}
					else
					{
						++i;
					}
				}
			}

			GUI::Properties::ObjectProperty * TrollSequence::DoCreateProperties()
			{
				return new GUI::Properties::SequenceProperties( *this );
			}
		}
	}
}
