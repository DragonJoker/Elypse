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
#ifndef ___GENERAL_SINGLETON_H___
#define ___GENERAL_SINGLETON_H___

#include "NonCopyable.h"

#include <memory>

namespace General
{
	namespace Theory
	{
		template <class T>
		class Singleton : d_noncopyable
		{
		protected:
			Singleton()
			{
				SetSingleton( reinterpret_cast< T * >( this ) );
			}

			~Singleton()
			{
			}

		public:
			static inline T * GetSingletonPtr()
			{
				return GetSingletonUPtr();
			}

			static inline T & GetSingleton()
			{
				return *GetSingletonUPtr();
			}

			static inline T * Create()
			{
				if ( !GetSingletonUPtr() )
				{
					SetSingleton( new T );
				}

				return GetSingletonPtr();
			}

			static inline void Destroy()
			{
				T * l_instance = GetSingletonUPtr();
				delete l_instance;
				SetSingleton( NULL );
			}

			static inline void SetSingleton( T * p_singleton )
			{
				GetSingletonUPtr() = p_singleton;
			}

		private:
			static inline T *& GetSingletonUPtr()
			{
				static T * sm_singleton;
				return sm_singleton;
			}
		};
	}
}

#define GENLIB_INIT_SINGLETON( p_name ) //template <> p_name * General::Theory::Singleton <p_name> ::sm_singleton = NULL
#define GENLIB_SET_SINGLETON() SetSingleton( this )
#define GENLIB_SINGLETON_FOR_DLL( p_name) static p_name & GetSingleton( void); static p_name * GetSingletonPtr( void)
#define GENLIB_INIT_SINGLETON_FOR_DLL( p_name) p_name * p_name::sm_singleton = NULL; p_name * p_name::GetSingletonPtr()	{ return sm_singleton; } p_name & p_name::GetSingleton()	{ return *sm_singleton; }
#define GENLIB_SET_SINGLETON_FOR_DLL() sm_singleton = this

#endif
