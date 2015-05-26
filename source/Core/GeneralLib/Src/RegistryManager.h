#ifndef ___REGISTRY_MANAGER_H___
#define ___REGISTRY_MANAGER_H___

#include "Config.h"
#if GENLIB_WINDOWS
#	include <string>
#	include <map>
#	include "STLMapMacros.h"

namespace General
{
	namespace Computer
	{
		template <typename T>
		class RegistryKeyBase;

		enum RegistryFolder
		{
			RF_HKLM,
			RF_HKCU
		};

		template <typename T>
		class RegistryManager
		{
		private:
			typedef RegistryKeyBase<T> RegistryKeyType;

			RegistryFolder m_folder;

			typedef std::map <T, RegistryKeyType *> RegistryMap;

			RegistryMap m_registeryKeys;

		public:
			RegistryManager()
				:	m_folder( RF_HKLM )
			{
			}
			~RegistryManager()
			{
				General::Utils::map::deleteAll( m_registeryKeys );
			}

		private:
			RegistryKeyType * _getKey( const T & p_keyName )
			{
				return General::Utils::map::findOrNull( m_registeryKeys, p_keyName );
			}

			RegistryKeyType * _createKey( const T & p_keyName )
			{
				RegistryKeyType * l_key = new RegistryKeyType( m_folder, p_keyName );
				m_registeryKeys.insert( RegistryMap::value_type( p_keyName, l_key ) );
				return l_key;
			}

			RegistryKeyType * _getOrCreateKey( const T & p_keyName )
			{
				RegistryKeyType * l_key = _getKey( p_keyName );

				if ( l_key == NULL )
				{
					l_key = _createKey( p_keyName );
				}

				return l_key;
			}

		public:
			void SetFolder( RegistryFolder p_folder )
			{
				m_folder = p_folder;
				General::Utils::map::deleteAll( m_registeryKeys );
			}

			bool KeyExists( const T & p_keyName )
			{
				return _getOrCreateKey( p_keyName )->IsValid();
			}

			bool CreateKey( const T & p_keyName )
			{
				return _getOrCreateKey( p_keyName )->Create();
			}

			RegistryKeyType * GetKey( const T & p_keyName )
			{
				RegistryKeyType * l_key = _getKey( p_keyName );

				if ( l_key != NULL )
				{
					l_key->Create();
					return l_key;
				}

				l_key = _createKey( p_keyName );

				if ( l_key->IsValid() )
				{
					l_key->Create();
					return l_key;
				}

				return NULL;
			}

			bool DeleteKey( const T & p_keyName )
			{
				return _getOrCreateKey( p_keyName )->Delete();
			}

		};
	}
}

#	endif //  ! GENLIB_WINDOWS
#endif
