/*
See LICENSE file in root folder
*/
#ifndef ___EMUSE_SCENE_FILE_PARSER__PARSERS_H___
#define ___EMUSE_SCENE_FILE_PARSER__PARSERS_H___

#include "Module_SceneFile.h"

namespace Elypse
{
	namespace SceneFile
	{
		//Root section
		DECLARE_SCENE_FILE_PARSER( Parser_RootAmbientLight )
		DECLARE_SCENE_FILE_PARSER( Parser_RootBackgroundColour )
		DECLARE_SCENE_FILE_PARSER( Parser_RootStartScene )
		DECLARE_SCENE_FILE_PARSER( Parser_RootScene )
		DECLARE_SCENE_FILE_PARSER( Parser_RootObject )
		DECLARE_SCENE_FILE_PARSER( Parser_RootSound )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPlaylist )
		DECLARE_SCENE_FILE_PARSER( Parser_RootVideo )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPub )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPortal )
		DECLARE_SCENE_FILE_PARSER( Parser_RootStartupScript )
		DECLARE_SCENE_FILE_PARSER( Parser_RootStartupScriptEnd )
		DECLARE_SCENE_FILE_PARSER( Parser_RootCode )
		DECLARE_SCENE_FILE_PARSER( Parser_RootCodeEnd )
		DECLARE_SCENE_FILE_PARSER( Parser_RootMirror )
		DECLARE_SCENE_FILE_PARSER( Parser_RootSceneNode )
		DECLARE_SCENE_FILE_PARSER( Parser_RootAnimationGroup )
		DECLARE_SCENE_FILE_PARSER( Parser_RootLight )
		DECLARE_SCENE_FILE_PARSER( Parser_RootCamera )
		DECLARE_SCENE_FILE_PARSER( Parser_RootOverlay )
		DECLARE_SCENE_FILE_PARSER( Parser_RootUniverse )
		DECLARE_SCENE_FILE_PARSER( Parser_RootShadows )
		DECLARE_SCENE_FILE_PARSER( Parser_RootForceShadows )
		DECLARE_SCENE_FILE_PARSER( Parser_RootParticleSystem )
		DECLARE_SCENE_FILE_PARSER( Parser_RootLoadingBar )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPostEffect )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPhysics )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPhysicsGroup )
		DECLARE_SCENE_FILE_PARSER( Parser_RootPhysicsSimulation )
		DECLARE_SCENE_FILE_PARSER( Parser_RootCollision )
		DECLARE_SCENE_FILE_PARSER( Parser_RootAutoPhysics )
		DECLARE_SCENE_FILE_PARSER( Parser_RootBillboardGroup )
		DECLARE_SCENE_FILE_PARSER( Parser_RootSequence )
		DECLARE_SCENE_FILE_PARSER( Parser_RootEnd )

		//Animation Group Section
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationGroupElement )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationGroupChainAnimation )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationGroupLoop )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationGroupEnd )

		//Animated Object Section
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationObject )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationObjectEnd )

		//Animation Section
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationLooped )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationTimeout )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationTimein )
		DECLARE_SCENE_FILE_PARSER( Parser_AnimationEnd )

		//Object Section
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectMesh )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectMeshAndSkeleton )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectShadow )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectAttach )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysics )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectSound )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPlaylist )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectVideo )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPub )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectHidden )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectSubmesh )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectEnd )

		//SubMesh
		DECLARE_SCENE_FILE_PARSER( Parser_SubMeshMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_SubMeshEnd )

		//Object Physics Section
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsMass )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingSphere )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingBox )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingMesh )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingPlane )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsBoundingPlane )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsStatic )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsFloating )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsPhantom )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsDisabled )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsInitialForce )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsInitialTorque )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsDamping )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_ObjectPhysicsEnd )

		//Physics Material Section
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsFriction )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsAddFriction )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsBouncyness )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsBounceLimit )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSoftERP )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSoftCFM )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsIndependantVel )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsAddVel )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsRollFriction )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsRollDampen )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsEnd )

		//Physics Simulation Section
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationERP )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationCFM )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationGravity )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationDisabled )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationTimescale )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationAutoUpdate )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationSteptime )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationSteptimeLimit )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationDefaultDamping )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationDefaultAutoDisable )
		DECLARE_SCENE_FILE_PARSER( Parser_PhysicsSimulationEnd )

		DECLARE_SCENE_FILE_PARSER( Parser_BoundingShapeTranslate )
		DECLARE_SCENE_FILE_PARSER( Parser_BoundingShapeRotate )
		DECLARE_SCENE_FILE_PARSER( Parser_BoundingShapeEnd )

		//Scene Node Section
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeParent )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodePosition )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodePositionFromMax )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeInheritScale )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeInheritOrientation )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeOrientation )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeScale )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeDirection )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeSound )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodePlaylist )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneNodeEnd )

		//Light Section
		DECLARE_SCENE_FILE_PARSER( Parser_LightType )
		DECLARE_SCENE_FILE_PARSER( Parser_LightPosition )
		DECLARE_SCENE_FILE_PARSER( Parser_LightDirection )
		DECLARE_SCENE_FILE_PARSER( Parser_LightDiffuse )
		DECLARE_SCENE_FILE_PARSER( Parser_LightSpecular )
		DECLARE_SCENE_FILE_PARSER( Parser_LightAttenuation )
		DECLARE_SCENE_FILE_PARSER( Parser_LightRange )
		DECLARE_SCENE_FILE_PARSER( Parser_LightAttach )
		DECLARE_SCENE_FILE_PARSER( Parser_LightShadows )
		DECLARE_SCENE_FILE_PARSER( Parser_LightEnd )

		//Camera Section
		DECLARE_SCENE_FILE_PARSER( Parser_CameraPosition )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraDirection )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraOrientation )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraAttach )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraPolygonMode )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraFovY )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraTex )
		DECLARE_SCENE_FILE_PARSER( Parser_CameraEnd )

		//Overlay Section
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayClass )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayBorderMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayBorderSize )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayPosition )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlaySize )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayText )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayTextColour )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayAlign )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayMouseOverMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayClickedMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayTextAlign )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayFontSize )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayFontName )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayShape )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayVideo )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayZIndex )
		DECLARE_SCENE_FILE_PARSER( Parser_OverlayEnd )

		//Sound Section
		DECLARE_SCENE_FILE_PARSER( Parser_SoundURL )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundName )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundGlobal )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundLooped )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundTimer )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundType )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundFadeInLength )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundFadeOutLength )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundVolumePercent )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundEnd )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundObjectEnd )
		DECLARE_SCENE_FILE_PARSER( Parser_SoundNodeEnd )

		//Playlist Section
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistSound )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistRandom )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistLooped )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistTimer )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistTimerTime )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistTimerMin )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistTimerMax )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistVolumePercent )
		DECLARE_SCENE_FILE_PARSER( Parser_PlaylistEnd )

		//Video Section
		DECLARE_SCENE_FILE_PARSER( Parser_VideoURL )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoLooped )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoEnabled )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoTokenURL )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoEnd )

		//Video Object Section
//		DECLARE_SCENE_FILE_PARSER( Parser_VideoObjectName )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoObjectVolume )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoObjectEnd )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoOverlayVolume )
		DECLARE_SCENE_FILE_PARSER( Parser_VideoOverlayEnd )

		//Pub Section
		DECLARE_SCENE_FILE_PARSER( Parser_PubURL )
		DECLARE_SCENE_FILE_PARSER( Parser_PubImageURL )
		DECLARE_SCENE_FILE_PARSER( Parser_PubClickURL )
		DECLARE_SCENE_FILE_PARSER( Parser_PubEnd )

		//Mirror Section
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorPosition )
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorDirection )
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorResolution )
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorBackground )
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorUpdates )
		DECLARE_SCENE_FILE_PARSER( Parser_MirrorEnd )

		//Scene Section
		DECLARE_SCENE_FILE_PARSER( Parser_SceneDataFile )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneDataRep )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneFile )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneLoadScriptFile )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneUnloadScriptFile )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneMuseFile )
		DECLARE_SCENE_FILE_PARSER( Parser_SceneEnd )

		//Particle System Section
		DECLARE_SCENE_FILE_PARSER( Parser_ParticleSystemClass )
		DECLARE_SCENE_FILE_PARSER( Parser_ParticleSystemAttach )
		DECLARE_SCENE_FILE_PARSER( Parser_ParticleSystemEnd )

		DECLARE_SCENE_FILE_PARSER( Parser_PortalTrigger )
		DECLARE_SCENE_FILE_PARSER( Parser_PortalZone )
		DECLARE_SCENE_FILE_PARSER( Parser_PortalTarget )
		DECLARE_SCENE_FILE_PARSER( Parser_PortalEnd )

		DECLARE_SCENE_FILE_PARSER( Parser_BillboardElement )
		DECLARE_SCENE_FILE_PARSER( Parser_BillboardSize )
		DECLARE_SCENE_FILE_PARSER( Parser_BillboardMaterial )
		DECLARE_SCENE_FILE_PARSER( Parser_BillboardAttachTo )
		DECLARE_SCENE_FILE_PARSER( Parser_BillboardEnd )

		DECLARE_SCENE_FILE_PARSER( Parser_ClickableShapeTranslate )
		DECLARE_SCENE_FILE_PARSER( Parser_ClickableShapeEnd )

		DECLARE_SCENE_FILE_PARSER( Parser_SequenceEvent )
		DECLARE_SCENE_FILE_PARSER( Parser_SequenceTrack )
		DECLARE_SCENE_FILE_PARSER( Parser_SequenceLooped )
		DECLARE_SCENE_FILE_PARSER( Parser_SequenceTarget )
		DECLARE_SCENE_FILE_PARSER( Parser_SequenceEnd )

		DECLARE_SCENE_FILE_PARSER( Parser_SequenceTrackKey )
		DECLARE_SCENE_FILE_PARSER( Parser_SequenceTrackInterpolation )
		DECLARE_SCENE_FILE_PARSER( Parser_SequenceTrackEnd )

	}
}

#endif
