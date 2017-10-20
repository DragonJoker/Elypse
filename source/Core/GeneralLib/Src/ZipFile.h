/*
See LICENSE file in root folder
*/
#ifndef ___ZIP_FILE_H___
#define ___ZIP_FILE_H___

#include "ZipBase.h"

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
			using StringType = std::basic_string< CharType >;

		public:
			inline ZipFileImpl( StringType const & p_name, StringType const & p_fullname, ZipDirectoryBase< CharType > * p_directory )
				: m_name{ p_name }
				, m_fullname{ p_fullname }
				, m_directory{ p_directory }
				, m_uncompressedSize{ 0 }
				, m_compressedSize{ 0 }
			{
			}

			inline void SetSize( uint64_t p_compSize, uint64_t p_uncompSize )
			{
				m_uncompressedSize = p_uncompSize;
				m_compressedSize = p_compSize;
			}

			inline StringType const & GetName()const
			{
				return m_name;
			}

			inline StringType const & GetFullName()const
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

		protected:
			StringType m_name;
			StringType m_fullname;
			ZipDirectoryBase< CharType > * m_directory;

			uint64_t m_uncompressedSize;
			uint64_t m_compressedSize;
		};

		template< typename CharType >
		class ZipFileBase
			: public ZipFileImpl< CharType >
		{
		protected:
			using StringType = ZipFileImpl< CharType >::StringType;

		public:
			inline ZipFileBase( uint64_t p_index, StringType const & p_name, StringType const & p_fullname, ZipDirectoryBase< CharType > * p_directory )
				: ZipFileImpl{ p_name, p_fullname, p_directory }
				, m_index{ p_index }
			{
			}

			inline bool Extract( StringType const & p_newPath, ZipArchiveBase< CharType > * p_archive )
			{
				static const uint32_t c_bufferSize{ 2048u };
				//Search for the file at given index
				struct zip_stat l_stat{ 0 };
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
				std::array< char, c_bufferSize > l_contents;
				zip_uint64_t l_read{ 0 };
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
						zip_int64_t l_tmp{ zip_fread( l_zipfile, l_contents.data(), std::min( zip_uint64_t( c_bufferSize ), this->GetUncompressedSize() ) ) };

						if ( l_tmp >= 0 )
						{
							l_outFile.write( l_contents.data(), l_tmp );
							l_read += l_tmp;
						}
						else
						{
							int l_zep{ 0 };
							int l_sep{ 0 };
							zip_error_get( l_zip, &l_zep, &l_sep );
							std::string l_error{ Zip::GetErrorDesc( l_zep ) + " - " + Zip::GetErrorDesc( l_sep ) };
							zip_fclose( l_zipfile );
							GENLIB_EXCEPTION( "Couldn't read ZIP archive file " + Utils::string_cast< char >( this->m_name ) + " : " + l_error );
						}
					}

					l_outFile.close();
				}

				zip_fclose( l_zipfile );
				return true;
			}

		protected:
			uint64_t m_index;
		};
	}
}

#endif
