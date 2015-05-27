
#include "PrecompiledHeader.h"

#include "GUI/ObjectProperties/ContinuousEventProperties.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"
/*
#include <Sequences/SequenceManager.h>
#include <Main/Context.h>
#include <Gui/EMOverlayGroup.h>
#include <OgreOverlayManager.h>
*/

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

extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( ContinuousEventProperties, wxPanel )
	EVT_COMBOBOX(	eventActionName,			ContinuousEventProperties::_onSelectAction )
	EVT_COMBOBOX(	eventTargetType,			ContinuousEventProperties::_onSelectTargetType )
	EVT_TEXT_ENTER(	eventTarget,				ContinuousEventProperties::_onEditTarget )
	EVT_TEXT_ENTER(	eventInterpolation,			ContinuousEventProperties::_onSelectInterpolation )
	EVT_COMBOBOX(	eventKeys,					ContinuousEventProperties::_onSelectKey )
END_EVENT_TABLE()


ContinuousEventProperties :: ContinuousEventProperties( TrollContinuousEvent * p_event, wxWindow * parent,
		const wxPoint & pos, const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_event( p_event ),
		m_actionName( NULL ),
		m_targetType( NULL ),
		m_target( NULL ),
		m_interpolation( NULL ),
		m_keys( NULL ),
		m_keyPanel( NULL ),
		m_viewedKeyFrame( NULL )
{
	UpdateProperties();
}



ContinuousEventProperties :: ~ContinuousEventProperties()
{
	/*
		if (m_viewedKeyFrame != NULL)
		{
			m_viewedKeyFrame->HideProperties();
			m_viewedKeyFrame = NULL;
		}
	*/
}



void ContinuousEventProperties :: UpdateProperties()
{
	m_updatingProperties = true;

	if ( ! DestroyChildren() )
	{
		std::cout << "ContinuousEventProperties :: UpdateProperties - Can't destroy children\n";
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

	m_actionName = BuildComboBox( this, l_position, eventActionName, wxT( "Action" ), l_actions, m_event->GetName() );
	wxArrayString l_targetTypes = _getTargetTypes( m_event->GetName() );
	m_targetType = BuildComboBox( this, l_position += wxPoint( 0, 20 ), eventTargetType, wxT( "Target Type" ), l_targetTypes, m_event->GetTargetType() );
	_buildTargetChoice( l_position += wxPoint( 0, 20 ), m_event->GetTargetType() );
	_buildInterpolationChoice( l_position += wxPoint( 0, 20 ) );
	_buildKeyFramesList( l_position += wxPoint( 0, 20 ) );
	m_keyPanel = new wxPanel( this, wxID_ANY, wxPoint( 10, l_position.y + 30 ), GetClientSize() - wxSize( 20, 110 ), wxBORDER_NONE );
	m_updatingProperties = false;
}



wxArrayString ContinuousEventProperties :: _getTargetTypes( const wxString & p_action )
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



wxString ContinuousEventProperties :: _buildTargetChoice( const wxPoint & p_position, const wxString & p_targetType )
{
	if ( m_target != NULL )
	{
		wxWindowBase * l_pWnd = static_cast <wxWindowBase *>( m_target );
		RemoveChild( l_pWnd );
		delete l_pWnd;
	}

	wxString l_action = m_event->GetName();

	if ( p_targetType == wxT( "function" ) )
	{
		m_target = BuildTextCtrl( this, p_position, eventTarget, wxT( "Target" ), m_event->GetTarget() );
		return wxEmptyString;
	}
	else
	{
		wxArrayString l_targets;
		wxString l_target;

		if ( p_targetType == wxT( "scene_node" ) )
		{
//			std::cout << "ContinuousEventProperties :: _buildTargetChoice - TrollSceneNode\n";
			g_mainFrame->GetCurrentProject()->GetMainScene()->GetNodesNames( l_targets );
		}
		else if ( p_targetType == wxT( "overlay" ) )
		{
//			std::cout << "ContinuousEventProperties :: _buildTargetChoice - Overlay\n";
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
			EMuse::Gui::EMOverlayMap l_elementMap;
			EMuse::Gui::EMOverlayMap::iterator l_elementIt;
			EMuse::Gui::OverlayGroupMap l_map = ScriptEngine::GetContext()->gui->GetAllOverlays();
			EMuse::Gui::OverlayGroupMap::iterator l_it = l_map.begin();
			wxString l_name;

			for ( ; l_it != l_map.end() ; ++l_it )
			{
				l_name = wxString( l_it->first.c_str(), wxMBConvLibc() );

				if ( l_name != wxString( wxT( "Core/DebugOverlay" ) ) + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() )
						&& l_name != wxString( wxT( "Core/LensFlare" ) ) + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() )
						&& l_name != wxString( wxT( "Core/LoadOverlay" ) ) + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() ) )
				{
					l_elementMap = l_it->second->GetElementMap();
					l_elementIt = l_elementMap.begin();

					for ( ; l_elementIt != l_elementMap.end() ; ++l_elementIt )
					{
						l_name = wxString( l_elementIt->first.c_str(), wxMBConvLibc() );
						l_targets.Add( l_name + wxString( ScriptEngine::GetContext()->instanceNum.c_str(), wxMBConvLibc() ) );
					}
				}
			}

//			std::cout << "ContinuousEventProperties :: _buildTargetChoice - OverlayElement\n";
//			g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlaysNames( l_targets);
		}

		l_target = m_event->GetTarget();
		m_target = BuildComboBox( this, p_position, eventTarget, wxT( "Target" ), l_targets, l_target );

		if ( l_targets.size() > 0 )
		{
			return l_targets[0];
		}
		else
		{
			return wxEmptyString;
		}
	}
}



wxString ContinuousEventProperties :: _buildInterpolationChoice( const wxPoint & p_position )
{
	if ( m_interpolation != NULL )
	{
		wxWindowBase * l_pWnd = static_cast <wxWindowBase *>( m_target );
		RemoveChild( l_pWnd );
		delete l_pWnd;
	}

	wxString l_action = m_event->GetName();
	wxArrayString l_interpolations;
	wxString l_interpolation;

	if ( l_action.find( wxT( "Translate" ) ) || l_action.find( wxT( "Scale" ) ) || l_action.find( wxT( "Resize" ) ) )
	{
		l_interpolations.push_back( wxT( "linear" ) );
		l_interpolations.push_back( wxT( "sinus" ) );
		l_interpolations.push_back( wxT( "bezier3" ) );
		l_interpolations.push_back( wxT( "bezier4" ) );
		l_interpolations.push_back( wxT( "bezier5" ) );
		l_interpolation = m_event->GetInterpolation();
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

	m_interpolation = BuildComboBox( this, p_position, eventInterpolation, wxT( "Interpolation" ), l_interpolations, l_interpolation );
	return l_interpolations[0];
}



void ContinuousEventProperties :: _buildKeyFramesList( const wxPoint & p_position )
{
	if ( m_keys != NULL )
	{
		RemoveChild( static_cast <wxWindowBase *>( m_keys ) );
		delete m_keys;
	}

	wxArrayString l_list;
	m_event->GetKeyFrames( l_list );
	l_list.push_back( wxT( "New..." ) );
	wxString l_selected = wxT( "New..." );
	m_keys = BuildComboBox( this, p_position, eventKeys, wxT( "KeyFrames" ), l_list, l_selected );
}



void ContinuousEventProperties :: _onSelectAction( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_event->SetActionName( m_actionName->GetValue() );
		wxPoint l_position = m_targetType->GetPosition();
		RemoveChild( m_targetType );
		delete m_targetType;
		wxArrayString l_targetTypes = _getTargetTypes( m_actionName->GetValue() );
		m_targetType = BuildComboBox( this, l_position, eventTargetType, wxT( "Target Type" ), l_targetTypes, l_targetTypes[0] );
		m_event->SetTargetType( l_targetTypes[0] );
		m_event->SetTarget( _buildTargetChoice( l_position += wxPoint( 0, 20 ), m_event->GetTargetType() ) );
	}
}



void ContinuousEventProperties :: _onSelectTargetType( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_event->SetTargetType( m_targetType->GetValue() );
		m_event->SetTarget( _buildTargetChoice( wxPoint( 0, 40 ), m_event->GetTargetType() ) );
	}
}



void ContinuousEventProperties :: _onEditTarget( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		if ( m_event->GetTargetType() == wxT( "function" ) )
		{
			m_event->SetTarget( static_cast <wxTextCtrl *>( m_target )->GetValue() );
		}
		else
		{
			m_event->SetTarget( static_cast <wxComboBox *>( m_target )->GetValue() );
		}
	}
}



void ContinuousEventProperties :: _onSelectInterpolation( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_event->SetInterpolation( m_interpolation->GetValue() );
	}
}



void ContinuousEventProperties :: _onSelectKey( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		if ( m_viewedKeyFrame != NULL )
		{
			m_viewedKeyFrame->HideProperties();
			m_viewedKeyFrame = NULL;
		}

		wxString l_value = m_keys->GetValue();

		if ( l_value == wxT( "New..." ) )
		{
			m_viewedKeyFrame = m_event->AddKeyFrame( 0.0, wxEmptyString );
		}
		else
		{
			std::cout << "ContinuousEventProperties :: _onSelectKey - " << l_value.c_str() << "\n";
			float l_tmp;

			if ( GetReal( l_value, l_tmp ) )
			{
				m_viewedKeyFrame = m_event->GetKeyFrame( l_tmp );
			}
		}

		if ( m_viewedKeyFrame != NULL )
		{
			m_viewedKeyFrame->BuildPanel( m_keyPanel, m_keyPanel->GetClientSize().x );
		}
	}
}


