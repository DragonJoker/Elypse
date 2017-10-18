#include "ContinuousEvent.h"

#include "GUI/Properties/Project/ContinuousEventProperties.h"
#include "GUI/Properties/Project/KeyFrameProperties.h"
#include "GUI/TimeSequence/ContinuousEventPanel.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

using namespace Troll::GUI;

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			//**********************************************************************************************

			void TrollKeyFrame::SetFireTime( float p_time, bool p_updateEvent )
			{
				m_fireTime = p_time;

				if ( p_updateEvent )
				{
					m_event->UpdateKeyFrame( this );
				}
			}

			void TrollKeyFrame::Write( wxTextOutputStream & p_stream )
			{
				wxString l_tmp;
				l_tmp << m_fireTime;
				p_stream << wxT( "\t\tkey " ) << l_tmp << wxT( " " ) << m_action << wxT( "\n" );
			}

			GUI::Properties::ObjectProperty * TrollKeyFrame::DoCreateProperties()
			{
				return new GUI::Properties::KeyFrameProperties( *this );
			}

			//**********************************************************************************************

			TrollContinuousEvent::TrollContinuousEvent( TrollSequence * p_sequence, BaseContinuousEvent * p_event, wxString const & p_name, wxString const & p_fileName )
				: TemporalObject( p_name, p_fileName, ttContinuousEvent )
				, m_museEvent( p_event )
				, m_duration( 0.0 )
				, m_panel( NULL )
				, m_sequence( p_sequence )
			{
			}

			TrollContinuousEvent::~TrollContinuousEvent()
			{
				General::Utils::map::deleteAll( m_keyFrames );
			}

			TrollKeyFrame * TrollContinuousEvent::AddKeyFrame( float p_time, wxString const & p_action )
			{
				if ( m_keyFrames.size() == 0 )
				{
					m_startTime = p_time;
					m_endTime = p_time;
				}

				if ( m_keyFrames.find( p_time ) == m_keyFrames.end() )
				{
					ContinuousEventArgs l_argsType;

					if ( m_name.find( wxT( "Translate" ) ) != wxString::npos || m_name.find( wxT( "Scale" ) ) != wxString::npos || m_name.find( wxT( "Resize" ) ) != wxString::npos )
					{
						if ( m_name.find( wxT( "Overlay" ) ) != wxString::npos )
						{
							l_argsType = ceVector2;
						}
						else
						{
							l_argsType = ceVector3;
						}
					}
					else if ( m_name == wxT( "Overlay_Rotate" ) )
					{
						l_argsType = ceReal;
					}
					else if ( m_name.find( wxT( "Rotate" ) ) != wxString::npos )
					{
						l_argsType = ceQuaternion;
					}

					TrollKeyFrame * l_keyFrame = new TrollKeyFrame( this, p_time, p_action, m_fileName, l_argsType );
					m_keyFrames[p_time] = l_keyFrame;
					m_startTime = min <float>( p_time, m_startTime );
					m_endTime = max <float>( p_time, m_endTime );
					m_duration = m_endTime - m_startTime;
					return l_keyFrame;
				}

				return m_keyFrames.find( p_time )->second;
			}

			void TrollContinuousEvent::GetKeyFrames( wxArrayString & p_list )
			{
				TrollKeyFrameMap::iterator l_it = m_keyFrames.begin();
				wxString l_tmp;

				for ( ; l_it != m_keyFrames.end(); ++l_it )
				{
					l_tmp << l_it->first;
					p_list.push_back( l_tmp );
					l_tmp.clear();
				}
			}

			void TrollContinuousEvent::UpdateKeyFrame( TrollKeyFrame * p_keyFrame )
			{
				bool l_found = false;
				TrollKeyFrameMap::iterator l_it = m_keyFrames.begin();
				wxString l_tmp;

				while ( l_it != m_keyFrames.end() && !l_found )
				{
					if ( l_it->second == p_keyFrame )
					{
						l_found = true;
						m_keyFrames.erase( l_it );
						m_keyFrames[p_keyFrame->GetFireTime()] = p_keyFrame;
						_computeTimes();
					}
					else
					{
						++l_it;
					}
				}

				if ( m_panel != NULL )
				{
					m_panel->Replace();
				}
			}

			void TrollContinuousEvent::UpdateTime( float p_newStartTime )
			{
				float l_diff = p_newStartTime - m_startTime;

				if ( l_diff != 0.0 )
				{
					m_startTime = p_newStartTime;
					TrollKeyFrameMap::iterator l_it = m_keyFrames.begin();
					TrollKeyFrameMap l_newMap;
					float l_startTime;

					while ( l_it != m_keyFrames.end() )
					{
						l_startTime = l_it->first + l_diff;
						//std::cout << "TrollContinuousEvent::UpdateTime - Before : " << l_it->first << " - New : " << l_startTime << " - Keyframe : " << l_it->second << "\n";
						l_it->second->SetFireTime( l_startTime );
						l_newMap.insert( std::make_pair( l_startTime, l_it->second ) );
						++l_it;
					}

					m_keyFrames.clear();
					m_keyFrames = l_newMap;
					_computeTimes();
					m_museEvent->UpdateTime( p_newStartTime );
				}
			}

			void TrollContinuousEvent::_computeTimes()
			{
				m_duration = 0.0;

				if ( m_keyFrames.size() == 0 )
				{
					m_startTime = 0.0;
					m_endTime = 0.0;
					return;
				}

				TrollKeyFrameMap::iterator l_it = m_keyFrames.begin();
				m_startTime = l_it->first;
				m_endTime = l_it->first;
				++l_it;

				while ( l_it != m_keyFrames.end() )
				{
					m_startTime = min <float>( l_it->first, m_startTime );
					m_endTime = max <float>( l_it->first, m_endTime );
					++l_it;
				}

				m_duration = m_endTime - m_startTime;
			}

			void TrollContinuousEvent::Write( wxTextOutputStream & p_stream )
			{
				p_stream << wxT( "\ttrack " ) << m_name << wxT( "\n\t{\n" );
				p_stream << wxT( "\t\ttarget " ) << m_targetTypeName << wxT( " " ) << m_targetName << wxT( "\n" );

				if ( !m_interpolationType.empty() )
				{
					p_stream << wxT( "\t\tinterpolation " ) << m_interpolationType << wxT( "\n" );
				}

				for ( auto && l_it : m_keyFrames )
				{
					l_it.second->Write( p_stream );
				}

				p_stream << wxT( "\t}\n\n" );
			}

			GUI::Properties::ObjectProperty * TrollContinuousEvent::DoCreateProperties()
			{
				return new GUI::Properties::ContinuousEventProperties( *this );
			}

			//**********************************************************************************************
		}
	}
}
