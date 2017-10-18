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
#include "PrecompiledHeader.h"

#include "Sequence.h"

#include "Context.h"

Sequence::Sequence( String const & p_name )
	: named( p_name )
	, m_target( NULL )
	, m_totalLength( 0.0 )
	, m_currentTime( 0.0 )
	, m_looped( false )
	, m_status( STOPPED )
{
	m_ponctualIterator = m_ponctualEvents.begin();
	m_continuousIterator = m_continuousEvents.begin();
}

Sequence::~Sequence()
{
	multimap::deleteAll( m_ponctualEvents );
	multimap::deleteAll( m_continuousEvents );
}

void Sequence::Start()
{
	m_ponctualIterator = m_ponctualEvents.begin();
	m_continuousIterator = m_continuousEvents.begin();
	multimap::cycle( m_continuousEvents, &BaseContinuousEvent::Reset );
	m_status = PLAYING;
}

void Sequence::Stop()
{
	m_status = STOPPED;
	m_currentTime = 0.0;
	m_ponctualIterator = m_ponctualEvents.begin();
	m_continuousIterator = m_continuousEvents.begin();
	multimap::cycle( m_continuousEvents, &BaseContinuousEvent::Reset );
}

void Sequence::Pause( bool p_pause )
{
	if ( p_pause )
	{
		m_status = PAUSED;
		return;
	}

	m_status = PLAYING;
}

void Sequence::AddPonctualEvent( BasePonctualEvent * p_event, Real p_when )
{
	m_ponctualEvents.insert( std::make_pair( p_when, p_event ) );
	m_totalLength = max( m_totalLength, p_when );
}

void Sequence::ChangePonctualEventTime( BasePonctualEvent * p_event, Real p_when )
{
	bool l_found = false;
	PonctualEventMap::iterator l_it = m_ponctualEvents.begin();

	while ( l_it != m_ponctualEvents.end() && ! l_found )
	{
		if ( l_it->second == p_event )
		{
			m_ponctualEvents.erase( l_it );
			m_ponctualEvents.insert( std::make_pair( p_when, p_event ) );
			l_found = true;
		}
		else
		{
			++l_it;
		}
	}
}

void Sequence::AddContinuousEvent( BaseContinuousEvent * p_event )
{
	p_event->CalcLength();
	m_continuousEvents.insert( std::make_pair( p_event->GetStartTime(), p_event ) );
	m_totalLength = max( m_totalLength, p_event->GetLength() + p_event->GetStartTime() );
}

void Sequence::ChangeContinuousEventTime( BaseContinuousEvent * p_event, Real p_when )
{
	bool l_found = false;
	ContinuousEventMap::iterator l_it = m_continuousEvents.begin();

	while ( l_it != m_continuousEvents.end() && ! l_found )
	{
		if ( l_it->second == p_event )
		{
			m_continuousEvents.erase( l_it );
			p_event->CalcLength();
			m_continuousEvents.insert( std::make_pair( p_event->GetStartTime(), p_event ) );
			m_totalLength = max( m_totalLength, p_event->GetLength() + p_event->GetStartTime() );
			l_found = true;
		}
		else
		{
			++l_it;
		}
	}
}

void Sequence::RemovePonctualEvent( BasePonctualEvent * p_event )
{
	bool l_found = false;
	PonctualEventMap::iterator l_it = m_ponctualEvents.begin();

	while ( l_it != m_ponctualEvents.end() && ! l_found )
	{
		if ( l_it->second == p_event )
		{
			m_ponctualEvents.erase( l_it );
			delete p_event;
			l_found = true;
		}
		else
		{
			++l_it;
		}
	}
}

void Sequence::RemoveContinuousEvent( BaseContinuousEvent * p_event )
{
	bool l_found = false;
	ContinuousEventMap::iterator l_it = m_continuousEvents.begin();

	while ( l_it != m_continuousEvents.end() && ! l_found )
	{
		if ( l_it->second == p_event )
		{
			m_continuousEvents.erase( l_it );
			delete p_event;
			l_found = true;
		}
		else
		{
			++l_it;
		}
	}
}

void Sequence::Update( Real p_time )
{
	if ( m_status != PLAYING )
	{
		return;
	}

	m_currentTime += p_time;

	while ( m_ponctualIterator != m_ponctualEvents.end() && m_currentTime >= m_ponctualIterator->first )
	{
		m_ponctualIterator->second->Apply();
		++ m_ponctualIterator;
	}

	while ( m_continuousIterator != m_continuousEvents.end() && m_currentTime >= m_continuousIterator->first )
	{
		m_currentEvents.insert( m_continuousIterator->second );
		++ m_continuousIterator;
	}

	ContinuousEventSet::iterator i = m_currentEvents.begin();
	const ContinuousEventSet::iterator & iend = m_currentEvents.end();

	for ( ; i != iend ; )
	{
		( * i )->Apply( p_time );

		if ( ( * i )->IsFinished() )
		{
			General::Utils::set::eraseIterator( m_currentEvents, i );
			continue;
		}

		++ i;
	}

	if ( m_currentTime >= m_totalLength )
	{
		Stop();

		if ( m_looped )
		{
			Start();
		}
	}
}
