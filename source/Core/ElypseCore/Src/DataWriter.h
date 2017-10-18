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
#ifndef ___DataWriter_H___
#define ___DataWriter_H___

#include <vector>

#include "Module_Download.h"

// The struct to stock blocks informations*
namespace Elypse
{
	namespace Download
	{
		class d_dll_export DataWriter
		{
		public:
			/**
				Adds a file to the list of files, without compressing it
					@params : p_file : [in]	the file to add to the list
					@params : p_eType : [in]	the block type
			*/
			bool AddFile( const std::string  & p_file, DataBlockType p_eType = EM_NB_BLOCK_TYPES );
			bool DelFile( uint32_t p_index );
			bool DownFile( uint32_t p_index );
			bool UpFile( uint32_t p_index );
			/**
				Compress the folder p_folder and add it's filename to the list of files
					@params : p_folder : [in]	the folder to compress
					@return : true if compression was successful
								false if not
			*/
			bool AddFolder( const std::string  & p_folder, DataBlockType p_eType );
			void ListFiles( const std::string  & p_folder );
			int CompressFile( const std::string  & p_infilename, const std::string  & p_outfilename );
			bool CompressFolder( const Path  & p_outFolder, const Path & p_outPath, DataBlockType p_eType );
			/**
				Writes the final file using p_filename as the file name ( ;) )
					@params : p_filename :[in/out]the name of the final file, mustn't contain extenxion
													(added by this function)
					@return : true if successful
								false if not
			*/
			bool Write( const std::string  & p_filename );
			void WriteZipFile( ZipBlockList p_list, FILE * p_file );
			void WriteHeader( size_t & l_nbBytes, FILE * p_file );
			void WriteZipHeader( ZipBlockList p_list, FILE * p_file );
			std::vector< std::string > GetFileList();

		private:
			BlockList m_header;
			std::vector< std::string > m_fileslist;
		};
	}
}

#endif

