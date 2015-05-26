#ifndef ___ZIP_FILE_H___
#define ___ZIP_FILE_H___

#include "Zip.h"

#include "Macros.h"

namespace General
{
	namespace Files
	{
		template <typename T>
		class ZipFileImpl
		{
		protected:
			T m_name;
			T m_fullname;

			ZipDirectoryBase <T> * m_directory;

			unsigned int m_uncompressedSize;
			unsigned int m_compressedSize;

		public:
			ZipFileImpl( const T & p_name, const T & p_fullname, ZipDirectoryBase <T> * p_directory )
				:	m_name( p_name ),
					m_fullname( p_fullname ),
					m_directory( p_directory ),
					m_uncompressedSize( 0 ),
					m_compressedSize( 0 )
			{}
			void SetSize( unsigned int p_compSize, unsigned int p_uncompSize )
			{
				m_uncompressedSize = p_uncompSize;
				m_compressedSize = p_compSize;
			}

		public:
			inline const T & GetName()const
			{
				return m_name;
			}
			inline const T & GetFullName()const
			{
				return m_fullname;
			}
			inline unsigned int GetCompressedSize()const
			{
				return m_compressedSize;
			}
			inline unsigned int GetUncompressedSize()const
			{
				return m_uncompressedSize;
			}
		};

		template <typename T>
		class d_dll ZipFileBase
		{
		};

		template <>
		class d_dll ZipFileBase <std::string> : public ZipFileImpl <std::string>
		{
		public:
			ZipFileBase( const std::string & p_name, const std::string & p_fullname, ZipDirectoryBase <std::string> * p_directory )
				: ZipFileImpl <std::string> ( p_name, p_fullname, p_directory )
			{
			}
		public:
			bool Extract( const std::string & p_newPath, ZipArchiveBase <std::string> * p_archive );
		};

#if GENLIB_WINDOWS

		template <>
		class d_dll ZipFileBase <std::wstring> : public ZipFileImpl <std::wstring>
		{
		public:
			ZipFileBase( const std::wstring & p_name, const std::wstring & p_fullname, ZipDirectoryBase <std::wstring> * p_directory )
				: ZipFileImpl <std::wstring> ( p_name, p_fullname, p_directory )
			{
			}
		public:
			bool Extract( const std::wstring & p_newPath, ZipArchiveBase <std::wstring> * p_archive );
		};

#endif

	}
}

#endif
