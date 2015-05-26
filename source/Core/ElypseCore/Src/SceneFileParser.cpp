/*********************************************************************************************************************

	Author :	Marc BILLON

	Company:	ForDev Studio - Copyright 2006

	Files :		SceneFileParser.h - SceneFileParser.cpp

	Desc :		Class used to build, create and modify ( with scripts ), the main scene, from a file

*********************************************************************************************************************/
#include "PrecompiledHeader.h"

#include "SceneFileParser.h"
#include "SceneFileParser_Parsers.h"

#include <OgreLog.h>
#include <OgreStringConverter.h>
#include <OgreDataStream.h>

#include "ScriptEngine.h"
#include "Context.h"
#include "ConfigFile.h"
#include "MuseFile.h"

#include "EMuseLogs.h"

#include <File.h>

SceneFileParser :: SceneFileParser( const String & p_appIndexStr, const Path & p_baseDirectory )
	:	m_baseDirectory( p_baseDirectory ),
		m_appIndexStr( p_appIndexStr )
{
	m_rootParsers					["background_colour"]	= Parser_RootBackgroundColour;
	m_rootParsers					["start_scene"]			= Parser_RootStartScene;
	m_rootParsers					["scene"]				= Parser_RootScene;
	m_rootParsers					["ambient_light"]		= Parser_RootAmbientLight;
	m_rootParsers					["object"]				= Parser_RootObject;
	m_rootParsers					["sound"]				= Parser_RootSound;
	m_rootParsers					["playlist"]			= Parser_RootPlaylist;
	m_rootParsers					["video"]				= Parser_RootVideo;
	m_rootParsers					["pub"]					= Parser_RootPub;
	m_rootParsers					["<code>"]				= Parser_RootCode;
	m_rootParsers					["</code>"]				= Parser_RootCodeEnd;
	m_rootParsers					["mirror"]				= Parser_RootMirror;
	m_rootParsers					["scene_node"]			= Parser_RootSceneNode;
	m_rootParsers					["animation_group"]		= Parser_RootAnimationGroup;
	m_rootParsers					["light"]				= Parser_RootLight;
	m_rootParsers					["camera"]				= Parser_RootCamera;
	m_rootParsers					["overlay"]				= Parser_RootOverlay;
	m_rootParsers					["universe"]			= Parser_RootUniverse;
	m_rootParsers					["post_effect"]			= Parser_RootPostEffect;
	m_rootParsers					["physics"]				= Parser_RootPhysics;
	m_rootParsers					["shadows"]				= Parser_RootShadows;
	m_rootParsers					["force_shadows"]		= Parser_RootForceShadows;
	m_rootParsers					["particle_system"]		= Parser_RootParticleSystem;
	m_rootParsers					["loading_bar"]			= Parser_RootLoadingBar;
	m_rootParsers					["physics_group"]		= Parser_RootPhysicsGroup;
	m_rootParsers					["physics_simulation"]	= Parser_RootPhysicsSimulation;
	m_rootParsers					["collision"]			= Parser_RootCollision;
	m_rootParsers					["portal"]				= Parser_RootPortal;
	m_rootParsers					["billboard"]			= Parser_RootBillboardGroup;
	m_rootParsers					["auto_physics"]		= Parser_RootAutoPhysics;
	m_rootParsers					["sequence"]			= Parser_RootSequence;
	m_rootParsers					["}"]					= Parser_RootEnd;
	m_animatedObjectGroupParsers	["add_object"]			= Parser_AnimationGroupElement;
	m_animatedObjectGroupParsers	["chain_animations"]	= Parser_AnimationGroupChainAnimation;
	m_animatedObjectGroupParsers	["loop_animation"]		= Parser_AnimationGroupLoop;
	m_animatedObjectGroupParsers	["}"]					= Parser_AnimationGroupEnd;
	m_animatedObjectParsers			["animation"]			= Parser_AnimationObject;
	m_animatedObjectParsers			["}"]					= Parser_AnimationObjectEnd;
	m_animationParsers				["looped"]				= Parser_AnimationLooped;
	m_animationParsers				["timeout"]				= Parser_AnimationTimeout;
	m_animationParsers				["timein"]				= Parser_AnimationTimein;
	m_animationParsers				["}"]					= Parser_AnimationEnd;
	m_objectParsers					["mesh"]				= Parser_ObjectMesh;
	m_objectParsers					["mesh_and_skeleton"]	= Parser_ObjectMeshAndSkeleton;
	m_objectParsers					["material"]			= Parser_ObjectMaterial;
	m_objectParsers					["shadows"]				= Parser_ObjectShadow;
	m_objectParsers					["attach_to"]			= Parser_ObjectAttach;
	m_objectParsers					["use_physics"]			= Parser_ObjectPhysics;
	m_objectParsers					["use_video"]			= Parser_ObjectVideo;
	m_objectParsers					["use_pub"]				= Parser_ObjectPub;
	m_objectParsers					["hidden"]				= Parser_ObjectHidden;
	m_objectParsers					["submesh"]				= Parser_ObjectSubmesh;
	m_objectParsers					["}"]					= Parser_ObjectEnd;
	m_physicsMaterialParsers		["friction"]			= Parser_PhysicsFriction;
	m_physicsMaterialParsers		["add_friction"]		= Parser_PhysicsAddFriction;
	m_physicsMaterialParsers		["bouncyness"]			= Parser_PhysicsBouncyness;
	m_physicsMaterialParsers		["bounce_limit"]		= Parser_PhysicsBounceLimit;
	m_physicsMaterialParsers		["soft_erp"]			= Parser_PhysicsSoftERP;
	m_physicsMaterialParsers		["soft_cfm"]			= Parser_PhysicsSoftCFM;
	m_physicsMaterialParsers		["independant_velocity"] = Parser_PhysicsIndependantVel;
	m_physicsMaterialParsers		["add_vel"]				= Parser_PhysicsAddVel;
	m_physicsMaterialParsers		["roll_friction"]		= Parser_PhysicsRollFriction;
	m_physicsMaterialParsers		["roll_dampen"]			= Parser_PhysicsRollDampen;
	m_physicsMaterialParsers		["}"]					= Parser_PhysicsEnd;
	m_physicsSimulationParsers		["erp"]					= Parser_PhysicsSimulationERP;
	m_physicsSimulationParsers		["cfm"]					= Parser_PhysicsSimulationCFM;
	m_physicsSimulationParsers		["gravity"]				= Parser_PhysicsSimulationGravity;
	m_physicsSimulationParsers		["timescale"]			= Parser_PhysicsSimulationTimescale;
	m_physicsSimulationParsers		["auto_update"]			= Parser_PhysicsSimulationAutoUpdate;
	m_physicsSimulationParsers		["steptime"]			= Parser_PhysicsSimulationSteptime;
	m_physicsSimulationParsers		["steptime_limit"]		= Parser_PhysicsSimulationSteptimeLimit;
	m_physicsSimulationParsers		["default_damping"]		= Parser_PhysicsSimulationDefaultDamping;
	m_physicsSimulationParsers		["default_auto_disable"] = Parser_PhysicsSimulationDefaultAutoDisable;
	m_physicsSimulationParsers		["disabled"]			= Parser_PhysicsSimulationDisabled;
	m_physicsSimulationParsers		["}"]					= Parser_PhysicsSimulationEnd;
	m_objectSubMeshParsers			["material"]			= Parser_SubMeshMaterial;
	m_objectSubMeshParsers			["}"]					= Parser_SubMeshEnd;
	m_objectPhysicsParsers			["mass"]				= Parser_ObjectPhysicsMass;
	m_objectPhysicsParsers			["bounding_sphere"]		= Parser_ObjectPhysicsBoundingSphere;
	m_objectPhysicsParsers			["bounding_box"]		= Parser_ObjectPhysicsBoundingBox;
	m_objectPhysicsParsers			["bounding_mesh"]		= Parser_ObjectPhysicsBoundingMesh;
	m_objectPhysicsParsers			["infinite_plane"]		= Parser_ObjectPhysicsBoundingPlane;
	m_objectPhysicsParsers			["static"]				= Parser_ObjectPhysicsStatic;
	m_objectPhysicsParsers			["phantom"]				= Parser_ObjectPhysicsPhantom;
	m_objectPhysicsParsers			["ignore_gravity"]		= Parser_ObjectPhysicsFloating;
	m_objectPhysicsParsers			["initial_force"]		= Parser_ObjectPhysicsInitialForce;
	m_objectPhysicsParsers			["initial_torque"]		= Parser_ObjectPhysicsInitialTorque;
	m_objectPhysicsParsers			["damping"]				= Parser_ObjectPhysicsDamping;
	m_objectPhysicsParsers			["disabled"]			= Parser_ObjectPhysicsDisabled;
	m_objectPhysicsParsers			["physics_material"]	= Parser_ObjectPhysicsMaterial;
	m_objectPhysicsParsers			["}"]					= Parser_ObjectPhysicsEnd;
	m_boundingShapeParsers			["translate"]			= Parser_BoundingShapeTranslate;
	m_boundingShapeParsers			["rotate"]				= Parser_BoundingShapeRotate;
	m_boundingShapeParsers			["}"]					= Parser_BoundingShapeEnd;
	m_nodeParsers					["parent"]				= Parser_SceneNodeParent;
	m_nodeParsers					["position"]			= Parser_SceneNodePosition;
	m_nodeParsers					["position_from_max"]	= Parser_SceneNodePositionFromMax;
	m_nodeParsers					["orientation"]			= Parser_SceneNodeOrientation;
	m_nodeParsers					["inherit_orientation"]	= Parser_SceneNodeInheritOrientation;
	m_nodeParsers					["inherit_scale"]		= Parser_SceneNodeInheritScale;
	m_nodeParsers					["scale"]				= Parser_SceneNodeScale;
	m_nodeParsers					["direction"]			= Parser_SceneNodeDirection;
	m_nodeParsers					["use_sound"]			= Parser_SceneNodeSound;
	m_nodeParsers					["use_playlist"]		= Parser_SceneNodePlaylist;
	m_nodeParsers					["}"]					= Parser_SceneNodeEnd;
	m_lightParsers					["type"]				= Parser_LightType;
	m_lightParsers					["position"]			= Parser_LightPosition;
	m_lightParsers					["direction"]			= Parser_LightDirection;
	m_lightParsers					["diffuse"]				= Parser_LightDiffuse;
	m_lightParsers					["specular"]			= Parser_LightSpecular;
	m_lightParsers					["attenuation"]			= Parser_LightAttenuation;
	m_lightParsers					["spotlight_range"]		= Parser_LightRange;
	m_lightParsers					["attach_to"]			= Parser_LightAttach;
	m_lightParsers					["shadows"	]			= Parser_LightShadows;
	m_lightParsers					["}"]					= Parser_LightEnd;
	m_cameraParsers					["position"]			= Parser_CameraPosition;
	m_cameraParsers					["direction"]			= Parser_CameraDirection;
	m_cameraParsers					["orientation"]			= Parser_CameraOrientation;
	m_cameraParsers					["attach_to"]			= Parser_CameraAttach;
	m_cameraParsers					["polygon_mode"]		= Parser_CameraPolygonMode;
	m_cameraParsers					["cam_tex"]				= Parser_CameraTex;
	m_cameraParsers					["fov"]					= Parser_CameraFovY;
	m_cameraParsers					["}"]					= Parser_CameraEnd;
	m_overlayParsers				["element"]				= Parser_RootOverlay;
	m_overlayParsers				["material"]			= Parser_OverlayMaterial;
	m_overlayParsers				["class"]				= Parser_OverlayClass;
	m_overlayParsers				["border_material"]		= Parser_OverlayBorderMaterial;
	m_overlayParsers				["border_size"]			= Parser_OverlayBorderSize;
	m_overlayParsers				["position"]			= Parser_OverlayPosition;
	m_overlayParsers				["size"]				= Parser_OverlaySize;
	m_overlayParsers				["text"]				= Parser_OverlayText;
	m_overlayParsers				["text_colour"]			= Parser_OverlayTextColour;
	m_overlayParsers				["align"]				= Parser_OverlayAlign;
	m_overlayParsers				["mouse_over_material"]	= Parser_OverlayMouseOverMaterial;
	m_overlayParsers				["clicked_material"]	= Parser_OverlayClickedMaterial;
	m_overlayParsers				["text_align"]			= Parser_OverlayTextAlign;
	m_overlayParsers				["font_size"]			= Parser_OverlayFontSize;
	m_overlayParsers				["font_name"]			= Parser_OverlayFontName;
	m_overlayParsers				["clickable_zone"]		= Parser_OverlayShape;
	m_overlayParsers				["use_video"]			= Parser_OverlayVideo;
	m_overlayParsers				["z_index"]				= Parser_OverlayZIndex;
	m_overlayParsers				["}"]					= Parser_OverlayEnd;
	m_soundParsers					["url"]					= Parser_SoundURL;
	m_soundParsers					["global"]				= Parser_SoundGlobal;
	m_soundParsers					["looped"]				= Parser_SoundLooped;
	m_soundParsers					["timer"]				= Parser_SoundTimer;
	m_soundParsers					["type"]				= Parser_SoundType;
	m_soundParsers					["fade_in"]				= Parser_SoundFadeInLength;
	m_soundParsers					["fade_out"]			= Parser_SoundFadeOutLength;
	m_soundParsers					["attenuation"]			= Parser_SoundVolumePercent;
	m_soundParsers					["}"]					= Parser_SoundEnd;
	m_soundNodeParsers				["looped"]				= Parser_SoundLooped;
	m_soundNodeParsers				["timer"]				= Parser_SoundTimer;
	m_soundNodeParsers				["fade_in"]				= Parser_SoundFadeInLength;
	m_soundNodeParsers				["fade_out"]			= Parser_SoundFadeOutLength;
	m_soundNodeParsers				["attenuation"]			= Parser_SoundVolumePercent;
	m_soundNodeParsers				["}"]					= Parser_SoundNodeEnd;
	m_soundPlaylistParsers			["add_sound"]			= Parser_PlaylistSound;
	m_soundPlaylistParsers			["random"]				= Parser_PlaylistRandom;
	m_soundPlaylistParsers			["looped"]				= Parser_PlaylistLooped;
	m_soundPlaylistParsers			["timer"]				= Parser_PlaylistTimer;
	m_soundPlaylistParsers			["timer_time"]			= Parser_PlaylistTimerTime;
	m_soundPlaylistParsers			["timer_min"]			= Parser_PlaylistTimerMin;
	m_soundPlaylistParsers			["timer_max"]			= Parser_PlaylistTimerMax;
	m_soundPlaylistParsers			["attenuation"]			= Parser_PlaylistVolumePercent;
	m_soundPlaylistParsers			["}"]					= Parser_PlaylistEnd;
	m_videoParsers					["url"]					= Parser_VideoURL;
	m_videoParsers					["looped"]				= Parser_VideoLooped;
	m_videoParsers					["token_url"]			= Parser_VideoTokenURL;
	m_videoParsers					["enabled"]				= Parser_VideoEnabled;
	m_videoParsers					["}"]					= Parser_VideoEnd;
//	m_videoObjectParsers			["name"]				= Parser_VideoObjectName;
	m_videoObjectParsers			["volume"]				= Parser_VideoObjectVolume;
	m_videoObjectParsers			["}"]					= Parser_VideoObjectEnd;
	m_videoOverlayParsers			["volume"]				= Parser_VideoOverlayVolume;
	m_videoOverlayParsers			["}"]					= Parser_VideoOverlayEnd;
	m_pubParsers					["url"]					= Parser_PubURL;
	m_pubParsers					["image_url"]			= Parser_PubImageURL;
	m_pubParsers					["click_url"]			= Parser_PubClickURL;
	m_pubParsers					["}"]					= Parser_PubEnd;
	m_mirrorParsers					["position"]			= Parser_MirrorPosition;
	m_mirrorParsers					["direction"]			= Parser_MirrorDirection;
	m_mirrorParsers					["resolution"]			= Parser_MirrorResolution;
	m_mirrorParsers					["background"]			= Parser_MirrorBackground;
	m_mirrorParsers					["material"]			= Parser_MirrorMaterial;
	m_mirrorParsers					["auto_updates"]		= Parser_MirrorUpdates;
	m_mirrorParsers					["}"]					= Parser_MirrorEnd;
	m_sceneParsers					["data_file"]			= Parser_SceneDataFile;
	m_sceneParsers					["scene_file"]			= Parser_SceneFile;
	m_sceneParsers					["load_script_file"]	= Parser_SceneLoadScriptFile;
	m_sceneParsers					["unload_script_file"]	= Parser_SceneUnloadScriptFile;
	m_sceneParsers					["muse_file"]			= Parser_SceneMuseFile;
	m_sceneParsers					["}"]					= Parser_SceneEnd;
	m_particleSystemParsers			["class"]				= Parser_ParticleSystemClass;
	m_particleSystemParsers			["attach_to"]			= Parser_ParticleSystemAttach;
	m_particleSystemParsers			["}"]					= Parser_ParticleSystemEnd;
	m_portalParsers					["trigger"]				= Parser_PortalTrigger;
	m_portalParsers					["target_zone"]			= Parser_PortalZone;
	m_portalParsers					["target_name"]			= Parser_PortalTarget;
	m_portalParsers					["}"]					= Parser_PortalEnd;
	m_billboardParsers				["material"]			= Parser_BillboardMaterial;
	m_billboardParsers				["size"]				= Parser_BillboardSize;
	m_billboardParsers				["element"]				= Parser_BillboardElement;
	m_billboardParsers				["attach_to"]			= Parser_BillboardAttachTo;
	m_billboardParsers				["}"]					= Parser_BillboardEnd;
	m_clickableShapeParsers			["translate"]			= Parser_ClickableShapeTranslate;
	m_clickableShapeParsers			["}"]					= Parser_ClickableShapeEnd;
	m_sequenceParsers				["event"]				= Parser_SequenceEvent;
	m_sequenceParsers				["track"]				= Parser_SequenceTrack;
	m_sequenceParsers				["looped"]				= Parser_SequenceLooped;
	m_sequenceParsers				["target"]				= Parser_SequenceTarget;
	m_sequenceParsers				["}"]					= Parser_SequenceEnd;
	m_sequenceTrackParsers			["key"]					= Parser_SequenceTrackKey;
	m_sequenceTrackParsers			["interpolation"]		= Parser_SequenceTrackInterpolation;
	m_sequenceTrackParsers			["target"]				= Parser_SequenceTarget;
	m_sequenceTrackParsers			["}"]					= Parser_SequenceTrackEnd;
}

SceneFileParser :: ~SceneFileParser()
{
}

bool SceneFileParser::_parseScriptLine( String & p_line )
{
	switch ( m_context->section )
	{
	case SS_NONE:
		return _invokeParser( p_line, m_rootParsers );

	case SS_SCENE:
		return _invokeParser( p_line, m_sceneParsers );

	case SS_OBJECT:
		return _invokeParser( p_line, m_objectParsers );

	case SS_PHYSICS:
		return _invokeParser( p_line, m_physicsMaterialParsers );

	case SS_SUBMESH:
		return _invokeParser( p_line, m_objectSubMeshParsers );

	case SS_SIMULATION:
		return _invokeParser( p_line, m_physicsSimulationParsers );

	case SS_OBJECT_PHYSICS:
		return _invokeParser( p_line, m_objectPhysicsParsers );

	case SS_BOUNDING_SHAPE:
		return _invokeParser( p_line, m_boundingShapeParsers );

	case SS_NODE:
		return _invokeParser( p_line, m_nodeParsers );

	case SS_LIGHT:
		return _invokeParser( p_line, m_lightParsers );

	case SS_CAMERA:
		return _invokeParser( p_line, m_cameraParsers );

	case SS_OVERLAY:
		return _invokeParser( p_line, m_overlayParsers );

	case SS_PORTAL:
		return _invokeParser( p_line, m_portalParsers );

	case SS_SOUND:
		return _invokeParser( p_line, m_soundParsers );

	case SS_OBJECT_SOUND:
		return _invokeParser( p_line, m_soundObjectParsers );

	case SS_NODE_SOUND:
		return _invokeParser( p_line, m_soundNodeParsers );

	case SS_PLAYLIST:
		return _invokeParser( p_line, m_soundPlaylistParsers );

	case SS_VIDEO:
		return _invokeParser( p_line, m_videoParsers );

	case SS_OBJECT_VIDEO:
		return _invokeParser( p_line, m_videoObjectParsers );

	case SS_OVERLAY_VIDEO:
		return _invokeParser( p_line, m_videoOverlayParsers );

	case SS_PUB:
		return _invokeParser( p_line, m_pubParsers );

	case SS_MIRROR:
		return _invokeParser( p_line, m_mirrorParsers );

	case SS_ANIMATED_OBJECT_GROUP:
		return _invokeParser( p_line, m_animatedObjectGroupParsers );

	case SS_ANIMATED_OBJECT:
		return _invokeParser( p_line, m_animatedObjectParsers );

	case SS_ANIMATION:
		return _invokeParser( p_line, m_animationParsers );

	case SS_PARTICLE_SYSTEM:
		return _invokeParser( p_line, m_particleSystemParsers );

	case SS_BILLBOARDSET:
		return _invokeParser( p_line, m_billboardParsers );

	case SS_CLICKABLESHAPE:
		return _invokeParser( p_line, m_clickableShapeParsers );

	case SS_SEQUENCE:
		return _invokeParser( p_line, m_sequenceParsers );

	case SS_SEQUENCE_TRACK:
		return _invokeParser( p_line, m_sequenceTrackParsers );

	case SS_CODE:
	{
		if ( p_line == "</code>" )
		{
			Parser_RootCodeEnd( p_line, m_context );
		}
		else
		{
			m_context->general_string_a += p_line;
			m_context->general_string_b += m_context->fullLine;
			m_context->general_string_b += "\n";
			return false;
		}

		break;
	}

	default :
		return _delegateParser( p_line );
	}

	return false;
}

bool SceneFileParser::_invokeParser( String & p_line, const AttributeParserMap & p_parsers )
{
	StringVector splitCmd = StringUtil::split( p_line, " \t", 1 );
	EMUSE_LOG_MESSAGE_DEBUG( "SceneFileParser : Line : " + p_line );
	const AttributeParserMap::const_iterator & l_iter = p_parsers.find( splitCmd[0] );

	if ( l_iter == p_parsers.end() )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Parser not found @ line # " + StringConverter::toString( m_context->lineNo ) + ": " + p_line );
		return false;
	}

	if ( splitCmd.size() >= 2 )
	{
		StringUtil::trim( splitCmd[1] );
		return l_iter->second( splitCmd[1], m_context );
	}

	return l_iter->second( EMPTY_STRING, m_context );
}

void SceneFileParser :: ParseScript( ConfigFile * p_configFile )
{
	bool nextIsOpenBrace = false;
	bool commented = false;
	const String & l_fileDescName = p_configFile->GetDescriptiveName();
	EMUSE_LOG_MESSAGE_RELEASE( "SceneFileParser : Parsing script [" + l_fileDescName + "]" );
	p_configFile->Use();
	std::ifstream l_stream;
	const Path & l_path = p_configFile->GetOwner()->GetCompletePath() / p_configFile->GetName();
	l_stream.open( l_path.c_str(), std::ios::in | std::ios::binary );

	if ( l_stream.fail() || l_stream.bad() )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "SceneFileParser : Error, file [" + l_fileDescName + "] does not exists" );
		return;
	}

//	FileStreamDataStream * FSDSstream = new FileStreamDataStream( p_configFile->GetName(), & l_stream, static_cast <size_t> (GetFileSize( p_configFile->GetFullPath())), false);	FileStreamDataStream * FSDSstream = new FileStreamDataStream( p_configFile->GetName(), & l_stream, static_cast <size_t> (GetFileSize( p_configFile->GetFullPath())), false);
//	FileStreamDataStream * FSDSstream = new FileStreamDataStream( l_stream, true);
	m_context->section = SS_NONE;
	m_context->lineNo = 0;
	m_context->general_int = 0;
	m_context->general_string_a.clear();
	m_context->general_string_b.clear();
	m_context->general_string_c.clear();
	m_context->baseDirectory = m_baseDirectory;
	m_context->scriptEngine->m_context->lastSceneFile = p_configFile->GetName();
	m_context->CurrentBackground = ColourValue::Black;
	m_context->instanceNum = m_appIndexStr;
	bool l_reuse = false;
	String l_line;

	while ( ! l_stream.eof() )
	{
		if ( ! l_reuse )
		{
			std::getline( l_stream, l_line ); //FSDSstream->getLine( false);
			m_context->lineNo ++;
		}
		else
		{
			l_reuse = false;
		}

		m_context->fullLine = l_line;
		StringUtil::trim( l_line );

		if ( l_line.empty() )
		{
			continue;
		}

		if ( l_line.size() >= 2 && l_line.substr( 0, 2 ) == "//" )
		{
			continue;
		}

		if ( ! commented )
		{
			if ( l_line.size() >= 2 && l_line.substr( 0, 2 ) == "/*" )
			{
				commented = true;
			}
			else
			{
				if ( nextIsOpenBrace )
				{
					if ( l_line != "{" )
					{
						String l_endLine = "}";
						nextIsOpenBrace = _parseScriptLine( l_endLine );
						l_reuse = true;
					}
					else
					{
						nextIsOpenBrace = false;
					}
				}
				else
				{
					nextIsOpenBrace = _parseScriptLine( l_line );
				}
			}
		}
		else
		{
			if ( l_line.size() >= 2 && l_line.substr( 0, 2 ) == "*/" )
			{
				commented = false;
			}
		}
	}

	if ( m_context->section != SS_NONE )
	{
		parsingError( m_context, "Parsing Error : ParseScript -> unexpected end of file" );
	}

	//TODO : gné ? O.o
//	delete FSDSstream;
	p_configFile->Release();
	EMUSE_LOG_MESSAGE_RELEASE( "SceneFileParser : Finished parsing script [" + l_fileDescName + "]" );
}
