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
