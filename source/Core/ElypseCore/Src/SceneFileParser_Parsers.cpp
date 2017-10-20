/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SoundObject.h"

#include "SceneFileParser.h"
#include "SceneFileParser_Parsers.h"

#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <OgrePanelOverlayElement.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreMeshManager.h>
#include <OgreMaterialManager.h>
#include <OgreOverlayManager.h>
#include <OgreViewport.h>
#include <OgreSubEntity.h>
#include <OgreLog.h>
#include <OgreStringConverter.h>
#include <OgreParticleSystem.h>
#include <OgreBillboardSet.h>
#include <OgreSkeletonManager.h>

#include "PhysicsEngine.h"
#include "PhysicsObject.h"
#include "PhysicsSimulation.h"
#include "PhysicsMaterial.h"
#include "BoundingMesh.h"
#include "SimpleBoundingShapes.h"
#include "Space.h"

#include "EMOverlayGroup.h"
#include "EMGui.h"
#include "EMOverlay.h"
#include "ClickableBox.h"
#include "ClickableCircle.h"
#include "ClickableImage.h"

#include "Universe.h"
#include "Zone.h"
#include "Portal.h"

#include "ElypseController.h"
#include "ElypseLoadingBar.h"
#include "ElypseInstance.h"
#include "Context.h"

#include "AnimatedObjectGroup.h"
#include "AnimatedObject.h"
#include "Animation.h"
#include "AnimationManager.h"

#include "Mirror.h"
#include "MirrorManager.h"
#include "CamTex.h"
#include "PostEffect_HDR.h"
#include "PostEffect_GaussianBlur.h"
#include "PostEffect_MotionBlur.h"
#include "PostEffect_LensFlare.h"

#include "PubManager.h"
#include "PubObject.h"
#include "PubInstance.h"

#include "ConfigFile.h"

#include "SoundManager.h"
#include "SoundPlaylist.h"
#include "SoundInstance.h"
#include "VideoManager.h"
#include "VideoObject.h"

#include "ScriptEngine.h"

#include "DownloadManager.h"
#include "MuseFile.h"

#include "SequenceManager.h"
#include "BaseEvent.h"

#include "ElypseLogs.h"

#include <Manager.h>
#include <File.h>
#include <StringConverter.h>
#include <StringUtils.h>

void Elypse::SceneFile::SceneFileParser::parsingError( Context * p_context, String const & l_error )
{
	EMUSE_LOG_MESSAGE_RELEASE( "Line #" + StringConverter::toString( p_context->lineNo ) + " / " + l_error );
}

Vector3 Elypse::SceneFile::SceneFileParser::Parser_Vector3Value( String const & p_params )
{
	const StringVector & l_split = StringUtil::split( p_params, ", \t" );

	if ( l_split.size() != 3 )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Error : SceneFileParser::Parser_Vector3Value, wrong number of substrings : " + p_params );
		return Vector3::ZERO;
	}

	return Vector3( StringConverter::parseReal( l_split[0] ),
					StringConverter::parseReal( l_split[1] ),
					StringConverter::parseReal( l_split[2] ) );
}

Vector4 Elypse::SceneFile::SceneFileParser::Parser_Vector4Value( String const & p_params )
{
	const StringVector & l_split = StringUtil::split( p_params, ", \t" );

	if ( l_split.size() != 4 )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Error : SceneFileParser::Parser_Vector4Value, wrong number of substrings : " + p_params );
		return Vector4::ZERO;
	}

	return Vector4( StringConverter::parseReal( l_split[0] ),
					StringConverter::parseReal( l_split[1] ),
					StringConverter::parseReal( l_split[2] ),
					StringConverter::parseReal( l_split[3] ) );
}

ColourValue Elypse::SceneFile::SceneFileParser::Parser_ColourValue( String const & p_params )
{
	const StringVector & l_split = StringUtil::split( p_params, ", \t" );

	if ( l_split.size() == 3 )
	{
		return ColourValue( StringConverter::parseReal( l_split[0] ),
							StringConverter::parseReal( l_split[1] ),
							StringConverter::parseReal( l_split[2] ),
							1.0 );
	}

	if ( l_split.size() == 4 )
	{
		return ColourValue( StringConverter::parseReal( l_split[0] ),
							StringConverter::parseReal( l_split[1] ),
							StringConverter::parseReal( l_split[2] ),
							StringConverter::parseReal( l_split[3] ) );
	}

	EMUSE_LOG_MESSAGE_RELEASE( "Error : Parser_ColourValue, wrong number of substrings : " + p_params );
	return ColourValue::ZERO;
}

/********************************************************************************/
/*																				*/
/*										Root									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_RootAmbientLight )
{
	p_context->sceneManager->setAmbientLight( SceneFileParser::Parser_ColourValue( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootBackgroundColour )
{
	if ( p_context->universe == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_StartScene -> no universe" );
		return false;
	}

	if ( p_context->universe->GetViewport() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_StartScene -> universe has no viewport" );
		return false;
	}

	p_context->CurrentBackground = SceneFileParser::Parser_ColourValue( p_params );
	p_context->universe->GetViewport()->setBackgroundColour( p_context->CurrentBackground );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootStartScene )
{
	if ( p_context->universe == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_StartScene -> no universe" );
		return false;
	}

	Zone * l_zone = p_context->universe->GetZone( p_params );

	if ( l_zone == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_StartScene -> scene [" + p_params + "] not found" );
		return false;
	}

	p_context->scriptEngine->LoadZone( l_zone );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootScene )
{
	p_context->scene = p_context->universe->GetZone( p_params );

	if ( p_context->scene == NULL )
	{
		Zone * l_zone = new Zone( p_params, *p_context->universe );
		p_context->scene = l_zone;
		p_context->universe->AddZone( l_zone );
	}

	p_context->section = SS_SCENE;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootBillboardGroup )
{
	if ( p_context->sceneManager->hasBillboardSet( p_params ) )
	{
		p_context->billboardSet = p_context->sceneManager->getBillboardSet( p_params );
	}
	else
	{
		p_context->billboardSet = p_context->sceneManager->createBillboardSet( p_params );
	}

	p_context->section = SS_BILLBOARDSET;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootObject )
{
	p_context->section = SS_OBJECT;
	p_context->general_string_a = p_params;
	p_context->object = NULL;
	p_context->subObject = NULL;

	if ( p_context->sceneManager->hasEntity( p_params ) )
	{
		p_context->object = p_context->sceneManager->getEntity( p_params );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPhysics )
{
	p_context->section = SS_PHYSICS;
	p_context->physicsMaterial = p_context->physicsEngine->CreateMaterial( p_params );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootSound )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_Sound -> Need a name for the sound object" );
		return true;
	}

	p_context->soundObject = SoundManager::GetSingletonPtr()->CreateSound( p_params );
	p_context->section = SS_SOUND;
	p_context->soundInstance = NULL;
	/*
	p_context->general_string_a = p_params;
	p_context->soundGlobal = false;
	p_context->soundLooped = false;
	p_context->general_string_b.clear();
	p_context->soundType = EM_MUSIC;
	p_context->soundTimer = 0.0;
	p_context->soundFadeInLength = 0.0;
	p_context->soundFadeOutLength = 0.0;
	p_context->currentReal = 0.0;
	*/
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPlaylist )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : ParsePlaylist -> Need a name for the playlist" );
		return true;
	}

	p_context->section = SS_PLAYLIST;
	SoundPlaylist * l_playlist = new SoundPlaylist( p_params, SoundManager::GetSingletonPtr()->GetSFXVolume(),
			SoundManager::GetSingletonPtr()->GetMusicVolume(),
			p_context->scene->GetName() );
	SoundManager::GetSingletonPtr()->AddPlaylist( l_playlist );
	p_context->soundPlaylist = l_playlist;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootVideo )
{
	p_context->section = SS_VIDEO;
	p_context->videoObject = VideoManager::GetSingletonPtr()->CreateVideoObject( p_params, p_context->plugin );

	if ( p_context->videoObject != NULL )
	{
		p_context->scene->AddZoneObject( p_context->videoObject );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPub )
{
	p_context->pubObject = PubManager::GetSingletonPtr()->CreatePubObject( p_params, p_context->plugin );
	p_context->section = SS_PUB;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootCode )
{
	p_context->section = SS_CODE;
	p_context->general_string_a.clear();
	p_context->general_string_b.clear();
	p_context->general_string_c.clear();
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootCodeEnd )
{
	p_context->section = SS_NONE;
	p_context->scriptEngine->AddScriptZone( p_context->general_string_a );
	p_context->scriptEngine->m_untrimmedCode.push_back( p_context->general_string_b );
	p_context->general_string_b.clear();
	p_context->general_string_a.clear();
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootMirror )
{
	p_context->section = SS_MIRROR;
	p_context->mirror = p_context->mirrorManager->CreateElement( p_params );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootSceneNode )
{
	p_context->general_string_a = p_params;
	p_context->section = SS_NODE;
	size_t l_index = p_params.find( ':' );

	if ( l_index != String::npos )
	{
		String l_parent = p_params.substr( l_index + 1 );
		String l_child = p_params.substr( 0, l_index );
		StringUtil::trim( l_parent );
		StringUtil::trim( l_child );

		if ( ! p_context->sceneManager->hasSceneNode( l_parent ) )
		{
			p_context->sceneNode = NULL;
			return true;
		}

		if ( p_context->sceneManager->hasSceneNode( l_child ) )
		{
			p_context->sceneNode = p_context->sceneManager->getSceneNode( l_child );
			return true;
		}

		SceneNode * l_parentNode = p_context->sceneManager->getSceneNode( l_parent );
		SceneNode * l_node = p_context->sceneManager->createSceneNode( l_child );
		l_node->setPosition( l_parentNode->getPosition() );
		l_node->setOrientation( l_parentNode->getOrientation() );
		l_node->setScale( l_parentNode->getScale() );
		l_node->setInheritOrientation( l_parentNode->getInheritOrientation() );
		l_node->setScale( l_parentNode->getScale() );

		if ( l_parentNode->isInSceneGraph() )
		{
			l_parentNode->getParentSceneNode()->addChild( l_node );
		}

		p_context->sceneNode = l_node;
		return true;
	}

	if ( ! p_context->sceneManager->hasSceneNode( p_params ) )
	{
		p_context->sceneNode = p_context->sceneManager->createSceneNode( p_context->general_string_a );
		p_context->sceneNode->setInheritOrientation( false );
		p_context->sceneNode->setInheritScale( false );
	}
	else
	{
		p_context->sceneNode = p_context->sceneManager->getSceneNode( p_params );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootAnimationGroup )
{
	p_context->section = SS_ANIMATED_OBJECT_GROUP ;
	p_context->animatedObjectGroup = p_context->animationManager->CreateElement( p_params, *p_context->animationManager );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootLight )
{
	p_context->section = SS_LIGHT;

	if ( ! p_context->sceneManager->hasLight( p_params ) )
	{
		p_context->light = p_context->sceneManager->createLight( p_params );
		p_context->general_int = -1;

		if ( p_context->usingShadow )
		{
			p_context->light->setCastShadows( true );
		}
	}
	else
	{
		p_context->light = p_context->sceneManager->getLight( p_params );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightShadows )
{
	if ( p_context->light == NULL )
	{
		return false;
	}

	p_context->light->setCastShadows( StringConverter::parseBool( p_params ) );
	return false;
}


DEFINE_SCENE_FILE_PARSER( Parser_RootCamera )
{
	p_context->section = SS_CAMERA;

	if ( p_params.empty() )
	{
		p_context->camera = p_context->sceneManager->getCamera( "RootCamera" );
		return true;
	}

	if ( p_context->sceneManager->hasCamera( p_params ) )
	{
		p_context->camera = p_context->sceneManager->getCamera( p_params );
	}
	else
	{
		p_context->camera = p_context->sceneManager->createCamera( p_params );
		p_context->camera->setNearClipDistance( 5.0 );
		p_context->camera->setAspectRatio( Real( 1.33 ) );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootOverlay )
{
	if ( p_context->section == SS_NONE )
	{
		p_context->section = SS_OVERLAY;
		p_context->general_int = 0;
		p_context->overlayGroup = p_context->gui->CreateOverlayGroup( p_params );
		p_context->overlay = p_context->overlayGroup->GetFirstElement();
	}
	else
	{
		if ( p_context->overlay != NULL )
		{
			p_context->overlay = p_context->overlay->CreateChild( p_params );
		}

//		p_context->general_string_a = p_params + p_context->instanceNum;
		p_context->general_int ++;
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootUniverse )
{
	auto l_universe = p_context->universeManager->GetElementByName( p_params );

	if ( l_universe == NULL )
	{
//		l_universe = new Universe( p_params);
		l_universe = p_context->universeManager->CreateUniverse( p_params );
		p_context->sceneManager = l_universe->GetManager();
	}

	p_context->universe = l_universe;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootShadows )
{
	if ( p_context->sceneManager == NULL )
	{
		return false;
	}

	if ( p_params == "texture" )
	{
		p_context->sceneManager->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_MODULATIVE );
		p_context->sceneManager->setShadowTextureCount( 8 );
		p_context->sceneManager->setShadowTextureSize( 64 );
		p_context->usingShadow = true;
	}
	else if ( p_params == "stencil" )
	{
		p_context->sceneManager->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_ADDITIVE );
		p_context->usingShadow = true;
	}
	else if ( p_params == "none" )
	{
		p_context->sceneManager->setShadowTechnique( Ogre::SHADOWTYPE_NONE );
		p_context->usingShadow = false;
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_Shadows -> option not recognized : " + p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootForceShadows )
{
	p_context->forceShadows = StringConverter::parseBool( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootParticleSystem )
{
	p_context->general_string_a = p_params;
	p_context->section = SS_PARTICLE_SYSTEM;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootLoadingBar )
{
	ElypseController::GetSingletonPtr()->GetLoadingBar()->SetLoadingBarGroupName( p_params );
	ElypseController::GetSingletonPtr()->GetLoadingBar()->SetLoadingBarOverlayName( OverlayManager::getSingletonPtr()->getOverlayElement( p_params + p_context->instanceNum )->getParent()->getName() );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPostEffect )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_RootPostEffect -> A name is needed to activate a post effect" );
	}

	if ( p_params == "HDR" )
	{
		p_context->postEffectManager->AddElement( std::make_shared< PostEffect_HDR >( "HDR", p_context->universe->GetViewport() ) );
	}
	else if ( p_params == "GaussianBlur" )
	{
		p_context->postEffectManager->AddElement( std::make_shared< PostEffect_GaussianBlur >( "GaussianBlur", p_context->universe->GetViewport() ) );
	}
	else if ( p_params == "LensFlare" )
	{
		p_context->postEffectManager->AddElement( std::make_shared< PostEffect_LensFlare >( "LensFlare", p_context->universe->GetViewport() ) );
	}
	else if ( p_params == "MotionBlur" )
	{
		p_context->postEffectManager->AddElement( std::make_shared< PostEffect_MotionBlur >( "MotionBlur", p_context->universe->GetViewport() ) );
	}
	else
	{
		p_context->postEffectManager->AddElement( std::make_shared< PostEffect >( p_params, p_context->universe->GetViewport() ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPhysicsSimulation )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_RootPhysicsSimulation -> A name is needed to create a new physics Simulation" );
		return true;
	}

	p_context->physicsSimulation = p_context->physicsEngine->CreateSimulation( p_params );
	p_context->physicsSpace = p_context->physicsSimulation->GetDynamicSpace();
	p_context->section = SS_SIMULATION;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPhysicsGroup )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_RootPhysicsGroup -> A name is needed to create a new physics group" );
		return false;
	}

	p_context->physicsSpace = p_context->physicsSimulation->CreateSpace( p_params, true );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootCollision )
{
	p_context->section = SS_OBJECT_PHYSICS;
	p_context->physicsObject = p_context->physicsSimulation->CreateObject( p_params );
	p_context->physicsObject->SetFloating( true );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootPortal )
{
	p_context->section = SS_PORTAL;

	if ( p_context->scene == NULL )
	{
		p_context->portal = NULL;
		return true;
	}

	p_context->portal = p_context->scene->GetPortal( p_params );

	if ( p_context->portal == NULL )
	{
		p_context->portal = new Portal( p_params );
		p_context->scene->AddPortal( p_context->portal );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootAutoPhysics )
{
	p_context->autoPhysicsPreName.clear();
	p_context->autoPhysicsPostName.clear();

	if ( p_params.empty() )
	{
		p_context->autoPhysics = false;
		return false;
	}

	p_context->autoPhysics = true;
	Ogre::StringVector l_split = StringUtil::split( p_params, "$", 1 );

	if ( l_split.size() == 2 )
	{
		p_context->autoPhysicsPreName = l_split[0];
		p_context->autoPhysicsPostName = l_split[1];
	}
	else if ( p_params[0] == '$' )
	{
		p_context->autoPhysicsPostName = l_split[0];
	}
	else
	{
		p_context->autoPhysicsPreName = l_split[0];
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootSequence )
{
	p_context->section = SS_SEQUENCE;
	p_context->sequence = p_context->sequenceManager->CreateElement( p_params );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_RootEnd )
{
	PARSING_ERROR( "Parsing Error : Parser_ScriptLine -> Unexpected '}'" );
	return false;
}

/********************************************************************************/
/*																				*/
/*								Animated Object Group							*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_AnimationGroupElement )
{
	p_context->section = SS_ANIMATED_OBJECT;

	if ( p_context->animatedObjectGroup == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationGroupElement -> object null" );
		return true;
	}

	if ( ! p_context->sceneManager->hasEntity( p_params ) )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationGroupElement -> object [" + p_params + "] does not exist" );
		return true;
	}

	Entity * l_entity = p_context->sceneManager->getEntity( p_params );

	if ( ! l_entity->hasSkeleton() &&  ! l_entity->hasVertexAnimation() )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationGroupElement -> object [" + p_params + "] is not animated" );
		return true;
	}

	p_context->animatedObject = p_context->animatedObjectGroup->AddEntity( l_entity );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationGroupChainAnimation )
{
	if ( p_context->animatedObjectGroup == NULL )
	{
		return false;
	}

	const StringVector & splitCmd = StringUtil::split( p_params, " \t" );

	if ( splitCmd.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationGroupElement animation group [" + p_context->animatedObjectGroup->GetName() + "]is empty ?" );
		return false;
	}

	p_context->animatedObjectGroup->ChainAnimation( splitCmd[0], splitCmd[1] );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationGroupLoop )
{
	if ( p_context->animatedObjectGroup == NULL )
	{
		return false;
	}

	p_context->animatedObjectGroup->SetAnimationLooped( p_params, true );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationGroupEnd )
{
	p_context->section = SS_NONE;
	p_context->animatedObjectGroup = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Animated Object								*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_AnimationObject )
{
	p_context->section = SS_ANIMATION;

	if ( p_context->animatedObject == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectAnimation -> no current object" );
		return true;
	}

	p_context->animation = p_context->animatedObject->GetAnimation( p_params );

	if ( p_context->animation == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectAnimation -> animation [" + p_params + "] does not exist for this object" );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationObjectEnd )
{
	p_context->section = SS_ANIMATED_OBJECT_GROUP;
	p_context->animatedObject = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Animation									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_AnimationLooped )
{
	if ( p_context->animation == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationLooped -> no current animation" );
		return false;
	}

	p_context->animation->SetLooped( StringConverter::parseBool( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationTimeout )
{
	if ( p_context->animation == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationTimeout -> no current animation" );
		return false;
	}

	p_context->animation->SetTimeOut( StringConverter::parseReal( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationTimein )
{
	if ( p_context->animation == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_AnimationTimein -> no current animation" );
		return false;
	}

	p_context->animation->SetTimeIn( StringConverter::parseReal( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_AnimationEnd )
{
	p_context->section = SS_ANIMATED_OBJECT;
	p_context->animation = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*										Object									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_ObjectMesh )
{
	if ( p_context->object != NULL )
	{
		//object already exists, so we ignore the "mesh" directive.
		return false;
	}

	try
	{
		MeshManager::getSingletonPtr()->load( p_params, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	}
	catch ( Exception & )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectMesh -> mesh named [" + p_params + "] does not exist" );
		p_context->object = NULL;
		return false;
	}

	p_context->object = p_context->sceneManager->createEntity( p_context->general_string_a, p_params );
	//TODO : doesn't work on 1.6.2
	//p_context->object->setNormaliseNormals( true);
	p_context->object->setCastShadows( p_context->forceShadows );
	EdgeData * l_list = p_context->object->getMesh()->getEdgeList();

	if ( l_list == NULL )
	{
		p_context->object->getMesh()->buildEdgeList();
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectMeshAndSkeleton )
{
	if ( p_context->object != NULL )
	{
		//object already exists, so we ignore the "mesh" directive.
		return false;
	}

	Ogre::StringVector l_params = StringUtil::split( p_params );

	if ( l_params.size() < 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectMeshAndSkeleton -> not enough params : " + StringConverter::toString( l_params.size() ) + "/2" );
		return false;
	}
	else if ( l_params.size() > 2 )
	{
		PARSING_ERROR( "Parsing Warning : Parser_ObjectMeshAndSkeleton -> too much params : " + StringConverter::toString( l_params.size() ) + "/2" );
	}

	MeshPtr l_mesh;

	try
	{
		l_mesh = MeshManager::getSingletonPtr()->load( l_params[0], ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	}
	catch ( Exception & )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectMesh -> mesh named [" + l_params[0] + "] does not exist" );
		p_context->object = NULL;
		return false;
	}

	if ( SkeletonManager::getSingletonPtr()->getByName( l_params[1] ).isNull() )
	{
		StringVector l_groups = ResourceGroupManager::getSingletonPtr()->getResourceGroups();
		Ogre::StringVectorPtr l_names;
		bool l_found = false;

		for ( size_t i = 0 ; i < l_groups.size() && ! l_found ; i++ )
		{
			l_names = ResourceGroupManager::getSingletonPtr()->listResourceNames( l_groups[i] );

			for ( size_t j = 0 ; j < l_names.getPointer()->size() ; j++ )
			{
				if ( l_names.getPointer()->operator []( j ) == l_params[1] )
				{
					l_found = true;
				}
			}
		}

		if ( ! l_found )
		{
			PARSING_ERROR( "Parsing Error : Parser_ObjectSkeleton -> skeleton (" + l_params[1] + ") not found" );
			return false;
		}
	}

	String l_meshName = l_params[0];
	size_t l_index = l_meshName.find_last_of( "." );
	l_meshName = l_meshName.substr( 0, l_index );
	String l_skeletonName = l_params[1];
	l_index = l_skeletonName.find_last_of( "." );
	l_skeletonName = l_skeletonName.substr( 0, l_index );
	l_mesh = l_mesh->clone( l_meshName + "_" + l_skeletonName, l_mesh->getGroup() );
	l_mesh->load();
	l_mesh->setSkeletonName( l_params[1] );
	p_context->object = p_context->sceneManager->createEntity( p_context->general_string_a, l_mesh->getName() );
	//TODO : doesn't work on 1.6.2
	//p_context->object->setNormaliseNormals( true);
	p_context->object->setCastShadows( p_context->forceShadows );
	EdgeData * l_list = p_context->object->getMesh()->getEdgeList();

	if ( l_list == NULL )
	{
		p_context->object->getMesh()->buildEdgeList();
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectShadow )
{
	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectShadow -> no object" );
		return false;
	}

	p_context->object->setCastShadows( StringConverter::parseBool( p_params ) );
	return false;
}


DEFINE_SCENE_FILE_PARSER( Parser_ObjectSubmesh )
{
	p_context->subObject = NULL;
	p_context->section = SS_SUBMESH;

	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectSubmesh -> no object" );
		return true;
	}

	if ( isdigit( p_params[0] ) == 0 )
	{
		p_context->subObject = p_context->object->getSubEntity( p_params );
		return true;
	}

	uint32_t l_numSubEnt = StringConverter::parseInt( p_params );

	if ( l_numSubEnt >= p_context->object->getNumSubEntities() )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectSubmesh -> submesh index out of bounds : [" + p_params + "] for object : " + p_context->object->getName() );
	}

	p_context->subObject = p_context->object->getSubEntity( l_numSubEnt );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectMaterial )
{
	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectMaterial -> no object" );
		return false;
	}

	if ( MaterialManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectMaterial -> material [" + p_params + "] not found" );
		return false;
	}

	p_context->object->setMaterialName( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysics )
{
	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectPhysics -> Current Object is null" );
		return true;
	}

	PhysicsMaterial * l_mat = p_context->physicsEngine->GetMaterial( p_params );

	if ( l_mat == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectPhysics -> Physics material [" + p_params + "] does not exist" );
		return true;
	}

	p_context->section = SS_OBJECT_PHYSICS;
	p_context->physicsObject = p_context->physicsSimulation->CreateObject( p_context->object );
	p_context->physicsObject->SetMaterial( l_mat );
	p_context->physicsSpace->AddObject( p_context->physicsObject );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectHidden )
{
	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjecHidden -> no object" );
		return false;
	}

	p_context->object->setVisible( false );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectAttach )
{
	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parser_AttachObject -> object NULL" );
		return false;
	}

	if ( p_context->object->isAttached() )
	{
		PARSING_ERROR( "Object [" + p_context->object->getName() + "] already attached" );
		return false;
	}

	if ( ! p_context->sceneManager->hasSceneNode( p_params ) )
	{
		PARSING_ERROR( "Parser_AttachObject -> SceneNode not found : [" + p_params + "]" );
		return false;
	}

	p_context->sceneManager->getSceneNode( p_params )->attachObject( p_context->object );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectSound )
{
	p_context->section = SS_OBJECT_SOUND;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectVideo )
{
	p_context->section = SS_OBJECT_VIDEO;

	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parser_ObjectVideo -> No current object" );
		return true;
	}

	VideoObject * l_video = VideoManager::GetSingletonPtr()->GetVideoObject( p_params );

	if ( l_video == NULL )
	{
		PARSING_ERROR( "Parser_ObjectVideo -> No video named : [" + p_params + "]" );
		return true;
	}

	l_video->CreateInstance( p_context->object );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPub )
{
	if ( p_context->object == NULL )
	{
		PARSING_ERROR( "Parser_ObjectPub -> No Object" );
		return false;
	}

	const StringVector & l_split = StringUtil::split( p_params, " \t", 1 );
	PubObject * l_object = NULL;

	if ( l_split.size() == 1 )
	{
		l_object = PubManager::GetSingletonPtr()->GetPubObject( p_params );
	}
	else
	{
		l_object = PubManager::GetSingletonPtr()->GetPubObject( l_split[0] );
	}

	if ( l_object == NULL )
	{
		PARSING_ERROR( "Parser_ObjectPub -> No pub named : [" + l_split[0] + "]" );
		return false;
	}

	PubInstance * l_instance = l_object->CreateInstance( p_context->object );

	if ( l_split.size() == 2 )
	{
		l_instance->SetSubMat( static_cast <uint32_t>( StringConverter::parseInt( l_split[1] ) ) );
	}

	l_instance->Apply();
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectEnd )
{
	if ( p_context->object != NULL )
	{
		if ( ! p_context->object->isAttached() )
		{
			p_context->sceneManager->getRootSceneNode()->attachObject( p_context->object );
		}

		if ( p_context->usingShadow || p_context->forceShadows )
		{
			p_context->object->getMesh()->buildEdgeList();
			p_context->object->getMesh()->prepareForShadowVolume();
		}

		if ( p_context->autoPhysics && p_context->object->getUserObjectBindings().getUserAny().isEmpty() )
		{
			String l_meshName = p_context->object->getMesh()->getName();
			String l_groupName = ResourceGroupManager::getSingletonPtr()->findGroupContainingResource( l_meshName );
			l_meshName.erase( l_meshName.size() - 5, 6 );
			Ogre::FileInfoListPtr filptr = ResourceGroupManager::getSingletonPtr()->findResourceFileInfo( l_groupName, p_context->autoPhysicsPreName + l_meshName + p_context->autoPhysicsPostName );

			if ( ! filptr->empty() )
			{
				PhysicsObject * l_object = p_context->physicsSimulation->CreateObject( p_context->object, true, false );
				l_object->ReassertSpatialState();

				for ( size_t i = 0 ; i < filptr->size() ; i ++ )
				{
					String const & collName = filptr.getPointer()->at( i ).basename;
					MeshPtr l_ptr = MeshManager::getSingletonPtr()->load( collName, l_groupName );

					if ( ! l_ptr.isNull() )
					{
						l_object->AddBound( new BoundingMesh( l_ptr.getPointer() ) );
					}
				}
			}
		}
	}

	p_context->section = SS_NONE;
	p_context->object = NULL;
	p_context->subObject = NULL;
	p_context->general_string_a.clear();
	return false;
}

/********************************************************************************/
/*																				*/
/*										SubMesh									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_SubMeshMaterial )
{
	if ( p_context->subObject == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SubMeshMaterial -> no subObject" );
		return false;
	}

	if ( MaterialManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SubMeshMaterial -> material [" + p_params + "] not found" );
		return false;
	}

	p_context->subObject->setMaterialName( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SubMeshEnd )
{
	p_context->section = SS_OBJECT;
	p_context->subObject = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Scene Node									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeParent )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeParent -> no current sceneNode" );
		return false;
	}

	if ( p_context->sceneNode->getParentSceneNode() != NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeParent -> sceneNode [" + p_context->sceneNode->getName() + "] "
					   "already attached to sceneNode : [" + p_context->sceneNode->getParentSceneNode()->getName() + "]" );
		return false;
	}

	if ( ! p_context->sceneManager->hasSceneNode( p_params ) )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeParent -> parent sceneNode [" + p_params + "] does not exist" );
		return false;
	}

	p_context->sceneManager->getSceneNode( p_params )->addChild( p_context->sceneNode );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodePosition )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodePosition -> no sceneNode" );
		return false;
	}

	p_context->sceneNode->setPosition( SceneFileParser::Parser_Vector3Value( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodePositionFromMax )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodePositionFromMax -> no sceneNode" );
		return false;
	}

	const Vector3 & l_vector = SceneFileParser::Parser_Vector3Value( p_params );
	p_context->sceneNode->setPosition( Vector3( l_vector.x, l_vector.z, -l_vector.y ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeScale )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeScale -> no sceneNode" );
		return false;
	}

	const Vector3 & l_scale = SceneFileParser::Parser_Vector3Value( p_params );

	if ( l_scale == Vector3::ZERO )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeScale -> scale must be non-zero" );
		return false;
	}

	p_context->sceneNode->setScale( l_scale );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeOrientation )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeOrientation -> no sceneNode" );
		return false;
	}

	const Vector4 & l_vector = SceneFileParser::Parser_Vector4Value( p_params );
	Quaternion q = Quaternion( Degree( l_vector.w ), Vector3( l_vector.x, l_vector.y, l_vector.z ) );
	q.normalise();
	p_context->sceneNode->setOrientation( q );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeDirection )
{
	if ( p_context->sceneNode != NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeDirection -> no sceneNode" );
		return false;
	}

	const Vector3 & l_vector = SceneFileParser::Parser_Vector3Value( p_params );

	if ( l_vector != Vector3::ZERO )
	{
		p_context->sceneNode->setDirection( l_vector.normalisedCopy(), Node::TS_WORLD );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeDirection -> direction zero ?" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeSound )
{
	p_context->section = SS_NODE_SOUND;
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( p_params );

	if ( l_sound == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeSound -> sound [" + p_params + "] does not exist" );
		return true;
	}

	p_context->soundInstance = l_sound->CreateInstance( p_context->sceneNode );
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodePlaylist )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : ParsePlaylistNode -> Param needed : playlist name" );
		return false;
	}

	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : ParsePlaylistNode -> Null Node" );
		return false;
	}

	SoundPlaylist * l_playlist = SoundManager::GetSingletonPtr()->GetSoundPlaylist( p_params );

	if ( ! l_playlist )
	{
		PARSING_ERROR( "Parsing Error : ParsePlaylistNode -> No playlist named [" + p_params + "]" );
	}
	else
	{
		l_playlist->SetNode( p_context->sceneNode, p_context->scene->GetName() );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeInheritOrientation )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeInheritOrientation -> no sceneNode" );
		return false;
	}

	bool l_inherit = StringConverter::parseBool( p_params );
	p_context->sceneNode->setInheritOrientation( l_inherit );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeInheritScale )
{
	if ( p_context->sceneNode == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneNodeInheritScale -> no sceneNode" );
		return false;
	}

	p_context->sceneNode->setInheritScale( StringConverter::parseBool( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneNodeEnd )
{
	p_context->section = SS_NONE;

	if ( p_context->sceneNode != NULL && p_context->sceneNode->getParentSceneNode() == NULL )
	{
		p_context->sceneManager->getRootSceneNode()->addChild( p_context->sceneNode );
	}

	p_context->sceneNode = NULL;
	p_context->general_string_a.clear();
	return false;
}

/********************************************************************************/
/*																				*/
/*										Light									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_LightType )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightType -> no light" );
		return false;
	}

	StringUtil::toLowerCase( p_params );

	if ( p_params == "directional" )
	{
		p_context->light->setType( Light::LT_DIRECTIONAL );
		p_context->general_int = 1;
	}
	else if ( p_params == "point" )
	{
		p_context->light->setType( Light::LT_POINT );
		p_context->general_int = 2;
	}
	else if ( p_params == "spotlight" )
	{
		p_context->light->setType( Light::LT_SPOTLIGHT );
		p_context->general_int = 3;
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_LightType -> wrong lightType" );
	}

	p_context->light->setCastShadows( true );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightPosition )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightPosition -> no light" );
		return false;
	}

	if ( p_context->light->getType() == Light::LT_DIRECTIONAL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightPosition -> Position is only for point lights or spot lights" );
		return false;
	}

	p_context->light->setPosition( SceneFileParser::Parser_Vector3Value( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightDirection )
{
	if	( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightDirection -> no light" );
		return false;
	}

	if ( p_context->light->getType() == Light::LT_POINT )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightDirection -> Position is only for directional lights or spot lights" );
		return false;
	}

	const Vector3 & l_vector = SceneFileParser::Parser_Vector3Value( p_params );

	if ( l_vector == Vector3::ZERO )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightDirection -> direction cannot be zero" );
		return false;
	}

	p_context->light->setDirection( l_vector.normalisedCopy() );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightDiffuse )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightDiffuse -> no light" );
		return false;
	}

	p_context->light->setDiffuseColour( SceneFileParser::Parser_ColourValue( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightSpecular )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightSpecular -> no light" );
		return false;
	}

	p_context->light->setSpecularColour( SceneFileParser::Parser_ColourValue( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightAttenuation )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightAttenuation -> no light" );
		return false;
	}

	if ( p_context->light->getType() == Light::LT_DIRECTIONAL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightAttenuation -> Attenuation is only for spot lights or spot lights" );
		return false;
	}

	const Vector4 & l_vector = SceneFileParser::Parser_Vector4Value( p_params );
	p_context->light->setAttenuation( l_vector.x, l_vector.y, l_vector.z, l_vector.w );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightRange )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightRange -> no light" );
		return false;
	}

	if ( p_context->light->getType() != Light::LT_SPOTLIGHT )
	{
		PARSING_ERROR( "Parsing Error : Parser_LightRange -> only for spotligts" );
		return false;
	}

	const Vector3 & l_vector = SceneFileParser::Parser_Vector3Value( p_params );

	if ( l_vector != Vector3::ZERO )
	{
		p_context->light->setSpotlightRange( Degree( l_vector.x ), Degree( l_vector.y ), l_vector.z );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightAttach )
{
	if ( p_context->light == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_AttachLight -> no light" );
		return false;
	}

	if ( p_context->light->isAttached() )
	{
		PARSING_ERROR( "Parsing Error : Parser_AttachLight -> Light [" + p_context->light->getName() + "] already attached" );
		return false;
	}

	if ( p_params == String( "root node" ) )
	{
		p_context->sceneManager->getRootSceneNode()->attachObject( p_context->light );
	}
	else
	{
		if ( p_context->sceneManager->hasSceneNode( p_params ) )
		{
			p_context->sceneManager->getSceneNode( p_params )->attachObject( p_context->light );
		}
		else
		{
			PARSING_ERROR( "Parsing Error : Parser_AttachLight -> sceneNode [ " + p_params + "]not found" );
		}
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_LightEnd )
{
	p_context->section = SS_NONE;
	p_context->light = NULL;
	p_context->general_int = -1;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Camera										*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_CameraPosition )
{
	if ( p_context->camera == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraPosition -> no camera" );
		return false;
	}

	p_context->camera->setPosition( SceneFileParser::Parser_Vector3Value( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraOrientation )
{
	if ( p_context->camera == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraOrientation -> no camera" );
		return false;
	}

	const Vector4 & l_orientation = SceneFileParser::Parser_Vector4Value( p_params );

	if ( l_orientation != Vector4::ZERO )
	{
		Quaternion q;
		q.FromAngleAxis( Degree( l_orientation.w ), Vector3( l_orientation.x, l_orientation.y, l_orientation.z ) );
		p_context->camera->setOrientation( q );
	}
	else
	{
		PARSING_ERROR( " Parsing Error : Parser_CameraOrientation -> direction is null ?" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraDirection )
{
	Vector3 V;

	if ( p_context->camera != NULL )
	{
		V = SceneFileParser::Parser_Vector3Value( p_params );

		if ( V != Vector3::ZERO )
		{
			V.normalise();
			p_context->camera->setDirection( V );
		}
		else
		{
			PARSING_ERROR( "Parsing Error : Parser_CameraDirection -> direction is zero ?" );
		}
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraDirection -> no camera" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraPolygonMode )
{
	if ( p_context->camera != NULL )
	{
		if ( p_params == "wireframe" )
		{
			p_context->camera->setPolygonMode( Ogre::PM_WIREFRAME );
		}
		else if ( p_params == "solid" )
		{
			p_context->camera->setPolygonMode( Ogre::PM_SOLID );
		}
		else if ( p_params == "point" )
		{
			p_context->camera->setPolygonMode( Ogre::PM_POINTS );
		}
		else
		{
			PARSING_ERROR( "Parsing Error : Parser_CameraPolygonMode -> wrong parameter for this function : " + p_params );
		}
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraPolygonMode -> no camera" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraFovY )
{
	if ( p_context->camera != NULL )
	{
		p_context->camera->setFOVy( Degree( StringConverter::parseReal( p_params ) ) );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraFovY -> no camera" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraTex )
{
	if ( p_context->camera == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraTex -> no camera" );
		return false;
	}

	if ( p_context->camTexManager->GetElementByName( p_context->camera->getName() ) != NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraTex -> camtex already exists : " + p_params );
		return false;
	}

	const StringVector & splitCmd = StringUtil::split( p_params, " \t" );

	if ( splitCmd.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_CameraTex -> wrong numbers of p_params : " + p_params );
		return false;
	}

	uint32_t l_width = static_cast <uint32_t>( StringConverter::parseInt( splitCmd[0] ) );
	uint32_t l_height = static_cast <uint32_t>( StringConverter::parseInt( splitCmd[1] ) );
	p_context->camTexManager->AddElement( std::make_shared< CamTex >( p_context->camera->getName(), p_context->sceneManager, p_context->camera, l_width, l_height ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraAttach )
{
	if ( p_context->camera == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_AttachCamera -> no camera" );
		return false;
	}

	if ( p_context->camera->isAttached() )
	{
		PARSING_ERROR( "Parsing Error : Parser_AttachCamera -> camera already attached" );
		return false;
	}

	if ( ! p_context->sceneManager->hasSceneNode( p_params ) )
	{
		PARSING_ERROR( "Parsing Error : Parser_AttachCamera -> sceneNode [" + p_params + "] does not exists" );
		return false;
	}

	p_context->sceneManager->getSceneNode( p_params )->attachObject( p_context->camera );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_CameraEnd )
{
	p_context->section = SS_NONE;
	p_context->camera = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Overlay										*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_OverlayClass )
{
//	OverlayContainer * cont;

	/*
	if (p_context->overlay == NULL)
	{
		if (OverlayManager::getSingleton().getOverlayElement( p_params, true) != NULL)
		{
			p_context->overlay = new EMOverlay( p_context->overlayGroup->GetOverlay(), OverlayManager::getSingletonPtr()->createOverlayElementFromTemplate( p_params, "", p_context->general_string_a, false));
			p_context->overlayGroup->AddElement(p_context->overlay);
			p_context->overlayGroup->SetFirstElement(p_context->overlay);
			p_context->gui->AddElement(p_context->overlay);
			p_context->overlayGroup->GetOverlay()->add2D( static_cast<OverlayContainer*>(p_context->overlay->GetOverlay()));
		}
		else
		{
			PARSING_ERROR("Parsing Error : Parser_OverlayClass -> overlay class named ["+p_params+"] does not exist");
		}
	}
	else
	{
		if( p_context->gui->GetElementByName( p_context->general_string_a) == NULL)
		{
			cont = (OverlayContainer *) p_context->overlay->GetOverlay();
			p_context->overlay = new EMOverlay (p_context->overlayGroup->GetOverlay(), OverlayManager::getSingletonPtr()->createOverlayElementFromTemplate(p_params,"",p_context->general_string_a,false),p_context->overlay);
			cont->addChild( (OverlayContainer *) p_context->overlay->GetOverlay());
			p_context->gui->AddElement( p_context->overlay);
			p_context->overlayGroup->AddElement( p_context->overlay);
		}
		else
		{
			p_context->overlay = p_context->gui->GetElementByName( p_context->general_string_a);
		}
	}
	*/
	if ( p_context->overlay != NULL )
	{
		p_context->overlay->Initialise( p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayMaterial )
{
	if ( p_context->overlay == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayMaterial -> no overlay" );
		return false;
	}

	if ( MaterialManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayMaterial -> material [" + p_params + "] does not exist" );
		return false;
	}

	p_context->overlay->SetBaseMaterial( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayBorderMaterial )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayBorderMaterial -> no overlay" );
		return false;
	}

	if ( MaterialManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayBorderMaterial -> material [" + p_params + "] does not exist" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "BorderPanel" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayBorderMaterial -> overlay [" + p_params + "] is not a borderPanel" );
		return false;
	}

	( static_cast <BorderPanelOverlayElement *>( p_context->overlay->GetOgreOverlayElement() ) )->setBorderMaterialName( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayBorderSize )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayBorderSize -> no overlay" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "BorderPanel" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayBorderSize -> overlay [" + p_context->overlay->GetOgreOverlayElement()->getName() + "] is not a border panel" );
		return false;
	}

	const Vector4 & l_vector = SceneFileParser::Parser_Vector4Value( p_params );
	( static_cast <BorderPanelOverlayElement *>( p_context->overlay->GetOgreOverlayElement() ) )->setBorderSize( l_vector.x / p_context->mainViewport->getActualWidth(),
			l_vector.y / p_context->mainViewport->getActualWidth(),
			l_vector.z / p_context->mainViewport->getActualHeight(),
			l_vector.w / p_context->mainViewport->getActualHeight() );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayPosition )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayPosition -> no overlay" );
		return false;
	}

	const StringVector & vecp_params = StringUtil::split( p_params, ", \t" );

	if ( vecp_params.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayPosition -> wrong numbers of parameters @ Overlay : " + p_context->overlay->GetOgreOverlayElement()->getName() );
		return false;
	}

	String l_stringX = vecp_params[0];
	Real x = StringConverter::parseReal( l_stringX );

	if ( l_stringX.find( "%" ) != String::npos )
	{
		if ( p_context->overlay->GetOgreOverlayElement()->getParent() != NULL )
		{
			x = ( x / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getParent()->getWidth();
		}
		else
		{
			x = ( x / 100.0f ) /* * p_context->mainViewport->getActualWidth()*/;
		}

		l_stringX.erase( 0, 1 );
		size_t l_index = min( l_stringX.find( "+" ), l_stringX.find( "-" ) );

		if ( l_index != String::npos )
		{
			x += StringConverter::parseReal( l_stringX.substr( l_index ) ) / p_context->mainViewport->getActualWidth();
		}
	}
	else
	{
		x /= p_context->mainViewport->getActualWidth();
	}

	String l_stringY = vecp_params[1];
	Real y = StringConverter::parseReal( l_stringY );

	if ( l_stringY.find( "%" ) != String::npos )
	{
		if ( p_context->overlay->GetOgreOverlayElement()->getParent() != NULL )
		{
			y = ( y / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getParent()->getHeight();
		}
		else
		{
			y = ( y / 100.0f ) /* * p_context->mainViewport->getActualHeight()*/ ;
		}

		l_stringX.erase( 0, 1 );
		size_t l_index = min( l_stringX.find( "+" ), l_stringX.find( "-" ) );

		if ( l_index != String::npos )
		{
			y += StringConverter::parseReal( l_stringY.substr( l_index ) ) / p_context->mainViewport->getActualHeight();
		}
	}
	else
	{
		y /= p_context->mainViewport->getActualHeight();
	}

	p_context->overlay->GetOgreOverlayElement()->setPosition( x, y );
	p_context->overlay->GetOgreOverlayElement()->_update();
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlaySize )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlaySize -> no overlay" );
		return false;
	}

	const StringVector & vecp_params = StringUtil::split( p_params, ", \t" );

	if ( vecp_params.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlaySize -> wrong numbers of parameters @ Overlay : " + p_context->overlay->GetOgreOverlayElement()->getName() );
		return false;
	}

	String l_stringX = vecp_params[0];
	Real x = StringConverter::parseReal( l_stringX );

	if ( l_stringX.find( "%" ) != String::npos )
	{
		if ( p_context->overlay->GetOgreOverlayElement()->getParent() != NULL )
		{
			x = ( x / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getParent()->getWidth();
		}
		else
		{
			x = ( x / 100.0f );
		}

		l_stringX.erase( 0, 1 );
		size_t l_index = min( l_stringX.find( "+" ), l_stringX.find( "-" ) );

		if ( l_index != String::npos )
		{
			x += StringConverter::parseReal( l_stringX.substr( l_index ) ) / p_context->mainViewport->getActualHeight();
		}
	}
	else
	{
		x /= p_context->mainViewport->getActualWidth();
	}

	String l_stringY = vecp_params[1];
	Real y = StringConverter::parseReal( l_stringY );

	if ( l_stringY.find( "%" ) != String::npos )
	{
		if ( p_context->overlay->GetOgreOverlayElement()->getParent() != NULL )
		{
			y = ( y / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getParent()->getHeight();
		}
		else
		{
			y = ( y / 100.0f );
		}

		l_stringY.erase( 0, 1 );
		size_t l_index = min( l_stringX.find( "+" ), l_stringX.find( "-" ) );

		if ( l_index != String::npos )
		{
			y += StringConverter::parseReal( l_stringY.substr( l_index ) ) / p_context->mainViewport->getActualHeight();
		}
	}
	else
	{
		y /= p_context->mainViewport->getActualHeight();
	}

	p_context->overlay->GetOgreOverlayElement()->setDimensions( x, y );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayAlign )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayAlign -> no overlay" );
		return false;
	}

	const StringVector & vecp_params = StringUtil::split( p_params, ", \t\n" );

	if ( vecp_params.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayAlign -> wrong number of parameters : " + p_params );
		return false;
	}

	if ( vecp_params[1] == "left" )
	{
		p_context->overlay->GetOgreOverlayElement()->setHorizontalAlignment( Ogre::GHA_LEFT );
	}
	else if ( vecp_params[1] == "center" )
	{
		p_context->overlay->GetOgreOverlayElement()->setHorizontalAlignment( Ogre::GHA_CENTER );
	}
	else if ( vecp_params[1] == "right" )
	{
		p_context->overlay->GetOgreOverlayElement()->setHorizontalAlignment( Ogre::GHA_RIGHT );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayAlign -> unrecognised horizontal option : " + vecp_params[1] );
	}

	if ( vecp_params[0] == "top" )
	{
		p_context->overlay->GetOgreOverlayElement()->setVerticalAlignment( Ogre::GVA_TOP );
	}
	else if ( vecp_params[0] == "center" )
	{
		p_context->overlay->GetOgreOverlayElement()->setVerticalAlignment( Ogre::GVA_CENTER );
	}
	else if ( vecp_params[0] == "bottom" )
	{
		p_context->overlay->GetOgreOverlayElement()->setVerticalAlignment( Ogre::GVA_BOTTOM );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayAlign -> unrecognised vertical option" + vecp_params[0] );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayTextColour )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayTextColour -> no overlay" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "TextArea" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayTextColour -> OverlayElement [" + p_context->overlay->GetOgreOverlayElement()->getName() + "] is not a textArea" );
		return false;
	}

	const ColourValue & l_colour = SceneFileParser::Parser_ColourValue( p_params );
	( static_cast <TextAreaOverlayElement *>( p_context->overlay->GetOgreOverlayElement() ) )->setColourTop( l_colour );
	( static_cast <TextAreaOverlayElement *>( p_context->overlay->GetOgreOverlayElement() ) )->setColourBottom( l_colour );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayTextAlign )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayTextAlign -> no overlay" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "TextArea" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayTextAlign -> OverlayElement [" + p_context->overlay->GetOgreOverlayElement()->getName() + "] is not a textArea" );
		return false;
	}

	if ( p_params == "right" )
	{
		p_context->overlay->SetAlignment( TextAreaOverlayElement::Right );
	}
	else if ( p_params == "center" )
	{
		p_context->overlay->SetAlignment( TextAreaOverlayElement::Center );
	}
	else
	{
		p_context->overlay->SetAlignment( TextAreaOverlayElement::Left );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayMouseOverMaterial )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayMouseOverMaterial -> no overlay" );
		return false;
	}

	if ( MaterialManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayMouseOverMaterial -> material [" + p_params + "] does not exist" );
		return false;
	}

	p_context->overlay->SetMouseOverMaterial( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayClickedMaterial )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayClickedMaterial -> no overlay" );
		return false;
	}

	if ( MaterialManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayClickedMaterial -> material [" + p_params + "] does not exist" );
		return false;
	}

	p_context->overlay->SetClickedMaterial( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayText )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayText -> no overlay" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "TextArea" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayText -> OverlayElement [" + p_context->overlay->GetOgreOverlayElement()->getName() + "] is not a text area" );
		return false;
	}

	p_params = string::replace( p_params, "|", "\n" );
	p_context->overlay->SetCaption( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayFontSize )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayFontSize -> no overlay" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "TextArea" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayFontSize -> OverlayElement [" + p_context->overlay->GetOgreOverlayElement()->getName() + "] is not a text area" );
		return false;
	}

	static_cast <TextAreaOverlayElement *>( p_context->overlay->GetOgreOverlayElement() )->setCharHeight( StringConverter::parseReal( p_params ) / p_context->mainViewport->getActualHeight() );

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayFontName )
{
	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayFontName -> no overlay" );
		return false;
	}

	if ( p_context->overlay->GetOgreOverlayElement()->getTypeName() != "TextArea" )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayFontName -> OverlayElement [" + p_context->overlay->GetOgreOverlayElement()->getName() + "] is not a text area" );
		return false;
	}

	if ( FontManager::getSingletonPtr()->getByName( p_params ).isNull() )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayFontName -> font [" + p_params + "] does not exist" );
		return false;
	}

//	String l_string = ResourceGroupManager::getPrefix();
//	ResourceGroupManager::setPrefix( "");
	static_cast <TextAreaOverlayElement *>( p_context->overlay->GetOgreOverlayElement() )->setFontName( p_params );
//	ResourceGroupManager::setPrefix( l_string);
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayShape )
{
	p_context->section = SS_CLICKABLESHAPE;

	if ( p_context->overlay == NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayShape -> no overlay" );
		return true;
	}

	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_OverlayShape -> parameters are required" );
		return true;
	}

	const StringVector & l_split = StringUtil::split( p_params, ", \t" );

	if ( l_split[0] == "box" )
	{
		if ( l_split.size() != 3 )
		{
			PARSING_ERROR( "Parsing Error : Parser_OverlayShape -> 3 parameters are required : box width height" );
			return true;
		}

		Real l_x = StringConverter::parseReal( l_split[1] );
		Real l_y = StringConverter::parseReal( l_split[2] );

		if ( l_split[1].find( "%" ) != String::npos )
		{
			l_x = ( l_x / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getWidth();
		}

		l_x /= p_context->mainViewport->getActualWidth();

		if ( l_split[2].find( "%" ) != String::npos )
		{
			l_y = ( l_y / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getHeight();
		}

		l_y /= p_context->mainViewport->getActualHeight();
		p_context->clickableShape = new ClickableBox( l_x, l_y );
		p_context->overlay->AddClickableShape( p_context->clickableShape );
	}
	else if ( l_split[0] == "circle" )
	{
		if ( l_split.size() != 2 )
		{
			PARSING_ERROR( "Parsing Error : Parser_OverlayShape -> 3 parameters are required : circle radius" );
			return true;
		}

		Real l_size = StringConverter::parseReal( l_split[1] );

		if ( l_split[1].find( "%" ) != String::npos )
		{
			l_size = ( l_size / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getHeight();
		}

		l_size /= p_context->mainViewport->getActualHeight();
		p_context->clickableShape = new ClickableCircle( l_size * 0.5f );
		p_context->overlay->AddClickableShape( p_context->clickableShape );
	}
	else if ( l_split[0] == "image" )
	{
		if ( l_split.size() != 2 )
		{
			PARSING_ERROR( "Parsing Error : Parser_OverlayShape -> 3 parameters are required : image name_of_the_file" );
			return true;
		}

		p_context->clickableShape = new ClickableImage( l_split[1], p_context->overlay->GetOgreOverlayElement()->getWidth() / p_context->mainViewport->getActualWidth(), p_context->overlay->GetOgreOverlayElement()->getHeight() / p_context->mainViewport->getActualHeight() );
		p_context->overlay->AddClickableShape( p_context->clickableShape );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayVideo )
{
	p_context->section = SS_OVERLAY_VIDEO;

	if ( p_context->overlay != NULL || p_context->overlay->GetOgreOverlayElement() == NULL )
	{
		VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_params );

		if ( l_object == NULL )
		{
			PARSING_ERROR( "Parsing Error : Parser_OverlayVideo -> video named " + p_params + " does not" );
			return true;
		}

		l_object->CreateOverlay( p_context->overlay );
	}

	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayZIndex )
{
	if ( p_context->overlayGroup != NULL )
	{
		p_context->overlayGroup->GetOverlay()->setZOrder( uint16_t( StringConverter::parseUnsignedInt( p_params ) ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_OverlayEnd )
{
	if ( p_context->general_int > 0 )
	{
		p_context->general_int --;

		if ( p_context->overlay != NULL )
		{
			p_context->overlay = p_context->overlay->GetParent();
		}
	}
	else
	{
		p_context->section = SS_NONE;
		p_context->overlay = NULL;
	}

	return false;
}

/********************************************************************************/
/*																				*/
/*									Sound										*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_SoundURL )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundURL -> not enough p_params " + p_params );
		return false;
	}

	if ( p_context->soundObject != NULL )
	{
		if ( p_params.substr( 0, 7 ) == "http://" )
		{
			p_context->soundObject->SetSoundUrl( p_params );
		}
		else
		{
			p_context->soundObject->SetSoundUrl( p_context->baseDirectory + "//" + p_params );
		}
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundGlobal )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundGlobal -> not enough p_params " + p_params );
		return false;
	}

	if ( p_context->soundObject != NULL && p_params == "false" )
	{
		p_context->soundObject->SetLocal( p_context->scene->GetName() );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundLooped )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundLooped -> not enough p_params " + p_params );
		return false;
	}

	if ( p_params != "true" && p_params != "false" )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundLooped -> wrong param type " + p_params );
		return false;
	}

	if ( p_context->soundObject != NULL )
	{
		p_context->soundObject->SetLooped( p_params == "true" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundTimer )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundTimer -> not enough p_params " + p_params );
		return false;
	}

	if ( p_context->soundObject != NULL && p_context->soundObject->GetGlobal() != NULL )
	{
		p_context->soundObject->GetGlobal()->SetTimerTime( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundType )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundType -> not enough p_params " + p_params );
		return false;
	}

	if ( p_context->soundObject == NULL )
	{
		return false;
	}

	if ( p_params == "sfx" || p_params == "2" )
	{
		p_context->soundObject->SetSoundType( EM_SFX );
	}
	else if ( p_params == "music" || p_params == "1" )
	{
		p_context->soundObject->SetSoundType( EM_MUSIC );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundName )
{
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundFadeInLength )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundFadeInLength -> not enough p_params " + p_params );
	}

	if ( p_context->soundObject != NULL && p_context->soundObject->GetGlobal() != NULL )
	{
		p_context->soundObject->GetGlobal()->SetFadeIn( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundFadeOutLength )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundFadeOutLength -> not enough p_params " + p_params );
	}

	if ( p_context->soundObject != NULL && p_context->soundObject->GetGlobal() != NULL )
	{
		p_context->soundObject->GetGlobal()->SetFadeOut( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundVolumePercent )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_SoundVolumePercent -> not enough p_params " + p_params );
	}

	if ( p_context->soundObject != NULL )
	{
		p_context->soundObject->SetVolumePercent( MAXVOLUME - StringConverter::parseReal( p_params ) );
	}
	else if ( p_context->soundInstance != NULL )
	{
		p_context->soundInstance->SetVolumePercent( MAXVOLUME - StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundEnd )
{
	if ( p_context->soundObject != NULL )
	{
		p_context->soundObject->Initialise();
	}

	p_context->section = SS_NONE;
	p_context->soundObject = NULL;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SoundNodeEnd )
{
	p_context->section = SS_NODE;
	p_context->soundInstance = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Playlist									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistSound )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistSound -> Param needed (true/false)" );
	}
	else
	{
		p_context->soundPlaylist->AddSoundObject( SoundManager::GetSingletonPtr()->GetSoundObject( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistRandom )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistRandom -> Param needed (true/false)" );
	}
	else if ( p_params == "true" || p_params == "false" )
	{
		p_context->soundPlaylist->SetRandom( StringConverter::parseBool( p_params ) );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistRandom -> Wrong param : " + p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistLooped )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistLooped -> Param needed (true/false)" );
	}
	else if ( p_params == "true" || p_params == "false" )
	{
		p_context->soundPlaylist->SetLooped( StringConverter::parseBool( p_params ) );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistLooped -> Wrong param : " + p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistTimer )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistTimer -> Param needed (fixed/random)" );
		return false;
	}

	p_context->soundPlaylist->SetTimerType( p_params == "random" ? TT_Random : TT_Fixed );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistTimerTime )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistTimerTime -> Float param needed" );
		return false;
	}

	p_context->soundPlaylist->SetTimerTime( StringConverter::parseReal( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistTimerMin )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistTimerMin -> Float param needed" );
	}
	else
	{
		p_context->soundPlaylist->SetTimerMin( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistTimerMax )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistTimerMax -> Float param needed" );
	}
	else
	{
		p_context->soundPlaylist->SetTimerMax( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistVolumePercent )
{
	if ( p_params.empty() )
	{
		PARSING_ERROR( "Parsing Error : Parser_PlaylistVolumePercent -> Float param needed" );
	}
	else
	{
		p_context->soundPlaylist->SetVolumePercent( 1.0f - StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PlaylistEnd )
{
	p_context->section = SS_NONE;
	return false;
}

/********************************************************************************/
/*																				*/
/*										Video									*/
/*																				*/
/********************************************************************************/


DEFINE_SCENE_FILE_PARSER( Parser_VideoURL )
{
	if ( p_context->videoObject != NULL )
	{
		p_context->videoObject->SetUrl( p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_VideoLooped )
{
	//TODO
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_VideoEnabled )
{
	p_context->videoObject->SetAutoInitialise( StringConverter::parseBool( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_VideoTokenURL )
{
	if ( p_context->videoObject != NULL )
	{
		p_context->videoObject->SetTokenUrl( p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_VideoEnd )
{
	if ( p_context->videoObject != NULL )
	{
		p_context->videoObject->SetBasePath( p_context->baseDirectory );

		if ( p_context->videoObject->IsAutoInitialised() )
		{
			p_context->videoObject->Initialise();
		}

		p_context->videoObject = NULL;
	}

	p_context->section = SS_NONE;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Video Object								*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_VideoObjectVolume )
{
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_VideoObjectEnd )
{
	p_context->section = SS_OBJECT;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Video Overlay								*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_VideoOverlayVolume )
{
//	p_context->general_int = StringConverter::parseInt( p_params);
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_VideoOverlayEnd )
{
	p_context->section = SS_OVERLAY;
	return false;
}

/********************************************************************************/
/*																				*/
/*										Pub										*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_PubURL )
{
	p_context->pubObject->SetUrl( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PubImageURL )
{
	p_context->pubObject->SetImageUrl( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PubClickURL )
{
	p_context->pubObject->SetLinkUrl( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PubEnd )
{
	p_context->pubObject->Finalise();
	p_context->pubObject = NULL;
	p_context->section = SS_NONE;
	p_context->general_string_a.clear();
	return false;
}

/********************************************************************************/
/*																				*/
/*										Mirror									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_MirrorPosition )
{
	if ( p_context->mirror != NULL )
	{
		p_context->mirror->SetPosition( SceneFileParser::Parser_Vector3Value( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_MirrorDirection )
{
	if ( p_context->mirror == NULL )
	{
		return false;
	}

	p_context->mirror->SetNormal( SceneFileParser::Parser_Vector3Value( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_MirrorResolution )
{
	if ( p_context->mirror == NULL )
	{
		return false;
	}

	const Ogre::StringVector & l_p_params = StringUtil::split( p_params, " \t" );

	if ( l_p_params.size() != 2 )
	{
		return false;
	}

	uint32_t l_width = StringConverter::parseUnsignedInt( l_p_params[0] );
	uint32_t l_height = StringConverter::parseUnsignedInt( l_p_params[1] );
	p_context->mirror->SetTextureResolution( l_width, l_height );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_MirrorBackground )
{
	if ( p_context->mirror == NULL )
	{
		return false;
	}

	p_context->mirror->SetBackground( SceneFileParser::Parser_ColourValue( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_MirrorMaterial )
{
	String l_matName;
	Real l_alpha;

	if ( p_context->mirror != NULL )
	{
		const Ogre::StringVector & l_p_params = StringUtil::split( p_params, " \t" );

		if ( ! l_p_params.empty() )
		{
			l_matName = l_p_params[0];

			if ( l_p_params.size() > 1 )
			{
				l_alpha = StringConverter::parseReal( l_p_params[1] );
			}
			else
			{
				l_alpha = 1.0;
			}

			p_context->mirror->AddMaterial( l_matName, l_alpha );
		}
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_MirrorUpdates )
{
	bool l_update1, l_update2;

	if ( p_context->mirror != NULL )
	{
		const Ogre::StringVector & l_p_params = StringUtil::split( p_params, " \t" );

		if ( ! l_p_params.empty() )
		{
			l_update1 = StringConverter::parseBool( l_p_params[0] );

			if ( l_p_params.size() > 1 )
			{
				l_update2 = StringConverter::parseBool( l_p_params[1] );
			}
			else
			{
				l_update2 = false;
			}

			p_context->mirror->SetAutoUpdate( l_update1, l_update2 );
		}
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_MirrorEnd )
{
	if ( p_context->mirror != NULL )
	{
		p_context->mirror->SetMainCamera( p_context->mainCamera );
		p_context->mirror->Initialise( p_context->mirrorManager->GetInstanceKey(), p_context->sceneManager );
	}

	p_context->section = SS_NONE;
	return false;
}

/********************************************************************************/
/*																				*/
/*										Scene									*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_SceneFile )
{
	if ( p_context->scene == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneFile -> no scene" );
		return false;
	}

	ConfigFile * l_cFile = NULL;
	size_t l_index = p_params.find( "@" );

	if ( l_index != String::npos )
	{
		String const & l_filename = p_params.substr( 0, l_index );
		String const & l_musepackName = p_params.substr( l_index, String::npos );

		if ( l_filename.empty() || l_musepackName.empty() )
		{
			PARSING_ERROR( "Parsing Error : Parser_SceneFile -> wrong syntax for this directive" );
		}

		MuseFile * l_mFile = ElypseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( l_musepackName );

		if ( l_mFile != NULL )
		{
			l_cFile = l_mFile->GetConfigFile( l_filename );
		}
	}
	else
	{
//		std::cout << "Add scene file : " << p_context->currentMuseFile->GetName() << " \\ " << p_params << std::endl;
		l_cFile = p_context->currentMuseFile->GetConfigFile( p_params );
	}

	if ( l_cFile == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneFile -> file : " + p_params + " does not exist" );
		return false;
	}

	p_context->scene->AddSceneFile( l_cFile );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneDataFile )
{
	if ( p_context->scene == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneDataFile -> no scene" );
		return false;
	}

	DataFile * l_dFile = NULL;
	size_t l_index = p_params.find( "@" );

	if ( l_index != String::npos )
	{
		String const & l_filename = p_params.substr( 0, l_index );
		String const & l_musepackName = p_params.substr( l_index, String::npos );

		if ( l_filename.empty() || l_musepackName.empty() )
		{
			PARSING_ERROR( "Parsing Error : Parser_SceneDataFile -> wrong syntax for this directive" );
		}

		MuseFile * l_mFile = ElypseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( l_musepackName );

		if ( l_mFile != NULL )
		{
			l_dFile = l_mFile->GetDataFile( l_filename );
		}
	}
	else
	{
		l_dFile = p_context->currentMuseFile->GetDataFile( p_params );
	}

	if ( l_dFile == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneDataFile -> file : " + p_params + " does not exist" );
		return false;
	}

	p_context->scene->AddDataFile( l_dFile );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneLoadScriptFile )
{
	if ( p_context->scene == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneLoadScriptFile -> no scene" );
		return false;
	}

	ConfigFile * l_cFile = NULL;
	size_t l_index = p_params.find( "@" );

	if ( l_index != String::npos )
	{
		String const & l_filename = p_params.substr( 0, l_index );
		String const & l_musepackName = p_params.substr( l_index, String::npos );

		if ( l_filename.empty() || l_musepackName.empty() )
		{
			PARSING_ERROR( "Parsing Error : Parser_SceneLoadScriptFile -> wrong syntax for this directive" );
		}

		MuseFile * l_mFile = ElypseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( l_musepackName );

		if ( l_mFile != NULL )
		{
			l_cFile = l_mFile->GetConfigFile( l_filename );
		}
	}
	else
	{
		l_cFile = p_context->currentMuseFile->GetConfigFile( p_params );
	}

	if ( l_cFile == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneLoadScriptFile -> file : " + p_params + " does not exist" );
		return false;
	}

	p_context->scene->AddLoadScript( l_cFile );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneUnloadScriptFile )
{
	if ( p_context->scene == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneUnloadScriptFile -> no scene" );
		return false;
	}

	ConfigFile * l_cFile = NULL;
	size_t l_index = p_params.find( "@" );

	if ( l_index != String::npos )
	{
		String const & l_filename = p_params.substr( 0, l_index );
		String const & l_musepackName = p_params.substr( l_index, String::npos );

		if ( l_filename.empty() || l_musepackName.empty() )
		{
			PARSING_ERROR( "Parsing Error : Parser_SceneUnloadScriptFile -> wrong syntax for this directive" );
		}

		MuseFile * l_mFile = ElypseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( l_musepackName );

		if ( l_mFile != NULL )
		{
			l_cFile = l_mFile->GetConfigFile( l_filename );
		}
	}
	else
	{
		l_cFile = p_context->currentMuseFile->GetConfigFile( p_params );
	}

	if ( l_cFile == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SceneUnloadScriptFile -> file : " + p_params + " does not exist" );
		return false;
	}

	p_context->scene->AddUnloadScript( l_cFile );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneMuseFile )
{
	p_context->baseURL.GetProtocol();
	Url l_url = p_params;

	if ( /*l_url.GetProtocol() == LOCAL || */ l_url.GetProtocol() == HTTP )
	{
		p_context->currentMuseFile = p_context->downloadManager->StartDownloadFile( l_url );
	}
	else
	{
		p_context->currentMuseFile = p_context->downloadManager->StartDownloadFile( p_context->baseURL.GetAllButFilename() + p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SceneEnd )
{
	p_context->section = SS_NONE;
	p_context->scene = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Particle System								*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_ParticleSystemClass )
{
	if ( p_context->particleSystem == NULL )
	{
		p_context->particleSystem = p_context->sceneManager->createParticleSystem( p_context->general_string_a, p_params );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ParticleSystemAttach )
{
	SceneNode * node;

	if ( p_context->particleSystem != NULL )
	{
		if ( ! p_context->particleSystem->isAttached() )
		{
			if ( p_context->sceneManager->hasSceneNode( p_params ) || p_params == "root node" )
			{
				if ( p_params == "root node" )
				{
					node = p_context->sceneManager->getRootSceneNode();
				}
				else
				{
					node = p_context->sceneManager->getSceneNode( p_params );
				}

				node->attachObject( p_context->particleSystem );
			}
			else
			{
				PARSING_ERROR( "Parsing Error : Parser_ParticleSystemAttach -> sceneNode not found : [" + p_params + "]" );
			}
		}
		else
		{
			PARSING_ERROR( "Parsing Error : Parser_ParticleSystemAttach -> Particle system [" + p_context->particleSystem->getName() + "] already attached" );
		}
	}
	else
	{
		PARSING_ERROR( "Parsing Error : Parser_ParticleSystemAttach -> no particle system" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ParticleSystemEnd )
{
	if ( p_context->particleSystem && !p_context->particleSystem->isAttached() )
	{
		p_context->sceneManager->getRootSceneNode()->attachObject( p_context->particleSystem );
	}

	p_context->section = SS_NONE;
	p_context->particleSystem = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Physics Material							*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsFriction )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetFriction( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsAddFriction )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetAddFriction( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsBouncyness )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetBouncyness( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsBounceLimit )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetVelocityThreshold( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSoftERP )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetErp( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSoftCFM )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetCfm( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsIndependantVel )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetIndependantVelocity( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsAddVel )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetAddVelocity( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsRollFriction )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetRollFrictionCoeff( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsRollDampen )
{
	if ( p_context->physicsMaterial )
	{
		p_context->physicsMaterial->SetRollFrictionDampen( StringConverter::parseReal( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsEnd )
{
	p_context->section = SS_NONE;
	p_context->physicsMaterial = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Object Physics								*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsMass )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	Real l_mass = StringConverter::parseReal( p_params );

	if ( l_mass <= 0.0 )
	{
		PARSING_ERROR( "A mass cannot be <= 0.0" );
	}
	else
	{
		p_context->physicsObject->SetMass( l_mass );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingSphere )
{
	if ( p_context->physicsObject == NULL )
	{
		return true;
	}

	BoundingSphere * l_bsphere = new BoundingSphere();
	l_bsphere->SetSpace( p_context->physicsSpace );

	if ( p_params.find( " " ) != String::npos || p_params.find( "\t" ) != String::npos )
	{
		Vector4 l_v4 = SceneFileParser::Parser_Vector4Value( p_params );
		l_bsphere->SetRadius( l_v4.x );
		l_bsphere->SetOffset( l_v4.y, l_v4.z, l_v4.w );
	}
	else
	{
		l_bsphere->SetRadius( StringConverter::parseReal( p_params ) );
	}

	p_context->physicsObject->AddBound( l_bsphere );
	p_context->boundingShape = l_bsphere;
	p_context->section = SS_BOUNDING_SHAPE;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingBox )
{
	if ( p_context->physicsObject == NULL )
	{
		return true;
	}

	BoundingBox * l_box = new BoundingBox();
	l_box->SetSpace( p_context->physicsSpace );
	Vector3 l_size = SceneFileParser::Parser_Vector3Value( p_params );
	l_box->SetExtends( l_size );
	p_context->physicsObject->AddBound( l_box );
	p_context->boundingShape = l_box;
	p_context->section = SS_BOUNDING_SHAPE;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingMesh )
{
	if ( p_context->physicsObject == NULL )
	{
		return true;
	}

	try
	{
		MeshManager::getSingletonPtr()->load( p_params, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
		Mesh * l_mesh = static_cast<MeshPtr>( MeshManager::getSingletonPtr()->getByName( p_params ) ).getPointer();

		if ( l_mesh != NULL )
		{
			BoundingMesh * l_bmesh = new BoundingMesh( l_mesh );
			p_context->physicsObject->AddBound( l_bmesh );
			p_context->boundingShape = l_bmesh;
			p_context->section = SS_BOUNDING_SHAPE;
			return true;
		}
		else
		{
			PARSING_ERROR( "Parsing Error : Parser_ObjectPhysicsBoundingMesh -> mesh [" + p_params + "]does not exist" );
		}
	}
	catch ( Ogre::Exception & p_e )
	{
		PARSING_ERROR( "Parsing Error : Parser_ObjectPhysicsBoundingMesh -> mesh [" + p_params + "] created an error : " + p_e.what() );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsStatic )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	p_context->physicsObject->SetStatic( true );
	p_context->physicsSimulation->GetStaticSpace()->AddObject( p_context->physicsObject );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsPhantom )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	p_context->physicsObject->SetPhantom( true );
	p_context->physicsSimulation->GetPhantomSpace()->AddObject( p_context->physicsObject );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsFloating )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	p_context->physicsObject->SetFloating( true );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsDisabled )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	p_context->physicsObject->SetEnabled( false );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsInitialForce )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	p_context->physicsObject->SetForce( SceneFileParser::Parser_Vector3Value( p_params ) / p_context->physicsSimulation->GetSteptime() );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsInitialTorque )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	p_context->physicsObject->SetTorque( SceneFileParser::Parser_Vector3Value( p_params ) / p_context->physicsSimulation->GetSteptime() );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsDamping )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	const StringVector & vecp_params = StringUtil::split( p_params, ", \t" );

	if ( vecp_params.size() != 2 )
	{
		PARSING_ERROR( "Warning : Parser_ObjectPhysicsDamping -> Wrong number of parameters : expected 2" );
	}
	else
	{
		p_context->physicsObject->SetLinearDamping( StringConverter::parseReal( vecp_params[0] ) );
		p_context->physicsObject->SetAngularDamping( StringConverter::parseReal( vecp_params[0] ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsMaterial )
{
	if ( p_context->physicsObject == NULL )
	{
		return false;
	}

	PhysicsMaterial * l_material = p_context->physicsEngine->GetMaterial( p_params );

	if ( l_material != NULL )
	{
		p_context->physicsObject->SetMaterial( l_material );
	}
	else
	{
		PARSING_ERROR( "Warning : physics material " + p_params + " does not exist" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsEnd )
{
	if ( p_context->object == NULL )
	{
		p_context->section = SS_NONE;
	}
	else
	{
		p_context->section = SS_OBJECT;
	}

	p_context->physicsObject = NULL;
	return false;
}

/********************************************************************************/
/*																				*/
/*									Physics Simulation							*/
/*																				*/
/********************************************************************************/

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationERP )
{
	Real l_erp = StringConverter::parseReal( p_params );

	if ( l_erp < 0.0 || l_erp > 1.0 )
	{
		PARSING_ERROR( "Parameter Error : Parser_PhysicsSimulationERP -> erp has to be between 0.0 and 1.0" );
	}
	else
	{
		p_context->physicsSimulation->SetERP( l_erp );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationCFM )
{
	Real l_cfm = StringConverter::parseReal( p_params );

	if ( l_cfm < 0.0 || l_cfm > 1.0 )
	{
		PARSING_ERROR( "Parameter Error : Parser_PhysicsSimulationCFM -> cfm has to be between 0.0 and 1.0" );
	}
	else
	{
		p_context->physicsSimulation->SetCFM( l_cfm );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationGravity )
{
	Vector3 l_gravity = SceneFileParser::Parser_Vector3Value( p_params );

	if ( l_gravity.squaredLength() > 1e+6 )
	{
		PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationGravity -> gravity is too high. Expect unstable simulation" );
	}

	p_context->physicsSimulation->SetGravity( l_gravity );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationTimescale )
{
	Real l_timescale = StringConverter::parseReal( p_params );

	if ( l_timescale < 0.0 )
	{
		PARSING_ERROR( "Parameter Error : Parser_PhysicsSimulationTimescale -> the timescale cannot be negative" );
	}
	else
	{
		if ( l_timescale > 1.0 )
		{
			PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationTimescale -> timescale is too high. Expect unstable simulation" );
		}
		else if ( l_timescale == 0.0 )
		{
			PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationTimescale -> timescale is 0.0, simulation will not run.Use of the keyword \"disabled\" is preferred" );
		}

		p_context->physicsSimulation->SetTimescale( l_timescale );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationAutoUpdate )
{
	p_context->physicsSimulation->SetAutoUpdated( StringConverter::parseBool( p_params ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationDisabled )
{
	p_context->physicsSimulation->SetEnabled( false );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationSteptime )
{
	Real l_steptime = StringConverter::parseReal( p_params );

	if ( l_steptime <= 0.0 )
	{
		PARSING_ERROR( "Parameter Error : Parser_PhysicsSimulationSteptime -> the steptime cannot be negative" );
	}
	else
	{
		if ( l_steptime > 0.1 )
		{
			PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationSteptime -> steptime is too high. Expect unstable / imprecise simulation" );
		}
		else if ( l_steptime < 0.001 )
		{
			PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationSteptime -> steptime is too low, expect very low framerates unless coupled with the timescale parameter." );
		}

		p_context->physicsSimulation->SetSteptime( l_steptime );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationSteptimeLimit )
{
	Real l_steptimeLimit = StringConverter::parseReal( p_params );

	if ( l_steptimeLimit <= 0.0 )
	{
		PARSING_ERROR( "Parameter Error : Parser_PhysicsSimulationSteptimeLimit -> the steptime limit cannot be negative" );
	}
	else
	{
		if ( l_steptimeLimit > 0.5 )
		{
			PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationSteptimeLimit -> steptime limit is too high. Expect possible freezes on low end systems" );
		}
		else if ( l_steptimeLimit < p_context->physicsSimulation->GetSteptime() )
		{
			PARSING_ERROR( "Parameter Warning : Parser_PhysicsSimulationSteptimeLimit -> steptime limit is low compared to steptime, expect jittery simulation" );
		}

		p_context->physicsSimulation->SetSteptimeLimit( l_steptimeLimit );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationDefaultDamping )
{
	const StringVector & vecp_params = StringUtil::split( p_params, ", \t" );

	if ( vecp_params.size() != 2 )
	{
		PARSING_ERROR( "Warning : Parser_PhysicsSimulationDefaultDamping -> Wrong number of parameters : expected 2" );
	}
	else
	{
		p_context->physicsSimulation->SetDefaultDamping( StringConverter::parseReal( vecp_params[0] ), StringConverter::parseReal( vecp_params[1] ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationDefaultAutoDisable )
{
	Vector3 l_v3 = SceneFileParser::Parser_Vector3Value( p_params );
	p_context->physicsSimulation->SetDefaultAutoDisable( l_v3.x, l_v3.y, l_v3.z );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PhysicsSimulationEnd )
{
	p_context->section = SS_NONE;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingPlane )
{
	/*
	const StringVector & vecp_params = StringUtil::split( p_params, ", \t");

	if (vecp_params.size() == 4)
	{

		BoundingPlane * l_plane = new BoundingPlane();

		l_plane->SetSpace( p_context->physicsSpace);

		Vector3 normal( StringConverter::parseReal( vecp_params[0]), StringConverter::parseReal( vecp_params[1]), StringConverter::parseReal( vecp_params[2]));

		l_plane->Define( Plane( normal, StringConverter::parseReal( vecp_params[3])));

		p_context->physicsObject->AddBound( l_plane);
	}
	else
	{
		PARSING_ERROR( "Error : Parser_PhysicsCollideBoundingPlane, wrong number of parameters 4 or 6 required : " + p_params);
	}
	*/
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BoundingShapeTranslate )
{
	const Vector3 & l_position = SceneFileParser::Parser_Vector3Value( p_params );
	p_context->boundingShape->SetPosition( l_position );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BoundingShapeRotate )
{
	const Vector4 & l_vector = SceneFileParser::Parser_Vector4Value( p_params );
	Quaternion q;
	q.FromAngleAxis( Degree( l_vector.w ), Vector3( l_vector.x, l_vector.y, l_vector.z ) );
	q.normalise();
	p_context->boundingShape->SetOrientation( q );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BoundingShapeEnd )
{
	p_context->section = SS_OBJECT_PHYSICS;
	p_context->boundingShape = NULL;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PortalTrigger )
{
	Entity * l_object;

	if ( p_context->portal == NULL )
	{
		PARSING_ERROR( "Parsing Error : parsePortalTrigger -> no currentPortal : [" + p_params + "]" );
		return false;
	}

	if ( ! p_context->sceneManager->hasEntity( p_params ) )
	{
		PARSING_ERROR( "Parsing Error : parsePortalTrigger -> object does not exist ? [" + p_params + "]" );
		return false;
	}

	l_object = p_context->sceneManager->getEntity( p_params );

	if ( ! l_object->isAttached() )
	{
		PARSING_ERROR( "Parsing Error : parsePortalTrigger -> trigger object not attached to a scene node : [" + p_params + "]" );
		return false;
	}

	String l_groupName = ResourceGroupManager::getSingletonPtr()->findGroupContainingResource( l_object->getMesh()->getName() );
	Ogre::FileInfoListPtr filptr = ResourceGroupManager::getSingletonPtr()->findResourceFileInfo( l_groupName, "coll_tr_" + l_object->getMesh()->getName() );

	if ( filptr.getPointer()->size() == 0 )
	{
		PARSING_ERROR( "Parsing Error : parsePortalTrigger -> BoundingMesh not found : coll_tr_" + l_object->getMesh()->getName() + " ? @ trigger : [" + p_params + "]" );
		return false;
	}

	String MeshName = filptr.getPointer()->at( 0 ).basename;
	PhysicsObject * l_obj = p_context->portal->GetTrigger();

	if ( l_obj == NULL )
	{
		l_obj = p_context->physicsSimulation->CreateObject( p_context->portal->GetName() );
		p_context->portal->SetTrigger( l_obj );
		l_obj->SetEntity( l_object );
	}

	MeshManager::getSingletonPtr()->load( MeshName, l_groupName );
	MeshPtr l_mesh = MeshManager::getSingletonPtr()->getByName( MeshName );

	if ( ! l_mesh.isNull() )
	{
		BoundingMesh * l_bmesh = new BoundingMesh( l_mesh.getPointer() );
		l_obj->AddBound( l_bmesh );

		if ( l_object->isAttached() )
		{
			l_obj->SetPosition( l_object->getParentSceneNode()->_getDerivedPosition() );
//			l_bmesh->SetPosition( l_object->getParentSceneNode()->getWorldPosition());
		}
	}
	else
	{
		PARSING_ERROR( "Mesh ptr null @ Parser_PortalTrigger : " + MeshName );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PortalZone )
{
	Zone * l_zone = p_context->universe->GetZone( p_params );

	if ( l_zone != NULL )
	{
		p_context->portal->SetTargetZone( l_zone );
		p_context->scene->AddPortal( p_context->portal );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : parsePortalZone -> zone null or wolrd manager null" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PortalTarget )
{
	if ( p_context->portal != NULL )
	{
		p_context->portal->SetTargetName( p_params );
	}
	else
	{
		PARSING_ERROR( "Parsing Error : parsePortalZone -> portal null" );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_PortalEnd )
{
	p_context->section = SS_NONE;
	p_context->portal = NULL;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BillboardElement )
{
	const Vector3 & l_v3 = SceneFileParser::Parser_Vector3Value( p_params );
	p_context->billboardSet->createBillboard( l_v3 );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BillboardSize )
{
	const StringVector & l_split = StringUtil::split( p_params, ", \t" );

	if ( l_split.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_BillboardSize -> p_params must be : width / height" );
		return false;
	}

	p_context->billboardSet->setDefaultDimensions( StringConverter::parseReal( l_split[0] ), StringConverter::parseReal( l_split[1] ) );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BillboardMaterial )
{
	p_context->billboardSet->setMaterialName( p_params );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BillboardAttachTo )
{
	if ( ! p_context->sceneManager->hasSceneNode( p_params ) )
	{
		PARSING_ERROR( "Parsing Error : Parser_BillboardAttachTo -> scene node named [" + p_params + "] does not exist" );
	}

	p_context->sceneManager->getSceneNode( p_params )->attachObject( p_context->billboardSet );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_BillboardEnd )
{
	if ( ! p_context->billboardSet->isAttached() )
	{
		p_context->sceneManager->getRootSceneNode()->attachObject( p_context->billboardSet );
	}

	p_context->section = SS_NONE;
	p_context->billboardSet = NULL;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ClickableShapeTranslate )
{
	if ( p_context->clickableShape == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_ClickableShapeTranslate -> clickable shape is null" );
		return false;
	}

	const StringVector & l_split = StringUtil::split( p_params, ", \t" );

	if ( l_split.size() != 2 )
	{
		PARSING_ERROR( "Parsing Error : Parser_ClickableShapeTranslate -> 2 parameters needed : x and y" );
		return false;
	}

	Real l_x = StringConverter::parseReal( l_split[0] );
	Real l_y = StringConverter::parseReal( l_split[1] );

	if ( l_split[0].find( "%" ) != String::npos )
	{
		l_x = ( l_x / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getWidth();
	}

	l_x /= p_context->mainViewport->getActualWidth();

	if ( l_split[0].find( "%" ) != String::npos )
	{
		l_y = ( l_y / 100.0f ) * p_context->overlay->GetOgreOverlayElement()->getHeight();
	}

	l_y /= p_context->mainViewport->getActualHeight();
	p_context->clickableShape->SetPosition( l_x, l_y );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_ClickableShapeEnd )
{
	p_context->clickableShape = NULL;
	p_context->section = SS_OVERLAY;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceEvent )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceEvent) 1");
	const StringVector & l_split = StringUtil::split( p_params, ", \t", 2 );
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceEvent) 2");
	BasePonctualEvent * l_event;

	if ( l_split.size() > 2 )
	{
		Ogre::StringVector l_vector = StringUtil::split( l_split[2], ", \t" );
		l_event = p_context->sequenceManager->CreatePonctualEvent( l_split[1], StringArray( l_vector.begin(), l_vector.end() ) );
	}
	else
	{
		StringArray l_tmp;
		l_event = p_context->sequenceManager->CreatePonctualEvent( l_split[1], l_tmp );
	}

//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceEvent) 3");

	if ( l_event == NULL )
	{
		PARSING_ERROR( "Parsing Error : Parser_SequenceEvent -> event factory not found or parameters unsatisfactory" );
		return false;
	}

	l_event->SetTarget( std::move( p_context->sequenceTarget ) );
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceEvent) 4");
	p_context->sequence->AddPonctualEvent( l_event, StringConverter::parseReal( l_split[0] ) );
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceEvent) 5");
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceTarget )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceTarget)");
	const StringVector & l_split = StringUtil::split( p_params, ", \t", 1 );

	if ( l_split[0] == "scene_node" )
	{
		p_context->sequenceTarget = make_target( p_context->sceneManager->getSceneNode( l_split[1] ) );

		if ( p_context->sequenceTarget == NULL )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTarget - Can't find node " + l_split[1] );
		}
	}
	else if ( l_split[0] == "function" )
	{
		p_context->sequenceTarget = make_target( p_context->scriptEngine->GetFunction( l_split[1] ) );

		if ( p_context->sequenceTarget == NULL )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTarget - Can't find function " + l_split[1] );
		}
	}
	else if ( l_split[0] == "overlay" )
	{
		p_context->sequenceTarget = make_target( p_context->gui->GetOverlayGroup( l_split[1] ) );

		if ( p_context->sequenceTarget == NULL )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTarget - Can't find overlay " + l_split[1] );
		}
	}
	else if ( l_split[0] == "overlayElement" )
	{
		p_context->sequenceTarget = make_target( p_context->gui->GetElementByName( l_split[1] ) ); //OverlayManager::getSingletonPtr()->getOverlayElement( l_split[1]);

		if ( p_context->sequenceTarget == NULL )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTarget - Can't find overlay element " + l_split[1] );
		}
	}
	else if ( l_split[0] == "sequence" )
	{
		p_context->sequenceTarget = make_target( p_context->sequenceManager->GetElementByName( l_split[1] ).get() );

		if ( p_context->sequenceTarget == NULL )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTarget - Can't find sequence " + l_split[1] );
		}
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceLooped )
{
	p_context->sequence->SetLooped( true );
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrack )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrack)");
	p_context->sequenceTrack = p_context->sequenceManager->CreateContinuousEvent( p_params );
	p_context->sequenceTrackType = p_context->sequenceTrack->GetType();

	if ( p_context->sequenceTrackType == VECTOR3 )
	{
		p_context->sequenceTrack_V3 = static_cast <BaseContinuousV3Event *>( p_context->sequenceTrack );
	}

	if ( p_context->sequenceTrackType == REAL )
	{
		p_context->sequenceTrack_R = static_cast <BaseContinuousREvent *>( p_context->sequenceTrack );
	}

	if ( p_context->sequenceTrackType == QUATERNION )
	{
		p_context->sequenceTrack_Q = static_cast <BaseContinuousQEvent *>( p_context->sequenceTrack );
	}

	p_context->section = SS_SEQUENCE_TRACK;
	return true;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceEnd )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceEnd)");
//	p_context->sequence->Start();
	p_context->sequence = NULL;
	p_context->section = SS_NONE;
	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrackKey )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrackKey)");
	if ( p_context->sequenceTrackType == VECTOR3 )
	{
		Vector4 l_v4 = SceneFileParser::Parser_Vector4Value( p_params );
		p_context->sequenceTrack_V3->AddFrame( l_v4.x, Vector3( l_v4.y, l_v4.z, l_v4.w ) );
	}

	if ( p_context->sequenceTrackType == REAL )
	{
		const StringVector & l_split = StringUtil::split( p_params, ", \t" );

		if ( l_split.size() != 2 )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTrackKey -> wrong number of parameters for this type of key. Expected : key [time] [real]" );
			return false;
		}

		p_context->sequenceTrack_R->AddFrame( StringConverter::parseReal( l_split[0] ) , StringConverter::parseReal( l_split[1] ) );
	}

	if ( p_context->sequenceTrackType == QUATERNION )
	{
		const StringVector & l_split = StringUtil::split( p_params, ", \t" );

		if ( l_split.size() != 5 )
		{
			PARSING_ERROR( "Parsing Error : Parser_SequenceTrackKey -> wrong number of parameters for this type of key. Expected : key [time] [axis_x] [axis_y] [axis_z] [angle]" );
			return false;
		}

		p_context->sequenceTrack_Q->AddFrame( StringConverter::parseReal( l_split[0] ) , Quaternion( Degree( StringConverter::parseReal( l_split[4] ) ),
											  Vector3( StringConverter::parseReal( l_split[1] ),
													  StringConverter::parseReal( l_split[2] ),
													  StringConverter::parseReal( l_split[3] ) ) ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrackInterpolation )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrackInterpolation)");
	if ( p_context->sequenceTrackType == VECTOR3 )
	{
		p_context->sequenceTrack_V3->SetInterpolator( p_context->sequenceManager->GetInterpolator_V3( p_params ) );
	}
	else if ( p_context->sequenceTrackType == REAL )
	{
		p_context->sequenceTrack_R->SetInterpolator( p_context->sequenceManager->GetInterpolator_R( p_params ) );
	}
	else if ( p_context->sequenceTrackType == QUATERNION )
	{
		p_context->sequenceTrack_Q->SetInterpolator( p_context->sequenceManager->GetInterpolator_Q( p_params ) );
	}

	return false;
}

DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrackEnd )
{
//	p_context->log->logMessage( "DEFINE_SCENE_FILE_PARSER( Parser_SequenceTrackEnd)");
	p_context->sequence->AddContinuousEvent( p_context->sequenceTrack );
	p_context->sequenceTrack->SetTarget( std::move( p_context->sequenceTarget ) );
	p_context->section = SS_SEQUENCE;
	return false;
}
