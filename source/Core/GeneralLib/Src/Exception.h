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
			GenException(	const std::string & p_description, const char * p_file,
							const char * p_function, unsigned int p_line )
				: m_line( p_line )
				, m_description( p_description )
				, m_filename( p_file )
				, m_functionName( p_function )
			{
			}
			virtual __declspec( nothrow ) ~GenException()
			{
			}

		public:
			inline virtual const char * what() const
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
