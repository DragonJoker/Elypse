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
		public:
			DownloadableFile( const std::wstring & p_name, const std::wstring & p_path );
			DownloadableFile( const std::wstring & p_name, const WStringArray & p_multiPath );

			~DownloadableFile();

		public:
			bool ExistsAndIsIdentical( const std::string & p_version );

		public:
			inline std::wstring GetPath()const
			{
				if ( ! m_usesMultiPath )
				{
					return m_path + m_name;
				}

				return m_multiPath[0] + m_name;
			}
			inline const std::wstring & GetBasePath()const
			{
				if ( ! m_usesMultiPath )
				{
					return m_path;
				}

				return m_multiPath[0];
			}
			inline void SetToExec( const std::wstring & p_params )
			{
				m_toExec = true;
				m_executionParams = p_params;
			}
			inline const std::wstring & GetExecParams()const
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
			inline const std::wstring & GetName()const
			{
				return m_name;
			}
			inline const std::string & GetHash()const
			{
				return m_hash;
			}

		private:
			bool _exists();
			void _getHash();

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
		};
	}
}

using General::Files::DownloadableFile;
using General::Files::WStringArray;

#endif
