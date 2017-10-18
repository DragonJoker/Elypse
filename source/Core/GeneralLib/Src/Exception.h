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
		class GenException
			: public std::exception
		{
		public:
			GenException( std::string const & p_description, char const * const p_file, char const * const p_function, uint32_t p_line )
				: m_line{ p_line }
				, m_description{ p_description }
				, m_filename{ p_file }
				, m_functionName{ p_function }
			{
			}
			virtual ~GenException() noexcept
			{
			}

			inline virtual char const * what()const noexcept
			{
				return m_description.c_str();
			}
			inline char const * const GetFilename()const
			{
				return m_filename;
			}
			inline char const * const GetFunction()const
			{
				return m_functionName;
			}
			inline uint32_t GetLine()const
			{
				return m_line;
			}
			inline std::string const & GetDescription()const
			{
				return m_description;
			}

		public:
			uint32_t const m_line;
			std::string const m_description;
			char const * const m_filename;
			char const * const m_functionName;
		};
	}
}

#define GENLIB_EXCEPTION( p_text ) throw General::Utils::GenException( p_text, __FILE__, __FUNCTION__, __LINE__)

#endif
