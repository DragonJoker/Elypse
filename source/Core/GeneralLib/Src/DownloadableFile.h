#ifndef ___DOWNLOADABLE_FILE_H___
#define ___DOWNLOADABLE_FILE_H___

#include <vector>
#include <string>

namespace General
{
	namespace Files
	{
		typedef std::vector <std::wstring> WStringArray;

		class DownloadableFile
		{
		private:
			std::wstring m_name;
			std::wstring m_path;
			std::wstring m_executionParams;
			std::string m_hash;
			bool m_executable;
			bool m_zipped;
			bool m_usesMultiPath;
			bool m_exists;
			bool m_toExec;
			unsigned int m_size;

		public:
			WStringArray m_multiPath;

		private:
			bool _exists();
			void _getHash();

		public:
			DownloadableFile( const std::wstring & p_name, const std::wstring & p_path );
			DownloadableFile( const std::wstring & p_name, const WStringArray & p_multiPath );

			~DownloadableFile();

		public:
			bool ExistsAndIsIdentical( const std::string & p_version );

		public:
			inline std::wstring			GetPath()const
			{
				if ( ! m_usesMultiPath )
				{
					return m_path + m_name;
				}

				return m_multiPath[0] + m_name;
			}
			inline const std::wstring &	GetBasePath()const
			{
				if ( ! m_usesMultiPath )
				{
					return m_path;
				}

				return m_multiPath[0];
			}
			inline void					SetToExec( const std::wstring & p_params )
			{
				m_toExec = true;
				m_executionParams = p_params;
			}
			inline const std::wstring &	GetExecParams()const
			{
				return m_executionParams;
			}
			inline bool					IsMultiPath()const
			{
				return m_usesMultiPath;
			}
			inline bool					IsZipped()const
			{
				return m_zipped;
			}
			inline bool					GetToExec()const
			{
				return m_toExec;
			}
			inline const std::wstring &	GetName()const
			{
				return m_name;
			}
			inline const std::string & GetHash()const
			{
				return m_hash;
			}
		};

	}
}

using General::Files::DownloadableFile;
using General::Files::WStringArray;

#endif
