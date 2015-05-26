#ifndef ___GENERAL_OR_H___
#define ___GENERAL_OR_H___

#if 0
//Deactivated : not finished, and not safe

#include "AutoSingleton.h"

/*
 *	Not threadsafe.
 *	Not quick.
 */

namespace General
{
	namespace Utils
	{
		class PlaceHolder : public AutoSingleton <PlaceHolder>
		{
		private:
			void * m_value;

		public:
			template <typename T> operator T * ()
			{
				return static_cast <T *>( m_value );
			}

			template <typename T>
			friend bool operator << ( T * p_ptr, PlaceHolder & p_place )
			{
				p_place.m_value = p_ptr;
				return p_ptr != NULL;
			}
		};
	}
}

#define OR << General::Utils::PlaceHolder::GetSingleton() ? General::Utils::PlaceHolder::GetSingleton() :

#endif

#endif
