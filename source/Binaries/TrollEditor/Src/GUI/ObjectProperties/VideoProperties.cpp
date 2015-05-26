//***********************************************************************************************************
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/VideoProperties.h"

#include "Project/Media/Video.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"
//***********************************************************************************************************
using namespace Troll::GUI;
using namespace Troll::Media;
//***********************************************************************************************************
enum VideoPropsIDs
{
	videoName,
	videoUrl,
	videoLooped,
	videoEnabled
};
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( VideoProperties, wxPanel )
	EVT_TEXT_ENTER(	videoName,				VideoProperties::_onEditName )
	EVT_BUTTON(	videoUrl,				VideoProperties::_onEditUrl )
	EVT_COMBOBOX(	videoLooped,			VideoProperties::_onSelectLooped )
	EVT_COMBOBOX(	videoEnabled,			VideoProperties::_onSelectEnabled )
END_EVENT_TABLE()
//***********************************************************************************************************

VideoProperties :: VideoProperties( TrollVideo * p_video, wxWindow * parent, const wxPoint & pos,
									const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_video( p_video )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_videoName = BuildTextCtrl( this, l_position, videoName, wxT( "Material Name" ), m_video->GetName() );
	m_videoUrl = BuildButton( this, l_position += wxPoint( 0, 20 ), videoUrl, wxT( "Video URL" ), m_video->GetUrl() );
	wxArrayString l_looped;
	l_looped.Add( wxT( "Oui" ) );
	l_looped.Add( wxT( "Non" ) );
	m_looped = BuildComboBox( this, l_position += wxPoint( 0, 20 ), videoLooped, wxT( "Looped" ), l_looped, ( m_video->IsLooped() ? wxT( "Oui" ) : wxT( "Non" ) ) );
	m_enabled = BuildComboBox( this, l_position += wxPoint( 0, 20 ), videoEnabled, wxT( "Enabled" ), l_looped, ( m_video->IsEnabled() ? wxT( "Oui" ) : wxT( "Non" ) ) );
}

//***********************************************************************************************************

VideoProperties :: ~VideoProperties()
{
}

//***********************************************************************************************************

void VideoProperties :: UpdateProperties()
{
	m_updatingProperties = true;
	m_updatingProperties = false;
}

//***********************************************************************************************************

void VideoProperties :: _onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_video->SetName( m_videoName->GetValue() );
	}
}

//***********************************************************************************************************

void VideoProperties :: _onEditUrl( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxFileDialog * l_dialog = new wxFileDialog( this, wxT( "Choisissez un fichier vidéo" ), wxEmptyString, wxEmptyString,
				wxT( "Video files (*.mpeg;*.avi;*.ogm;*.wmv)|*.mpeg;*.avi;*.ogm;*.wmv" ) );

		if ( l_dialog->ShowModal() )
		{
			wxString l_filePath = l_dialog->GetPath();
			m_video->SetUrl( l_filePath );
		}
	}
}

//***********************************************************************************************************

void VideoProperties :: _onSelectLooped( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_looped->GetValue();
		m_video->SetLooped( l_value == wxT( "Oui" ) );
	}
}

//***********************************************************************************************************

void VideoProperties :: _onSelectEnabled( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = p_event.GetString();
		m_video->SetEnabled( l_value == wxT( "Oui" ) );
	}
}

//***********************************************************************************************************
