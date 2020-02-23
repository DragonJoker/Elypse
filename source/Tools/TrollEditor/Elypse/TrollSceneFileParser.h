/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_SCENE_FILE_PARSER_H___
#define ___TROLL_SCENE_FILE_PARSER_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <SceneFileParser.h>
#include <Module_SceneFile.h>

namespace Troll
{
	namespace ElypseRW
	{
		class TrollSceneFileParser
			: public SceneFileParser
		{
		private:
			String m_fileName;
			bool m_lineToAdd;

		public:
			TrollSceneFileParser( String const & p_instance, String const & p_baseDirectory );
			virtual ~TrollSceneFileParser();

			virtual void ParseScript( ConfigFile * p_file );

			inline String const & GetFileName()const
			{
				return m_fileName;
			}
		};

		extern TrollSceneFileParser * g_fileParser;
		extern wxString g_fileName;
	}
}

#endif
