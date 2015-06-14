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
#ifndef ___STL_MACROS3_H___
#define ___STL_MACROS3_H___

#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

namespace General
{
	namespace Utils
	{
		class multimap
		{
		public:
			template< typename T, typename U >
			static inline void deleteAll( std::multimap< T, U * > & p_map )
			{
				for ( auto && i : p_map )
				{
					delete i.second;
				}

				p_map.clear();
			}

			template< typename T, typename U >
			static inline bool hasCouple( const std::multimap< T, U > & p_map, const T & p_key, const U & p_subkey )
			{
				auto && i = p_map.find( p_key );
				auto && iend = p_map.cend();

				while ( i != iend && i->first == p_key )
				{
					if ( i->second == p_subkey )
					{
						return true;
					}

					++ i;
				}

				return false;
			}

			template< typename T, typename U >
			static inline unsigned int count( const std::multimap< T, U > & p_map, const T & p_key )
			{
				auto && i = p_map.find( p_key );
				auto && iend = p_map.cend();
				unsigned int l_count = 0;

				while ( i != iend && i->first == p_key )
				{
					++ i;
					++ l_count;
				}

				return l_count;
			}

			template< typename T, typename U >
			static inline bool has( const std::multimap< T, U > & p_map, const T & p_key )
			{
				return p_map.find( p_key ) != p_map.cend();
			}

			template< typename T, typename U, typename Func, typename ... Params >
			static inline void cycle( const std::multimap< T, U * > & p_map, Func p_func, Params && ... p_params )
			{
				for ( auto && i : p_map )
				{
					( i.second->* p_func )( p_params... );
				}
			}
		};

		class vector
		{
		public:
			template< typename T >
			static inline void deleteAll( std::vector< T * > & p_vector )
			{
				for ( auto && i : p_vector )
				{
					delete i;
				}

				p_vector.clear();
			}

			template< typename T >
			static inline void eraseValue( std::vector< T > & p_vector, const T & p_key )
			{
				for ( size_t i = 0 ; i < p_vector.size() ; )
				{
					if ( p_vector[i] == p_key )
					{
						p_vector.erase( p_vector.begin() + i );
					}
					else
					{
						++ i;
					}
				}
			}

			template< typename T >
			static inline T * getOrNull( const std::vector< T * > & p_vector, size_t p_index )
			{
				T * l_return = NULL;

				if ( p_index < p_vector.size() )
				{
					l_return = p_vector[p_index];
				}

				return l_return;
			}

			template< typename T >
			static inline T * findOrNull( const std::vector< T * > & p_vector, T const * p_key )
			{
				T * l_return = NULL;
				auto && l_it = std::find( p_vector.cbegin(), p_vector.cend(), p_key );

				if ( l_it != p_vector.end() )
				{
					l_return = ( *l_it );
				}

				return l_return;
			}

			template< typename T, typename Func >
			static inline T * findOrNull( const std::vector< T * > & p_vector, Func p_func )
			{
				T * l_return = NULL;
				auto && l_it = std::find_if( p_vector.cbegin(), p_vector.cend(), p_func );

				if ( l_it != p_vector.end() )
				{
					l_return = ( *l_it );
				}

				return l_return;
			}

			template< typename T >
			static inline bool has( const std::vector< T > & p_vector, const T & p_key )
			{
				return std::find( p_vector.cbegin(), p_vector.cend(), p_key ) != p_vector.end();
			}

			template< typename T >
			static inline size_t find( const std::vector< T > & p_vector, const T & p_key )
			{
				for ( size_t i = 0 ; i < p_vector.size() ; )
				{
					if ( p_vector[i] == p_key )
					{
						return i;
					}

					++ i;
				}

				return std::string::npos;
			}

			template< typename T, typename Func, typename ... Params >
			static inline void cycle( const std::vector< T * > & p_vector, Func p_func, Params && ... p_params )
			{
				for ( auto && l_element : p_vector )
				{
					( l_element->* p_func )( p_params... );
				}
			}
		};

		class list
		{
		public:
			template< typename T >
			static inline void deleteAll( std::list< T * > & p_list )
			{
				for ( auto && i : p_list )
				{
					delete i;
				}

				p_list.clear();
			}
		};
	}
}

#endif
