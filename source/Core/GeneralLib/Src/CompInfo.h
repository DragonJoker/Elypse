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
		private:
			unsigned int m_numScreens;
			unsigned int m_mainScreenX;
			unsigned int m_mainScreenY;
			OSVERSIONINFO m_operatingSystem;
			MEMORYSTATUSEX m_memoryStatus;

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
		};
	}
}

#	endif //WIN32 only

#endif
