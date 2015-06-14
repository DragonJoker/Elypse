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
#ifndef ___GENERAL_FUNCTION_TRACKER___
#define ___GENERAL_FUNCTION_TRACKER___

#include <iostream>

#define d_track General::Utils::FunctionTracker l_tracker ## __LINE__ ## __COUNTER__( __FUNCTION__, __FILE__, __LINE__)

namespace General
{
	namespace Utils
	{
		class FunctionTracker
		{
		public:
			FunctionTracker( const char * p_function, const char * p_file, unsigned int p_line )
				: m_function( p_function )
				, m_file( p_file )
				, m_line( p_line )
			{
				std::cout << "Entered Function : " << m_function << " in " << m_file << ", line " << m_line << std::endl;
			}
			~FunctionTracker()
			{
				std::cout << "Exited Function : " << m_function << " in " << m_file << ", line " << m_line << std::endl;
			}

		private:
			const char * m_file;
			const char * m_function;
			const unsigned int m_line;
		};
	}
}

#endif
