/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_DATA_H___
#define ___MODULE_DATA_H___

#include "Elypse.h"

namespace Elypse::Data
{
	class DataFile;
	class ConfigFile;
	class MuseFile;

	struct MuseFileDeleter
	{
		void operator()( MuseFile * )const noexcept;
	};
	
	struct DataFileDeleter
	{
		void operator()( DataFile * )const noexcept;
	};
	
	struct ConfigFileDeleter
	{
		void operator()( ConfigFile * )const noexcept;
	};

	using MuseFilePtr = std::unique_ptr< MuseFile, MuseFileDeleter >;
	using DataFilePtr = std::unique_ptr< DataFile, DataFileDeleter >;
	using ConfigFilePtr = std::unique_ptr< ConfigFile, ConfigFileDeleter >;

	typedef std::map< String , DataFilePtr > DataFileMap;
	typedef std::vector< DataFile *> DataFileArray;
	typedef std::map< String , ConfigFilePtr > ConfigFileMap;
	typedef std::vector< ConfigFile *> ConfigFileArray;
	typedef std::map< String , MuseFilePtr > MuseFileMap;
}

using namespace Elypse::Data;

#endif
