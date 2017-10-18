#include "PonctualEvent.h"

#include "Sequence.h"
#include "GUI/Properties/Project/PonctualEventProperties.h"
#include "GUI/TimeSequence/PonctualEventPanel.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TrollPonctualEvent::TrollPonctualEvent( TrollSequence * p_sequence, BasePonctualEvent * p_event, wxString const & p_targetTypeName, wxString const & p_targetName, float p_fireTime, wxString const & p_eventAction, wxString const & p_name, wxString const & p_fileName )
				: TemporalObject( p_name, p_fileName, ttPonctualEvent )
				, m_museEvent( p_event )
				, m_targetTypeName( p_targetTypeName )
				, m_targetName( p_targetName )
				, m_fireTime( p_fireTime )
				, m_actionArguments( p_eventAction )
				, m_panel( NULL )
				, m_sequence( p_sequence )
			{
			}

			TrollPonctualEvent::~TrollPonctualEvent()
			{
			}

			void TrollPonctualEvent::SetFireTime( float p_time )
			{
				m_fireTime = p_time;
				m_panel->Replace();
				m_sequence->ChangePonctualEventTime( this );
			}

			void TrollPonctualEvent::SetActionName( wxString const & p_name )
			{
				if ( m_name != p_name )
				{
					m_name = p_name;
					m_sequence->UpdatePonctualEvent( this );
				}
			}

			void TrollPonctualEvent::SetTargetType( wxString const & p_type )
			{
				std::cout << "TrollPonctualEvent::SetTargetType - " << p_type << "\n";
				m_targetTypeName = p_type;
			}

			void TrollPonctualEvent::SetTarget( wxString const & p_target )
			{
				std::cout << "TrollPonctualEvent::SetTarget - " << p_target << "\n";
				m_targetName = p_target;

				if ( m_museEvent == NULL )
				{
					return;
				}

				if ( m_targetTypeName == wxT( "scene_node" ) )
				{
					if ( ScriptEngine::GetContext()->sceneManager->hasSceneNode( make_string( p_target ) ) )
					{
						m_museEvent->SetTarget( ScriptEngine::GetContext()->sceneManager->getSceneNode( make_string( p_target ) ) );
					}
					else
					{
						m_museEvent->SetTarget( NULL );
					}
				}
				//TODO : we need to talk
				//else if (p_type == "function")
				//{
				//	m_museEvent->SetTarget( ScriptEngine::GetContext()->scriptEngine->GetFunction( make_string( p_target )));
				//}

				else if ( m_targetTypeName == wxT( "overlay" ) )
				{
					m_museEvent->SetTarget( ScriptEngine::GetContext()->gui->GetOverlayGroup( make_string( p_target ) ) );
				}
				else if ( m_targetTypeName == wxT( "overlayElement" ) )
				{
					m_museEvent->SetTarget( OverlayManager::getSingletonPtr()->getOverlayElement( make_string( p_target ) ) );
				}
				else if ( m_targetTypeName == wxT( "sequence" ) )
				{
					m_museEvent->SetTarget( ScriptEngine::GetContext()->sequenceManager->GetElementByName( make_string( p_target ) ).get() );
				}
			}

			void TrollPonctualEvent::SetActionArgs( wxString const & p_args )
			{
				m_actionArguments = p_args;
			}

			void TrollPonctualEvent::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "\ttarget " ) + m_targetTypeName + wxT( " " ) + m_targetName + wxT( "\n" ) );
				wxString l_tmp;
				l_tmp << m_fireTime;
				p_stream.WriteString( wxT( "\tevent " ) + l_tmp + wxT( " " ) + m_name + ( m_actionArguments.empty() ? wxT( "" ) : wxT( " " ) + m_actionArguments ) + wxT( "\n" ) );
			}

			GUI::Properties::ObjectProperty * TrollPonctualEvent::DoCreateProperties()
			{
				return new GUI::Properties::PonctualEventProperties( *this );
			}
		}
	}
}
