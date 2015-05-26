//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "Project/Media/VideoObject.h"
#include "Project/Media/Video.h"
#include "Project/Object.h"
#include "GUI/ObjectProperties/VideoObjectProperties.h"
/*
#include <VideoObject.h>
#include <VideoOverlay.h>
#include <wx/txtstrm.h>
*/
//***********************************************************************************************************
using namespace Troll;
using namespace Troll::Media;
using namespace Troll::GUI;
using namespace EMuse::Media;
//***********************************************************************************************************

TrollVideoObject :: TrollVideoObject( const wxString & p_name, const wxString & p_fileName )
	:	MediaObject( p_name, p_fileName, ttVideoObject ),
		m_video( NULL ),
		m_object( NULL ),
		m_attenuation( 0.0 )
{
}

//***********************************************************************************************************

TrollVideoObject :: ~TrollVideoObject()
{
}

//***********************************************************************************************************

int TrollVideoObject :: BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 20;
	wxSize l_returnSize( p_width, l_height );

	if ( m_propertiesPanel )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
	}

	m_propertiesPanel = new VideoObjectProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
	return l_height;
}

//***********************************************************************************************************
/*
void TrollVideoObject :: SetMuseMediaEntity( MediaEntity * p_me)
{
	m_museMediaEntity = p_me;
}
*/
//***********************************************************************************************************

void TrollVideoObject :: SetAttenuation( float p_att )
{
	m_attenuation = p_att;
}

//***********************************************************************************************************

void TrollVideoObject :: SetMuseVideoObject( VideoObject * p_videoObject )
{
	m_videoOverlay = NULL;
	m_videoObject = p_videoObject;
}

//***********************************************************************************************************

void TrollVideoObject :: SetMuseVideoOverlay( VideoOverlay * p_videoOverlay )
{
	m_videoOverlay = p_videoOverlay;
	m_videoObject = NULL;
}

//***********************************************************************************************************

void TrollVideoObject :: Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "\tuse_video " ) + m_video->GetName() + wxT( "\n\t{\n" ) );
	wxString l_tmp;
	l_tmp.Printf( wxT( "%.2f" ), m_attenuation );
	l_tmp.Replace( wxT( "," ), wxT( "." ) );
	p_stream->WriteString( wxT( "\t\tvolume " ) + l_tmp + wxT( "\n\t}\n" ) );
	l_tmp.clear();
}

//***********************************************************************************************************
