//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/ObjectProperties/PonctualEventProperties.h"

#include "Project/Temporal/PonctualEvent.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"
/*
#include <Sequences/SequenceManager.h>
#include <Main/Context.h>
#include <OgreOverlayManager.h>
*/
//***********************************************************************************************************
using namespace Troll::GUI;
using namespace Troll::Temporal;
//***********************************************************************************************************
enum EventPropsIDs
{
	eventActionName,
	eventTargetType,
	eventTargetCB,
	eventTargetTxt,
	eventTime,
	eventParamX,
	eventParamY,
	eventParamZ
};
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( PonctualEventProperties, wxPanel )
	EVT_COMBOBOX(	eventActionName,			PonctualEventProperties::_onSelectAction )
	EVT_COMBOBOX(	eventTargetType,			PonctualEventProperties::_onSelectTargetType )
	EVT_COMBOBOX(	eventTargetCB,				PonctualEventProperties::_onEditTarget )
	EVT_TEXT_ENTER(	eventTargetTxt,				PonctualEventProperties::_onEditTarget )
	EVT_TEXT_ENTER(	eventTime,					PonctualEventProperties::_onEditTime )
	EVT_TEXT_ENTER(	eventParamX,				PonctualEventProperties::_onEditXParam )
	EVT_TEXT_ENTER(	eventParamY,				PonctualEventProperties::_onEditYParam )
	EVT_TEXT_ENTER(	eventParamZ,				PonctualEventProperties::_onEditZParam )
END_EVENT_TABLE()
//***********************************************************************************************************

PonctualEventProperties :: PonctualEventProperties( TrollPonctualEvent * p_event, wxWindow * parent,
		const wxPoint & pos, const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_event( p_event ),
		m_targetCB( NULL ),
		m_targetTC( NULL ),
		m_v3ArgX( NULL ),
		m_v3ArgY( NULL ),
		m_v3ArgZ( NULL ),
		m_actionName( NULL ),
		m_targetType( NULL ),
		m_fireTime( NULL )
{
	UpdateProperties();
}

//***********************************************************************************************************

PonctualEventProperties :: ~PonctualEventProperties()
{
}

//***********************************************************************************************************

void PonctualEventProperties :: UpdateProperties()
{
	m_updatingProperties = true;

	if ( ! DestroyChildren() )
	{
		std::cout << "PonctualEventProperties :: UpdateProperties - Can't destroy children\n";
		m_updatingProperties = false;
		return;
	}

	wxSize l_size = GetClientSize();
	wxPoint l_position = wxPoint( l_size.x / 2, 0 );
	StringArray l_tmp = ScriptEngine::GetContext()->sequenceManager->GetAvailablePonctualEvents();
	wxArrayString l_actions;

	for ( size_t i = 0 ; i < l_tmp.size() ; i++ )
	{
		l_actions.push_back( make_wxString( l_tmp[i] ) );
	}

	m_actionName = BuildComboBox( this, l_position, eventActionName, wxT( "Action" ), l_actions, m_event->GetName() );
	wxArrayString l_targetTypes = _getTargetTypes( m_event->GetName() );
	m_targetType = BuildComboBox( this, l_position += wxPoint( 0, 20 ), eventTargetType, wxT( "Target Type" ), l_targetTypes, m_event->GetTargetType() );
	_buildTargetChoice( l_position += wxPoint( 0, 20 ), m_event->GetTargetType() );
	m_fireTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), eventTime, wxT( "Time" ), m_event->GetFireTime() );
	_buildActionArguments( l_position, m_event->GetName() );
	m_updatingProperties = false;
}

//***********************************************************************************************************

wxArrayString PonctualEventProperties :: _getTargetTypes( const wxString & p_action )
{
	wxArrayString l_result;

	if ( p_action.find( wxT( "TrollSceneNode" ) ) != wxString::npos )
	{
		l_result.push_back( wxT( "scene_node" ) );
	}
	else if ( p_action.find( wxT( "Function" ) ) != wxString::npos )
	{
		l_result.push_back( wxT( "function" ) );
	}
	else if ( p_action.find( wxT( "Overlay_Hide" ) ) != wxString::npos || p_action.find( wxT( "Overlay_Show" ) ) != wxString::npos )
	{
		l_result.push_back( wxT( "overlay" ) );
	}
	else if ( p_action.find( wxT( "Overlay" ) ) != wxString::npos )
	{
		l_result.push_back( wxT( "overlayElement" ) );
	}
	else if ( p_action.find( wxT( "Sequence" ) ) != wxString::npos )
	{
		l_result.push_back( wxT( "sequence" ) );
	}

	return l_result;
}

//***********************************************************************************************************

wxString PonctualEventProperties :: _buildTargetChoice( const wxPoint & p_position, const wxString & p_targetType )
{
	if ( m_targetCB != NULL )
	{
		RemoveChild( m_targetCB );
		delete m_targetCB;
		m_targetCB = NULL;
	}

	if ( m_targetTC != NULL )
	{
		RemoveChild( m_targetTC );
		delete m_targetTC;
		m_targetTC = NULL;
	}

	wxString l_action = m_event->GetName();

	if ( p_targetType == wxT( "function" ) )
	{
		m_targetTC = BuildTextCtrl( this, p_position, eventTargetTxt, wxT( "Target" ), m_event->GetTarget() );
		return wxEmptyString;
	}
	else
	{
		wxArrayString l_targets;
		wxString l_target;

		if ( p_targetType == wxT( "scene_node" ) )
		{
			g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_targets );
		}
		else if ( p_targetType == wxT( "overlay" ) )
		{
			Ogre::OverlayManager::OverlayMapIterator l_it = OverlayManager::getSingletonPtr()->getOverlayIterator();
			wxString l_name;

			while ( l_it.hasMoreElements() )
			{
				l_name = make_wxString( l_it.getNext()->getName() );

				if ( l_name != wxT( "Core/DebugOverlay" ) && l_name != wxT( "Core/LensFlare" ) && l_name != wxT( "Core/LoadOverlay" ) )
				{
					l_targets.Add( l_name );
				}
			}
		}
		else if ( p_targetType == wxT( "overlayElement" ) )
		{
			g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlaysNames( l_targets );
		}
		else if ( p_targetType == wxT( "sequence" ) )
		{
			g_mainFrame->GetCurrentProject()->GetMainScene()->GetSequencesNames( l_targets );
		}

		l_target = m_event->GetTarget();
		m_targetCB = BuildComboBox( this, p_position, eventTargetCB, wxT( "Target" ), l_targets, l_target );
		return l_targets[0];
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _buildActionArguments( wxPoint & p_position, const wxString & p_action )
{
	if ( m_v3ArgX != NULL )
	{
		RemoveChild( m_v3ArgX );
		delete m_v3ArgX;
		m_v3ArgX = NULL;
	}

	if ( m_v3ArgY != NULL )
	{
		RemoveChild( m_v3ArgY );
		delete m_v3ArgY;
		m_v3ArgY = NULL;
	}

	if ( m_v3ArgZ != NULL )
	{
		RemoveChild( m_v3ArgZ );
		delete m_v3ArgZ;
		m_v3ArgZ = NULL;
	}

	if ( p_action.find( wxT( "TrollSceneNode" ) ) != wxString::npos )
	{
		Ogre::StringVector l_params = StringUtil::split( p_action.char_str().data(), ", \t" );

		if ( l_params.size() > 2 )
		{
			Vector3 l_vector = Vector3( StringConverter::parseReal( l_params[0] ),
										StringConverter::parseReal( l_params[1] ),
										StringConverter::parseReal( l_params[2] ) );
			m_v3ArgX = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), eventParamX, wxT( "Target X" ), l_vector.x );
			m_v3ArgY = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), eventParamY, wxT( "Target Y" ), l_vector.y );
			m_v3ArgZ = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), eventParamZ, wxT( "Target Z" ), l_vector.z );
		}
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onSelectAction( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;
		m_event->SetActionName( m_actionName->GetValue() );
		wxPoint l_position = m_targetType->GetPosition();
		wxArrayString l_targetTypes = _getTargetTypes( m_actionName->GetValue() );
		RemoveChild( m_targetType );
		delete m_targetType;
		m_targetType = BuildComboBox( this, l_position, eventTargetType, wxT( "Target Type" ), l_targetTypes, l_targetTypes[0] );
		m_event->SetTarget( l_targetTypes[0], _buildTargetChoice( l_position += wxPoint( 0, 20 ), l_targetTypes[0] ) );
		_buildActionArguments( l_position += wxPoint( 0, 40 ), m_event->GetName() );
		m_updatingProperties = false;
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onSelectTargetType( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;
		wxSize l_size = GetClientSize();
		m_event->SetTarget( m_targetType->GetValue(), _buildTargetChoice( wxPoint( l_size.x / 2, 40 ), m_targetType->GetValue() ) );
		wxPoint l_ptTmp( 0, 80 );
		_buildActionArguments( l_ptTmp, m_event->GetName() );
		m_updatingProperties = false;
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onEditTarget( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;

		if ( m_event->GetTargetType() == wxT( "function" ) )
		{
			m_event->SetTarget( m_event->GetTargetType(), m_targetTC->GetValue() );
		}
		else
		{
			m_event->SetTarget( m_event->GetTargetType(), m_targetCB->GetValue() );
		}

		m_updatingProperties = false;
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onEditTime( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;
		wxString l_value = m_fireTime->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_event->SetFireTime( l_res );
		}

		m_updatingProperties = false;
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onEditXParam( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;
		wxString l_value = m_v3ArgX->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			wxString l_param = l_value + wxT( " " ) + m_v3ArgY->GetValue() + wxT( " " ) + m_v3ArgZ->GetValue();
			m_event->SetActionArgs( l_param );
		}

		m_updatingProperties = false;
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onEditYParam( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;
		wxString l_value = m_v3ArgY->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			wxString l_param = m_v3ArgX->GetValue() + wxT( " " ) + l_value + wxT( " " ) + m_v3ArgZ->GetValue();
			m_event->SetActionArgs( l_param );
		}

		m_updatingProperties = false;
	}
}

//***********************************************************************************************************

void PonctualEventProperties :: _onEditZParam( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_updatingProperties = true;
		wxString l_value = m_v3ArgZ->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			wxString l_param = m_v3ArgX->GetValue() + wxT( " " ) + m_v3ArgY->GetValue() + wxT( " " ) + l_value;
			m_event->SetActionArgs( l_param );
		}

		m_updatingProperties = false;
	}
}

//***********************************************************************************************************
