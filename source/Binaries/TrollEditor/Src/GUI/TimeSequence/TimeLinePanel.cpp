
#include "PrecompiledHeader.h"

#include "GUI/TimeSequence/TimeLinePanel.h"
#include "GUI/TimeSequence/TimePanel.h"
#include "GUI/TimeSequence/LinePanel.h"
#include "GUI/TimeSequence/TimeLineContainer.h"
#include "GUI/TimeSequence/SequencePanel.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/MainFrame.h"
/*
#include <Sequences/SequenceManager.h>
#include <Sequences/Sequence.h>
#include <Sequences/BaseEvent.h>
#include <Main/Context.h>
*/

using namespace Troll;
using namespace Troll::Temporal;
using namespace Troll::GUI;

extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( TimeLinePanel, wxPanel )
END_EVENT_TABLE()


TimeLinePanel :: TimeLinePanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position,
								const wxSize & p_size )
	:	wxPanel( p_parent, p_id, p_position, p_size ),
		m_totalWidth( p_size.x ),
		m_totalHeight( p_size.y ),
		m_currentTop( 0 ),
		m_currentLeft( 0 ),
		m_displayedSequences( 0 )
{
	SequencePanel * l_sequencePanel;

	for ( unsigned int i = 0 ; i < 20 ; i++ )
	{
		l_sequencePanel = new SequencePanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 200, 20 ) );
		l_sequencePanel->Hide();
		m_sequences.push_back( l_sequencePanel );
	}

	SetBackgroundColour( wxColour( 255, 255, 255 ) );
}



TimeLinePanel :: ~TimeLinePanel()
{
}



void TimeLinePanel :: UpdateSequence( TrollSequence * p_sequence )
{
	TrollSequenceStrMap::iterator l_it = m_alreadyAddedSequences.find( p_sequence->TrollObject::GetName() );

	if ( l_it != m_alreadyAddedSequences.end() )
	{
		wxPoint l_position( int( p_sequence->GetStartTime() * 100 ), p_sequence->GetLinePanel()->GetPosition().y );
		wxSize l_size( int( p_sequence->GetTotalLength() * 100 ), 80 );
		l_it = m_alreadyAddedSequences.begin();
		m_totalWidth = 0;

		for ( ; l_it != m_alreadyAddedSequences.end() ; ++l_it )
		{
			m_totalWidth = max <int>( m_totalWidth, l_it->second->GetLinePanel()->GetPosition().x + l_it->second->GetLinePanel()->GetSize().x );
		}

		float l_first = g_mainFrame->m_timePanel->GetFirst();
		g_mainFrame->m_timeLineContainer->Scroll( 0, 0 );
		SetSize( m_totalWidth + 20, m_totalHeight );
		int l_verticalScroll = g_mainFrame->m_timeLineContainer->GetScrollPos( wxVERTICAL );
//		std::cout << "TimeLinePanel :: UpdateSequence - " << l_verticalScroll << "\n";
		g_mainFrame->m_timeLineContainer->SetScrollbars( 20, 20, ( m_totalWidth + 20 ) / 20, m_totalHeight / 20, 0, 0 );
		g_mainFrame->m_timeLineContainer->Scroll( int( l_first >= 0.1 ? l_first * 5 + 1 : l_first * 5 ), l_verticalScroll );
	}
}



void TimeLinePanel :: UpdateSequences( float p_time )
{
	TrollSequenceStrMap::iterator l_it = m_alreadyAddedSequences.begin();

	for ( ; l_it != m_alreadyAddedSequences.end() ; ++l_it )
	{
		l_it->second->Update( p_time );
	}
}



LinePanel * TimeLinePanel :: GetSequencePanel( const wxString & p_name )
{
	TrollSequenceStrMap::iterator l_it = m_alreadyAddedSequences.find( p_name );

	if ( l_it != m_alreadyAddedSequences.end() )
	{
		return l_it->second->GetLinePanel();
	}

	return NULL;
}



TrollSequence * TimeLinePanel :: AddSequence( TrollSequence * p_sequence )
{
	if ( m_displayedSequences >= 20 )
	{
		return NULL;
	}

	if ( m_alreadyAddedSequences.find( p_sequence->TrollObject::GetName() ) == m_alreadyAddedSequences.end() )
	{
//		std::cout << "TimeLinePanel :: AddSequence - left : " << m_currentLeft << " - top : " << m_currentTop << " - width : " << l_size.x << " - height : " << l_size.y << "\n";
		SequencePanel * l_sequencePanel = m_sequences[m_displayedSequences++];
		l_sequencePanel->SetSequence( p_sequence );
		l_sequencePanel->SetPosition( wxPoint( m_currentLeft, m_currentTop ) );
		wxSize l_size = l_sequencePanel->GetSize();
		m_alreadyAddedSequences.insert( TrollSequenceStrMap::value_type( p_sequence->TrollObject::GetName(), p_sequence ) );
//		m_currentLeft += l_size.x;
		m_currentTop += l_size.y;

		if ( m_currentLeft + l_size.x > m_totalWidth )
		{
			m_totalWidth = m_currentLeft + l_size.x;
		}

		if ( m_currentTop > m_totalHeight )
		{
			m_totalHeight = m_currentTop;
		}

		std::cout << "TimeLinePanel :: AddSequence - Total width : " << m_totalWidth << " - Total Height : " << m_totalHeight << "\n";
		SetSize( m_totalWidth + 20, m_totalHeight );
		return p_sequence;
	}

	return m_alreadyAddedSequences.find( p_sequence->TrollObject::GetName() )->second;
}


