/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_NAMED_ANY_H___
#define ___GENERAL_NAMED_ANY_H___

#include <string>

#include "Assertions.h"

namespace General
{
	namespace Theory
	{
		template< typename T = std::string >
		class namedBase
		{
		public:
			namedBase( T const & p_name )
				: m_name{ p_name }
			{
				genlib_assert( !m_name.empty() );
			}
			~namedBase()
			{
			}

		public:
			inline T const & GetName()const
			{
				return m_name;
			}

		protected:
			T m_name;
		};

		typedef namedBase< std::string > named;
		typedef namedBase< std::wstring > wnamed;
	}
}

#endif
