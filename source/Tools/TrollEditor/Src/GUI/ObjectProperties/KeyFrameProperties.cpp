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

#include "KeyFrameProperties.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum KeyPropsIDs
	{
		keyTime,
		keyReal,
		keyV3X,
		keyV3Y,
		keyV3Z
	};

	BEGIN_EVENT_TABLE( KeyFrameProperties, wxPanel )
		EVT_TEXT_ENTER( keyTime, KeyFrameProperties::OnEditFireTime )
		EVT_TEXT_ENTER( keyReal, KeyFrameProperties::OnEditReal )
		EVT_TEXT_ENTER( keyV3X, KeyFrameProperties::OnEditV3X )
		EVT_TEXT_ENTER( keyV3Y, KeyFrameProperties::OnEditV3Y )
		EVT_TEXT_ENTER( keyV3Z, KeyFrameProperties::OnEditV3Z )
	END_EVENT_TABLE()

	KeyFrameProperties::KeyFrameProperties( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollKeyFrame * p_keyFrame, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_key( p_keyFrame )
		, m_fireTime( NULL )
		, m_real( NULL )
		, m_v3X( NULL )
		, m_v3Y( NULL )
		, m_v3Z( NULL )
	{
		UpdateProperties();
	}

	KeyFrameProperties::~KeyFrameProperties()
	{
	}

	void KeyFrameProperties::UpdateProperties()
	{
		m_updatingProperties = true;

		if ( ! DestroyChildren() )
		{
			std::cout << "KeyFrameProperties::UpdateProperties - Can't destroy children\n";
			m_updatingProperties = false;
			return;
		}

		wxSize l_size = GetClientSize();
		wxPoint l_position = wxPoint( l_size.x / 2, 0 );
		m_fireTime = BuildTextCtrl( this, l_position, keyTime, wxT( "Time" ), m_key->GetFireTime() );
		_buildArguments( l_position += wxPoint( 0, 20 ) );
		m_updatingProperties = false;
	}

	void KeyFrameProperties::_buildArguments( const wxPoint & p_position )
	{
		if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceReal )
		{
			float l_tmp;

			if ( GetReal( m_key->GetAction(), l_tmp ) )
			{
				m_real = BuildTextCtrl( this, p_position, keyReal, wxT( "Valeur" ), l_tmp );
			}
		}
		else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector2 )
		{
			Vector3 l_tmp;
			Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key->GetAction() ), ", \t", 2 );

			if ( l_split.size() > 2 )
			{
				std::cout << "KeyFrameProperties::_buildArguments - " << l_split[0] << " - " << l_split[1] << "\n";

				if ( GetReal( l_split[0], l_tmp.x ) && GetReal( l_split[1], l_tmp.y ) )
				{
					wxPoint l_position = p_position;
					m_v3X = BuildTextCtrl( this, l_position, keyV3X, wxT( "X" ), l_tmp.x );
					m_v3Y = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Y, wxT( "Y" ), l_tmp.y );
				}
			}
		}
		else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector3 )
		{
			Vector3 l_tmp;
			Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key->GetAction() ), ", \t", 2 );

			if ( l_split.size() > 2 )
			{
				std::cout << "KeyFrameProperties::_buildArguments - " << l_split[0] << " - " << l_split[1] << " - " << l_split[2] << "\n";

				if ( GetReal( l_split[0], l_tmp.x ) && GetReal( l_split[1], l_tmp.y ) && GetReal( l_split[2], l_tmp.z ) )
				{
					wxPoint l_position = p_position;
					m_v3X = BuildTextCtrl( this, l_position, keyV3X, wxT( "X" ), l_tmp.x );
					m_v3Y = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Y, wxT( "Y" ), l_tmp.y );
					m_v3Z = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Z, wxT( "Z" ), l_tmp.z );
				}
			}
		}
		else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceQuaternion )
		{
			Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key->GetAction() ), ", \t", 3 );

			if ( l_split.size() > 3 )
			{
				float l_tmpf;
				Vector3 l_tmp;

				if ( GetReal( l_split[0], l_tmp.x ) && GetReal( l_split[1], l_tmp.y ) && GetReal( l_split[2], l_tmp.z ) && GetReal( l_split[3], l_tmpf ) )
				{
					wxPoint l_position = p_position;
					m_real = BuildTextCtrl( this, l_position, keyReal, _( "Angle" ), l_tmpf );
					m_v3X = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3X, _( "X Axis" ), l_tmp.x );
					m_v3Y = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Y, _( "Y Axis" ), l_tmp.y );
					m_v3Z = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Z, _( "Z Axis" ), l_tmp.z );
				}
			}
		}
	}

	void KeyFrameProperties::OnEditFireTime( wxCommandEvent & p_event )
	{
		wxString l_value = m_fireTime->GetValue();
		float l_tmp;

		if ( GetReal( l_value, l_tmp ) )
		{
			m_key->SetFireTime( l_tmp, true );
		}
	}

	void KeyFrameProperties::OnEditReal( wxCommandEvent & p_event )
	{
		wxString l_value = m_real->GetValue();
		float l_tmp;

		if ( GetReal( l_value, l_tmp ) )
		{
			if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceReal )
			{
				m_key->SetAction( l_value );
			}
			else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceQuaternion )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
		}
	}

	void KeyFrameProperties::OnEditV3X( wxCommandEvent & p_event )
	{
		wxString l_value = m_v3X->GetValue();
		float l_tmp;

		if ( GetReal( l_value, l_tmp ) )
		{
			if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector3 || m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector2 )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
			else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceQuaternion )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
		}
	}

	void KeyFrameProperties::OnEditV3Y( wxCommandEvent & p_event )
	{
		wxString l_value = m_v3Y->GetValue();
		float l_tmp;

		if ( GetReal( l_value, l_tmp ) )
		{
			if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector3 || m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector2 )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
			else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceQuaternion )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
		}
	}

	void KeyFrameProperties::OnEditV3Z( wxCommandEvent & p_event )
	{
		wxString l_value = m_v3Z->GetValue();
		float l_tmp;

		if ( GetReal( l_value, l_tmp ) )
		{
			if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceVector3 )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
			else if ( m_key->GetArgsType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ceQuaternion )
			{
				l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
				l_value.Replace( wxT( "," ), wxT( "." ) );
				m_key->SetAction( l_value );
			}
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
