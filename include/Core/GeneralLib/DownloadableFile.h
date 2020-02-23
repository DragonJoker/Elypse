/*
See LICENSE file in root folder
*/
#ifndef ___DOWNLOADABLE_FILE_H___
#define ___DOWNLOADABLE_FILE_H___

#include <vector>
#include <string>

namespace General
{
	namespace Files
	{
		using WStringArray = std::vector< std::wstring >;

		class DownloadableFile
		{
		public:
			DownloadableFile( std::wstring const & p_name, std::wstring const & p_path );
			DownloadableFile( std::wstring const & p_name, WStringArray const & p_multiPath );

			~DownloadableFile();

			bool ExistsAndIsIdentical( std::string const & p_version );

			inline std::wstring GetPath()const
			{
				if ( !m_usesMultiPath )
				{
					return m_path + m_name;
				}

				return m_multiPath[0] + m_name;
			}
			inline std::wstring const & GetBasePath()const
			{
				if ( !m_usesMultiPath )
				{
					return m_path;
				}

				return m_multiPath[0];
			}
			inline void SetToExec( std::wstring const & p_params )
			{
				m_toExec = true;
				m_executionParams = p_params;
			}
			inline std::wstring const & GetExecParams()const
			{
				return m_executionParams;
			}
			inline bool IsMultiPath()const
			{
				return m_usesMultiPath;
			}
			inline bool IsZipped()const
			{
				return m_zipped;
			}
			inline bool GetToExec()const
			{
				return m_toExec;
			}
			inline std::wstring const & GetName()const
			{
				return m_name;
			}
			inline std::string const & GetHash()const
			{
				return m_hash;
			}

		private:
			bool doExists();
			void doGetHash();

		private:
			std::wstring m_name;
			std::wstring m_path;
			std::wstring m_executionParams;
			std::string m_hash;
			bool m_executable{ false };
			bool m_zipped{ false };
			bool m_usesMultiPath{ false };
			bool m_exists{ false };
			bool m_toExec{ false };
			uint32_t m_size{ 0 };
			WStringArray m_multiPath;
		};
	}
}

using General::Files::DownloadableFile;
using General::Files::WStringArray;

#endif
