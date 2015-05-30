#include "PrecompiledHeader.h"

#include "SceneFile/TrollSceneFileParser.h"
#include "GUI/main.h"
#include "SceneFile/TrollSceneFileParser_Parsers.h"
#include "GUI/MainFrame.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/Module_GUI.h"
/*
#include <OgreLog.h>
#include <OgreStringConverter.h>
#include <OgreDataStream.h>
#include <ScriptEngine.h>
#include <Main/Context.h>
#include <EMuseLogs.h>
*/

using namespace Troll::GUI;

namespace Troll
{
	namespace Scene
	{
		TrollSceneFileParser * g_fileParser;
		wxString g_fileName;

		TrollSceneFileParser::TrollSceneFileParser( const String & p_appIndexStr, const String & p_baseDirectory )
			:	SceneFileParser( p_appIndexStr, p_baseDirectory )
		{
			g_fileParser = this;
			m_rootParsers					["ambient_light"]		= TrollParser_RootAmbientLight;
			m_rootParsers					["shadows"]				= TrollParser_RootShadows;
			m_rootParsers					["force_shadows"]		= TrollParser_RootForceShadows;
			m_rootParsers					["object"]				= TrollParser_RootObject;
			m_rootParsers					["sound"]				= TrollParser_RootSound;
			m_rootParsers					["playlist"]			= TrollParser_RootPlaylist;
			m_rootParsers					["video"]				= TrollParser_RootVideo;
			m_rootParsers					["pub"]					= TrollParser_RootPub;
			m_rootParsers					["mirror"]				= TrollParser_RootMirror;
			m_rootParsers					["scene_node"]			= TrollParser_RootSceneNode;
			m_rootParsers					["animation_group"]		= TrollParser_RootAnimationGroup;
			m_rootParsers					["light"]				= TrollParser_RootLight;
			m_rootParsers					["camera"]				= TrollParser_RootCamera;
			m_rootParsers					["overlay"]				= TrollParser_RootOverlay;
			m_rootParsers					["universe"]			= TrollParser_RootUniverse;
			m_rootParsers					["post_effect"]			= TrollParser_RootPostEffect;
			m_rootParsers					["physics"]				= TrollParser_RootPhysics;
			m_rootParsers					["particle_system"]		= TrollParser_RootParticleSystem;
			m_rootParsers					["physics_group"]		= TrollParser_RootPhysicsGroup;
			m_rootParsers					["physics_simulation"]	= TrollParser_RootPhysicsSimulation;
			m_rootParsers					["collision"]			= TrollParser_RootCollision;
			m_rootParsers					["portal"]				= TrollParser_RootPortal;
			m_rootParsers					["billboard"]			= TrollParser_RootBillboardGroup;
			m_rootParsers					["auto_physics"]		= TrollParser_RootAutoPhysics;
			m_rootParsers					["sequence"]			= TrollParser_RootSequence;
			m_animatedObjectGroupParsers	["add_object"]			= TrollParser_AnimationGroupElement;
			m_animatedObjectGroupParsers	["chain_animations"]	= TrollParser_AnimationGroupChainAnimation;
			m_animationParsers				["}"]					= TrollParser_AnimationEnd;
			m_objectParsers					["}"]					= TrollParser_ObjectEnd;
			m_physicsMaterialParsers		["}"]					= TrollParser_PhysicsEnd;
			m_physicsSimulationParsers		["}"]					= TrollParser_PhysicsSimulationEnd;
			m_objectSubMeshParsers			["}"]					= TrollParser_SubMeshEnd;
			m_objectPhysicsParsers			["}"]					= TrollParser_ObjectPhysicsEnd;
			m_boundingShapeParsers			["}"]					= TrollParser_BoundingShapeEnd;
			m_rootParsers					["scene_node"]			= TrollParser_RootSceneNode;
			m_nodeParsers					["}"]					= TrollParser_SceneNodeEnd;
			m_lightParsers					["}"]					= TrollParser_LightEnd;
			m_cameraParsers					["}"]					= TrollParser_CameraEnd;
			m_overlayParsers				["element"]				= TrollParser_RootOverlay;
			m_overlayParsers				["border_material"]		= TrollParser_OverlayBorderMaterial;
			m_overlayParsers				["border_size"]			= TrollParser_OverlayBorderSize;
			m_overlayParsers				["use_video"]			= TrollParser_OverlayVideo;
			m_overlayParsers				["}"]					= TrollParser_OverlayEnd;
			m_soundParsers					["url"]					= TrollParser_SoundURL;
			m_soundParsers					["}"]					= TrollParser_SoundEnd;
			m_soundNodeParsers				["}"]					= TrollParser_SoundNodeEnd;
			m_soundPlaylistParsers			["}"]					= TrollParser_PlaylistEnd;
			m_videoParsers					["}"]					= TrollParser_VideoEnd;
			m_videoObjectParsers			["}"]					= TrollParser_VideoObjectEnd;
			m_videoOverlayParsers			["}"]					= TrollParser_VideoOverlayEnd;
			m_pubParsers					["}"]					= TrollParser_PubEnd;
			m_mirrorParsers					["}"]					= TrollParser_MirrorEnd;
			m_sceneParsers					["}"]					= TrollParser_SceneEnd;
			m_particleSystemParsers			["}"]					= TrollParser_ParticleSystemEnd;
			m_portalParsers					["}"]					= TrollParser_PortalEnd;
			m_billboardParsers				["}"]					= TrollParser_BillboardEnd;
			m_clickableShapeParsers			["}"]					= TrollParser_ClickableShapeEnd;
			m_sequenceParsers				["event"]				= TrollParser_SequenceEvent;
			m_sequenceParsers				["track"]				= TrollParser_SequenceTrack;
			m_sequenceParsers				["target"]				= TrollParser_SequenceTarget;
			m_sequenceParsers				["}"]					= TrollParser_SequenceEnd;
			m_sequenceTrackParsers			["key"]					= TrollParser_SequenceTrackKey;
			m_sequenceTrackParsers			["interpolation"]		= TrollParser_SequenceTrackInterpolation;
			m_sequenceTrackParsers			["target"]				= TrollParser_SequenceTarget;
			m_sequenceTrackParsers			["}"]					= TrollParser_SequenceTrackEnd;
		}

		TrollSceneFileParser::~TrollSceneFileParser()
		{
		}

		void TrollSceneFileParser::ParseScript( ConfigFile * p_file )
		{
			g_fileName = Troll::GUI::make_wxString( p_file->GetName() );

			if ( p_file->GetName() != "SceneEditor.emscene" && p_file->GetName() != "OverlayEditor.emscene" )
			{
				g_mainFrame->GetCurrentProject()->GetMainScene()->SetCurrentSceneFile( Troll::GUI::make_wxString( p_file->GetName() ) );
			}

			SceneFileParser::ParseScript( p_file );
		}
	}
}
