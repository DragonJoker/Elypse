//***********************************************************************************************************
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/KeyFrameProperties.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "GUI/MainFrame.h"
/*
#include <OgreString.h>
*/
//***********************************************************************************************************
using namespace Troll::GUI;
using namespace Troll::Temporal;
//***********************************************************************************************************
enum KeyPropsIDs
{
	keyTime,
	keyReal,
	keyV3X,
	keyV3Y,
	keyV3Z
};
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( KeyFrameProperties, wxPanel )
	EVT_TEXT_ENTER(	keyTime,	KeyFrameProperties::_onEditFireTime )
	EVT_TEXT_ENTER(	keyReal,	KeyFrameProperties::_onEditReal )
	EVT_TEXT_ENTER(	keyV3X,		KeyFrameProperties::_onEditV3X )
	EVT_TEXT_ENTER(	keyV3Y,		KeyFrameProperties::_onEditV3Y )
	EVT_TEXT_ENTER(	keyV3Z,		KeyFrameProperties::_onEditV3Z )
END_EVENT_TABLE()
//***********************************************************************************************************

KeyFrameProperties :: KeyFrameProperties( TrollKeyFrame * p_keyFrame, wxWindow * parent,
		const wxPoint & pos, const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_key( p_keyFrame ),
		m_fireTime( NULL ),
		m_real( NULL ),
		m_v3X( NULL ),
		m_v3Y( NULL ),
		m_v3Z( NULL )
{
	UpdateProperties();
}

//***********************************************************************************************************

KeyFrameProperties :: ~KeyFrameProperties()
{
}

//***********************************************************************************************************

void KeyFrameProperties :: UpdateProperties()
{
	m_updatingProperties = true;

	if ( ! DestroyChildren() )
	{
		std::cout << "KeyFrameProperties :: UpdateProperties - Can't destroy children\n";
		m_updatingProperties = false;
		return;
	}

	wxSize l_size = GetClientSize();
	wxPoint l_position = wxPoint( l_size.x / 2, 0 );
	m_fireTime = BuildTextCtrl( this, l_position, keyTime, wxT( "Time" ), m_key->GetFireTime() );
	_buildArguments( l_position += wxPoint( 0, 20 ) );
	m_updatingProperties = false;
}

//***********************************************************************************************************

void KeyFrameProperties :: _buildArguments( const wxPoint & p_position )
{
	if ( m_key->GetArgsType() == ceReal )
	{
		float l_tmp;

		if ( GetReal( m_key->GetAction(), l_tmp ) )
		{
			m_real = BuildTextCtrl( this, p_position, keyReal, wxT( "Valeur" ), l_tmp );
		}
	}
	else if ( m_key->GetArgsType() == ceVector2 )
	{
		Vector3 l_tmp;
		Ogre::StringVector l_split = Ogre::StringUtil::split( m_key->GetAction().char_str().data(), ", \t", 2 );

		if ( l_split.size() > 2 )
		{
			std::cout << "KeyFrameProperties :: _buildArguments - " << l_split[0] << " - " << l_split[1] << "\n";

			if ( GetReal( l_split[0], l_tmp.x ) && GetReal( l_split[1], l_tmp.y ) )
			{
				wxPoint l_position = p_position;
				m_v3X = BuildTextCtrl( this, l_position, keyV3X, wxT( "X" ), l_tmp.x );
				m_v3Y = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Y, wxT( "Y" ), l_tmp.y );
			}
		}
	}
	else if ( m_key->GetArgsType() == ceVector3 )
	{
		Vector3 l_tmp;
		Ogre::StringVector l_split = Ogre::StringUtil::split( m_key->GetAction().char_str().data(), ", \t", 2 );

		if ( l_split.size() > 2 )
		{
			std::cout << "KeyFrameProperties :: _buildArguments - " << l_split[0] << " - " << l_split[1] << " - " << l_split[2] << "\n";

			if ( GetReal( l_split[0], l_tmp.x ) && GetReal( l_split[1], l_tmp.y ) && GetReal( l_split[2], l_tmp.z ) )
			{
				wxPoint l_position = p_position;
				m_v3X = BuildTextCtrl( this, l_position, keyV3X, wxT( "X" ), l_tmp.x );
				m_v3Y = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Y, wxT( "Y" ), l_tmp.y );
				m_v3Z = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Z, wxT( "Z" ), l_tmp.z );
			}
		}
	}
	else if ( m_key->GetArgsType() == ceQuaternion )
	{
		Ogre::StringVector l_split = Ogre::StringUtil::split( m_key->GetAction().char_str().data(), ", \t", 3 );

		if ( l_split.size() > 3 )
		{
			float l_tmpf;
			Vector3 l_tmp;

			if ( GetReal( l_split[0], l_tmp.x ) && GetReal( l_split[1], l_tmp.y ) && GetReal( l_split[2], l_tmp.z ) && GetReal( l_split[3], l_tmpf ) )
			{
				wxPoint l_position = p_position;
				m_real = BuildTextCtrl( this, l_position, keyReal, wxT( "Angle" ), l_tmpf );
				m_v3X = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3X, wxT( "Axe X" ), l_tmp.x );
				m_v3Y = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Y, wxT( "Axe Y" ), l_tmp.y );
				m_v3Z = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), keyV3Z, wxT( "Axe Z" ), l_tmp.z );
			}
		}
	}
}

//***********************************************************************************************************

void KeyFrameProperties :: _onEditFireTime( wxCommandEvent & p_event )
{
	wxString l_value = m_fireTime->GetValue();
	float l_tmp;

	if ( GetReal( l_value, l_tmp ) )
	{
		m_key->SetFireTime( l_tmp, true );
	}
}

//***********************************************************************************************************

void KeyFrameProperties :: _onEditReal( wxCommandEvent & p_event )
{
	wxString l_value = m_real->GetValue();
	float l_tmp;

	if ( GetReal( l_value, l_tmp ) )
	{
		if ( m_key->GetArgsType() == ceReal )
		{
			m_key->SetAction( l_value );
		}
		else if ( m_key->GetArgsType() == ceQuaternion )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
	}
}

//***********************************************************************************************************

void KeyFrameProperties :: _onEditV3X( wxCommandEvent & p_event )
{
	wxString l_value = m_v3X->GetValue();
	float l_tmp;

	if ( GetReal( l_value, l_tmp ) )
	{
		if ( m_key->GetArgsType() == ceVector3 || m_key->GetArgsType() == ceVector2 )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
		else if ( m_key->GetArgsType() == ceQuaternion )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
	}
}

//***********************************************************************************************************

void KeyFrameProperties :: _onEditV3Y( wxCommandEvent & p_event )
{
	wxString l_value = m_v3Y->GetValue();
	float l_tmp;

	if ( GetReal( l_value, l_tmp ) )
	{
		if ( m_key->GetArgsType() == ceVector3 || m_key->GetArgsType() == ceVector2 )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
		else if ( m_key->GetArgsType() == ceQuaternion )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
	}
}

//***********************************************************************************************************

void KeyFrameProperties :: _onEditV3Z( wxCommandEvent & p_event )
{
	wxString l_value = m_v3Z->GetValue();
	float l_tmp;

	if ( GetReal( l_value, l_tmp ) )
	{
		if ( m_key->GetArgsType() == ceVector3 )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
		else if ( m_key->GetArgsType() == ceQuaternion )
		{
			l_value = m_v3X->GetValue() + wxT( " " ) + m_v3Y->GetValue() + wxT( " " ) + m_v3Z->GetValue() + wxT( " " ) + m_real->GetValue();
			l_value.Replace( wxT( "," ), wxT( "." ) );
			m_key->SetAction( l_value );
		}
	}
}

//***********************************************************************************************************
