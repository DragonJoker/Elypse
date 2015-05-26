#ifndef ___ZIP_DIRECTORY_H___
#define ___ZIP_DIRECTORY_H___

#include "Zip.h"
#include "ZipFile.h"
#include "STLMapMacros.h"
#include "File.h"
#include "Converter.h"

namespace General
{
	namespace Files
	{
		template <typename T>
		class ZipDirectoryBase
		{
		private:
			typedef std::map	<T, ZipDirectoryBase <T> *>	ZipDirectoryMapType;
			typedef std::map	<T, ZipFileBase <T> *>	ZipFileMapType;

		private:
			T m_name;
			T m_fullpath;
			ZipDirectoryMapType m_subDirectories;
			ZipFileMapType m_files;
			ZipDirectoryBase <T> * m_parent;
			unsigned int m_uncompressedSize;
			unsigned int m_compressedSize;

		public:
			ZipDirectoryBase( const T & p_name, ZipDirectoryBase <T> * p_parent )
				:	m_name( p_name ),
					m_parent( p_parent ),
					m_uncompressedSize( 0 ),
					m_compressedSize( 0 )
			{
				if ( m_parent )
				{
					m_fullpath = m_parent->GetPath() + m_name + General::Utils::ConvertTo <T> ::From( "/" );
				}
			}

			~ZipDirectoryBase()
			{
				General::Utils::map::deleteAll( m_subDirectories );
				General::Utils::map::deleteAll( m_files );
			}

		public:
			bool Extract( const T & p_newPath, ZipArchiveBase <T> * p_archive )
			{
				if ( ! DirectoryExists( p_newPath + m_fullpath ) )
				{
					DirectoryCreate( p_newPath + m_fullpath );
				}

				{
					typename ZipDirectoryMapType::iterator i = m_subDirectories.begin();
					const typename ZipDirectoryMapType::iterator & iend = m_subDirectories.end();

					for ( ; i != iend ; ++ i )
					{
						if ( ! i->second->Extract( p_newPath, p_archive ) )
						{
							return false;
						}
					}
				}

				typename ZipFileMapType::iterator i = m_files.begin();
				const typename ZipFileMapType::iterator & iend = m_files.end();

				for ( ; i != iend ; ++ i )
				{
					if ( ! i->second->Extract( p_newPath, p_archive ) )
					{
						return false;
					}
				}

				return true;
			}

			ZipFileBase <T> * AddFile( const T & p_filename, const T & p_fullpath )
			{
				const typename ZipFileMapType::iterator & ifind = m_files.find( p_filename );

				if ( ifind != m_files.end() )
				{
					return ifind->second;
				}

				ZipFileBase <T> * l_file = new ZipFileBase <T> ( p_filename, p_fullpath, this );
				m_files.insert( typename ZipFileMapType::value_type( p_filename, l_file ) );
				return l_file;
			}

			ZipDirectoryBase <T> * AddDirectory( const T & p_name )
			{
				const typename ZipDirectoryMapType::iterator & ifind = m_subDirectories.find( p_name );

				if ( ifind != m_subDirectories.end() )
				{
					return ifind->second;
				}

				ZipDirectoryBase <T> * l_directory = new ZipDirectoryBase <T> ( p_name, this );
				m_subDirectories.insert( typename ZipDirectoryMapType::value_type( p_name, l_directory ) );
				return l_directory;
			}

			ZipFileArray RecurseFindFile( const T & p_name, const T & p_currentDir = T() )
			{
				T l_currentDir = p_currentDir + "/" + m_name;
				ZipFileArray l_array;
				{
					typename ZipDirectoryMapType::iterator i = m_subDirectories.begin();
					const typename ZipDirectoryMapType::iterator & iend = m_subDirectories.end();

					for ( ; i != iend ; ++ i )
					{
						const ZipFileArray & l_tempArray = i->second->RecurseFindFile( p_name, l_currentDir );

						if ( ! l_tempArray.empty() )
						{
							l_array.insert( l_array.end(), l_tempArray.begin(), l_tempArray.end() );
						}
					}
				}
				const typename ZipFileMapType :: iterator & ifind = m_files.find( p_name );

				if ( ifind != m_files.end() )
				{
					l_array.push_back( ifind->second );
				}

				return l_array;
			}

			void CheckDirectorySize()
			{
				m_uncompressedSize = 0;
				m_compressedSize = 0;
				{
					typename ZipDirectoryMapType::iterator i = m_subDirectories.begin();
					const typename ZipDirectoryMapType::iterator & iend = m_subDirectories.end();

					for ( ; i != iend ; ++ i )
					{
						i->second->CheckDirectorySize();
						m_uncompressedSize += i->second->GetUncompressedSize();
						m_compressedSize += i->second->GetCompressedSize();
					}
				}
				typename ZipFileMapType::iterator i = m_files.begin();
				const typename ZipFileMapType::iterator & iend = m_files.end();

				for ( ; i != iend ; ++ i )
				{
					m_uncompressedSize += i->second->GetUncompressedSize();
					m_compressedSize += i->second->GetCompressedSize();
				}
			}

		public:
			inline const T & GetName()const
			{
				return m_name;
			}
			inline unsigned int GetCompressedSize()const
			{
				return m_compressedSize;
			}
			inline unsigned int GetUncompressedSize()const
			{
				return m_uncompressedSize;
			}
			inline const T & GetPath()const
			{
				return m_fullpath;
			}
		};
	}
}

#endif
