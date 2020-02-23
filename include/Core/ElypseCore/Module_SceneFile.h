/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_SCENE_FILE_H___
#define ___MODULE_SCENE_FILE_H___

#include "Elypse.h"
#include "Module_Main.h"

namespace Elypse
{
	namespace SceneFile
	{
		enum ScriptSection
		{
			SS_NONE,
			SS_SCENE,
			SS_OBJECT,
			SS_PHYSICS,
			SS_OBJECT_PHYSICS,
			SS_BILLBOARDSET,
			SS_BOUNDING_SHAPE,
			SS_SIMULATION,
			SS_SUBMESH,
			SS_PORTAL,
			SS_NODE,
			SS_CAMERA,
			SS_LIGHT,
			SS_STARTUP_SCRIPT,
			SS_CODE,
			SS_OVERLAY,
			SS_SOUND,
			SS_OBJECT_SOUND,
			SS_NODE_SOUND,
			SS_VIDEO,
			SS_OBJECT_VIDEO,
			SS_OVERLAY_VIDEO,
			SS_PUB,
			SS_MIRROR,
			SS_ANIMATION,
			SS_ANIMATED_OBJECT,
			SS_ANIMATED_OBJECT_GROUP,
			SS_PARTICLE_SYSTEM,
			SS_PLAYLIST,
			SS_CLICKABLESHAPE,
			SS_SEQUENCE,
			SS_SEQUENCE_TRACK,
			SS_DELEGATE
		};

		class SceneFileParser;
		class SceneFileSaver;

		typedef bool ( * ParserFunction )( String & p_params, Context * p_context );

		typedef std::map <String, String>				OverlayCodeMap;
		typedef std::map <String, ParserFunction>		AttributeParserMap;

#define DECLARE_SCENE_FILE_PARSER( X) bool d_dll_export X( String & p_params, Context * p_context);
#define DECLARE_SCENE_FILE_PARSER_NO_EXPORT( X) bool X( String & p_params, Context * p_context);
#define DEFINE_SCENE_FILE_PARSER( X) bool Elypse::SceneFile::X( String & p_params, Context * p_context)
#define PARSING_ERROR( p_error) SceneFileParser::parsingError( p_context, p_error)
#define PARSING_ERROR2( p_error) SceneFileParser::parsingError( p_context, /* String( "Parsing Error (") + String(__FUNCTION__).substr( 18, 1000) + String(") -> ") + */ p_error)
	}
}

using namespace Elypse::SceneFile;

#endif
