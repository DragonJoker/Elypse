/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_COMPILE_EXCEPTION_H___
#define ___ELYPSE_COMPILE_EXCEPTION_H___

#include <Exception.h>

namespace Elypse
{
	namespace Script
	{
		class CompileError
			: public General::Utils::GenException
		{
		public:
			CompileError( std::string const & p_description, char const * const p_file, char const * const p_function, uint32_t p_line )
				: General::Utils::GenException( p_description, p_file, p_function, p_line )
			{
			}

			virtual ~CompileError() noexcept
			{
			}
		};
	}
}

#define COMPILE_EXCEPTION( p_text) throw Elypse::Script::CompileError( p_text, __FILE__, __FUNCTION__, __LINE__)

#endif
