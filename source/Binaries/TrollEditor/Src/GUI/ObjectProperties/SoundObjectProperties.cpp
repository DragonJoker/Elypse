
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/SoundObjectProperties.h"

#include "Project/Media/SoundObject.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"

using namespace Troll::GUI;
using namespace Troll::Media;

enum SoundObjectPropsIDs
{
	soundAttenuation,
	soundLooped,
	soundTimer,
	soundFadeIn,
	soundFadeOut
};

extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( SoundObjectProperties, wxPanel )
	EVT_TEXT_ENTER(	soundAttenuation,		SoundObjectProperties::_onEditAttenuation )
	EVT_COMBOBOX(	soundLooped,			SoundObjectProperties::_onSelectLooped )
	EVT_TEXT_ENTER(	soundTimer,				SoundObjectProperties::_onEditTimer )
	EVT_TEXT_ENTER(	soundFadeIn,			SoundObjectProperties::_onEditFadeIn )
	EVT_TEXT_ENTER(	soundFadeOut,			SoundObjectProperties::_onEditFadeOut )
END_EVENT_TABLE()


SoundObjectProperties :: SoundObjectProperties( TrollSoundObject * p_soundObject, wxWindow * parent,
		const wxPoint & pos, const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_soundObject( p_soundObject )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_attenuation = BuildTextCtrl( this, l_position, soundAttenuation, wxT( "Attenuation" ), m_soundObject->GetInstance().m_attenuation );
	wxArrayString l_looped;
	l_looped.Add( wxT( "Oui" ) );
	l_looped.Add( wxT( "Non" ) );
	m_looped = BuildComboBox( this, l_position += wxPoint( 0, 20 ), soundLooped, wxT( "Looped" ), l_looped, ( m_soundObject->GetInstance().m_looped ? wxT( "Oui" ) : wxT( "Non" ) ) );
	m_timer = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundTimer, wxT( "Timer time" ), m_soundObject->GetInstance().m_timer );
	m_fadeInTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeIn, wxT( "Fade In Time" ), m_soundObject->GetInstance().m_fadeInTime );
	m_fadeOutTime = BuildTextCtrl( this, l_position += wxPoint( 0, 20 ), soundFadeOut, wxT( "Fade Out Time" ), m_soundObject->GetInstance().m_fadeOutTime );
}



SoundObjectProperties :: ~SoundObjectProperties()
{
}



void SoundObjectProperties :: UpdateProperties()
{
	m_updatingProperties = true;
	m_updatingProperties = false;
}



void SoundObjectProperties :: _onEditAttenuation( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_attenuation->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				m_soundObject->SetAttenuation( l_res );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0.0 et 1.0" ), wxT( "Erreur" ) );
			}
		}
	}
}



void SoundObjectProperties :: _onSelectLooped( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_looped->GetValue();
		m_soundObject->SetLooped( l_value == wxT( "Oui" ) );
	}
}



void SoundObjectProperties :: _onEditTimer( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_timer->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_soundObject->SetTimer( l_res );
		}
	}
}



void SoundObjectProperties :: _onEditFadeIn( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_fadeInTime->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_soundObject->SetFadeIn( l_res );
		}
	}
}



void SoundObjectProperties :: _onEditFadeOut( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_fadeOutTime->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			m_soundObject->SetFadeOut( l_res );
		}
	}
}


