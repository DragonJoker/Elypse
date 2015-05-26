#ifndef ___ZIP_ARCHIVE_H___
#define ___ZIP_ARCHIVE_H___

#include "Zip.h"
#include "ZipDirectory.h"

#include "Macros.h"
#include "Path.h"

#include <iostream>

namespace General
{
	namespace Files
	{
		template <typename T>
		class d_dll ZipArchiveImpl
		{
		protected:
			typedef std::map <T, ZipDirectoryBase <T> *> ZipDirectoryMapType;
			typedef std::map <T, ZipFileBase <T> *> ZipFileMapType;
			typedef std::vector <ZipFileBase <T> > ZipFileArrayType;

		protected:
			T m_basename;
			T m_path;
			T m_fullPath;

			zzip_dir * m_internalArchive;

			ZipDirectoryMapType m_directories;
			ZipFileMapType m_files;
			ZipDirectoryBase <T> * m_rootDirectory;

		public:
			ZipArchiveImpl( const T & p_name, const T & p_path )
				:	m_basename( p_name ),
					m_path( p_path ),
					m_fullPath( p_path ),
					m_internalArchive( NULL )
			{
				m_fullPath.push_back( d_path_slash );
				m_fullPath += p_name;
				m_rootDirectory = new ZipDirectoryBase <T> ( T(), NULL );
			}

			~ZipArchiveImpl()
			{
				delete m_rootDirectory;

				if ( m_internalArchive )
				{
					zzip_dir_close( m_internalArchive );
				}
			}

		public:
			bool HasFile( const T & p_filename )const
			{
				return ( m_files.find( p_filename ) != m_files.end() );
			}

			bool HasDirectory( const T & p_filename )const
			{
				return ( m_directories.find( p_filename ) != m_directories.end() );
			}

			ZipFileBase <T> * GetFile( const T & p_name )const
			{
				return General::Utils::map::findOrNull( m_files, p_name );
			}

			ZipDirectoryBase <T> * GetDirectory( const T & p_name )const
			{
				return General::Utils::map::findOrNull( m_directories, p_name );
			}

			ZipFileArrayType RecurseFindFile( const T & p_name )
			{
				return m_rootDirectory->RecurseFindFile( p_name );
			}

		public:
			static const char * GetErrorDesc( zzip_error_t p_errorCode )
			{
				switch ( p_errorCode )
				{
				case ZZIP_NO_ERROR:
				{
					return "No error";
				}

				case ZZIP_OUTOFMEM:
				{
					return "Out of memory.";
				}

				case ZZIP_DIR_OPEN:
				{
					return "Unable to read zip file (open).";
				}

				case ZZIP_DIR_STAT:
				{
					return "Unable to read zip file (stat).";
				}

				case ZZIP_DIR_SEEK:
				{
					return "Unable to read zip file (seek).";
				}

				case ZZIP_DIR_READ:
				{
					return "Unable to read zip file (read).";
				}

				case ZZIP_UNSUPP_COMPR:
				{
					return "Unsupported compression format.";
				}

				case ZZIP_CORRUPTED:
				{
					return "Corrupted archive.";
				}

				case ZZIP_DIR_EDH_MISSING:
				{
					return "zip-file central directory not found";
				}

				default:
				{
					return "Unknown error. code";
				}
				}
			}

		public:
			inline const T & GetName()const
			{
				return m_basename;
			}
			inline const T & GetBasePath()const
			{
				return m_path;
			}
			inline const T & GetFullPath()const
			{
				return m_fullPath;
			}
			inline zzip_dir * GetArchive()const
			{
				return m_internalArchive;
			}
		};

		template <>
		class d_dll ZipArchiveBase <std::string> : public ZipArchiveImpl <std::string>
		{
		protected:
			bool _load();

			void _checkPath( std::string & p_path )
			{
				//if (p_path.empty() || p_path[p_path.size() - 1] != '/')
				{
					p_path.push_back( '/' );
				}
			}

		public:
			ZipArchiveBase( const std::string & p_name, const std::string & p_path )
				:	ZipArchiveImpl <std::string> ( p_name, p_path )
			{}

		public:
			bool Extract( const std::string & p_newPath )
			{
				if ( ! _load() )
				{
					d_coucou;
					return false;
				}

				std::string l_path = p_newPath;
				_checkPath( l_path );
				return m_rootDirectory->Extract( l_path, this );
			}
		};

#if GENLIB_WINDOWS

		template <>
		class d_dll ZipArchiveBase <std::wstring> : public ZipArchiveImpl <std::wstring>
		{
		protected:
			bool _load();

		public:
			ZipArchiveBase( const std::wstring & p_name, const std::wstring & p_path )
				:	ZipArchiveImpl <std::wstring> ( p_name, p_path )
			{}

		public:
			bool Extract( const std::wstring & p_newPath )
			{
				if ( ! _load() )
				{
					return false;
				}

				return m_rootDirectory->Extract( p_newPath, this );
			}
		};

#endif

	}
}

#endif
