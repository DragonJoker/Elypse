#include "Video.h"

#include "VideoObject.h"
#include "GUI/Properties/Project/VideoProperties.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			TrollVideo::TrollVideo( wxString const & p_name, wxString const & p_fileName )
				: MediaObject( p_name, p_fileName, ttVideo )
				, m_filePath( wxEmptyString )
				, m_looped( false )
				, m_museVideo( NULL )
			{
			}

			TrollVideo::~TrollVideo()
			{
				General::Utils::map::deleteAll( m_objects );
			}

			TrollVideoObject * TrollVideo::AddObject( Object * p_object )
			{
				if ( !p_object )
				{
					return NULL;
				}

				wxString l_name = p_object->GetObjectName();
				TrollVideoObjectMap::iterator l_it = m_objects.find( l_name );

				if ( l_it != m_objects.end() )
				{
					return l_it->second;
				}

				TrollVideoObject * l_result = new TrollVideoObject( l_name, m_fileName );
				l_result->SetVideo( this );
				m_objects.insert( std::make_pair( l_name, l_result ) );
				return l_result;
			}

			void TrollVideo::SetMuseVideo( VideoObject * p_museVideo )
			{
				m_museVideo = p_museVideo;
				m_filePath = make_wxString( m_museVideo->GetMediaUrl() );
				m_looped = m_museVideo->IsLooped();
				m_enabled = m_museVideo->IsAutoInitialised();
			}

			void TrollVideo::SetUrl( wxString const & p_url )
			{
				m_filePath = p_url;
				m_museVideo->SetUrl( make_string( p_url ) );
			}

			void TrollVideo::SetLooped( bool p_looped )
			{
				m_looped = p_looped;
				m_museVideo->SetLooped( p_looped );
			}

			void TrollVideo::SetEnabled( bool p_enabled )
			{
				m_enabled = p_enabled;
				m_museVideo->SetAutoInitialise( p_enabled );
			}

			void TrollVideo::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "video " ) + m_name + wxT( "\n{\n" ) );
				p_stream.WriteString( wxT( "\turl " ) + m_filePath + wxT( "\n" ) );
				p_stream.WriteString( wxT( "\tlooped " ) + ( m_looped ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n" ) );
				p_stream.WriteString( wxT( "\tenabled " ) + ( m_enabled ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n}\n\n" ) );
			}

			GUI::Properties::ObjectProperty * TrollVideo::DoCreateProperties()
			{
				return new GUI::Properties::VideoProperties( *this );
			}
		}
	}
}
