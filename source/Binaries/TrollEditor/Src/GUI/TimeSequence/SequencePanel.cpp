
#include "PrecompiledHeader.h"

#include "GUI/TimeSequence/SequencePanel.h"
#include "GUI/TimeSequence/PonctualEventPanel.h"
#include "GUI/TimeSequence/ContinuousEventPanel.h"
#include "Project/Temporal/Sequence.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/ContinuousEvent.h"
#include "GUI/TimeSequence/LinePanel.h"
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

enum SequencePanelIDs
{
	addPonctualEvent,
	addContinuousEvent
};

BEGIN_EVENT_TABLE( SequencePanel, wxPanel )
	EVT_LEFT_UP(	SequencePanel::_onLeftMouseUp )
	EVT_RIGHT_UP(	SequencePanel::_onRightMouseUp )
	EVT_MENU( addPonctualEvent,		SequencePanel::_onAddPonctualEvent )
	EVT_MENU( addContinuousEvent,	SequencePanel::_onAddContinuousEvent )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


SequencePanel :: SequencePanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position,
								const wxSize & p_size )
	:	wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE ),
		m_currentLeft( 20 ),
		m_maxTop( 30 )
{
	Hide();
	m_sequencePanel = new LinePanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 10, 20 ) );
	m_sequencePanel->Hide();

	for ( unsigned int i = 0 ; i < 40 ; i++ )
	{
		ContinuousEventPanel * l_cpanel = new ContinuousEventPanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 10, 20 ) );
		l_cpanel->Hide();
		m_continuousEvents.push_back( l_cpanel );
		PonctualEventPanel * l_ppanel = new PonctualEventPanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 10, 20 ) );
		l_ppanel->Hide();
		m_ponctualEvents.push_back( l_ppanel );
	}

	SetBackgroundColour( wxColour( 255, 255, 255 ) );
}



SequencePanel :: ~SequencePanel()
{
}



TrollSequence * SequencePanel :: SetSequence( TrollSequence * p_sequence )
{
	if ( p_sequence != NULL )
	{
		m_sequence = p_sequence;
		wxSize l_size;
		l_size.x = int( p_sequence->GetTotalLength() * 100 );
		m_sequencePanel->SetSequence( p_sequence );
		m_sequencePanel->SetSize( l_size.x, 20 );
		m_sequencePanel->Show();
		TrollPonctualEventMap l_ponctualEvents = p_sequence->GetPonctualEvents();
		TrollPonctualEventMap::iterator l_ponctualIt = l_ponctualEvents.begin();

		while ( l_ponctualIt != l_ponctualEvents.end() )
		{
			_addPonctualEvent( l_ponctualIt->second, l_ponctualIt->first );
			++l_ponctualIt;
		}

		TrollContinuousEventMap l_continuousEvents = p_sequence->GetContinuousEvents();
		TrollContinuousEventMap::iterator l_continuousIt = l_continuousEvents.begin();

		while ( l_continuousIt != l_continuousEvents.end() )
		{
			_addContinuousEvent( l_continuousIt->second );
			++l_continuousIt;
		}

		l_size.x += 20;
		l_size.y = m_maxTop + 20;
		SetSize( l_size );
		Show();
	}

	return p_sequence;
}



void SequencePanel :: AddPonctualEvent( TrollPonctualEvent * p_event, Real p_time )
{
	if ( m_ponctualEvents.size() == 0 )
	{
		return;
	}

	PonctualEventPanel * l_eventPanel = m_ponctualEvents[0];
	m_ponctualEvents.erase( m_ponctualEvents.begin() );
	l_eventPanel->SetPonctualEvent( p_event, p_time );
	int l_ancientMax = m_maxTop;
	int l_top = _getTopForPonctualEvent( p_event );

	if ( m_maxTop > l_ancientMax )
	{
		SetSize( GetSize().x, m_maxTop + 20 );
	}

	l_eventPanel->SetTop( l_top );
}



void SequencePanel :: AddContinuousEvent( TrollContinuousEvent * p_event )
{
	if ( m_continuousEvents.size() == 0 )
	{
		return;
	}

	ContinuousEventPanel * l_eventPanel = m_continuousEvents[0];
	m_continuousEvents.erase( m_continuousEvents.begin() );
	l_eventPanel->SetContinuousEvent( p_event );
	int l_ancientMax = m_maxTop;
	int l_top = _getTopForContinuousEvent( p_event );

	if ( m_maxTop > l_ancientMax )
	{
		SetSize( GetSize().x, m_maxTop + 20 );
	}

	l_eventPanel->SetTop( l_top );
}



void SequencePanel :: _addPonctualEvent( TrollPonctualEvent * p_event, Real p_time )
{
	if ( m_ponctualEvents.size() == 0 )
	{
		return;
	}

	PonctualEventPanel * l_eventPanel = m_ponctualEvents[0];
	m_ponctualEvents.erase( m_ponctualEvents.begin() );
	l_eventPanel->SetPonctualEvent( p_event, p_time );
	l_eventPanel->SetTop( _getTopForPonctualEvent( p_event ) );
}



void SequencePanel :: _addContinuousEvent( TrollContinuousEvent * p_event )
{
	if ( m_continuousEvents.size() == 0 )
	{
		return;
	}

	ContinuousEventPanel * l_eventPanel = m_continuousEvents[0];
	m_continuousEvents.erase( m_continuousEvents.begin() );
	l_eventPanel->SetContinuousEvent( p_event );
	l_eventPanel->SetTop( _getTopForContinuousEvent( p_event ) );
}



void SequencePanel :: RemovePonctualEvent( Temporal::TrollPonctualEvent * p_event )
{
	PonctualEventPanel * l_eventPanel = p_event->GetEventPanel();
	l_eventPanel->SetPonctualEvent( NULL, 0.0 );
	m_ponctualEvents.push_back( l_eventPanel );
}



void SequencePanel :: RemoveContinuousEvent( Temporal::TrollContinuousEvent * p_event )
{
	ContinuousEventPanel * l_eventPanel = p_event->GetEventPanel();
	l_eventPanel->SetContinuousEvent( NULL );
	m_continuousEvents.push_back( l_eventPanel );
}



int SequencePanel :: _getTopForContinuousEvent( Temporal::TrollContinuousEvent * p_event )
{
	return _getTop( int( p_event->GetStartTime() * 100 ), int( p_event->GetLength() * 100 ) );
}



int SequencePanel :: _getTopForPonctualEvent( Temporal::TrollPonctualEvent * p_event )
{
	return _getTop( int( p_event->GetFireTime() * 100 ), 20 );
}



int SequencePanel :: _getTop( int p_left, int p_width )
{
	if ( m_addedPanelsDimensions.size() == 0 )
	{
		IntIMultiMap l_map;
		l_map.insert( IntIMultiMap::value_type( p_left, p_width ) );
		m_addedPanelsDimensions.insert( DimensionMap::value_type( 30, l_map ) );
		return 30;
	}

	DimensionMap::iterator l_it = m_addedPanelsDimensions.begin();
	IntIMultiMap::iterator l_it2;
	bool l_found = false;
	int l_left, l_dim;

	while ( l_it != m_addedPanelsDimensions.end() )
	{
		l_it2 = l_it->second.begin();

		while ( l_it2 != l_it->second.end() && ! l_found )
		{
			l_left = l_it2->first;
			l_dim = l_left + l_it2->second;

			if ( ( l_left <= p_left && l_dim > p_left )
					|| ( l_left < p_left + p_width && l_dim >= p_left + p_width )
					|| ( l_left >= p_left && l_dim <= p_left + p_width ) )
			{
				l_found = true;
			}

			++l_it2;
		}

		if ( l_found )
		{
			l_found = false;
			++l_it;
		}
		else
		{
			l_it->second.insert( IntIMultiMap::value_type( p_left, p_width ) );
			return l_it->first;
		}
	}

	if ( l_it == m_addedPanelsDimensions.end() )
	{
		IntIMultiMap l_map;
		l_map.insert( IntIMultiMap::value_type( p_left, p_width ) );
		m_maxTop += 30;
		m_addedPanelsDimensions.insert( DimensionMap::value_type( m_maxTop, l_map ) );
		return m_maxTop;
	}
	else
	{
		return l_it->first;
	}
}



void SequencePanel :: _showContextMenu( const wxPoint & p_pos )
{
	wxMenu l_menu;
	l_menu.Append( addPonctualEvent, _T( "Ajouter Evènement Ponctuel" ) );
	l_menu.Append( addContinuousEvent, _T( "Ajouter Evènement Continu" ) );
	m_currentLeft = p_pos.x;
	int l_modulo = m_currentLeft % 10;

	if ( l_modulo >= 5 )
	{
		m_currentLeft += 10 - l_modulo;
	}
	else
	{
		m_currentLeft -= l_modulo;
	}

	PopupMenu( & l_menu, p_pos.x, p_pos.y );
}



void SequencePanel :: _onAddPonctualEvent( wxCommandEvent & p_event )
{
	g_mainFrame->AddPonctualEvent( m_sequencePanel->GetSequence(), m_currentLeft / 100.0f );
}



void SequencePanel :: _onAddContinuousEvent( wxCommandEvent & p_event )
{
	std::cout << "SequencePanel :: _onAddContinuousEvent - " << m_currentLeft << " - " << m_currentLeft / 100.0 << "\n";
	g_mainFrame->AddContinuousEvent( m_sequencePanel->GetSequence(), m_currentLeft / 100.0f );
}



void SequencePanel :: _onLeftMouseUp( wxMouseEvent & p_event )
{
}



void SequencePanel :: _onRightMouseUp( wxMouseEvent & p_event )
{
	_showContextMenu( p_event.GetPosition() );
}


