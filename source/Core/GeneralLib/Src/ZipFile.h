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
#ifndef ___ZIP_FILE_H___
#define ___ZIP_FILE_H___

#include "Zip.h"

#include "Macros.h"
#include "StringUtils.h"

#include <fstream>

namespace General
{
	namespace Files
	{
		template< typename CharType >
		class ZipFileImpl
		{
		protected:
			typedef std::basic_string< CharType > StringType;
			StringType m_name;
			StringType m_fullname;
			ZipDirectoryBase< CharType > * m_directory;

			uint64_t m_uncompressedSize;
			uint64_t m_compressedSize;

		public:
			ZipFileImpl( const StringType & p_name, const StringType & p_fullname, ZipDirectoryBase< CharType > * p_directory )
				: m_name( p_name )
				, m_fullname( p_fullname )
				, m_directory( p_directory )
				, m_uncompressedSize( 0 )
				, m_compressedSize( 0 )
			{}
			void SetSize( uint64_t p_compSize, uint64_t p_uncompSize )
			{
				m_uncompressedSize = p_uncompSize;
				m_compressedSize = p_compSize;
			}

		public:
			inline const StringType & GetName()const
			{
				return m_name;
			}
			inline const StringType & GetFullName()const
			{
				return m_fullname;
			}
			inline uint64_t GetCompressedSize()const
			{
				return m_compressedSize;
			}
			inline uint64_t GetUncompressedSize()const
			{
				return m_uncompressedSize;
			}
		};


		template< typename CharType >
		class d_dll ZipFileBase
			: public ZipFileImpl< CharType >
		{
		protected:
			typedef typename ZipFileImpl< CharType >::StringType StringType;
			uint64_t m_index;

		public:
			ZipFileBase( uint64_t p_index, const StringType & p_name, const StringType & p_fullname, ZipDirectoryBase< CharType > * p_directory )
				: ZipFileImpl< CharType > ( p_name, p_fullname, p_directory )
				, m_index( p_index )
			{
			}
		public:
			bool Extract( const StringType & p_newPath, ZipArchiveBase< CharType > * p_archive )
			{
				static const unsigned int c_bufferSize = 2048;
				//Search for the file at given index
				struct zip_stat l_stat = { 0 };
				zip * l_zip = p_archive->GetArchive();
				zip_file * l_zipfile = zip_fopen_index( l_zip, this->m_index, ZIP_FL_UNCHANGED );

				if ( !l_zipfile )
				{
					int l_zep, l_sep;
					zip_error_get( l_zip, &l_zep, &l_sep );
					std::string l_error = Zip::GetErrorDesc( l_zep ) + " - " + Zip::GetErrorDesc( l_sep );
					GENLIB_EXCEPTION( "Couldn't retrieve file from ZIP archive : " + l_error );
				}

				//Alloc memory for its uncompressed contents
				std::vector< char > l_contents( c_bufferSize );
				zip_uint64_t l_read = 0;
				std::ofstream l_outFile;
				l_outFile.open( ( p_newPath + this->m_fullname ).c_str(), std::ios::out | std::ios::binary );

				if ( !l_outFile.is_open() )
				{
					GENLIB_EXCEPTION( "Couldn't open file on disk" );
				}
				else
				{
					while ( l_read < this->GetUncompressedSize() )
					{
						zip_uint64_t l_tmp = zip_fread( l_zipfile, l_contents.data(), std::min( zip_uint64_t( c_bufferSize ), this->GetUncompressedSize() ) );

						if ( l_tmp >= 0 )
						{
							l_outFile.write( l_contents.data(), l_tmp );
							l_read += l_tmp;
						}
						else
						{
							int l_zep, l_sep;
							zip_error_get( l_zip, &l_zep, &l_sep );
							std::string l_error = Zip::GetErrorDesc( l_zep ) + " - " + Zip::GetErrorDesc( l_sep );
							zip_fclose( l_zipfile );
							GENLIB_EXCEPTION( "Couldn't read ZIP archive file " + Utils::string_cast< char >( this->m_name ) + " : " + l_error );
						}
					}

					l_outFile.close();
				}

				zip_fclose( l_zipfile );
				return true;
			}
		};

	}
}

#endif
