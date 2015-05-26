//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/ObjectProperties/PonctualEventProperties.h"
#include "GUI/TimeSequence/PonctualEventPanel.h"
/*
#include <General/STLMacros.h>
#include <Sequences/Sequence.h>
#include <Sequences/BaseEvent.h>
#include <Main/Context.h>
#include <wx/txtstrm.h>
*/
//***********************************************************************************************************
using namespace Troll::Temporal;
using namespace Troll::GUI;
//***********************************************************************************************************

TrollPonctualEvent :: TrollPonctualEvent( TrollSequence * p_sequence, BasePonctualEvent * p_event,
		const wxString & p_targetTypeName, const wxString & p_targetName,
		float p_fireTime, const wxString & p_eventAction,
		const wxString & p_name, const wxString & p_fileName )
	:	TemporalObject( p_name, p_fileName, ttPonctualEvent ),
		m_museEvent( p_event ),
		m_targetTypeName( p_targetTypeName ),
		m_targetName( p_targetName ),
		m_fireTime( p_fireTime ),
		m_actionArguments( p_eventAction ),
		m_panel( NULL ),
		m_sequence( p_sequence )
{
}

//***********************************************************************************************************

TrollPonctualEvent :: ~TrollPonctualEvent()
{
}

//***********************************************************************************************************

int TrollPonctualEvent :: BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 80 + ( m_targetTypeName == wxT( "scene_node" ) ? 60 : 0 );

	if ( m_propertiesPanel )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new PonctualEventProperties( this, p_parent, wxPoint( 0, 0 ), wxSize( p_width, l_height ) );
	return l_height;
}

//***********************************************************************************************************

void TrollPonctualEvent :: SetFireTime( float p_time )
{
	m_fireTime = p_time;
	m_panel->Replace();
	m_sequence->ChangePonctualEventTime( this );
}

//***********************************************************************************************************

void TrollPonctualEvent :: SetActionName( const wxString & p_name )
{
	if ( m_name != p_name )
	{
		m_name = p_name;
		m_sequence->UpdatePonctualEvent( this );
	}
}

//***********************************************************************************************************

void TrollPonctualEvent :: SetTarget( const wxString & p_type, const wxString & p_target )
{
	std::cout << "TrollPonctualEvent :: SetTarget - " << p_type << " - " << p_target << "\n";
	m_targetTypeName = p_type;
	m_targetName = p_target;

	if ( m_museEvent == NULL )
	{
		return;
	}

	if ( p_type == wxT( "scene_node" ) )
	{
		if ( ScriptEngine::GetContext()->sceneManager->hasSceneNode( p_target.char_str().data() ) )
		{
			m_museEvent->SetTarget( ScriptEngine::GetContext()->sceneManager->getSceneNode( p_target.char_str().data() ) );
		}
		else
		{
			m_museEvent->SetTarget( NULL );
		}
	}
	//TODO : we need to talk
	/*
	else if (p_type == "function")
	{
		m_museEvent->SetTarget( ScriptEngine::GetContext()->scriptEngine->GetFunction( p_target.char_str().data()));
	}
	*/
	else if ( p_type == wxT( "overlay" ) )
	{
		m_museEvent->SetTarget( ScriptEngine::GetContext()->gui->GetOverlayGroup( p_target.char_str().data() ) );
	}
	else if ( p_type == wxT( "overlayElement" ) )
	{
		m_museEvent->SetTarget( OverlayManager::getSingletonPtr()->getOverlayElement( p_target.char_str().data() ) );
	}
	else if ( p_type == wxT( "sequence" ) )
	{
		m_museEvent->SetTarget( ScriptEngine::GetContext()->sequenceManager->GetElementByName( p_target.char_str().data() ) );
	}
}

//***********************************************************************************************************

void TrollPonctualEvent :: SetActionArgs( const wxString & p_args )
{
	m_actionArguments = p_args;
}

//***********************************************************************************************************

void TrollPonctualEvent :: Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "\ttarget " ) + m_targetTypeName + wxT( " " ) + m_targetName + wxT( "\n" ) );
	wxString l_tmp;
	l_tmp << m_fireTime;
	p_stream->WriteString( wxT( "\tevent " ) + l_tmp + wxT( " " ) + m_name + ( m_actionArguments.empty() ? wxT( "" ) : wxT( " " ) + m_actionArguments ) + wxT( "\n" ) );
}

//***********************************************************************************************************
