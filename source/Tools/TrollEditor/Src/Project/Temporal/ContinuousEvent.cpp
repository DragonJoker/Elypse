#include "PrecompiledHeader.h"

#include "ContinuousEvent.h"

#include "GUI/ObjectProperties/ContinuousEventProperties.h"
#include "GUI/ObjectProperties/KeyFrameProperties.h"
#include "GUI/TimeSequence/ContinuousEventPanel.h"

using namespace Troll::Temporal;
using namespace Troll::GUI;


int TrollKeyFrame::BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 80;

	if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
		m_propertiesPanel = NULL;
	}

	m_propertiesPanel = new KeyFrameProperties( this, p_parent, wxPoint( 0, 0 ), wxSize( p_width, l_height ) );
	return l_height;
}



void TrollKeyFrame::SetFireTime( float p_time, bool p_updateEvent )
{
	m_fireTime = p_time;

	if ( p_updateEvent )
	{
		m_event->UpdateKeyFrame( this );
	}
}



void TrollKeyFrame::Write( wxTextOutputStream * p_stream )
{
	wxString l_tmp;
	l_tmp << m_fireTime;
	p_stream->WriteString( wxT( "\t\tkey " ) + l_tmp + wxT( " " ) + m_action + wxT( "\n" ) );
}



TrollContinuousEvent::TrollContinuousEvent( TrollSequence * p_sequence, BaseContinuousEvent * p_event,
		const wxString & p_name, const wxString & p_fileName )
	:	TemporalObject( p_name, p_fileName, ttContinuousEvent ),
		m_museEvent( p_event ),
		m_duration( 0.0 ),
		m_panel( NULL ),
		m_sequence( p_sequence )
{
}



TrollContinuousEvent::~TrollContinuousEvent()
{
	General::Utils::map::deleteAll( m_keyFrames );
}



TrollKeyFrame * TrollContinuousEvent::AddKeyFrame( float p_time, const wxString & p_action )
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

	for ( ; l_it != m_keyFrames.end() ; ++l_it )
	{
		l_tmp << l_it->first;
		p_list.push_back( l_tmp );
		l_tmp.clear();
	}
}



int TrollContinuousEvent::BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 200;

	if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new ContinuousEventProperties( *this, p_parent, wxPoint( 0, 0 ), wxSize( p_width, l_height ) );
	return l_height;
}



void TrollContinuousEvent::UpdateKeyFrame( TrollKeyFrame * p_keyFrame )
{
	bool l_found = false;
	TrollKeyFrameMap::iterator l_it = m_keyFrames.begin();
	wxString l_tmp;

	while ( l_it != m_keyFrames.end() && ! l_found )
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
//			std::cout << "TrollContinuousEvent::UpdateTime - Before : " << l_it->first << " - New : " << l_startTime << " - Keyframe : " << l_it->second << "\n";
			l_it->second->SetFireTime( l_startTime );
			l_newMap.insert( TrollKeyFrameMap::value_type( l_startTime, l_it->second ) );
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



void TrollContinuousEvent::Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "\ttrack " ) + m_name + wxT( "\n\t{\n" ) );
	p_stream->WriteString( wxT( "\t\ttarget " ) + m_targetTypeName + wxT( " " ) + m_targetName + wxT( "\n" ) );

	if ( ! m_interpolationType.empty() )
	{
		p_stream->WriteString( wxT( "\t\tinterpolation " ) + m_interpolationType + wxT( "\n" ) );
	}

	TrollKeyFrameMap::iterator l_it = m_keyFrames.begin();

	while ( l_it != m_keyFrames.end() )
	{
		l_it->second->Write( p_stream );
		++l_it;
	}

	p_stream->WriteString( wxT( "\t}\n\n" ) );
}


