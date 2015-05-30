#ifndef ___GENERAL_DYNAMIC_LIBRARY_H___
#define ___GENERAL_DYNAMIC_LIBRARY_H___

#ifndef __GNUG__
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

#include <string>

namespace General
{ namespace Utils
{
	class DynamicLibrary
	{
	private:
		void * m_library;

	public:
		DynamicLibrary()
			:	m_library (NULL)
		{
		}

		~DynamicLibrary()
		{
			_close();
		}

	private:
		void _close()
		{
			if (m_library != NULL)
			{
#ifndef __GNUG__
				FreeLibrary( static_cast <HMODULE> ( m_library));
#else
				dlclose( m_library);
#endif
			}
		}
	public:

		bool Open( const char * p_name)
		{
			_close();
#ifndef __GNUG__
			m_library = LoadLibraryA( p_name);
#else
			m_library = dlopen( p_name, RTLD_LAZY);
#endif
			return m_library != NULL;
		}

		bool Open( const std::string & p_name)
		{
			return Open( p_name.c_str());
		}

		void * GetFunction( const char * p_name)
		{
			if (m_library != NULL)
			{
#ifndef __GNUG__
				return GetProcAddress( static_cast <HMODULE> ( m_library), p_name);
#else
				return dlsym( m_library, p_name);
#endif
			}

			return NULL;
		}

		void * GetFunction( const std::string & p_name)
		{
			return GetFunction( p_name.c_str());
		}

		inline bool IsOpen()const { return m_library != NULL; }
	};
}
}

#endif
