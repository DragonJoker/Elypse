/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "TrollEditorPch.h"

#include "TimeLinePanel.h"

#include "TimePanel.h"
#include "LinePanel.h"
#include "TimeLineContainer.h"
#include "SequencePanel.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	BEGIN_EVENT_TABLE( TimeLinePanel, wxPanel )
	END_EVENT_TABLE()

	TimeLinePanel::TimeLinePanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position, const wxSize & p_size )
		: wxPanel( p_parent, p_id, p_position, p_size )
		, m_totalWidth( p_size.x )
		, m_totalHeight( p_size.y )
		, m_currentTop( 0 )
		, m_currentLeft( 0 )
		, m_displayedSequences( 0 )
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

	TimeLinePanel::~TimeLinePanel()
	{
	}

	void TimeLinePanel::UpdateSequence( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence )
	{
		auto && l_it = m_alreadyAddedSequences.find( p_sequence->Object::GetObjectName() );

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

			GUI::MainFrame::GetInstance()->UpdateSequence( m_totalWidth, m_totalHeight );
		}
	}

	void TimeLinePanel::UpdateSequences( float p_time )
	{
		auto && l_it = m_alreadyAddedSequences.begin();

		for ( ; l_it != m_alreadyAddedSequences.end() ; ++l_it )
		{
			l_it->second->Update( p_time );
		}
	}

	LinePanel * TimeLinePanel::GetSequencePanel( const wxString & p_name )
	{
		auto && l_it = m_alreadyAddedSequences.find( p_name );

		if ( l_it != m_alreadyAddedSequences.end() )
		{
			return l_it->second->GetLinePanel();
		}

		return NULL;
	}

	TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * TimeLinePanel::AddSequence( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence )
	{
		if ( m_displayedSequences >= 20 )
		{
			return NULL;
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
END_TROLL_GUI_TIME_NAMESPACE
