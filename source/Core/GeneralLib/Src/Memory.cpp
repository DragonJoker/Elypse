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
#include <fstream>
#include <time.h>
#include <iostream>
#include <string>

#include "Memory.h"

#ifndef ____GENLIB_NO_MEMORY_DEBUG____

#pragma warning( disable:4996 )

#undef new
#undef delete

using namespace General::Utils;

uint32_t MemoryManager::sm_initialised = 0;

MemoryManager::MemoryManager( char const * const p_logLocation )
	: m_locked{ false }
	, m_currentMemoryAllocated{ 0 }
	, m_maximumMemoryAllocated{ 0 }
	, m_totalObjectsAllocated{ 0 }
	, m_totalArraysAllocated{ 0 }
	, m_totalMemoryAllocated{ 0 }
	, m_logLocation{ p_logLocation }
{
	sm_initialised = 1;
}

MemoryManager::~MemoryManager()
{
	sm_initialised = 0;
	doFinalReport();
}

MemoryManager & MemoryManager::operator<<( MemoryBlock const & p_block )
{
	m_lastBlock = p_block;
	return * this;
}

void MemoryManager::AddLocation( size_t p_size, void * p_pointer, bool p_typeArray )
{
	RecordAllocation( p_size );

	if ( p_typeArray )
	{
		m_totalArraysAllocated++;
	}
	else
	{
		m_totalObjectsAllocated++;
	}

	m_memoryMap.insert( { p_pointer, MemoryBlock{ p_pointer, p_size, p_typeArray } } );
}

bool MemoryManager::RemoveLocation( void * p_pointer, bool p_isArray )
{
	auto ifind = m_memoryMap.find( p_pointer );

	if ( ifind != m_memoryMap.end() )
	{
		RecordDeallocation( ifind->second.size );
		m_memoryMap.erase( ifind );
		return true;
	}

	//m_failedDeletes.push_back( MemoryBlock( m_tempFileName, m_tempFuncName, m_tempLineNum, 0, p_isArray));
	return false;
}

void MemoryManager::FailedAlloc( size_t p_size, bool p_isArray )
{
	//m_failedNews.push_back( MemoryBlock( m_tempFileName, m_tempFuncName, m_tempLineNum, p_size, p_isArray));
}

void MemoryManager::RecordAllocation( size_t p_size )
{
	m_currentMemoryAllocated += p_size;
	m_totalMemoryAllocated += p_size;

	if ( m_currentMemoryAllocated > m_maximumMemoryAllocated )
	{
		m_maximumMemoryAllocated = m_currentMemoryAllocated;
	}
}

void MemoryManager::RecordDeallocation( size_t p_size )
{
	m_currentMemoryAllocated -= p_size;
}

void MemoryManager::MemoryLeaksReport( std::string const & p_filename )
{
	MemoryManager::Lock();
	std::ostream * out{ nullptr };
	std::ofstream * fout{ nullptr };

	if ( !p_filename.empty() )
	{
		fout = new std::ofstream;
		fout->open( p_filename.c_str() );

		if ( fout->bad() || fout->eof() )
		{
			return;
		}

		out = fout;
	}
	else
	{
		out = &std::cerr;
	}

	time_t rawtime{ 0 };
	tm * ti{ nullptr };
	time( &rawtime );
	ti = localtime( &rawtime );
	*out << "Memoryleak.log : " << ti->tm_mday << "/" << ( ti->tm_mon + 1 ) << "/" << ( ti->tm_year + 1900 ) << " @ " << ti->tm_hour << ":" << ti->tm_min << ":" << ti->tm_sec << std::endl << std::endl;
	*out << "Total memory leaked : " << m_currentMemoryAllocated << " bytes" << std::endl;
	*out << "Maximum memory usage : " << m_maximumMemoryAllocated << " bytes" << std::endl;
	*out << "Total memory allocated : " << m_totalMemoryAllocated << " bytes on " << m_totalObjectsAllocated << " objects and " << m_totalArraysAllocated << " arrays" << std::endl;
	*out << std::endl;

	if ( !m_memoryMap.empty() )
	{
		for ( auto l_it : m_memoryMap )
		{
			*out << ( l_it.second.isArray ? "Array" : "Object" ) << " leaked : " << l_it.second.size << " bytes, created in " << l_it.second.function << "() , line " << l_it.second.line << " of file " << l_it.second.file << std::endl;
		}
	}
	else
	{
		*out << "No memory leaked" << std::endl;
	}

	*out << std::endl;

	if ( !m_failedDeletes.empty() )
	{
		for ( auto l_it : m_failedDeletes )
		{
			*out << "Deletion of an invalid " << ( l_it.isArray ? "array" : "object" ) << " pointer @ " << l_it.function << "() , line " << l_it.line << " of file " << l_it.file << std::endl;
		}
	}
	else
	{
		*out << "No invalid pointers" << std::endl;
	}

	*out << std::endl;

	if ( ! m_failedNews.empty() )
	{
		for ( auto l_it : m_failedNews )
		{
			*out << "Failed allocation of an " << ( l_it.isArray ? "array" : "object" ) << " pointer @ " << l_it.function << "() , line " << l_it.line << " of file " << l_it.file << std::endl;
		}
	}
	else
	{
		*out << "No failed allocation" << std::endl;
	}

	if ( !p_filename.empty() )
	{
		fout->close();
		delete fout;
	}

	MemoryManager::Unlock();
}

void MemoryManager::doFinalReport()
{
	FILE * l_file = fopen( m_logLocation, "w" );

	if ( l_file )
	{
		time_t rawtime{ 0 };
		tm * ti{ nullptr };
		time( &rawtime );
		ti = localtime( &rawtime );
		ti->tm_year += 1900;
		ti->tm_mon++;
		fprintf( l_file, "Memoryleak.log : %.2d/%.2d/%.4d @ %.2d:%.2d:%.2d\n", ti->tm_mday, ti->tm_mon, ti->tm_year, ti->tm_hour, ti->tm_min, ti->tm_sec );
		fprintf( l_file, "Total memory leaked : %lld bytes\n", m_currentMemoryAllocated );
		fprintf( l_file, "Maximum memory usage : %lld bytes\n", m_maximumMemoryAllocated );
		fprintf( l_file, "Total memory allocated : %lld bytes on %lld objects and %lld arrays\n", m_totalMemoryAllocated, m_totalObjectsAllocated, m_totalArraysAllocated );

		if ( !m_memoryMap.empty() )
		{
			for ( auto l_it : m_memoryMap )
			{
				if ( !l_it.second.file )
				{
					fprintf( l_file, "Outside object leaked : %lld bytes\n", l_it.second.size );
				}
				else
				{
					fprintf( l_file, "%s (%p)leaked : %lld bytes, created in %s(), line %d of file %s\n", ( l_it.second.isArray ? "Array" : "Object" ), &l_it.second.ptr, l_it.second.size, l_it.second.function, l_it.second.line, l_it.second.file );
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

void MemoryManager::doLocalise( void * p_ptr )
{
	auto ifind = m_memoryMap.find( p_ptr );

	if ( ifind != m_memoryMap.end() )
	{
		ifind->second = m_lastBlock;
	}

	m_lastBlock.Clear();
}

#endif
