#include "VideoObject.h"

#include "Video.h"
#include "Project/Object.h"
#include "GUI/Properties/Project/VideoObjectProperties.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

using namespace Elypse::Media;

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			TrollVideoObject::TrollVideoObject( wxString const & p_name, wxString const & p_fileName )
				: MediaObject( p_name, p_fileName, ttVideoObject )
				, m_video( NULL )
				, m_object( NULL )
				, m_attenuation( 0.0 )
			{
			}

			TrollVideoObject::~TrollVideoObject()
			{
			}

			//void TrollVideoObject::SetMuseMediaEntity( MediaEntity * p_me)
			//{
			//	m_museMediaEntity = p_me;
			//}

			void TrollVideoObject::SetAttenuation( float p_att )
			{
				m_attenuation = p_att;
			}

			void TrollVideoObject::SetMuseVideoObject( VideoObject * p_videoObject )
			{
				m_videoOverlay = NULL;
				m_videoObject = p_videoObject;
			}

			void TrollVideoObject::SetMuseVideoOverlay( VideoOverlay * p_videoOverlay )
			{
				m_videoOverlay = p_videoOverlay;
				m_videoObject = NULL;
			}

			void TrollVideoObject::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "\tuse_video " ) + m_video->GetObjectName() + wxT( "\n\t{\n" ) );
				wxString l_tmp;
				l_tmp.Printf( wxT( "%.2f" ), m_attenuation );
				l_tmp.Replace( wxT( "," ), wxT( "." ) );
				p_stream.WriteString( wxT( "\t\tvolume " ) + l_tmp + wxT( "\n\t}\n" ) );
				l_tmp.clear();
			}

			GUI::Properties::ObjectProperty * TrollVideoObject::DoCreateProperties()
			{
				return new GUI::Properties::VideoObjectProperties( *this );
			}
		}
	}
}
