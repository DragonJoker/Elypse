#ifndef ___CLASS_NAME_H___
#define ___CLASS_NAME_H___

#if 0
#include "EMuse.h"

using namespace EMuse;

using std::min;
using std::max;

namespace Program
{
	namespace Module
	{
		class ClassName;

		typedef std::map <String, ClassName *>	ClassNameMap;
		typedef std::map <int, ClassName *>		ClassNameIntMap;
		typedef std::map <Real, ClassName *>	ClassNameRealMap;

		typedef std::vector	<ClassName *>	ClassNameArray;
		typedef std::vector	<int *>			IntPtrArray;
		typedef std::vector	<Real *>		RealPtrArray;

		enum TypeMachin
		{
			TM_BIDULE_ONE	= 0x01,
			TM_BIDULE_TWO	= 0x02,
			TM_BIDULE_THREE	= 0x03,
			TM_BIDULE_FOUR	= 0x04
		};

		struct BlahStructure
		{
			int membre;
			BlahStructure() : membre( 0 ) {}
		};

		class ClassName
		{
		private:
			static int sm_staticMember;
		private:
			int m_int;
			String m_name;

		public:
			ClassName();
			~ClassName();

		private:
			void _privateFunc()const {}

		public:
			void Print( const String & p_string, int p_int, const char * p_charStar );

		public:
			inline const String & GetName()const
			{
				return m_name;
			}

			inline void SetName( const String & p_name )
			{
				m_name = p_name;
			}

		public:
			static int GetInt()
			{
				return sm_staticMember;
			}
		};
	}
}

#endif
#endif
