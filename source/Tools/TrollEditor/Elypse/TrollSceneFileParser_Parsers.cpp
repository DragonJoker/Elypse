/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Elypse/TrollSceneFileParser_Parsers.h"

#include "Elypse/TrollSceneFileParser.h"

#include "Project/3D/Entity.h"
#include "Project/3D/Mesh.h"
#include "Project/3D/Light.h"
#include "Project/3D/Camera.h"
#include "Project/3D/SceneNode.h"
#include "Project/2D/Overlay.h"
#include "Project/Temporal/AnimationGroup.h"
#include "Project/Temporal/AnimatedObject.h"
#include "Project/Temporal/Animation.h"
#include "Project/Temporal/Sequence.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Media/Sound.h"
#include "Project/Media/SoundObject.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"
#include "Project/Project.h"
#include "Project/Scene.h"

#include "TrollEditorApplication.h"
#include "GUI/MainFrame.h"
#include "GUI/ObjectsTree.h"
#include "GUI/TimeSequence/TimeLinePanel.h"
#include "GUI/TimeSequence/TimeLineContainer.h"

#define DEFINE_SCENE_FILE_TROLL_PARSER(X) bool X( String & p_params , Context * p_context)

namespace Troll
{
	namespace ElypseRW
	{
		wxFile * g_unusedLines = nullptr;

		ProjectComponents::Object * g_currentObject = nullptr;
		ProjectComponents::Objects2D::TrollOverlay * g_currentOverlay = nullptr;
		std::shared_ptr< ProjectComponents::Temporal::TrollSequence > g_currentSequence = nullptr;
		ProjectComponents::Temporal::TrollContinuousEvent * g_continuousEvent = nullptr;
		ProjectComponents::Temporal::TrollAnimationGroup * g_currentAnimationGroup = nullptr;
		ProjectComponents::Temporal::TrollAnimatedObject * g_currentAnimatedObject = nullptr;

		wxString g_targetType;
		wxString g_target;

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootAmbientLight )
		{
			if ( !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetCurrentSceneFile()->m_ambientLight = SceneFileParser::Parser_ColourValue( p_params );
			}

			return Parser_RootAmbientLight( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootShadows )
		{
			if ( !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_params == "texture" )
				{
					wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetCurrentSceneFile()->m_shadowType = ProjectComponents::stTextureAdditive;
				}
				else if ( p_params == "stencil" )
				{
					wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetCurrentSceneFile()->m_shadowType = ProjectComponents::stStencilAdditive;
				}
				else if ( p_params == "none" )
				{
					wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetCurrentSceneFile()->m_shadowType = ProjectComponents::stNone;
				}
			}

			return Parser_RootShadows( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootForceShadows )
		{
			if ( !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetCurrentSceneFile()->m_forceShadows = ( p_params == "true" );
			}

			return Parser_RootForceShadows( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootObject )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				//std::cout << "TrollParser_RootObject - " << g_fileName << "\n";
				g_currentObject = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddEntity( make_wxString( p_params ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( g_currentObject );
				}
			}

			return Parser_RootObject( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootSound )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_currentObject = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddSound( make_wxString( p_params ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( g_currentObject );
				}
			}

			return Parser_RootSound( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPlaylist )
		{
			return Parser_RootPlaylist( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootVideo )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				ProjectComponents::Media::TrollVideo * l_video = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddVideo( make_wxString( p_params ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() || wxGetApp().GetMainFrame()->IsEditingOverlays() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( reinterpret_cast< ProjectComponents::Object * >( l_video ) );
				}
			}

			return Parser_RootVideo( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPub )
		{
			return Parser_RootPub( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPortal )
		{
			return Parser_RootPortal( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootMirror )
		{
			return Parser_RootMirror( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootSceneNode )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddSceneNode( make_wxString( p_params ), g_fileName );
			}

			return Parser_RootSceneNode( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootAnimationGroup )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_currentAnimationGroup = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddAnimationGroup( make_wxString( p_params ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( g_currentAnimationGroup );
				}
			}

			return Parser_RootAnimationGroup( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootLight )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_currentObject = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddLight( make_wxString( p_params ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( g_currentObject );
				}
			}

			return Parser_RootLight( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootCamera )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_currentObject = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddCamera( make_wxString( p_params ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( g_currentObject );
				}
			}

			return Parser_RootCamera( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootOverlay )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				String l_parentName;
				l_parentName.clear();
				g_currentOverlay = nullptr;

				if ( p_context->section != SS_NONE && p_context->overlay != nullptr )
				{
					l_parentName = p_context->overlay->GetName();
				}

				g_currentOverlay = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddOverlay( make_wxString( p_params ), make_wxString( l_parentName ), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingOverlays() && g_currentOverlay != nullptr )
				{
					g_currentOverlay = wxGetApp().GetMainFrame()->GetObjectsList()->AddOverlay( g_currentOverlay, make_wxString( l_parentName ) );
				}
			}

			return Parser_RootOverlay( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayBorderMaterial )
		{
			if ( g_currentOverlay != nullptr )
			{
				g_currentOverlay->SetBorderMaterial( make_wxString( p_params ), false );
			}

			return Parser_OverlayBorderMaterial( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayBorderSize )
		{
			if ( g_currentOverlay != nullptr )
			{
				const Vector4 & l_vector = SceneFileParser::Parser_Vector4Value( p_params );
				wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetOverlay( make_wxString( p_context->overlay->GetName() ) )->SetBorderSize( l_vector.x, l_vector.y, l_vector.z, l_vector.w, false );
			}

			return Parser_OverlayBorderSize( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootUniverse )
		{
			return Parser_RootUniverse( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootParticleSystem )
		{
			return Parser_RootParticleSystem( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPostEffect )
		{
			return Parser_RootPostEffect( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPhysics )
		{
			return Parser_RootPhysics( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPhysicsGroup )
		{
			return Parser_RootPhysicsGroup( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootPhysicsSimulation )
		{
			return Parser_RootPhysicsSimulation( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootCollision )
		{
			return Parser_RootCollision( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootAutoPhysics )
		{
			return Parser_RootAutoPhysics( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootBillboardGroup )
		{
			return Parser_RootBillboardGroup( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootSequence )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				p_context->section = SS_SEQUENCE;
				g_currentSequence = std::make_shared< ProjectComponents::Temporal::TrollSequence >( make_wxString( p_params ), g_fileName );
				p_context->sequence = g_currentSequence;
				//if ( ! wxGetApp().GetMainFrame()->IsEditingTimeLine())
				//{
				//	p_context->sequenceManager->AddElement( p_context->sequence);
				//}
			}
			else
			{
				return Parser_RootSequence( p_params, p_context );
			}

			return true;
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_AnimationGroupElement )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_currentObject = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetEntity( make_wxString( p_params ) );
				g_currentAnimatedObject = g_currentAnimationGroup->CreateAnimatedObject( g_currentObject, g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingScene() )
				{
					wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( g_currentAnimatedObject );
				}
			}

			return Parser_AnimationGroupElement( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_AnimationGroupChainAnimation )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				const StringVector & splitCmd = StringUtil::split( p_params, " \t" );
				g_currentAnimationGroup->AddChainedAnimations( make_wxString( splitCmd[0] ), make_wxString( splitCmd[1] ) );
			}

			return Parser_AnimationGroupChainAnimation( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_ObjectEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->object != nullptr )
				{
					String l_parentName;

					if ( p_context->object->getParentSceneNode() )
					{
						l_parentName = p_context->object->getParentSceneNode()->getName();
					}

					ProjectComponents::Objects3D::TrollEntity * l_entity = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetEntity( make_wxString( p_context->object->getName() ) );
					ProjectComponents::Objects3D::TrollSceneNode * l_parent = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );
					l_entity->SetOgreEntity( p_context->object );
					l_entity->AttachTo( l_parent );
					Mesh * l_ogreMesh = p_context->object->getMesh().get();
					ProjectComponents::Objects3D::TrollMesh * l_mesh = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddMesh( make_wxString( l_ogreMesh->getName() ) );
					auto l_nbSubMesh = l_ogreMesh->getNumSubMeshes();

					for ( auto i = 0u; i < l_nbSubMesh; i++ )
					{
						l_mesh->AddSubMaterial( make_wxString( l_ogreMesh->getSubMesh( i )->getMaterialName() ) );
					}

					l_entity->SetMesh( l_mesh );
				}
			}

			return Parser_ObjectEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->overlay != nullptr && g_currentOverlay )
				{
					//std::cout << "SceneFileParser_Parsers::OverlayEnd - " << l_overlay->GetName().c_str() << "\n";
					g_currentOverlay->SetMuseOverlay( p_context->overlay );
					g_currentOverlay = g_currentOverlay->GetParent();
				}
			}

			return Parser_OverlayEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SubMeshEnd )
		{
			return Parser_SubMeshEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_ObjectPhysicsEnd )
		{
			return Parser_ObjectPhysicsEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_PhysicsEnd )
		{
			return Parser_PhysicsEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_PhysicsSimulationEnd )
		{
			return Parser_PhysicsSimulationEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_BoundingShapeEnd )
		{
			return Parser_BoundingShapeEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SceneNodeEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->sceneNode != nullptr )
				{
					String l_parentName;

					if ( p_context->sceneNode->getParentSceneNode() )
					{
						l_parentName = p_context->sceneNode->getParentSceneNode()->getName();
					}

					std::cout << "TrollParser_SceneNodeEnd - " << p_context->sceneNode->getName() << " - parent : " << l_parentName << "\n";
					ProjectComponents::Objects3D::TrollSceneNode * l_node = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSceneNode( make_wxString( p_context->sceneNode->getName() ) );

					if ( l_node != nullptr )
					{
						ProjectComponents::Objects3D::TrollSceneNode * l_parent = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );
						l_node->SetOgreNode( p_context->sceneNode );
						wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AttachNode( l_node, l_parent );

						if ( wxGetApp().GetMainFrame()->IsEditingScene() )
						{
							wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneNode( make_wxString( p_context->sceneNode->getName() ), make_wxString( l_parentName ) );
						}
					}
				}
			}

			return Parser_SceneNodeEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_LightEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->light != nullptr )
				{
					String l_parentName;

					if ( p_context->light->getParentSceneNode() )
					{
						l_parentName = p_context->light->getParentSceneNode()->getName();
					}

					ProjectComponents::Objects3D::TrollLight * l_light = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetLight( make_wxString( p_context->light->getName() ) );
					ProjectComponents::Objects3D::TrollSceneNode * l_parent = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );

					if ( l_light != nullptr )
					{
						l_light->SetOgreLight( p_context->light );
						l_light->AttachTo( l_parent );
					}
				}
			}

			return Parser_LightEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_CameraEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->camera != nullptr )
				{
					String l_parentName;

					if ( p_context->camera->getParentSceneNode() )
					{
						l_parentName = p_context->camera->getParentSceneNode()->getName();
					}

					ProjectComponents::Objects3D::TrollCamera * l_camera = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetCamera( make_wxString( p_context->camera->getName() ) );
					ProjectComponents::Objects3D::TrollSceneNode * l_parent = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );

					if ( l_camera != nullptr )
					{
						l_camera->SetOgreCamera( p_context->camera );
						l_camera->AttachTo( l_parent );
					}
				}
			}

			return Parser_CameraEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SoundURL )
		{
			if ( !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				ProjectComponents::Media::TrollSound * l_sound = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSound( make_wxString( p_context->soundObject->GetName() ) );
				l_sound->SetUrl( make_wxString( p_params ) );
			}

			return Parser_SoundURL( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SoundEnd )
		{
			SoundObject * l_museSound = p_context->soundObject;
			bool l_result = Parser_SoundEnd( p_params, p_context );

			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( l_museSound != nullptr )
				{
					ProjectComponents::Media::TrollSound * l_sound = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSound( make_wxString( l_museSound->GetName() ) );
					l_sound->SetMuseSound( l_museSound );
				}
			}

			return l_result;
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SoundNodeEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->sceneNode != nullptr && p_context->soundInstance != nullptr && p_context->soundObject != nullptr )
				{
					ProjectComponents::Media::TrollSound * l_sound = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSound( make_wxString( p_context->soundObject->GetName() ) );

					if ( l_sound != nullptr )
					{
						ProjectComponents::Objects3D::TrollSceneNode * l_node = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetSceneNode( make_wxString( p_context->sceneNode->getName() ) );
						ProjectComponents::Media::TrollSoundObject * l_sndObject = l_sound->AddObject( l_node );
						l_sndObject->SetMuseInstance( p_context->soundInstance );
						l_node->SetSound( l_sndObject );

						if ( wxGetApp().GetMainFrame()->IsEditingScene() )
						{
							wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( reinterpret_cast< ProjectComponents::Object * >( l_sndObject ) );
						}
					}
					else
					{
						std::cout << "TrollScenFileParser::TrollParser_SoundNodeEnd - nullptr Sound\n";
					}
				}
			}

			return Parser_SoundNodeEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_PlaylistEnd )
		{
			return Parser_PlaylistEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_VideoEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				ProjectComponents::Media::TrollVideo * l_video = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetVideo( make_wxString( p_context->videoObject->GetName() ) );
				l_video->SetMuseVideo( p_context->videoObject );
			}

			return Parser_VideoEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_VideoObjectEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->object != nullptr )
				{
					ProjectComponents::Media::TrollVideo * l_video = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetVideo( make_wxString( p_context->videoObject->GetName() ) );
					ProjectComponents::Objects3D::TrollEntity * l_object = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetEntity( make_wxString( p_context->object->getName() ) );
					ProjectComponents::Media::TrollVideoObject * l_videoObject = l_video->AddObject( l_object );
					l_object->SetVideo( l_videoObject );

					if ( wxGetApp().GetMainFrame()->IsEditingScene() )
					{
						wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( reinterpret_cast< ProjectComponents::Object * >( l_videoObject ) );
					}
				}
			}

			return Parser_VideoObjectEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayVideo )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->overlay != nullptr )
				{
					VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_params );

					if ( l_object != nullptr )
					{
						ProjectComponents::Media::TrollVideo * l_video = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetVideo( make_wxString( l_object->GetName() ) );
						ProjectComponents::Objects2D::TrollOverlay * l_overlay = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetOverlay( make_wxString( p_context->overlay->GetName() ) );

						if ( l_overlay != nullptr )
						{
							ProjectComponents::Media::TrollVideoObject * l_videoObject = l_video->AddObject( l_overlay );
							l_overlay->SetVideo( l_videoObject );

							if ( wxGetApp().GetMainFrame()->IsEditingOverlays() )
							{
								wxGetApp().GetMainFrame()->GetObjectsList()->AddSceneObject( reinterpret_cast< ProjectComponents::Object * >( l_videoObject ) );
							}
						}
					}
				}
			}

			return Parser_OverlayVideo( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_VideoOverlayEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				if ( p_context->overlay != nullptr )
				{
					ProjectComponents::Objects2D::TrollOverlay * l_overlay = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetOverlay( make_wxString( p_context->overlay->GetName() ) );

					if ( l_overlay != nullptr )
					{
						ProjectComponents::Media::TrollVideoObject * l_videoObject = l_overlay->GetVideo();

						if ( l_videoObject != nullptr )
						{
							l_videoObject->SetMuseVideoObject( VideoManager::GetSingletonPtr()->GetVideoObject( make_string( l_videoObject->GetVideo()->GetObjectName() ) ) );
						}
					}
				}
			}

			return Parser_VideoOverlayEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_PubEnd )
		{
			return Parser_PubEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_MirrorEnd )
		{
			return Parser_MirrorEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SceneEnd )
		{
			return Parser_SceneEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_ParticleSystemEnd )
		{
			return Parser_ParticleSystemEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_PortalEnd )
		{
			return Parser_PortalEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_BillboardEnd )
		{
			return Parser_BillboardEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_ClickableShapeEnd )
		{
			return Parser_ClickableShapeEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_AnimationEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				ProjectComponents::Temporal::TrollAnimation * l_animation = new ProjectComponents::Temporal::TrollAnimation( make_wxString( p_context->animation->GetName() ), g_fileName );
				l_animation->SetMuseAnimation( p_context->animation );
				g_currentAnimatedObject->AddAnimation( l_animation );
			}

			return Parser_AnimationEnd( p_params, p_context );
		}



		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceEvent )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				const StringVector & l_split = StringUtil::split( p_params, ", \t", 2 );
				BasePonctualEvent * l_event;

				if ( l_split.size() > 2 )
				{
					Ogre::StringVector l_vector = Ogre::StringUtil::split( l_split[2], ", \t" );
					l_event = p_context->sequenceManager->CreatePonctualEvent( l_split[1], StringArray( l_vector.begin(), l_vector.end() ) );
					g_currentSequence->AddPonctualEvent( l_event, g_targetType, g_target, StringConverter::parseReal( l_split[0] ), make_wxString( l_split[2] ), make_wxString( l_split[1] ), g_fileName );
				}
				else
				{
					StringArray l_tmp;
					l_event = p_context->sequenceManager->CreatePonctualEvent( l_split[1], l_tmp );
					g_currentSequence->AddPonctualEvent( l_event, g_targetType, g_target, StringConverter::parseReal( l_split[0] ), wxT( "" ), make_wxString( l_split[1] ), g_fileName );
				}

				if ( l_event == nullptr )
				{
					PARSING_ERROR( "Parsing Error : TrollParser_SequenceEvent -> event factory not found or parameters unsatisfactory" );
					return false;
				}

				l_event->SetTarget( std::move( p_context->sequenceTarget ) );
				return false;
			}

			return Parser_SequenceEvent( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTarget )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				const StringVector & l_split = StringUtil::split( p_params, ", \t", 1 );
				g_targetType = make_wxString( l_split[0] );
				g_target = make_wxString( l_split[1] );

				if ( g_continuousEvent != nullptr )
				{
					g_continuousEvent->SetTarget( g_targetType, g_target );
				}
			}

			return Parser_SequenceTarget( p_params, p_context );
		}


		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrack )
		{
			Parser_SequenceTrack( p_params, p_context );

			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_continuousEvent = new ProjectComponents::Temporal::TrollContinuousEvent( g_currentSequence.get(), p_context->sequenceTrack, make_wxString( p_params ), g_fileName );
			}

			return true;
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddSequence( g_currentSequence.get(), g_fileName );

				if ( wxGetApp().GetMainFrame()->IsEditingTimeLine() )
				{
					wxGetApp().GetMainFrame()->GetTimelineEditor()->AddSequence( g_currentSequence.get() );
					wxGetApp().GetMainFrame()->GetTimeLineContainer()->SetScrollbars( 20, 20, wxGetApp().GetMainFrame()->GetTimelineEditor()->GetClientSize().x / 20, wxGetApp().GetMainFrame()->GetTimelineEditor()->GetClientSize().y / 20, 0, 0 );
				}
			}

			return Parser_SequenceEnd( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrackKey )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				const StringVector & l_split = StringUtil::split( p_params, ", \t", 1 );
				g_continuousEvent->AddKeyFrame( StringConverter::parseReal( l_split[0] ), make_wxString( l_split[1] ) );
			}

			return Parser_SequenceTrackKey( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrackInterpolation )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_continuousEvent->SetInterpolation( make_wxString( p_params ) );
			}

			return Parser_SequenceTrackInterpolation( p_params, p_context );
		}

		DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrackEnd )
		{
			if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && !wxGetApp().GetMainFrame()->IsTestingScene() )
			{
				g_currentSequence->AddContinuousEvent( g_continuousEvent );
				g_continuousEvent = nullptr;
			}

			return Parser_SequenceTrackEnd( p_params, p_context );
		}
	}
}
