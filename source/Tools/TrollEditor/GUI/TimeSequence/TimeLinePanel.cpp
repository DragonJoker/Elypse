/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TimeLinePanel.h"

#include "TimePanel.h"
#include "LinePanel.h"
#include "TimeLineContainer.h"
#include "SequencePanel.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			BEGIN_EVENT_TABLE( TimeLinePanel, wxPanel )
			END_EVENT_TABLE()

			TimeLinePanel::TimeLinePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxPanel( p_parent, p_id, p_position, p_size )
				, m_totalWidth( p_size.x )
				, m_totalHeight( p_size.y )
				, m_currentTop( 0 )
				, m_currentLeft( 0 )
				, m_displayedSequences( 0 )
				, m_projectFrame{ p_projectFrame }
			{
				SequencePanel * l_sequencePanel;

				for ( uint32_t i = 0; i < 20; i++ )
				{
					l_sequencePanel = new SequencePanel( this, m_projectFrame, wxID_ANY, wxPoint( 0, 0 ), wxSize( 200, 20 ) );
					l_sequencePanel->Hide();
					m_sequences.push_back( l_sequencePanel );
				}

				SetBackgroundColour( wxColour( 255, 255, 255 ) );
			}

			TimeLinePanel::~TimeLinePanel()
			{
			}

			void TimeLinePanel::UpdateSequence( ProjectComponents::Temporal::TrollSequence * p_sequence )
			{
				auto && l_it = m_alreadyAddedSequences.find( p_sequence->Object::GetObjectName() );

				if ( l_it != m_alreadyAddedSequences.end() )
				{
					wxPoint l_position( int( p_sequence->GetStartTime() * 100 ), p_sequence->GetLinePanel()->GetPosition().y );
					wxSize l_size( int( p_sequence->GetTotalLength() * 100 ), 80 );
					l_it = m_alreadyAddedSequences.begin();
					m_totalWidth = 0;

					for ( ; l_it != m_alreadyAddedSequences.end(); ++l_it )
					{
						m_totalWidth = max <int>( m_totalWidth, l_it->second->GetLinePanel()->GetPosition().x + l_it->second->GetLinePanel()->GetSize().x );
					}

					m_projectFrame->UpdateSequence( m_totalWidth, m_totalHeight );
				}
			}

			void TimeLinePanel::UpdateSequences( float p_time )
			{
				auto && l_it = m_alreadyAddedSequences.begin();

				for ( ; l_it != m_alreadyAddedSequences.end(); ++l_it )
				{
					l_it->second->Update( p_time );
				}
			}

			LinePanel * TimeLinePanel::GetSequencePanel( wxString const & p_name )
			{
				auto && l_it = m_alreadyAddedSequences.find( p_name );

				if ( l_it != m_alreadyAddedSequences.end() )
				{
					return l_it->second->GetLinePanel();
				}

				return nullptr;
			}

			ProjectComponents::Temporal::TrollSequence * TimeLinePanel::AddSequence( ProjectComponents::Temporal::TrollSequence * p_sequence )
			{
				if ( m_displayedSequences >= 20 )
				{
					return nullptr;
				}

				if ( m_alreadyAddedSequences.find( p_sequence->GetObjectName() ) == m_alreadyAddedSequences.end() )
				{
					SequencePanel * l_sequencePanel = m_sequences[m_displayedSequences++];
					l_sequencePanel->SetSequence( p_sequence );
					l_sequencePanel->SetPosition( wxPoint( m_currentLeft, m_currentTop ) );
					wxSize l_size = l_sequencePanel->GetSize();
					m_alreadyAddedSequences.insert( std::make_pair( p_sequence->GetObjectName(), p_sequence ) );
					//m_currentLeft += l_size.x;
					m_currentTop += l_size.y;

					if ( m_currentLeft + l_size.x > m_totalWidth )
					{
						m_totalWidth = m_currentLeft + l_size.x;
					}

					if ( m_currentTop > m_totalHeight )
					{
						m_totalHeight = m_currentTop;
					}

					std::cout << "TimeLinePanel::AddSequence - Total width : " << m_totalWidth << " - Total Height : " << m_totalHeight << "\n";
					SetSize( m_totalWidth + 20, m_totalHeight );
					return p_sequence;
				}

				return m_alreadyAddedSequences.find( p_sequence->GetObjectName() )->second;
			}
		}
	}
}
