/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DataWriter.h - DataWriter.cpp

	Desc :		Class used to write the datas used in EMuse.

*********************************************************************************************************************/
#ifndef ___DataWriter_H___
#define ___DataWriter_H___

#include <vector>
#include <iostream>

#include <wx/textctrl.h>

#include <zlib.h>
#include <MD5.h>

#include <Module_Download.h>

// The struct to stock blocks informations*
namespace EMuse
{
	namespace Download
	{
		static const wxString DataTypeExtension[] =
		{
			wxT( "" ),
			wxT( ".zip" ),		// EM_BLOCK_ZIPDATA			= 1,	/* .zip files */
			wxT( ".gzip" ),		// EM_BLOCK_GZIPDATA		= 2,	/* unused */
			wxT( ".emscene" ),	// EM_BLOCK_SCENE			= 3,	/* .emscene files */
			wxT( ".emscript" ),	// EM_BLOCK_INTERACTIONS	= 4,	/* .emscript files */
			wxT( ".snd" ),		// EM_BLOCK_SNDDATA			= 5,	/* unused */
			wxT( ".cfg" ),		// EM_BLOCK_CONFIG			= 6,	/* unused */
			wxT( ".emsnd" ),	// EM_BLOCK_ZIPSNDDATA		= 7,	/* .emsnd files */
			wxT( ".emcfg" ),	// EM_BLOCK_EMCONFIG		= 8,	/* unused */
		};

		struct EM_Block
		{
			short m_blocktype;
			short m_namesize;
			wxString m_name;
			int m_blocsize;
			wxString m_completeUrl;
			char * m_hash;

			~EM_Block()
			{
				delete [] m_hash;
			}
		};
		typedef std::vector <EM_Block *> BlockList;
		struct EM_ZipBlock
		{
			int BlockSize;
			short NameSize;
			wxString Name;
			wxString CompleteURL;
		};
		typedef std::vector <EM_ZipBlock *> ZipBlockList;
	}

	class DataWriter
	{
	private:
		EMuse::Download::BlockList m_header;
		std::vector <wxString> m_fileslist;
		FILE * m_file;
		wxTextCtrl * m_log;
		General::Utils::MD5 m_md5;

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
			Sets the log text control
				@params :	p_log : [in]	The text control
		*/
		void SetLog( wxTextCtrl * p_log )
		{
			m_log = p_log;
		}
		/**
			Adds a file to the list of files, without compressing it
				@params :	p_file : [in]	the file to add to the list
				@params :	p_eType : [in]	the block type
		*/
		bool AddFile( const wxString & p_file, EMuse::Download::DataBlockType p_eType = EMuse::Download::EM_NB_BLOCK_TYPES );
		bool DelFile( const wxString & p_file );
		bool DownFile( const wxString & p_filename );
		bool UpFile( const wxString & p_filename );
		/**
			Compress the folder p_folder and add it's filename to the list of files
				@params :	p_folder : [in]	the folder to compress
				@return :	true if compression was successful
							false if not
		*/
		bool AddFolder( const wxString & p_folder, EMuse::Download::DataBlockType p_eType );
		void ListFiles( const wxString & p_folder );
		int CompressFile( const wxString & p_infilename, const wxString & p_outfilename );
		bool CompressFolder( const wxString & p_outFolder, const wxString & p_outPath, EMuse::Download::DataBlockType p_eType );
		/**
			Writes the final file using p_filename as the file name ( ;) )
				@params :	p_filename :[in/out]the name of the final file, mustn't contain extenxion
												(added by this function)
				@return :	true if successful
							false if not
		*/
		bool Write( const wxString & p_filename );
		void WriteZipFile( EMuse::Download::ZipBlockList p_list, FILE * p_file );
		void WriteHeader( unsigned long long & l_nbBytes );
		void WriteZipHeader( EMuse::Download::ZipBlockList p_list, FILE * p_file );
		int GetType( const wxString & p_filename );
		wxString GetName( const wxString & p_fileurl );
		wxString GetNameWithoutExtension( const wxString & p_fileurl );
		wxString GetExtension( const wxString & p_filename );
		wxString GetParentFolder( const wxString & p_folder );
		void UpdateFileList( wxTextCtrl * );
	};
}

#endif

