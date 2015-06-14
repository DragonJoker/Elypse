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
#ifndef ___STL_MAP_MACROS_H___
#define ___STL_MAP_MACROS_H___

#include <map>
#include <string>

namespace General
{
	namespace Utils
	{
		class map
		{
		public:
			template< typename T, typename U >
			static inline void deleteAll( std::map< T, U * > & p_map )
			{
				for ( auto && i : p_map )
				{
					delete i.second;
				}

				p_map.clear();
			}

			template< typename T, typename U >
			static inline U * findOrNull( const std::map< T, U * > & p_map, const T & p_key )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					return ifind->second;
				}

				return NULL;
			}

			template< typename T, typename U >
			static inline U findOrNull( const std::map< T, U > & p_map, const T & p_key )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					return ifind->second;
				}

				return U();
			}

			template< typename T, typename U >
			static inline bool has( const std::map< T, U > & p_map, const T & p_key )
			{
				return p_map.find( p_key ) != p_map.cend();
			}

			template< typename T, typename U >
			static inline bool erase( const std::map< T, U > & p_map, const T & p_key )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					p_map.erase( ifind );
					return true;
				}

				return false;
			}

			template< typename T, typename U >
			static inline void eraseIterator( std::map< T, U > & p_map, typename std::map< T, U > ::iterator & p_where )
			{
				auto && l_erase = p_where;
				++ p_where;
				p_map.erase( l_erase );
			}

			template< typename T, typename U >
			static inline bool deleteValue( std::map< T, U * > & p_map, const T & p_key )
			{
				bool l_return = false;
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					delete ifind->second;
					p_map.erase( ifind );
					l_return = true;
				}

				return l_return;
			}

			template< typename T, typename U >
			static inline bool deleteValue( std::map< T *, U * > & p_map, T * p_key )
			{
				bool l_return = false;

				if ( p_key )
				{
					auto && ifind = p_map.find( p_key );

					if ( ifind != p_map.cend() )
					{
						delete ifind->second;
						p_map.erase( ifind );
						l_return = true;
					}
				}

				return l_return;
			}

			template< typename U >
			static inline bool deleteValue( std::map< std::string, U * > & p_map, const std::string & p_key )
			{
				bool l_return = false;

				if ( !p_key.empty() )
				{
					auto && ifind = p_map.find( p_key );

					if ( ifind != p_map.cend() )
					{
						delete ifind->second;
						p_map.erase( ifind );
						l_return = true;
					}
				}

				return l_return;
			}

			template< typename T, typename U, typename ... Params >
			static inline U * insert( std::map< T, U * > & p_map, const T & p_key, Params && ... p_params )
			{
				auto && ifind = p_map.find( p_key );
				U * l_object = NULL;

				if ( ifind == p_map.end() )
				{
					l_object = new U( p_params ... );
					p_map.insert( std::make_pair( p_key, l_object ) );
				}
				else
				{
					l_object = ifind->second;
				}

				return l_object;
			}

			template< typename T, typename U, typename Func, typename ... Params >
			static inline void cycle( const std::map <T, U *> & p_map, Func p_func, Params && ... p_params )
			{
				for ( auto && i : p_map )
				{
					( i.second->* p_func )( p_params... );
				}
			}

			template< typename T, typename U >
			static inline std::map< T, U > get_intersection( const std::map< T, U > & p_mapA, const std::map< T, U > & p_mapB )
			{
				std::map< T, U > l_map;

				if ( p_mapA.empty() || p_mapB.empty() )
				{
					return l_map;
				}

				auto && i = p_mapA.begin();
				auto && j = p_mapB.begin();
				auto && iend = p_mapA.cend();
				auto && jend = p_mapB.cend();

				while ( i != iend )
				{
					while ( j->first < i->first )
					{
						++ j;

						if ( j == jend )
						{
							return l_map;
						}
					}

					if ( * i == * j )
					{
						l_map.insert( l_map.end(), * i );
					}

					++ i;
				}

				return l_map;
			}
		};
	}
}

#endif
