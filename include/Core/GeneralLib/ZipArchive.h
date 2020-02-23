/*
See LICENSE file in root folder
*/
#ifndef ___ZIP_ARCHIVE_H___
#define ___ZIP_ARCHIVE_H___

#include "ZipBase.h"
#include "ZipDirectory.h"

#include "Exception.h"
#include "Macros.h"
#include "Path.h"
#include "StringConverter.h"
#include "StringUtils.h"

#include <iostream>

#include <limits>
#include <cstdint>
#include <memory>

#if defined( MSDOS ) || defined( OS2 ) || defined( _WIN32 ) || defined( __CYGWIN__ )
#	include <fcntl.h>
#	include <io.h>
#	define SET_BINARY_MODE( file ) setmode( fileno( file ), O_BINARY )
#else
#	define SET_BINARY_MODE(file)
#endif

namespace General
{
	typedef std::vector< uint8_t > ByteArray;

	namespace Files
	{
		template< typename CharType >
		class ZipArchiveImpl
			: d_noncopyable
		{
		protected:
			typedef std::basic_string< CharType > StringType;
			typedef std::map< StringType, ZipDirectoryBase< CharType > * > ZipDirectoryMapType;
			typedef std::map< StringType, ZipFileBase< CharType > * > ZipFileMapType;
			typedef std::vector< ZipFileBase< CharType > > ZipFileArrayType;

		public:
			inline ZipArchiveImpl( StringType const & p_name, StringType const & p_path )
				: m_basename{ p_name }
				, m_path{ p_path }
				, m_fullPath{ p_path }
				, m_zip{ nullptr }
				, m_rootDirectory{ new ZipDirectoryBase< CharType >{ StringType{}, nullptr } }
			{
				m_fullPath.push_back( d_path_slash );
				m_fullPath += p_name;
			}

			inline ~ZipArchiveImpl()
			{
				delete m_rootDirectory;
				d_debug_assert( !m_zip );
			}

			inline bool HasFile( StringType const & p_filename )const
			{
				return ( m_files.find( p_filename ) != m_files.end() );
			}

			inline bool HasDirectory( StringType const & p_filename )const
			{
				return ( m_directories.find( p_filename ) != m_directories.end() );
			}

			inline ZipFileBase< CharType > * GetFile( StringType const & p_name )const
			{
				return General::Utils::map::findOrNull( m_files, p_name );
			}

			inline ZipDirectoryBase< CharType > * GetDirectory( StringType const & p_name )const
			{
				return General::Utils::map::findOrNull( m_directories, p_name );
			}

			inline ZipFileArrayType RecurseFindFile( StringType const & p_name )
			{
				return m_rootDirectory->RecurseFindFile( p_name );
			}

			inline StringType const & GetName()const
			{
				return m_basename;
			}

			inline StringType const & GetBasePath()const
			{
				return m_path;
			}

			inline StringType const & GetFullPath()const
			{
				return m_fullPath;
			}

			inline zip * GetArchive()const
			{
				return m_zip;
			}

		protected:
			StringType m_basename;
			StringType m_path;
			StringType m_fullPath;

			zip * m_zip;

			ZipDirectoryMapType m_directories;
			ZipFileMapType m_files;
			ZipDirectoryBase< CharType > * m_rootDirectory;
		};

		template< typename CharType >
		class ZipArchiveBase
			: public ZipArchiveImpl< CharType >
		{
		protected:
			typedef typename ZipArchiveImpl< CharType >::StringType StringType;

		public:
			inline ZipArchiveBase( StringType const & p_name, StringType const & p_path )
				: ZipArchiveImpl< CharType >( p_name, p_path )
			{
			}

			inline bool Extract( StringType const & p_newPath )
			{
				try
				{
					if ( !doLoad() )
					{
						return false;
					}

					StringType l_path = p_newPath;
					doCheckPath( l_path );
					return this->m_rootDirectory->Extract( l_path, this );
				}
				catch ( Utils::GenException & )
				{
					return false;
				}
			}

		protected:
			inline bool doLoad()
			{
				int l_result = 0;
				this->m_zip = zip_open( Utils::string_cast< char >( this->m_fullPath ).c_str(), ZIP_CHECKCONS, &l_result );

				if ( !this->m_zip )
				{
					GENLIB_EXCEPTION( "Couldn't open archive file " + Zip::GetErrorDesc( l_result ) );
				}
				else if ( l_result != ZIP_ER_OK )
				{
					GENLIB_EXCEPTION( "Couldn't open archive file " + Zip::GetErrorDesc( l_result ) );
				}

				ZipDirectoryBase< CharType > * l_currentDirectory = this->m_rootDirectory;
				std::cout << "ZipArchive::load : " << this->m_fullPath << std::endl;
				int l_count = zip_get_num_files( this->m_zip );

				if ( l_count == -1 )
				{
					GENLIB_EXCEPTION( "Couldn't retrieve ZIP archive files count" );
				}

				for ( int64_t i = 0; i < l_count; ++i )
				{
					//Search for the file at given index
					struct zip_stat l_stat = { 0 };

					if ( zip_stat_index( this->m_zip, i, 0, &l_stat ) == -1 )
					{
						int l_zep, l_sep;
						zip_error_get( this->m_zip, &l_zep, &l_sep );
						std::string l_error = Zip::GetErrorDesc( l_zep ) + " - " + Zip::GetErrorDesc( l_sep );
						GENLIB_EXCEPTION( "Couldn't retrieve ZIP archive file informations : " + l_error );
					}

					if ( l_stat.size > 0 )
					{
						StringType l_fullPath = Utils::string_cast< CharType >( std::string( l_stat.name ) );
						size_t l_index = l_fullPath.find_last_of( CharType( '/' ) );

						if ( l_index == StringType::npos )
						{
							l_index = l_fullPath.find_last_of( CharType( '\\' ) );
						}

						StringType l_path = l_fullPath.substr( 0, l_index + 1 );
						StringType l_name = l_fullPath.substr( l_index + 1 );

						if ( !l_path.empty() )
						{
							l_path.erase( l_path.size() - 1 );

							if ( l_currentDirectory->GetName() != l_path && !l_path.empty() )
							{
								l_currentDirectory = this->GetDirectory( l_path );
							}

							if ( !l_currentDirectory )
							{
								l_currentDirectory = this->m_rootDirectory->AddDirectory( l_path );
							}

							this->m_directories.insert( { l_path, l_currentDirectory } );
						}
						else
						{
							l_currentDirectory = this->m_rootDirectory;
						}

						ZipFile * l_file = l_currentDirectory->AddFile( i, l_name, l_fullPath );
						l_file->SetSize( l_stat.comp_size, l_stat.size );
						this->m_files.insert( { l_fullPath, l_file } );
					}
				}

				this->m_rootDirectory->CheckDirectorySize();
				return true;
			}

			inline void doCheckPath( StringType & p_path )
			{
				//if (p_path.empty() || p_path[p_path.size() - 1] != '/')
				{
					p_path.push_back( CharType( '/' ) );
				}
			}
		};
	}
}

#endif
