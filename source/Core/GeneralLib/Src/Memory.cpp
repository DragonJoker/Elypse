#include <fstream>
#include <time.h>
#include <iostream>
#include <string>

#include "Memory.h"

#pragma warning( disable:4996 )

#undef new
#undef delete

using namespace General::Utils;

unsigned int MemoryManager :: sm_initialised = 0;

MemoryManager :: MemoryManager( const char * p_logLocation )
	:	m_locked( false ),
		m_currentMemoryAllocated( 0 ),
		m_maximumMemoryAllocated( 0 ),
		m_totalObjectsAllocated( 0 ),
		m_totalArraysAllocated( 0 ),
		m_totalMemoryAllocated( 0 ),
		m_logLocation( p_logLocation )
{
	sm_initialised = 1;
}

MemoryManager :: ~MemoryManager()
{
	sm_initialised = 0;
	_finalReport();
}

MemoryManager & MemoryManager :: operator <<( const MemoryBlock & p_block )
{
	m_lastBlock = p_block;
	return * this;
}

void MemoryManager :: _localise( void * p_ptr )
{
	const MemoryBlockMap::iterator & ifind = m_memoryMap.find( p_ptr );

	if ( ifind != m_memoryMap.end() )
	{
		ifind->second = m_lastBlock;
	}

	m_lastBlock.Clear();
}

void MemoryManager :: AddLocation( size_t p_size, void * p_pointer, bool p_typeArray )
{
	RecordAllocation( p_size );

	if ( p_typeArray )
	{
		m_totalArraysAllocated ++;
	}
	else
	{
		m_totalObjectsAllocated ++;
	}

	m_memoryMap.insert( MemoryBlockMap::value_type( p_pointer, MemoryBlock( p_pointer, p_size, p_typeArray ) ) );
}

bool MemoryManager :: RemoveLocation( void * p_pointer, bool p_isArray )
{
	const MemoryBlockMap::iterator & ifind = m_memoryMap.find( p_pointer );

	if ( ifind != m_memoryMap.end() )
	{
		RecordDeallocation( ifind->second.size );
		m_memoryMap.erase( ifind );
		return true;
	}

//	m_failedDeletes.push_back( MemoryBlock( m_tempFileName, m_tempFuncName, m_tempLineNum, 0, p_isArray));
	return false;
}

void MemoryManager :: FailedAlloc( size_t p_size, bool p_isArray )
{
//	m_failedNews.push_back( MemoryBlock( m_tempFileName, m_tempFuncName, m_tempLineNum, p_size, p_isArray));
}

void MemoryManager :: RecordAllocation( size_t p_size )
{
	m_currentMemoryAllocated += p_size;
	m_totalMemoryAllocated += p_size;

	if ( m_currentMemoryAllocated > m_maximumMemoryAllocated )
	{
		m_maximumMemoryAllocated = m_currentMemoryAllocated;
	}
}

void MemoryManager :: RecordDeallocation( size_t p_size )
{
	m_currentMemoryAllocated -= p_size;
}

void MemoryManager :: _finalReport()
{
	FILE * l_file = fopen( m_logLocation, "w" );

	if ( l_file != NULL )
	{
		time_t rawtime;
		tm * ti;
		time( & rawtime );
		ti = localtime( & rawtime );
		ti->tm_year += 1900;
		ti->tm_mon ++;
		fprintf( l_file, "Memoryleak.log : %.2d/%.2d/%.4d @ %.2d:%.2d:%.2d\n", ti->tm_mday, ti->tm_mon, ti->tm_year, ti->tm_hour, ti->tm_min, ti->tm_sec );
		fprintf( l_file, "Total memory leaked : %d bytes\n", m_currentMemoryAllocated );
		fprintf( l_file, "Maximum memory usage : %d bytes\n", m_maximumMemoryAllocated );
		fprintf( l_file, "Total memory allocated : %lld bytes on %d objects and %d arrays\n", m_totalMemoryAllocated, m_totalObjectsAllocated, m_totalArraysAllocated );

		if ( ! m_memoryMap.empty() )
		{
			MemoryBlockMap::iterator iter = m_memoryMap.begin();
			const MemoryBlockMap::iterator & iend = m_memoryMap.end();

			for ( ; iter != iend ; ++ iter )
			{
				const MemoryBlock & l_block = iter->second;

				if ( l_block.file == NULL )
				{
					fprintf( l_file, "Outside object leaked : %d bytes\n", l_block.size );
				}
				else
				{
					fprintf( l_file, "%s (%p)leaked : %d bytes, created in %s(), line %d of file %s\n", ( l_block.isArray ? "Array" : "Object" ), &l_block.ptr, l_block.size, l_block.function, l_block.line, l_block.file );
				}
			}
		}
		else
		{
			fprintf( l_file, "No memory leaks\n" );
		}

		fclose( l_file );
	}
}

void MemoryManager :: MemoryLeaksReport( const std::string & p_filename )
{
	MemoryManager::Lock();
	std::ostream  * out;
	std::ofstream * fout;

	if ( ! p_filename.empty() )
	{
		fout = new std::ofstream;

		if ( fout == NULL )
		{
			return;
		}

		fout->open( p_filename.c_str() );

		if ( fout->bad() || fout->eof() )
		{
			return;
		}

		out = fout;
	}
	else
	{
		out = & std::cerr;
	}

	time_t rawtime;
	tm * ti;
	time( & rawtime );
	ti = localtime( & rawtime );
	* out << "Memoryleak.log : " << ti->tm_mday << "/" << ( ti->tm_mon + 1 ) << "/" << ( ti->tm_year + 1900 ) << " @ " << ti->tm_hour << ":" << ti->tm_min << ":" << ti->tm_sec << std::endl << std::endl;
	* out << "Total memory leaked : " << m_currentMemoryAllocated << " bytes" << std::endl;
	* out << "Maximum memory usage : " << m_maximumMemoryAllocated << " bytes" << std::endl;
	* out << "Total memory allocated : " << m_totalMemoryAllocated << " bytes on " << m_totalObjectsAllocated << " objects and " << m_totalArraysAllocated << " arrays" << std::endl;
	* out << std::endl;

	if ( ! m_memoryMap.empty() )
	{
		MemoryBlockMap::iterator iter = m_memoryMap.begin();
		const MemoryBlockMap::iterator & iend = m_memoryMap.end();

		for ( ; iter != iend ; ++ iter )
		{
			const MemoryBlock & l_block = iter->second;
			* out << ( l_block.isArray ? "Array" : "Object" ) << " leaked : " << l_block.size << " bytes, created in " << l_block.function << "() , line " << l_block.line << " of file " << l_block.file << std::endl;
		}
	}
	else
	{
		* out << "No memory leaked" << std::endl;
	}

	* out << std::endl;

	if ( ! m_failedDeletes.empty() )
	{
		size_t imax = m_failedDeletes.size();

		for ( size_t i = 0 ; i < imax ; i++ )
		{
			const MemoryBlock & l_block = m_failedDeletes[i];
			* out << "Deletion of an invalid " << ( l_block.isArray ? "array" : "object" ) << " pointer @ " << l_block.function << "() , line " << l_block.line << " of file " << l_block.file << std::endl;
		}
	}
	else
	{
		* out << "No invalid pointers" << std::endl;
	}

	* out << std::endl;

	if ( ! m_failedNews.empty() )
	{
		size_t imax = m_failedNews.size();

		for ( size_t i = 0 ; i < imax ; i++ )
		{
			const MemoryBlock & l_block = m_failedNews[i];
			* out << "Failed allocation of an " << ( l_block.isArray ? "array" : "object" ) << " pointer @ " << l_block.function << "() , line " << l_block.line << " of file " << l_block.file << std::endl;
		}
	}
	else
	{
		* out << "No failed allocation" << std::endl;
	}

	if ( ! p_filename.empty() )
	{
		fout->close();
		delete fout;
	}

	MemoryManager::Unlock();
}