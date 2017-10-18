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
#ifndef ___COMPUTER_INFOS_H___
#define ___COMPUTER_INFOS_H___

#include "Config.h"

#if !GENLIB_WINDOWS
#	error Available only on windows
#else

#	include <Windows.h>
#	include <string>
#	include "StringConverter.h"

namespace General
{
	namespace Computer
	{
		class ComputerInfo
		{
		public:
			ComputerInfo()
			{
				m_operatingSystem.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
				GetVersionEx( &m_operatingSystem );
				m_memoryStatus.dwLength = sizeof( m_memoryStatus );
				ActualiseMemory();
			}

			void ActualiseMemory()
			{
				GlobalMemoryStatusEx( &m_memoryStatus );
			}

			inline uint32_t GetNumScreens()const
			{
				return m_numScreens;
			}
			inline uint32_t GetMainScreenWidth()const
			{
				return m_mainScreenX;
			}
			inline uint32_t GetMainScreenHeight()const
			{
				return m_mainScreenY;
			}
			inline int GetOSMajorVersion()const
			{
				return m_operatingSystem.dwMajorVersion;
			}
			inline int GetOSMinorVersion()const
			{
				return m_operatingSystem.dwMinorVersion;
			}
			inline std::string GetOSSubVersion()const
			{
				return General::Utils::ToString( m_operatingSystem.szCSDVersion );
			}
			inline uint64_t GetTotalPhysicalMemory()const
			{
				return m_memoryStatus.ullTotalPhys >> 10;
			}
			inline uint64_t GetFreePhysicalMemory()const
			{
				return m_memoryStatus.ullAvailPhys >> 10;
			}
			inline uint32_t GetMemoryUsage()const
			{
				return m_memoryStatus.dwMemoryLoad;
			}
			inline bool IsLittleEndian()const
			{
				unsigned char SwapTest[2] { 1, 0 };
				return ( ( *reinterpret_cast< short * >( SwapTest ) ) == 1 );
			}

		private:
			uint32_t m_numScreens{ ::GetSystemMetrics( SM_CMONITORS ) };
			uint32_t m_mainScreenX{ ::GetSystemMetrics( SM_CXSCREEN ) };
			uint32_t m_mainScreenY{ ::GetSystemMetrics( SM_CYSCREEN ) };
			OSVERSIONINFO m_operatingSystem{ 0 };
			MEMORYSTATUSEX m_memoryStatus{ 0 };
		};
	}
}

#	endif //WIN32 only

#endif
