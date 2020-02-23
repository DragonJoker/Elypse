/*
See LICENSE file in root folder
*/
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
		template< typename T >
		class RegistryKeyBase;

		enum RegistryFolder
		{
			RF_HKLM,
			RF_HKCU
		};

		template< typename T >
		class RegistryManager
		{
		private:
			typedef RegistryKeyBase< T > RegistryKeyType;
			typedef std::map< T, RegistryKeyType > RegistryMap;

		public:
			RegistryManager()
				: m_folder{ RF_HKLM }
			{
			}
			~RegistryManager()
			{
			}

			void SetFolder( RegistryFolder p_folder )
			{
				m_folder = p_folder;
				m_registeryKeys.clear();
			}

			bool KeyExists( T const & p_keyName )
			{
				return DoGetOrCreateKey( p_keyName ).IsValid();
			}

			bool CreateKey( T const & p_keyName )
			{
				return DoGetOrCreateKey( p_keyName ).Create();
			}

			RegistryKeyType * GetKey( T const & p_keyName )
			{
				RegistryKeyType * l_return{ nullptr };
				auto l_it = DoGetKey( p_keyName );

				if ( l_it != m_registeryKeys.end() )
				{
					l_return = &l_it->second;
					l_return->Create();
				}
				else
				{
					l_it = DoCreateKey( p_keyName );
					l_return = &l_it->second;

					if ( l_return->IsValid() )
					{
						l_return->Create();
					}
				}

				return l_return;
			}

			bool DeleteKey( T const & p_keyName )
			{
				return DoGetOrCreateKey( p_keyName )->Delete();
			}

		private:
			typename RegistryMap::iterator DoGetKey( T const & p_keyName )
			{
				return m_registeryKeys.find( p_keyName );
			}

			typename RegistryMap::iterator DoCreateKey( T const & p_keyName )
			{
				return m_registeryKeys.insert( { p_keyName, RegistryKeyType{ m_folder, p_keyName } } ).first;
			}

			typename RegistryMap::iterator DoGetOrCreateKey( T const & p_keyName )
			{
				auto && l_it = DoGetKey( p_keyName );

				if ( l_it == m_registeryKeys.end() )
				{
					l_it = DoCreateKey( p_keyName );
				}

				return l_it;
			}

		private:
			RegistryFolder m_folder;
			RegistryMap m_registeryKeys;
		};
	}
}

#	endif //  ! GENLIB_WINDOWS
#endif
