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
#ifndef ___EMUSE_SCENE_FILE_PARSER_H___
#define ___EMUSE_SCENE_FILE_PARSER_H___

#include "Module_SceneFile.h"

#include "Module_Script.h"
#include "Module_Data.h"

namespace Elypse
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
			SceneFileParser( String const & p_instance, const Path & p_baseDirectory );
			virtual ~SceneFileParser();

		public:
			virtual void ParseScript( ConfigFile * p_file );
			virtual inline bool _delegateParser( String & p_line )
			{
				return false;
			}

		public:
			static void parsingError( Context * context, String const & s );

			static Vector3 Parser_Vector3Value( String const & params );
			static Vector4 Parser_Vector4Value( String const & params );
			static ColourValue Parser_ColourValue( String const & params );
		};
	}
}

#endif
