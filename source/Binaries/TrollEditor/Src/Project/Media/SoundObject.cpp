//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "Project/Media/SoundObject.h"
#include "Project/Media/Sound.h"
#include "GUI/ObjectProperties/SoundObjectProperties.h"
#include "Project/Object.h"
/*
#include <SoundInstance.h>
#include <wx/txtstrm.h>
*/
//***********************************************************************************************************
using namespace Troll::Media;
using namespace Troll::GUI;
//***********************************************************************************************************

TrollSoundObject :: TrollSoundObject( const wxString & p_name, const wxString & p_fileName )
	:	MediaObject( p_name, p_fileName, ttSoundObject ),
		m_object( NULL )
{
}

//***********************************************************************************************************

TrollSoundObject :: ~TrollSoundObject()
{
}

//***********************************************************************************************************

int TrollSoundObject :: BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 100;
	wxSize l_returnSize( p_width, l_height );

	if ( m_propertiesPanel )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new SoundObjectProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
	return l_height;
}

//***********************************************************************************************************

void TrollSoundObject :: SetMuseInstance( EMuse::Media::SoundInstance * p_instance )
{
	m_museInstance = p_instance;
}

//***********************************************************************************************************

void TrollSoundObject :: SetAttenuation( float p_att )
{
	m_instance.m_attenuation = p_att;
	m_museInstance->SetVolumePercent( p_att );
}

//***********************************************************************************************************

void TrollSoundObject :: SetLooped( bool p_looped )
{
	m_instance.m_looped = p_looped;
	m_museInstance->SetLooped( p_looped );
}

//***********************************************************************************************************

void TrollSoundObject :: SetTimer( float p_time )
{
	m_instance.m_timer = p_time;
	m_museInstance->SetTimerTime( p_time );
}

//***********************************************************************************************************

void TrollSoundObject :: SetFadeIn( float p_time )
{
	m_instance.m_fadeInTime = p_time;
	m_museInstance->SetFadeIn( p_time );
}

//***********************************************************************************************************

void TrollSoundObject :: SetFadeOut( float p_time )
{
	m_instance.m_fadeOutTime = p_time;
	m_museInstance->SetFadeOut( p_time );
}

//***********************************************************************************************************

void TrollSoundObject :: Write( wxTextOutputStream * p_stream )
{
	wxString l_tmp;
	p_stream->WriteString( wxT( "\tuse_sound " ) + m_sound->GetName() + wxT( "\n\t{\n" ) );
	l_tmp.Printf( wxT( "%.2f" ), m_instance.m_attenuation );
	l_tmp.Replace( wxT( "," ), wxT( "." ) );
	p_stream->WriteString( wxT( "\t\tattenuation " ) + l_tmp + wxT( "\n" ) );
	l_tmp.clear();
	p_stream->WriteString( wxT( "\t\tlooped " ) + ( m_instance.m_looped ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n" ) );

	if ( m_instance.m_timer > 0.0 )
	{
		l_tmp.Printf( wxT( "%.2f" ), m_instance.m_timer );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\t\ttimer " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_instance.m_fadeInTime > 0.0 )
	{
		l_tmp.Printf( wxT( "%.2f" ), m_instance.m_fadeInTime );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\t\tfade_in " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_instance.m_fadeOutTime > 0.0 )
	{
		l_tmp.Printf( wxT( "%.2f" ), m_instance.m_fadeOutTime );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( wxT( "\t\tfade_out " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	p_stream->WriteString( wxT( "\t}\n" ) );
}

//***********************************************************************************************************
