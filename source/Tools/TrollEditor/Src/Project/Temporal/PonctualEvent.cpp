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

#include "PonctualEvent.h"

#include "Sequence.h"
#include "GUI/ObjectProperties/PonctualEventProperties.h"
#include "GUI/TimeSequence/PonctualEventPanel.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	TrollPonctualEvent::TrollPonctualEvent( TrollSequence * p_sequence, BasePonctualEvent * p_event, const wxString & p_targetTypeName, const wxString & p_targetName, float p_fireTime, const wxString & p_eventAction, const wxString & p_name, const wxString & p_fileName )
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

	int TrollPonctualEvent::BuildPanel( wxWindow * p_parent, int p_width )
	{
		int l_height = 80 + ( m_targetTypeName == wxT( "scene_node" ) ? 60 : 0 );

		if ( m_propertiesPanel )
		{
			m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
			delete m_propertiesPanel;
		}

		m_propertiesPanel = new TROLL_GUI_PROPERTIES_NAMESPACE::PonctualEventProperties( this, p_parent, wxPoint( 0, 0 ), wxSize( p_width, l_height ) );
		return l_height;
	}

	void TrollPonctualEvent::SetFireTime( float p_time )
	{
		m_fireTime = p_time;
		m_panel->Replace();
		m_sequence->ChangePonctualEventTime( this );
	}

	void TrollPonctualEvent::SetActionName( const wxString & p_name )
	{
		if ( m_name != p_name )
		{
			m_name = p_name;
			m_sequence->UpdatePonctualEvent( this );
		}
	}

	void TrollPonctualEvent::SetTarget( const wxString & p_type, const wxString & p_target )
	{
		std::cout << "TrollPonctualEvent::SetTarget - " << p_type << " - " << p_target << "\n";
		m_targetTypeName = p_type;
		m_targetName = p_target;

		if ( m_museEvent == NULL )
		{
			return;
		}

		if ( p_type == wxT( "scene_node" ) )
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

		else if ( p_type == wxT( "overlay" ) )
		{
			m_museEvent->SetTarget( ScriptEngine::GetContext()->gui->GetOverlayGroup( make_string( p_target ) ) );
		}
		else if ( p_type == wxT( "overlayElement" ) )
		{
			m_museEvent->SetTarget( OverlayManager::getSingletonPtr()->getOverlayElement( make_string( p_target ) ) );
		}
		else if ( p_type == wxT( "sequence" ) )
		{
			m_museEvent->SetTarget( ScriptEngine::GetContext()->sequenceManager->GetElementByName( make_string( p_target ) ) );
		}
	}

	void TrollPonctualEvent::SetActionArgs( const wxString & p_args )
	{
		m_actionArguments = p_args;
	}

	void TrollPonctualEvent::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "\ttarget " ) + m_targetTypeName + wxT( " " ) + m_targetName + wxT( "\n" ) );
		wxString l_tmp;
		l_tmp << m_fireTime;
		p_stream->WriteString( wxT( "\tevent " ) + l_tmp + wxT( " " ) + m_name + ( m_actionArguments.empty() ? wxT( "" ) : wxT( " " ) + m_actionArguments ) + wxT( "\n" ) );
	}
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE
