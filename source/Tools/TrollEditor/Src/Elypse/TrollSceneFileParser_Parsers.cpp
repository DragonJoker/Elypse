/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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

#include "GUI/MainFrame.h"
#include "GUI/ObjectsTree.h"
#include "GUI/TimeSequence/TimeLinePanel.h"
#include "GUI/TimeSequence/TimeLineContainer.h"

#define DEFINE_SCENE_FILE_TROLL_PARSER(X) bool X( String & p_params , Context * p_context)

BEGIN_TROLL_ELYPSE_NAMESPACE
{
	wxFile * g_unusedLines = NULL;

	TROLL_PROJECT_NAMESPACE::Object * g_currentObject = NULL;
	TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * g_currentOverlay = NULL;
	TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * g_currentSequence = NULL;
	TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * g_continuousEvent = NULL;
	TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollAnimationGroup * g_currentAnimationGroup = NULL;
	TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollAnimatedObject * g_currentAnimatedObject = NULL;

	wxString g_targetType;
	wxString g_target;

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootAmbientLight )
	{
		if ( ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCurrentSceneFile()->m_ambientLight = SceneFileParser::Parser_ColourValue( p_params );
		}

		return Parser_RootAmbientLight( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootShadows )
	{
		if ( ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_params == "texture" )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCurrentSceneFile()->m_shadowType = TROLL_PROJECT_NAMESPACE::stTextureAdditive;
			}
			else if ( p_params == "stencil" )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCurrentSceneFile()->m_shadowType = TROLL_PROJECT_NAMESPACE::stStencilAdditive;
			}
			else if ( p_params == "none" )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCurrentSceneFile()->m_shadowType = TROLL_PROJECT_NAMESPACE::stNone;
			}
		}

		return Parser_RootShadows( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootForceShadows )
	{
		if ( ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCurrentSceneFile()->m_forceShadows = ( p_params == "true" );
		}

		return Parser_RootForceShadows( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootObject )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
	//		std::cout << "TrollParser_RootObject - " << g_fileName << "\n";
			g_currentObject = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddEntity( make_wxString( p_params ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( g_currentObject );
			}
		}

		return Parser_RootObject( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootSound )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_currentObject = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddSound( make_wxString( p_params ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( g_currentObject );
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideo * l_video = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddVideo( make_wxString( p_params ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() || GUI::MainFrame::GetInstance()->IsEditingOverlays() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( reinterpret_cast< TROLL_PROJECT_NAMESPACE::Object * >( l_video ) );
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddSceneNode( make_wxString( p_params ), g_fileName );
		}

		return Parser_RootSceneNode( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootAnimationGroup )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_currentAnimationGroup = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddAnimationGroup( make_wxString( p_params ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( g_currentAnimationGroup );
			}
		}

		return Parser_RootAnimationGroup( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootLight )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_currentObject = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddLight( make_wxString( p_params ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( g_currentObject );
			}
		}

		return Parser_RootLight( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootCamera )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_currentObject = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddCamera( make_wxString( p_params ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( g_currentObject );
			}
		}

		return Parser_RootCamera( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_RootOverlay )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			String l_parentName;
			l_parentName.clear();
			g_currentOverlay = NULL;

			if ( p_context->section != SS_NONE && p_context->overlay != NULL )
			{
				l_parentName = p_context->overlay->GetName();
			}

			g_currentOverlay = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddOverlay( make_wxString( p_params ), make_wxString( l_parentName ), g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && g_currentOverlay != NULL )
			{
				g_currentOverlay = GUI::MainFrame::GetInstance()->GetObjectsList()->AddOverlay( g_currentOverlay, make_wxString( l_parentName ) );
			}
		}

		return Parser_RootOverlay( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayBorderMaterial )
	{
		if ( g_currentOverlay != NULL )
		{
			g_currentOverlay->SetBorderMaterial( make_wxString( p_params ), false );
		}

		return Parser_OverlayBorderMaterial( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayBorderSize )
	{
		if ( g_currentOverlay != NULL )
		{
			const Vector4 & l_vector = SceneFileParser::Parser_Vector4Value( p_params );
			GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_context->overlay->GetName() ) )->SetBorderSize( l_vector.x, l_vector.y, l_vector.z, l_vector.w, false );
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			p_context->section = SS_SEQUENCE;
			g_currentSequence = new TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence( make_wxString( p_params ), g_fileName );
			p_context->sequence = static_cast <Sequence *>( g_currentSequence );
			//if ( ! GUI::MainFrame::GetInstance()->IsEditingTimeLine())
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_currentObject = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetEntity( make_wxString( p_params ) );
			g_currentAnimatedObject = g_currentAnimationGroup->CreateAnimatedObject( g_currentObject, g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
			{
				GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( g_currentAnimatedObject );
			}
		}

		return Parser_AnimationGroupElement( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_AnimationGroupChainAnimation )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			const StringVector & splitCmd = StringUtil::split( p_params, " \t" );
			g_currentAnimationGroup->AddChainedAnimations( make_wxString( splitCmd[0] ), make_wxString( splitCmd[1] ) );
		}

		return Parser_AnimationGroupChainAnimation( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_ObjectEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->object != NULL )
			{
				String l_parentName;

				if ( p_context->object->getParentSceneNode() )
				{
					l_parentName = p_context->object->getParentSceneNode()->getName();
				}

				TROLL_PROJECT_3D_NAMESPACE::TrollEntity * l_entity = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetEntity( make_wxString( p_context->object->getName() ) );
				TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );
				l_entity->SetOgreEntity( p_context->object );
				l_entity->AttachTo( l_parent );
				Mesh * l_ogreMesh = p_context->object->getMesh().getPointer();
				TROLL_PROJECT_3D_NAMESPACE::TrollMesh * l_mesh = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddMesh( make_wxString( l_ogreMesh->getName() ) );
				unsigned short l_nbSubMesh = l_ogreMesh->getNumSubMeshes();

				for ( unsigned short i = 0 ; i < l_nbSubMesh ; i++ )
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->overlay != NULL && g_currentOverlay )
			{
	//			std::cout << "SceneFileParser_Parsers::OverlayEnd - " << l_overlay->GetName().c_str() << "\n";
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->sceneNode != NULL )
			{
				String l_parentName;

				if ( p_context->sceneNode->getParentSceneNode() )
				{
					l_parentName = p_context->sceneNode->getParentSceneNode()->getName();
				}

				std::cout << "TrollParser_SceneNodeEnd - " << p_context->sceneNode->getName() << " - parent : " << l_parentName << "\n";
				TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_node = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( p_context->sceneNode->getName() ) );

				if ( l_node != NULL )
				{
					TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );
					l_node->SetOgreNode( p_context->sceneNode );
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AttachNode( l_node, l_parent );

					if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
					{
						GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneNode( make_wxString( p_context->sceneNode->getName() ), make_wxString( l_parentName ) );
					}
				}
			}
		}

		return Parser_SceneNodeEnd( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_LightEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->light != NULL )
			{
				String l_parentName;

				if ( p_context->light->getParentSceneNode() )
				{
					l_parentName = p_context->light->getParentSceneNode()->getName();
				}

				TROLL_PROJECT_3D_NAMESPACE::TrollLight * l_light = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( p_context->light->getName() ) );
				TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );

				if ( l_light != NULL )
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->camera != NULL )
			{
				String l_parentName;

				if ( p_context->camera->getParentSceneNode() )
				{
					l_parentName = p_context->camera->getParentSceneNode()->getName();
				}

				TROLL_PROJECT_3D_NAMESPACE::TrollCamera * l_camera = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( p_context->camera->getName() ) );
				TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_parent = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_parentName ) );

				if ( l_camera != NULL )
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
		if ( ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::TrollSound * l_sound = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSound( make_wxString( p_context->soundObject->GetName() ) );
			l_sound->SetUrl( make_wxString( p_params ) );
		}

		return Parser_SoundURL( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SoundEnd )
	{
		SoundObject * l_museSound = p_context->soundObject;
		bool l_result = Parser_SoundEnd( p_params, p_context );

		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( l_museSound != NULL )
			{
				TROLL_PROJECT_MEDIA_NAMESPACE::TrollSound * l_sound = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSound( make_wxString( l_museSound->GetName() ) );
				l_sound->SetMuseSound( l_museSound );
			}
		}

		return l_result;
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SoundNodeEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->sceneNode != NULL && p_context->soundInstance != NULL && p_context->soundObject != NULL )
			{
				TROLL_PROJECT_MEDIA_NAMESPACE::TrollSound * l_sound = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSound( make_wxString( p_context->soundObject->GetName() ) );

				if ( l_sound != NULL )
				{
					TROLL_PROJECT_3D_NAMESPACE::TrollSceneNode * l_node = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( p_context->sceneNode->getName() ) );
					TROLL_PROJECT_MEDIA_NAMESPACE::TrollSoundObject * l_sndObject = l_sound->AddObject( l_node );
					l_sndObject->SetMuseInstance( p_context->soundInstance );
					l_node->SetSound( l_sndObject );

					if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
					{
						GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( reinterpret_cast< TROLL_PROJECT_NAMESPACE::Object * >( l_sndObject ) );
					}
				}
				else
				{
					std::cout << "TrollScenFileParser::TrollParser_SoundNodeEnd - NULL Sound\n";
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideo * l_video = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetVideo( make_wxString( p_context->videoObject->GetName() ) );
			l_video->SetMuseVideo( p_context->videoObject );
		}

		return Parser_VideoEnd( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_VideoObjectEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->object != NULL )
			{
				TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideo * l_video = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetVideo( make_wxString( p_context->videoObject->GetName() ) );
				TROLL_PROJECT_3D_NAMESPACE::TrollEntity * l_object = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetEntity( make_wxString( p_context->object->getName() ) );
				TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * l_videoObject = l_video->AddObject( l_object );
				l_object->SetVideo( l_videoObject );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() )
				{
					GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( reinterpret_cast< TROLL_PROJECT_NAMESPACE::Object * >( l_videoObject ) );
				}
			}
		}

		return Parser_VideoObjectEnd( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_OverlayVideo )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->overlay != NULL )
			{
				VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_params );

				if ( l_object != NULL )
				{
					TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideo * l_video = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetVideo( make_wxString( l_object->GetName() ) );
					TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * l_overlay = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_context->overlay->GetName() ) );

					if ( l_overlay != NULL )
					{
						TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * l_videoObject = l_video->AddObject( l_overlay );
						l_overlay->SetVideo( l_videoObject );

						if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() )
						{
							GUI::MainFrame::GetInstance()->GetObjectsList()->AddSceneObject( reinterpret_cast< TROLL_PROJECT_NAMESPACE::Object * >( l_videoObject ) );
						}
					}
				}
			}
		}

		return Parser_OverlayVideo( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_VideoOverlayEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			if ( p_context->overlay != NULL )
			{
				TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * l_overlay = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_context->overlay->GetName() ) );

				if ( l_overlay != NULL )
				{
					TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * l_videoObject = l_overlay->GetVideo();

					if ( l_videoObject != NULL )
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
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollAnimation * l_animation = new TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollAnimation( make_wxString( p_context->animation->GetName() ), g_fileName );
			l_animation->SetMuseAnimation( p_context->animation );
			g_currentAnimatedObject->AddAnimation( l_animation );
		}

		return Parser_AnimationEnd( p_params, p_context );
	}



	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceEvent )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
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

			if ( l_event == NULL )
			{
				PARSING_ERROR( "Parsing Error : TrollParser_SequenceEvent -> event factory not found or parameters unsatisfactory" );
				return false;
			}

			l_event->SetTarget( p_context->sequenceTarget );
			return false;
		}

		return Parser_SequenceEvent( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTarget )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			const StringVector & l_split = StringUtil::split( p_params, ", \t", 1 );
			g_targetType = make_wxString( l_split[0] );
			g_target = make_wxString( l_split[1] );

			if ( g_continuousEvent != NULL )
			{
				g_continuousEvent->SetTarget( g_targetType, g_target );
			}
		}

		return Parser_SequenceTarget( p_params, p_context );
	}


	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrack )
	{
		Parser_SequenceTrack( p_params, p_context );

		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_continuousEvent = new TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent( g_currentSequence, p_context->sequenceTrack, make_wxString( p_params ), g_fileName );
		}

		return true;
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->AddSequence( g_currentSequence, g_fileName );

			if ( GUI::MainFrame::GetInstance()->IsEditingTimeLine() )
			{
				GUI::MainFrame::GetInstance()->GetTimelineEditor()->AddSequence( g_currentSequence );
				GUI::MainFrame::GetInstance()->GetTimeLineContainer()->SetScrollbars( 20, 20, GUI::MainFrame::GetInstance()->GetTimelineEditor()->GetClientSize().x / 20, GUI::MainFrame::GetInstance()->GetTimelineEditor()->GetClientSize().y / 20, 0, 0 );
			}
		}

		return Parser_SequenceEnd( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrackKey )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			const StringVector & l_split = StringUtil::split( p_params, ", \t", 1 );
			g_continuousEvent->AddKeyFrame( StringConverter::parseReal( l_split[0] ), make_wxString( l_split[1] ) );
		}

		return Parser_SequenceTrackKey( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrackInterpolation )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_continuousEvent->SetInterpolation( make_wxString( p_params ) );
		}

		return Parser_SequenceTrackInterpolation( p_params, p_context );
	}

	DEFINE_SCENE_FILE_TROLL_PARSER( TrollParser_SequenceTrackEnd )
	{
		if ( g_fileName != wxT( "SceneEditor.emscene" ) && g_fileName != wxT( "OverlayEditor.emscene" ) && ! GUI::MainFrame::GetInstance()->IsTestingScene() )
		{
			g_currentSequence->AddContinuousEvent( g_continuousEvent );
			g_continuousEvent = NULL;
		}

		return Parser_SequenceTrackEnd( p_params, p_context );
	}
}
END_TROLL_ELYPSE_NAMESPACE