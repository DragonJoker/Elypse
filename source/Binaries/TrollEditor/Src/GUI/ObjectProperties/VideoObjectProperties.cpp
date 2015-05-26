//***********************************************************************************************************
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/VideoObjectProperties.h"

#include "Project/Media/VideoObject.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/MainFrame.h"
//***********************************************************************************************************
using namespace Troll::GUI;
using namespace Troll::Media;
//***********************************************************************************************************
enum VideoObjectPropsIDs
{
	videoAttenuation,
};
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( VideoObjectProperties, wxPanel )
	EVT_TEXT_ENTER(	videoAttenuation,		VideoObjectProperties::_onEditAttenuation )
END_EVENT_TABLE()
//***********************************************************************************************************

VideoObjectProperties :: VideoObjectProperties( TrollVideoObject * p_videoObject, wxWindow * parent,
		const wxPoint & pos, const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_videoObject( p_videoObject )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_attenuation = BuildTextCtrl( this, l_position, videoAttenuation, wxT( "Attenuation" ), m_videoObject->GetAttenuation() );
}

//***********************************************************************************************************

VideoObjectProperties :: ~VideoObjectProperties()
{
}

//***********************************************************************************************************

void VideoObjectProperties :: UpdateProperties()
{
	m_updatingProperties = true;
	m_updatingProperties = false;
}

//***********************************************************************************************************

void VideoObjectProperties :: _onEditAttenuation( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		wxString l_value = m_attenuation->GetValue();
		float l_res;

		if ( GetReal( l_value, l_res ) )
		{
			if ( l_res >= 0.0 && l_res <= 1.0 )
			{
				m_videoObject->SetAttenuation( l_res );
			}
			else
			{
				wxMessageBox( wxT( "Veuillez entrer une valeur flottante entre 0.0 et 1.0" ), wxT( "Erreur" ) );
			}
		}
	}
}

//***********************************************************************************************************
