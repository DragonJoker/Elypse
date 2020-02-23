/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "SceneFile.h"

#include "Scene.h"
#include "Project.h"
#include "3D/Entity.h"
#include "3D/Mesh.h"
#include "3D/SceneNode.h"
#include "3D/Light.h"
#include "3D/Camera.h"
#include "2D/Overlay.h"
#include "Media/Sound.h"
#include "Media/SoundObject.h"
#include "Media/Video.h"
#include "Media/VideoObject.h"
#include "Temporal/AnimationGroup.h"
#include "Temporal/Sequence.h"
#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
#include "TrollEditorApplication.h"

using namespace Troll::GUI;
using namespace General::Utils;

namespace Troll
{
	namespace ProjectComponents
	{
		SceneFile::SceneFile()
			: m_forceShadows( false )
			, m_shadowType( stNone )
			, m_ambientLight( ColourValue::White )
		{
		}

		SceneFile::~SceneFile()
		{
			General::Utils::map::deleteAll( m_entities );
			General::Utils::map::deleteAll( m_allOverlays );
			m_overlayGroups.clear();
			General::Utils::map::deleteAll( m_lights );
			General::Utils::map::deleteAll( m_cameras );
			General::Utils::map::deleteAll( m_allSceneNodes );
			m_sceneNodes.clear();
			General::Utils::map::deleteAll( m_animationGroups );
			General::Utils::map::deleteAll( m_sounds );
			General::Utils::map::deleteAll( m_videos );
			General::Utils::map::deleteAll( m_sequences );
		}

		Objects3D::TrollEntity * SceneFile::GetEntity( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_entities, p_name );
		}

		Objects2D::TrollOverlay * SceneFile::GetOverlay( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_allOverlays, p_name );
		}

		Objects3D::TrollSceneNode * SceneFile::GetSceneNode( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_allSceneNodes, p_name );
		}

		Objects3D::TrollLight * SceneFile::GetLight( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_lights, p_name );
		}

		Objects3D::TrollCamera * SceneFile::GetCamera( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_cameras, p_name );
		}

		Temporal::TrollAnimationGroup * SceneFile::GetAnimationGroup( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_animationGroups, p_name );
		}

		Media::TrollSound * SceneFile::GetSound( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_sounds, p_name );
		}

		Media::TrollVideo * SceneFile::GetVideo( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_videos, p_name );
		}

		Temporal::TrollSequence * SceneFile::GetSequence( wxString const & p_name )const
		{
			return General::Utils::map::findOrNull( m_sequences, p_name );
		}

		void SceneFile::DoWriteUnusedFilePieces( wxTextOutputStream * p_textStream )const
		{
			String l_strFilePath = make_string( wxGetApp().GetMainFrame()->GetAppDir() + m_fileName + wxT( "Unused.emscene" ) );

			if ( General::Files::FileExists( l_strFilePath ) )
			{
				FILE * l_file = fopen( l_strFilePath.c_str(), "rb" );

				if ( l_file != nullptr )
				{
					char l_char;

					while ( !feof( l_file ) )
					{
						if ( fread( &l_char, sizeof( char ), 1, l_file ) == 1 )
						{
							if ( l_char == '\r' )
							{
								if ( fread( &l_char, sizeof( char ), 1, l_file ) < 1 )
								{
									l_char = 0;
								}
							}

							//if (l_char != '{' && l_char != '}')
							//{
							//	std::cout << "(" << (int)l_char << ")" << l_char;
							//}

							char l_szTmp[2] = { l_char, 0 };
							p_textStream->WriteString( make_wxString( l_szTmp ) );
						}
					}

					fclose( l_file );
				}
			}
		}

		void SceneFile::Write( wxString const & p_path )const
		{
			LogDebug( wxT( "Writing SceneFile " ) + p_path + m_fileName );
			wxFileOutputStream l_output( p_path + m_fileName );

			if ( l_output.IsOk() )
			{
				wxTextOutputStream l_textStream( l_output );
				wxString l_tmp;

				if ( m_ambientLight != ColourValue::White )
				{
					l_tmp.Printf( wxT( "%.2f %.2f %.2f 0" ), m_ambientLight.r, m_ambientLight.g, m_ambientLight.b );
					l_tmp.Replace( wxT( "," ), wxT( "." ) );
					l_textStream.WriteString( wxT( "ambient_light " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
				}

				if ( m_shadowType != stNone )
				{
					l_tmp = ( m_shadowType == stStencilAdditive ? wxT( "stencil" ) : wxT( "texture" ) );
					l_textStream.WriteString( wxT( "shadows " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
				}

				if ( m_forceShadows )
				{
					l_textStream.WriteString( wxT( "force_shadows true\n" ) );
				}

				l_textStream.WriteString( wxT( "\n" ) );

				if ( m_sounds.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// SOUNDS\n\n" ) );
					General::Utils::map::cycle( m_sounds, &Media::TrollSound::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_videos.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// VIDEOS\n\n" ) );
					General::Utils::map::cycle( m_videos, &Media::TrollVideo::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_overlayGroups.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// OVERLAYS\n\n" ) );
					General::Utils::map::cycle( m_overlayGroups, &Objects2D::TrollOverlay::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_sceneNodes.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// SCENE NODES\n\n" ) );
					General::Utils::map::cycle( m_sceneNodes, &Objects3D::TrollSceneNode::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_entities.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// OBJECTS\n\n" ) );
					General::Utils::map::cycle( m_entities, &Objects3D::TrollEntity::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_lights.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// LIGHTS\n\n" ) );
					General::Utils::map::cycle( m_lights, &Objects3D::TrollLight::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_cameras.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// CAMERAS\n\n" ) );
					General::Utils::map::cycle( m_cameras, &Objects3D::TrollCamera::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_animationGroups.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// ANIMATION GROUPS\n\n" ) );
					General::Utils::map::cycle( m_animationGroups, &Temporal::TrollAnimationGroup::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				if ( m_sequences.size() > 0 )
				{
					l_textStream.WriteString( wxT( "// SEQUENCES\n\n" ) );
					General::Utils::map::cycle( m_sequences, &Temporal::TrollSequence::Write, l_textStream );
					l_textStream.WriteString( wxT( "\n" ) );
				}

				l_output.Close();
			}
		}
	}
}
