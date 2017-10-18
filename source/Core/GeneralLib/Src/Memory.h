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
#ifndef ___MEMORY_MANAGER_H___
#define ___MEMORY_MANAGER_H___

#include "Config.h"

#if VLD_AVAILABLE
#	define ____GENLIB_NO_MEMORY_DEBUG____
#endif

#ifndef GENLIB_LOG_LOCATION
#	if GENLIB_WINDOWS
#		define GENLIB_LOG_LOCATION "c:\\memoryleaks.log"
#	else
#		define GENLIB_LOG_LOCATION "~/memoryleaks.log"
#	endif
#endif

#include <map>
#include <vector>

#include "Mutex.h"
#include "AutoSingleton.h"

//NO one touches my defines
#ifdef NEW
#	undef NEW
#endif

#ifdef NEW_ARRAY
#	undef NEW_ARRAY
#endif

#ifdef DELETE
#	undef DELETE
#endif

#ifdef DELETE_ARRAY
#	undef DELETE_ARRAY
#endif

#ifdef new
#	undef new
#endif
/*
#define NEW(X) new X
#define NEW_ARRAY( X, Y) new X[Y]
#define DELETE( X) delete X
#define DELETE_ARRAY( X) delete [] X
*/
#ifndef ____GENLIB_NO_MEMORY_DEBUG____

#	define MEMORY_MANAGER_START() General::Utils::MemoryManager::GetSingleton()

/*
#	if defined(_MT) || defined(__MT__)
#		pragma comment( lib, "GeneralMemory_mt.lib")
#		pragma comment( lib, "libcpmt.lib")
#	else
#		pragma comment( lib, "GeneralMemory_md.lib")
#		pragma comment( lib, "msvcprt.lib")
#	endif
*/

#include "Memory_Impl.h"
/*
	void operator delete[]( void * p_pointer);
	void operator delete( void * p_pointer);
	void * operator new[]( size_t p_size);
	void * operator new( size_t p_size);
*/
namespace General
{
	namespace Utils
	{
		class MemoryBlock
		{
		public:
			MemoryBlock()
				: file{ nullptr }
				, function{ nullptr }
				, line{ 0 }
				, ptr{ nullptr }
				, size{ 0 }
				, isArray{ false }
			{
			}

			MemoryBlock( char const * p_file, char const * p_function, uint32_t p_line )
				: file{ p_file }
				, function{ p_function }
				, line{ p_line }
				, ptr{ nullptr }
				, size{ 0 }
				, isArray{ false }
			{
			}

			MemoryBlock( void * p_ptr, size_t p_size, bool p_array )
				: file{ nullptr }
				, function{ nullptr }
				, ptr{ p_ptr }
				, line{ 0 }
				, size{ p_size }
				, isArray{ p_array }
			{
			}

			~MemoryBlock()
			{
			}

			MemoryBlock & operator=( MemoryBlock const & p_other )
			{
				file = p_other.file;
				function = p_other.function;
				line = p_other.line;
				return * this;
			}

			void Clear()
			{
				file = nullptr;
				function = nullptr;
				ptr = nullptr;
				line = 0;
				size = 0;
				isArray = false;
			}

		public:
			char const * file;
			char const * function;
			void * ptr;
			uint32_t line;
			uint64_t size;
			bool isArray;
		};

		class MemoryManager
			: public General::Theory::AutoSingleton< MemoryManager >
		{
		public:
			MemoryManager( char const * const p_logLocation = GENLIB_LOG_LOCATION );
			~MemoryManager();

			void AddLocation( size_t p_size, void * p_pointer, bool p_typeArray );
			bool RemoveLocation( void * p_pointer, bool p_isArray );
			void FailedAlloc( size_t p_size, bool p_isArray );
			void MemoryLeaksReport( std::string const & p_filename = std::string() );
			void RecordAllocation( size_t p_size );
			void RecordDeallocation( size_t p_size );

			MemoryManager & operator<<( MemoryBlock const & p_block );

			template< typename T >
			inline T * operator<<( T * p_ptr )
			{
				doLocalise( p_ptr );
				return p_ptr;
			}

			static inline void Lock()
			{
				GetSingleton().m_locked = true;
			}
			static inline void Unlock()
			{
				GetSingleton().m_locked = false;
			}
			static inline bool IsLocked()
			{
				return GetSingleton().m_locked;
			}
			static inline bool Exists()
			{
				return ( sm_initialised == 1 );
			}

		private:
			void doFinalReport();
			void doLocalise( void * p_ptr );

		private:
			static uint32_t sm_initialised;
			char const * const m_logLocation;

			typedef std::map< void *, MemoryBlock > MemoryBlockMap;
			typedef std::vector< MemoryBlock > MemoryBlockArray;

		public:
			std::recursive_mutex m_mutex;
			MemoryBlock m_lastBlock;
			MemoryBlockMap m_memoryMap;
			MemoryBlockArray m_failedNews;
			MemoryBlockArray m_failedDeletes;

			bool m_locked;
			uint64_t m_currentMemoryAllocated;
			uint64_t m_maximumMemoryAllocated;
			uint64_t m_totalObjectsAllocated;
			uint64_t m_totalArraysAllocated;
			uint64_t m_totalMemoryAllocated;
		};
	}
}

#	define new General::Utils::MemoryManager::GetSingleton() << General::Utils::MemoryBlock( __FILE__, __FUNCTION__, __LINE__) << new

#else

#	define MEMORY_MANAGER_START

#endif //____GENLIB_NO_MEMORY_DEBUG____

#endif	//___MEMORY_MANAGER_H___
