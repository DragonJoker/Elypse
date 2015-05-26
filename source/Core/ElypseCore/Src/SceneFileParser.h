/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___EMUSE_SCENE_FILE_PARSER_H___
#define ___EMUSE_SCENE_FILE_PARSER_H___

#include "Module_SceneFile.h"

#include "Module_Script.h"
#include "Module_Data.h"

namespace EMuse
{
	namespace SceneFile
	{
		class d_dll_export SceneFileParser d_debug_check_noncopyable
		{
		protected :
			AttributeParserMap m_rootParsers;
			AttributeParserMap m_sceneParsers;
			AttributeParserMap m_objectParsers;
			AttributeParserMap m_objectSubMeshParsers;
			AttributeParserMap m_objectPhysicsParsers;
			AttributeParserMap m_physicsMaterialParsers;
			AttributeParserMap m_physicsSimulationParsers;
			AttributeParserMap m_boundingShapeParsers;
			AttributeParserMap m_clickableShapeParsers;
			AttributeParserMap m_nodeParsers;
			AttributeParserMap m_cameraParsers;
			AttributeParserMap m_lightParsers;
			AttributeParserMap m_overlayParsers;
			AttributeParserMap m_soundParsers;
			AttributeParserMap m_soundObjectParsers;
			AttributeParserMap m_soundNodeParsers;
			AttributeParserMap m_soundPlaylistParsers;
			AttributeParserMap m_videoParsers;
			AttributeParserMap m_videoObjectParsers;
			AttributeParserMap m_videoOverlayParsers;
			AttributeParserMap m_pubParsers;
			AttributeParserMap m_mirrorParsers;
			AttributeParserMap m_particleSystemParsers;
			AttributeParserMap m_animationParsers;
			AttributeParserMap m_animatedObjectParsers;
			AttributeParserMap m_animatedObjectGroupParsers;
			AttributeParserMap m_portalParsers;
			AttributeParserMap m_billboardParsers;
			AttributeParserMap m_sequenceParsers;
			AttributeParserMap m_sequenceTrackParsers;

			Path m_baseDirectory;
			String m_appIndexStr;

		public:
			Context * m_context;
			ScriptEngine * m_scriptEngine;

		protected:
			bool _parseScriptLine( String & p_line );
			bool _invokeParser( String & p_line, const AttributeParserMap & p_parsers );

		public:
			SceneFileParser( const String & p_instance, const Path & p_baseDirectory );
			virtual ~SceneFileParser();

		public:
			virtual void ParseScript( ConfigFile * p_file );
			virtual inline bool _delegateParser( String & p_line )
			{
				return false;
			}

		public:
			static void parsingError( Context * context, const String & s );

			static Vector3 Parser_Vector3Value( const String & params );
			static Vector4 Parser_Vector4Value( const String & params );
			static ColourValue Parser_ColourValue( const String & params );
		};
	}
}

#endif
