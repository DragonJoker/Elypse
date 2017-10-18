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
#ifndef ___MANAGER_H___
#define ___MANAGER_H___

#include <map>
#include <string>
#include <memory>
#include <NonCopyable.h>

#include "STLMapMacros.h"

/**
 * A collection class, allowing you to store named objects, removing, finding or adding them as you wish.
 * Autodeletion of all the contained items on deletion of the manager.
**/

namespace General
{
	namespace Templates
	{
		template< typename TObj >
		class Manager
			: private General::Theory::noncopyable
		{
		public:
			using TObjPtr = std::shared_ptr< TObj >;
			using TypeMap = std::map< std::string, TObjPtr >;

		public:
			inline decltype( auto ) begin()const
			{
				return m_objectMap.begin();
			}

			inline decltype( auto ) begin()
			{
				return m_objectMap.begin();
			}

			inline decltype( auto ) end()const
			{
				return m_objectMap.end();
			}

			inline decltype( auto ) end()
			{
				return m_objectMap.end();
			}

			void Clear() noexcept
			{
				m_objectMap.clear();
			}

			bool AddElement( TObjPtr p_element )
			{
				auto && l_it = m_objectMap.find( p_element->GetName() );

				if ( l_it != m_objectMap.end() )
				{
					return false;
				}

				m_objectMap.insert( { p_element->GetName(), p_element } );
				return true;
			}

			template< typename ... TParams >
			TObjPtr CreateElement( std::string const & p_elementName, TParams && ... p_params )
			{
				auto l_it = m_objectMap.find( p_elementName );

				if ( l_it == m_objectMap.end() )
				{
					l_it = m_objectMap.insert( { p_elementName, std::make_shared< TObj >( p_elementName, std::forward< TParams >( p_params )... ) } ).first;
				}

				return l_it->second;
			}

			bool HasElement( std::string const & p_key )const
			{
				return m_objectMap.find( p_key ) != m_objectMap.end();
			}

			bool HasElement( TObjPtr p_element )const
			{
				return m_objectMap.find( p_element->GetName() ) != m_objectMap.end();
			}

			TObjPtr RemoveElement( std::string const & p_key )
			{
				auto && l_it = m_objectMap.find( p_key );

				if ( l_it == m_objectMap.end() )
				{
					return nullptr;
				}

				TObjPtr l_ret = l_it->second;
				m_objectMap.erase( l_it );
				return l_ret;
			}

			bool RemoveElement( TObjPtr p_element )
			{
				return ( RemoveElement( p_element->GetName() ) != nullptr );
			}

			TObjPtr GetElementByName( std::string const & p_key )
			{
				return General::Utils::map::findOrNull( m_objectMap, p_key );
			}

			bool DestroyElement( TObjPtr & p_element )
			{
				auto l_return = RemoveElement( p_element );

				if ( l_return )
				{
					p_element.reset();
				}

				return l_return;
			}

		protected:
			TypeMap m_objectMap;
		};
	}
}

#endif
