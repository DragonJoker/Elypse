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

#if ! GENLIB_WINDOWS
#	error Available only on windows
#else

#	include <windows.h>
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
				m_numScreens = static_cast< int >( GetSystemMetrics( SM_CMONITORS ) );
				m_mainScreenX = static_cast< int >( GetSystemMetrics( SM_CXSCREEN ) );
				m_mainScreenY = static_cast< int >( GetSystemMetrics( SM_CYSCREEN ) );
				ZeroMemory( & m_operatingSystem, sizeof( OSVERSIONINFO ) );
				m_operatingSystem.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
				GetVersionEx( & m_operatingSystem );
				m_memoryStatus.dwLength = sizeof( m_memoryStatus );
				ActualiseMemory();
			}

			~ComputerInfo() {}

		public:
			void ActualiseMemory()
			{
				GlobalMemoryStatusEx( & m_memoryStatus );
			}

		public:
			inline unsigned int GetNumScreens()const
			{
				return m_numScreens;
			}
			inline unsigned int GetMainScreenWidth()const
			{
				return m_mainScreenX;
			}
			inline unsigned int GetMainScreenHeight()const
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
			inline unsigned long long GetTotalPhysicalMemory()const
			{
				return m_memoryStatus.ullTotalPhys >> 10;
			}
			inline unsigned long long GetFreePhysicalMemory()const
			{
				return m_memoryStatus.ullAvailPhys >> 10;
			}
			inline unsigned int GetMemoryUsage()const
			{
				return m_memoryStatus.dwMemoryLoad;
			}
			inline bool IsLittleEndian()const
			{
				unsigned char SwapTest[2] = { 1, 0 };
				return ( *( short * ) SwapTest == 1 );
			}

		private:
			unsigned int m_numScreens;
			unsigned int m_mainScreenX;
			unsigned int m_mainScreenY;
			OSVERSIONINFO m_operatingSystem;
			MEMORYSTATUSEX m_memoryStatus;
		};
	}
}

#	endif //WIN32 only

#endif
