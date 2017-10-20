/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_DATA_H___
#define ___MODULE_DATA_H___

#include "Elypse.h"

namespace Elypse
{
	namespace Data
	{
		class DataFile;
		class ConfigFile;
		class MuseFile;

		/*
		enum ConfigFileType
		{
			EM_SCENE_FILE,
			EM_SCRIPT_FILE
		};
		*/

		typedef std::map< String , DataFile * > DataFileMap;
		typedef std::vector< DataFile *> DataFileArray;
		typedef std::map< String , ConfigFile * > ConfigFileMap;
		typedef std::vector< ConfigFile *> ConfigFileArray;
		typedef std::map< String , MuseFile * > MuseFileMap;
	}
}

using namespace Elypse::Data;

#endif
