#ifndef ___CONVERTER_H___
#define ___CONVERTER_H___

#include "StringConverter.h"

namespace General
{
	namespace Utils
	{
		template <typename T>
		class ConvertTo
		{
		};

		template <>
		class ConvertTo <std::string>
		{
		public:
			template <typename T>
			static std::string From( const T & p_value )
			{
				return ToString( p_value );
			}
		};

		template <>
		class ConvertTo <std::wstring>
		{
		public:
			template <typename T>
			static std::wstring From( const T & p_value )
			{
				return ToWString( p_value );
			}
		};
	}
}

#endif
