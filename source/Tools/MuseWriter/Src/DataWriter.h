/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DataWriter.h - DataWriter.cpp

	Desc :		Class used to write the datas used in EMuse.

*********************************************************************************************************************/
#ifndef ___DataWriter_H___
#define ___DataWriter_H___

#include <vector>

#include <Module_Download.h>

// The struct to stock blocks informations*
namespace EMuse
{
	namespace Download
	{
		static const std::string  DataTypeExtension[] =
		{
			"",
			".zip",			// EM_BLOCK_ZIPDATA			= 1,	/* .zip files */
			".gzip",		// EM_BLOCK_GZIPDATA		= 2,	/* unused */
			".emscene",		// EM_BLOCK_SCENE			= 3,	/* .emscene files */
			".emscript",	// EM_BLOCK_INTERACTIONS	= 4,	/* .emscript files */
			".snd",			// EM_BLOCK_SNDDATA			= 5,	/* unused */
			".cfg",			// EM_BLOCK_CONFIG			= 6,	/* unused */
			".emsnd",		// EM_BLOCK_ZIPSNDDATA		= 7,	/* .emsnd files */
			".emcfg",		// EM_BLOCK_EMCONFIG		= 8,	/* unused */
		};

		struct EM_Block
		{
			uint8_t m_blocktype;
			uint8_t m_namesize;
			std::string m_name;
			uint32_t m_blocsize;
			std::string m_completeUrl;
			std::string m_hash;
		};

		struct EM_ZipBlock
		{
			uint32_t BlockSize;
			uint16_t NameSize;
			std::string Name;
			std::string CompleteURL;
		};

		typedef std::vector< EM_Block > BlockList;
		typedef std::vector< EM_ZipBlock > ZipBlockList;
	}

	class DataWriter
	{
	private:
		EMuse::Download::BlockList m_header;
		std::vector< std::string > m_fileslist;

	public:
		/**
			Default constructor, makes nothing at all
		*/
		DataWriter();
		/**
			Destructor, clears the list of files
		*/
		~DataWriter();
		/**
			Adds a file to the list of files, without compressing it
				@params :	p_file : [in]	the file to add to the list
				@params :	p_eType : [in]	the block type
		*/
		bool AddFile( const std::string  & p_file, EMuse::Download::DataBlockType p_eType = EMuse::Download::EM_NB_BLOCK_TYPES );
		bool DelFile( uint32_t p_index );
		bool DownFile( uint32_t p_index );
		bool UpFile( uint32_t p_index );
		/**
			Compress the folder p_folder and add it's filename to the list of files
				@params :	p_folder : [in]	the folder to compress
				@return :	true if compression was successful
							false if not
		*/
		bool AddFolder( const std::string  & p_folder, EMuse::Download::DataBlockType p_eType );
		void ListFiles( const std::string  & p_folder );
		int CompressFile( const std::string  & p_infilename, const std::string  & p_outfilename );
		bool CompressFolder( const std::string  & p_outFolder, const std::string  & p_outPath, EMuse::Download::DataBlockType p_eType );
		/**
			Writes the final file using p_filename as the file name ( ;) )
				@params :	p_filename :[in/out]the name of the final file, mustn't contain extenxion
												(added by this function)
				@return :	true if successful
							false if not
		*/
		bool Write( const std::string  & p_filename );
		void WriteZipFile( EMuse::Download::ZipBlockList p_list, FILE * p_file );
		void WriteHeader( size_t & l_nbBytes, FILE * p_file );
		void WriteZipHeader( EMuse::Download::ZipBlockList p_list, FILE * p_file );
		std::vector< std::string > GetFileList();
	};
}

#endif

