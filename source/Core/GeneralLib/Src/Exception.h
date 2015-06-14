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
#ifndef ___EMUSE_EXCEPTION_H___
#define ___EMUSE_EXCEPTION_H___

#include <exception>
#include <string>

#include "Macros.h"

namespace General
{
	namespace Utils
	{
		class GenException : public std::exception
		{
		public:
			GenException( const std::string & p_description, const char * p_file,
							const char * p_function, unsigned int p_line )
				: m_line( p_line )
				, m_description( p_description )
				, m_filename( p_file )
				, m_functionName( p_function )
			{
			}
			virtual ~GenException() d_no_throw
			{
			}

		public:
			inline virtual const char * what() const d_no_throw
			{
				return m_description.c_str();
			}
			inline const char * GetFilename()const
			{
				return m_filename;
			}
			inline const char * GetFunction()const
			{
				return m_functionName;
			}
			inline unsigned int GetLine()const
			{
				return m_line;
			}
			inline const std::string & GetDescription()const
			{
				return m_description;
			}

		public:
			unsigned int m_line;
			std::string m_description;
			const char * m_filename;
			const char * m_functionName;
			const char * m_typeDesc;
		};
	}
}

#define GENLIB_EXCEPTION( p_text) throw General::Utils::GenException( p_text, __FILE__, __FUNCTION__, __LINE__)

#endif
