#include "PrecompiledHeader.h"

#include "GUI/ObjectProperties/ContinuousEventProperties.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

#include <Context.h>
#include <EMGui.h>
#include <EMOverlayGroup.h>
#include <ScriptEngine.h>
#include <SequenceManager.h>

using namespace Troll::GUI;
using namespace Troll::Temporal;

enum EventPropsIDs
{
	eventActionName,
	eventTargetType,
	eventTarget,
	eventInterpolation,
	eventKeys
};

BEGIN_EVENT_TABLE( ContinuousEventProperties, wxPanel )
	EVT_COMBOBOX( eventActionName, ContinuousEventProperties::OnSelectAction )
	EVT_COMBOBOX( eventTargetType, ContinuousEventProperties::OnSelectTargetType )
	EVT_TEXT_ENTER( eventTarget, ContinuousEventProperties::OnEditTarget )
	EVT_TEXT_ENTER( eventInterpolation, ContinuousEventProperties::OnSelectInterpolation )
	EVT_COMBOBOX( eventKeys, ContinuousEventProperties::OnSelectKey )
END_EVENT_TABLE()

ContinuousEventProperties::ContinuousEventProperties( TrollContinuousEvent & p_event, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
	: ObjectProperties( parent, pos, size, style )
	, m_event( p_event )
	, m_actionName( NULL )
	, m_targetType( NULL )
	, m_target( NULL )
	, m_interpolation( NULL )
	, m_keys( NULL )
	, m_keyPanel( NULL )
	, m_viewedKeyFrame( NULL )
{
	UpdateProperties();
}

ContinuousEventProperties::~ContinuousEventProperties()
{
	//if (m_viewedKeyFrame != NULL)
	//{
	//	m_viewedKeyFrame->HideProperties();
	//	m_viewedKeyFrame = NULL;
	//}
}

void ContinuousEventProperties::UpdateProperties()
{
	m_updatingProperties = true;

	if ( !DestroyChildren() )
	{
		std::cout << "ContinuousEventProperties::UpdateProperties - Can't destroy children\n";
		m_updatingProperties = false;
		return;
	}

	wxSize l_size = GetClientSize();
	wxPoint l_position = wxPoint( l_size.x / 2, 0 );
	StringArray l_tmp = ScriptEngine::GetContext()->sequenceManager->GetAvailableContinuousEvents();
	wxArrayString l_actions;

	for ( size_t i = 0 ; i < l_tmp.size() ; i++ )
	{
		l_actions.push_back( wxString( l_tmp[i].c_str(), wxMBConvLibc() ) );
	}

	m_actionName = BuildComboBox( this, l_position, eventActionName, _( "Action" ), l_actions, m_event.GetName() );
	wxArrayString l_targetTypes = DoGetTargetTypes( m_event.GetName() );
	m_targetType = BuildComboBox( this, l_position += wxPoint( 0, 20 ), eventTargetType, _( "Target Type" ), l_targetTypes, m_event.GetTargetType() );
	DoBuildTargetChoice( l_position += wxPoint( 0, 20 ), m_event.GetTargetType() );
	DoBuildInterpolationChoice( l_position += wxPoint( 0, 20 ) );
	DoBuildKeyFramesList( l_position += wxPoint( 0, 20 ) );
	m_keyPanel = new wxPanel( this, wxID_ANY, wxPoint( 10, l_position.y + 30 ), GetClientSize() - wxSize( 20, 110 ), wxBORDER_NONE );
	m_updatingProperties = false;
}

wxArrayString ContinuousEventProperties::DoGetTargetTypes( const wxString & p_action )
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

wxString ContinuousEventProperties::DoBuildTargetChoice( const wxPoint & p_position, const wxString & p_targetType )
{
	if ( m_target != NULL )
	{
		wxWindowBase * l_pWnd = static_cast <wxWindowBase *>( m_target );
		RemoveChild( l_pWnd );
		delete l_pWnd;
	}

	wxString l_action = m_event.GetName();

	if ( p_targetType == wxT( "function" ) )
	{
		m_target = BuildTextCtrl( this, p_position, eventTarget, _( "Target" ), m_event.GetTarget() );
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
				l_name = wxString( l_it.getNext()->getName().c_str(), wxMBConvLibc() );

				if ( l_name != wxT( "Core/DebugOverlay" ) && l_name != wxT( "Core/LensFlare" ) && l_name != wxT( "Core/LoadOverlay" ) )
				{
					l_targets.Add( l_name );
				}
			}
		}
		else if ( p_targetType == wxT( "overlayElement" ) )
		{
			for ( auto && l_overlay : ScriptEngine::GetContext()->gui->GetAllOverlays() )
			{
				wxString l_name = wxString( l_overlay.first.c_str(), wxMBConvLibc() );

				if ( l_name != wxT( "Core/DebugOverlay" ) + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() )
						&& l_name != wxT( "Core/LensFlare" ) + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() )
						&& l_name != wxT( "Core/LoadOverlay" ) + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() ) )
				{
					for ( auto && l_element : l_overlay.second->GetElementMap() )
					{
						l_name = wxString( l_element.first.c_str(), wxMBConvLibc() );
						l_targets.Add( l_name + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() ) );
					}
				}
			}

			//g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlaysNames( l_targets );
		}

		l_target = m_event.GetTarget();
		m_target = BuildComboBox( this, p_position, eventTarget, wxT( "Target" ), l_targets, l_target );

		if ( l_targets.empty() )
		{
			return wxEmptyString;
		}
		else
		{
			return l_targets[0];
		}
	}
}

wxString ContinuousEventProperties::DoBuildInterpolationChoice( const wxPoint & p_position )
{
	if ( m_interpolation != NULL )
	{
		wxWindowBase * l_pWnd = static_cast <wxWindowBase *>( m_target );
		RemoveChild( l_pWnd );
		delete l_pWnd;
	}

	wxString l_action = m_event.GetName();
	wxArrayString l_interpolations;
	wxString l_interpolation;

	if ( l_action.find( wxT( "Translate" ) ) || l_action.find( wxT( "Scale" ) ) || l_action.find( wxT( "Resize" ) ) )
	{
		l_interpolations.push_back( wxT( "linear" ) );
		l_interpolations.push_back( wxT( "sinus" ) );
		l_interpolations.push_back( wxT( "bezier3" ) );
		l_interpolations.push_back( wxT( "bezier4" ) );
		l_interpolations.push_back( wxT( "bezier5" ) );
		l_interpolation = m_event.GetInterpolation();
	}
	else if ( l_action == wxT( "Overlay_Rotate" ) )
	{
		l_interpolations.push_back( wxT( "linear" ) );
		l_interpolation = wxT( "linear" );
	}
	else if ( l_action.find( wxT( "Rotate" ) ) )
	{
		l_interpolations.push_back( wxT( "linear" ) );
		l_interpolation = wxT( "linear" );
	}

	m_interpolation = BuildComboBox( this, p_position, eventInterpolation, _( "Interpolation" ), l_interpolations, l_interpolation );
	return l_interpolations[0];
}

void ContinuousEventProperties::DoBuildKeyFramesList( const wxPoint & p_position )
{
	if ( m_keys != NULL )
	{
		RemoveChild( static_cast< wxWindowBase * >( m_keys ) );
		delete m_keys;
	}

	wxArrayString l_list;
	m_event.GetKeyFrames( l_list );
	l_list.push_back( _( "New..." ) );
	wxString l_selected = _( "New..." );
	m_keys = BuildComboBox( this, p_position, eventKeys, _( "KeyFrames" ), l_list, l_selected );
}

void ContinuousEventProperties::OnSelectAction( wxCommandEvent & p_event )
{
	if ( !m_updatingProperties )
	{
		m_event.SetActionName( m_actionName->GetValue() );
		wxPoint l_position = m_targetType->GetPosition();
		RemoveChild( m_targetType );
		delete m_targetType;
		wxArrayString l_targetTypes = DoGetTargetTypes( m_actionName->GetValue() );
		m_targetType = BuildComboBox( this, l_position, eventTargetType, _( "Target Type" ), l_targetTypes, l_targetTypes[0] );
		m_event.SetTargetType( l_targetTypes[0] );
		m_event.SetTarget( DoBuildTargetChoice( l_position += wxPoint( 0, 20 ), m_event.GetTargetType() ) );
	}
}

void ContinuousEventProperties::OnSelectTargetType( wxCommandEvent & p_event )
{
	if ( !m_updatingProperties )
	{
		m_event.SetTargetType( m_targetType->GetValue() );
		m_event.SetTarget( DoBuildTargetChoice( wxPoint( 0, 40 ), m_event.GetTargetType() ) );
	}
}

void ContinuousEventProperties::OnEditTarget( wxCommandEvent & p_event )
{
	if ( !m_updatingProperties )
	{
		if ( m_event.GetTargetType() == wxT( "function" ) )
		{
			m_event.SetTarget( static_cast <wxTextCtrl *>( m_target )->GetValue() );
		}
		else
		{
			m_event.SetTarget( static_cast <wxComboBox *>( m_target )->GetValue() );
		}
	}
}

void ContinuousEventProperties::OnSelectInterpolation( wxCommandEvent & p_event )
{
	if ( !m_updatingProperties )
	{
		m_event.SetInterpolation( m_interpolation->GetValue() );
	}
}

void ContinuousEventProperties::OnSelectKey( wxCommandEvent & p_event )
{
	if ( !m_updatingProperties )
	{
		if ( m_viewedKeyFrame != NULL )
		{
			m_viewedKeyFrame->HideProperties();
			m_viewedKeyFrame = NULL;
		}

		wxString l_value = m_keys->GetValue();

		if ( l_value == _( "New..." ) )
		{
			m_viewedKeyFrame = m_event.AddKeyFrame( 0.0, wxEmptyString );
		}
		else
		{
			std::cout << "ContinuousEventProperties::_onSelectKey - " << l_value.c_str() << "\n";
			float l_tmp;

			if ( GetReal( l_value, l_tmp ) )
			{
				m_viewedKeyFrame = m_event.GetKeyFrame( l_tmp );
			}
		}

		if ( m_viewedKeyFrame != NULL )
		{
			m_viewedKeyFrame->BuildPanel( m_keyPanel, m_keyPanel->GetClientSize().x );
		}
	}
}
