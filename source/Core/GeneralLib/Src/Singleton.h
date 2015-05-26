#ifndef ___GENERAL_SINGLETON_H___
#define ___GENERAL_SINGLETON_H___

#include "NonCopyable.h"

namespace General
{
	namespace Theory
	{
		template <class T>
		class Singleton : d_noncopyable
		{
		protected:
			static T * sm_singleton;

		protected:
			Singleton()
			{
				sm_singleton = reinterpret_cast <T *>( this );
			}

			~Singleton()
			{
			}

		public:
			static inline T * GetSingletonPtr()
			{
				return sm_singleton;
			}
			static inline T & GetSingleton()
			{
				return *sm_singleton;
			}
			static inline T * Create()
			{
				if ( sm_singleton == NULL )
				{
					sm_singleton = new T();
				}

				return sm_singleton;
			}

			static inline void Destroy()
			{
				if ( sm_singleton != NULL )
				{
					delete sm_singleton;
				}

				sm_singleton = NULL;
			}
			static inline void SetSingleton( T * p_singleton )
			{
				sm_singleton = p_singleton;
			}
		};
	}
}

#define GENLIB_INIT_SINGLETON( p_name) template <> p_name * General::Theory::Singleton <p_name> ::sm_singleton = NULL
#define GENLIB_SINGLETON_FOR_DLL( p_name) static p_name & GetSingleton( void); static p_name * GetSingletonPtr( void)
#define GENLIB_INIT_SINGLETON_FOR_DLL( p_name) p_name * p_name::sm_singleton = NULL; p_name * p_name :: GetSingletonPtr()	{ return sm_singleton; } p_name & p_name :: GetSingleton()	{ return *sm_singleton; }
#define GENLIB_SET_SINGLETON()  sm_singleton = this

#endif
