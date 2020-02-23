/*
See LICENSE file in root folder
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
