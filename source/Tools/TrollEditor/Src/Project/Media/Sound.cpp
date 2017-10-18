#include "Sound.h"

#include "SoundObject.h"
#include "Project/3D/SceneNode.h"
#include "GUI/Properties/Project/SoundProperties.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"
#include "Project/Project.h"
#include "Project/Scene.h"

namespace Troll
{
	using namespace GUI;

	namespace ProjectComponents
	{
		namespace Media
		{
			TrollSound::TrollSound( wxString const & p_name, wxString const & p_fileName )
				: MediaObject( p_name, p_fileName, ttSound )
				, m_soundType( Sfx )
				, m_isGlobal( true )
				, m_museSound( NULL )
				, m_globalInstance( NULL )
			{
			}

			TrollSound::~TrollSound()
			{
				m_globalInstance = NULL;
				m_museSound = NULL;
				m_objects.clear();
			}

			TrollSoundObject * TrollSound::AddObject( ProjectComponents::Objects3D::TrollSceneNode * p_node )
			{
				if ( !p_node )
				{
					return NULL;
				}

				wxString l_name = p_node->GetObjectName();
				auto && l_it = m_objects.find( l_name );

				if ( l_it != m_objects.end() )
				{
					return l_it->second;
				}

				TrollSoundObject * l_result = new TrollSoundObject( l_name, m_fileName );
				l_result->SetSound( this );
				m_objects[l_name] = l_result;
				return l_result;
			}

			void TrollSound::SetMuseSound( Elypse::Media::SoundObject * p_sound )
			{
				m_museSound = p_sound;
				m_soundType = static_cast< TrollSoundType >( m_museSound->GetSoundType() );
				m_isGlobal = m_museSound->IsGlobal();

				if ( m_isGlobal )
				{
					m_globalInstance = new SoundInstance();
					Elypse::Media::SoundInstance * l_global = m_museSound->GetGlobal();
					m_globalInstance->m_attenuation = 1.0f - l_global->GetVolumePercent();
					m_globalInstance->m_fadeInTime = l_global->GetFadeIn();
					m_globalInstance->m_fadeOutTime = l_global->GetFadeOut();
					m_globalInstance->m_looped = l_global->IsLooped();
					m_globalInstance->m_timer = l_global->GetTimerTime();
				}
			}

			void TrollSound::SetUrl( wxString const & p_filePath )
			{
				std::cout << "TrollSound::SetUrl - " << p_filePath << "\n";
				m_filePath = p_filePath;

				if ( m_museSound != NULL )
				{
					m_museSound->SetSoundUrl( make_string( p_filePath ) );
				}
			}

			void TrollSound::SetSoundType( TrollSoundType p_type )
			{
				m_soundType = p_type;
				m_museSound->SetSoundType( static_cast < SoundType >( p_type ) );
			}

			void TrollSound::SetLocal()
			{
				m_isGlobal = false;
				m_globalInstance = new SoundInstance;
				m_museSound->SetLocal( make_string( wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetName() ) );
			}

			void TrollSound::SetGlobalAttenuation( float p_att )
			{
				m_globalInstance->m_attenuation = p_att;
				m_museSound->GetGlobal()->SetVolumePercent( p_att );
			}

			void TrollSound::SetGlobalLooped( bool p_looped )
			{
				m_globalInstance->m_looped = p_looped;
				m_museSound->GetGlobal()->SetLooped( p_looped );
			}

			void TrollSound::SetGlobalTimer( float p_time )
			{
				m_globalInstance->m_timer = p_time;
				m_museSound->GetGlobal()->SetTimerTime( p_time );
			}

			void TrollSound::SetGlobalFadeIn( float p_time )
			{
				m_globalInstance->m_fadeInTime = p_time;
				m_museSound->GetGlobal()->SetFadeIn( p_time );
			}

			void TrollSound::SetGlobalFadeOut( float p_time )
			{
				m_globalInstance->m_fadeOutTime = p_time;
				m_museSound->GetGlobal()->SetFadeOut( p_time );
			}

			void TrollSound::Write( wxTextOutputStream & p_stream )
			{
				wxString l_tmp;
				p_stream.WriteString( wxT( "sound " ) + m_name + wxT( "\n{\n" ) );
				p_stream.WriteString( wxT( "\turl " ) + m_filePath + wxT( "\n" ) );
				p_stream.WriteString( wxT( "\ttype " ) + ( m_soundType == Sfx ? wxString( wxT( "sfx" ) ) : wxString( wxT( "music" ) ) ) + wxT( "\n" ) );
				p_stream.WriteString( wxT( "\tglobal " ) + ( m_isGlobal ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n" ) );

				if ( m_isGlobal && m_globalInstance != NULL )
				{
					l_tmp.Printf( wxT( "%.2f" ), m_globalInstance->m_attenuation );
					l_tmp.Replace( wxT( "," ), wxT( "." ) );
					p_stream.WriteString( wxT( "\tattenuation " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
					p_stream.WriteString( wxT( "\tlooped " ) + ( m_globalInstance->m_looped ? wxString( wxT( "true" ) ) : wxString( wxT( "false" ) ) ) + wxT( "\n" ) );

					if ( m_globalInstance->m_timer > 0.0 )
					{
						l_tmp.Printf( wxT( "%.2f" ), m_globalInstance->m_timer );
						l_tmp.Replace( wxT( "," ), wxT( "." ) );
						p_stream.WriteString( wxT( "\ttimer " ) + l_tmp + wxT( "\n" ) );
						l_tmp.clear();
					}

					if ( m_globalInstance->m_fadeInTime > 0.0 )
					{
						l_tmp.Printf( wxT( "%.2f" ), m_globalInstance->m_fadeInTime );
						l_tmp.Replace( wxT( "," ), wxT( "." ) );
						p_stream.WriteString( wxT( "\tfade_in " ) + l_tmp + wxT( "\n" ) );
						l_tmp.clear();
					}

					if ( m_globalInstance->m_fadeOutTime > 0.0 )
					{
						l_tmp.Printf( wxT( "%.2f" ), m_globalInstance->m_fadeOutTime );
						l_tmp.Replace( wxT( "," ), wxT( "." ) );
						p_stream.WriteString( wxT( "\tfade_out " ) + l_tmp + wxT( "\n" ) );
						l_tmp.clear();
					}
				}

				p_stream.WriteString( wxT( "}\n\n" ) );
			}

			GUI::Properties::ObjectProperty * TrollSound::DoCreateProperties()
			{
				return new GUI::Properties::SoundProperties( *this );
			}
		}
	}
}
