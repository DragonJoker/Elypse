/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_SCENE_FILE_PARSER__PARSERS_H___
#define ___TROLL_SCENE_FILE_PARSER__PARSERS_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <SceneFileParser_Parsers.h>

namespace Troll
{
	namespace ElypseRW
	{
		// Root parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootAmbientLight )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootShadows )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootForceShadows )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootObject )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootSound )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPlaylist )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootVideo )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPub )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPortal )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootMirror )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootSceneNode )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootAnimationGroup )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootLight )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootCamera )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootOverlay )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootUniverse )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootParticleSystem )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPostEffect )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPhysics )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPhysicsGroup )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootPhysicsSimulation )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootCollision )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootAutoPhysics )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootBillboardGroup )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootSequence )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_RootVideo )

		// Animation group parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_AnimationGroupElement )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_AnimationGroupChainAnimation )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_AnimationEnd )

		// Entity parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_ObjectEnd )

		//Overlay Parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_OverlayBorderMaterial )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_OverlayBorderSize )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_OverlayVideo )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_OverlayEnd )

		// Submesh parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SubMeshEnd )

		// Physics object parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_ObjectPhysicsEnd )

		// Physics parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_PhysicsEnd )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_PhysicsSimulationEnd )

		// Physics simulation parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_BoundingShapeEnd )

		// Scene node parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SceneNodeEnd )

		// Light parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_LightEnd )

		// Camera parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_CameraEnd )

		// Sound parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SoundURL )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SoundEnd )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SoundNodeEnd )

		// Playlist parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_PlaylistEnd )

		// Video parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_VideoEnd )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_VideoObjectEnd )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_VideoOverlayEnd )

		// Pub parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_PubEnd )

		// Mirror parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_MirrorEnd )

		// Scene parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SceneEnd )

		// Particle system parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_ParticleSystemEnd )

		// Portal parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_PortalEnd )

		// Billboard parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_BillboardEnd )

		// Clickable shape parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_ClickableShapeEnd )

		// Time sequence parsers
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceEnd )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceTrack )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceEvent )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceTarget )

		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceTrackKey )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceTrackInterpolation )
		DECLARE_SCENE_FILE_PARSER_NO_EXPORT( TrollParser_SequenceTrackEnd )
	}
}

#endif
