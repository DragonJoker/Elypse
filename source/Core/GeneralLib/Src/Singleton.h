/*
See LICENSE file in root folder
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
		class Singleton
			: d_noncopyable
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

			template< typename ... Params >
			static inline T * Create( Params && ... p_params )
			{
				if ( !GetSingletonUPtr() )
				{
					SetSingleton( new T{ std::forward< Params >( p_params )... } );
				}

				return GetSingletonPtr();
			}

			static inline void Destroy()
			{
				T * l_instance = GetSingletonUPtr();
				delete l_instance;
				SetSingleton( nullptr );
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
