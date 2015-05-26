#ifndef ___TROLL_SCENE_FILE_PARSER_H___
#define ___TROLL_SCENE_FILE_PARSER_H___

#include <SceneFileParser.h>
#include <Module_SceneFile.h>

namespace Troll
{
	namespace Scene
	{
		class TrollSceneFileParser: public SceneFileParser
		{
		private:
			String m_fileName;
			bool m_lineToAdd;

		public:
			TrollSceneFileParser( const String & p_instance, const String & p_baseDirectory );
			virtual ~TrollSceneFileParser();

			virtual void ParseScript( ConfigFile * p_file );

			inline const String & GetFileName()const
			{
				return m_fileName;
			}
		};
	}
}

using namespace Troll::Scene;

#endif
