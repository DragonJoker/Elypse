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

#include "PonctualEventProperties.h"

#include "Project/Temporal/PonctualEvent.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
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

	BEGIN_EVENT_TABLE( PonctualEventProperties, wxPanel )
		EVT_COMBOBOX( eventActionName, PonctualEventProperties::OnSelectAction )
		EVT_COMBOBOX( eventTargetType, PonctualEventProperties::OnSelectTargetType )
		EVT_COMBOBOX( eventTargetCB, PonctualEventProperties::OnEditTarget )
		EVT_TEXT_ENTER( eventTargetTxt, PonctualEventProperties::OnEditTarget )
		EVT_TEXT_ENTER( eventTime, PonctualEventProperties::OnEditTime )
		EVT_TEXT_ENTER( eventParamX, PonctualEventProperties::OnEditXParam )
		EVT_TEXT_ENTER( eventParamY, PonctualEventProperties::OnEditYParam )
		EVT_TEXT_ENTER( eventParamZ, PonctualEventProperties::OnEditZParam )
	END_EVENT_TABLE()

	PonctualEventProperties::PonctualEventProperties( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_event( p_event )
		, m_targetCB( NULL )
		, m_targetTC( NULL )
		, m_v3ArgX( NULL )
		, m_v3ArgY( NULL )
		, m_v3ArgZ( NULL )
		, m_actionName( NULL )
		, m_targetType( NULL )
		, m_fireTime( NULL )
	{
		UpdateProperties();
	}

	PonctualEventProperties::~PonctualEventProperties()
	{
	}

	void PonctualEventProperties::UpdateProperties()
	{
		m_updatingProperties = true;

		if ( ! DestroyChildren() )
		{
			std::cout << "PonctualEventProperties::UpdateProperties - Can't destroy children\n";
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

		m_actionName = BuildComboBox( this, l_position, eventActionName, _( "Action" ), l_actions, m_event->GetObjectName() );
		wxArrayString l_targetTypes = _getTargetTypes( m_event->GetObjectName() );
		m_targetType = BuildComboBox( this, l_position += wxPoint( 0, 20 ), eventTargetType, _( "Target Type" ), l_targetTypes, m_event->GetTargetType() );
		_buildTargetChoice( l_position += wxPoint( 0, 20 ), m_event->GetTargetType() );
		m_fireTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), eventTime, _( "Time" ), m_event->GetFireTime() );
		_buildActionArguments( l_position, m_event->GetObjectName() );
		m_updatingProperties = false;
	}

	wxArrayString PonctualEventProperties::_getTargetTypes( const wxString & p_action )
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

	wxString PonctualEventProperties::_buildTargetChoice( const wxPoint & p_position, const wxString & p_targetType )
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

		wxString l_action = m_event->GetObjectName();

		if ( p_targetType == wxT( "function" ) )
		{
			m_targetTC = BuildTextCtrl( this, p_position, eventTargetTxt, _( "Target" ), m_event->GetTarget() );
			return wxEmptyString;
		}
		else
		{
			wxArrayString l_targets;
			wxString l_target;

			if ( p_targetType == wxT( "scene_node" ) )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetNodesNames( l_targets );
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
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlaysNames( l_targets );
			}
			else if ( p_targetType == wxT( "sequence" ) )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSequencesNames( l_targets );
			}

			l_target = m_event->GetTarget();
			m_targetCB = BuildComboBox( this, p_position, eventTargetCB, _( "Target" ), l_targets, l_target );
			return l_targets[0];
		}
	}

	void PonctualEventProperties::_buildActionArguments( wxPoint & p_position, const wxString & p_action )
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
			Ogre::StringVector l_params = StringUtil::split( make_string( p_action ), ", \t" );

			if ( l_params.size() > 2 )
			{
				Vector3 l_vector = Vector3( StringConverter::parseReal( l_params[0] ),
											StringConverter::parseReal( l_params[1] ),
											StringConverter::parseReal( l_params[2] ) );
				m_v3ArgX = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), eventParamX, _( "X Target" ), l_vector.x );
				m_v3ArgY = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), eventParamY, _( "Y Target" ), l_vector.y );
				m_v3ArgZ = BuildTextCtrl( this, p_position += wxPoint( 0, 20 ), eventParamZ, _( "Z Target" ), l_vector.z );
			}
		}
	}

	void PonctualEventProperties::OnSelectAction( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_updatingProperties = true;
			m_event->SetActionName( m_actionName->GetValue() );
			wxPoint l_position = m_targetType->GetPosition();
			wxArrayString l_targetTypes = _getTargetTypes( m_actionName->GetValue() );
			RemoveChild( m_targetType );
			delete m_targetType;
			m_targetType = BuildComboBox( this, l_position, eventTargetType, _( "Target Type" ), l_targetTypes, l_targetTypes[0] );
			m_event->SetTarget( l_targetTypes[0], _buildTargetChoice( l_position += wxPoint( 0, 20 ), l_targetTypes[0] ) );
			_buildActionArguments( l_position += wxPoint( 0, 40 ), m_event->GetObjectName() );
			m_updatingProperties = false;
		}
	}

	void PonctualEventProperties::OnSelectTargetType( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_updatingProperties = true;
			wxSize l_size = GetClientSize();
			m_event->SetTarget( m_targetType->GetValue(), _buildTargetChoice( wxPoint( l_size.x / 2, 40 ), m_targetType->GetValue() ) );
			wxPoint l_ptTmp( 0, 80 );
			_buildActionArguments( l_ptTmp, m_event->GetObjectName() );
			m_updatingProperties = false;
		}
	}

	void PonctualEventProperties::OnEditTarget( wxCommandEvent & p_event )
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

	void PonctualEventProperties::OnEditTime( wxCommandEvent & p_event )
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

	void PonctualEventProperties::OnEditXParam( wxCommandEvent & p_event )
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

	void PonctualEventProperties::OnEditYParam( wxCommandEvent & p_event )
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

	void PonctualEventProperties::OnEditZParam( wxCommandEvent & p_event )
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
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
