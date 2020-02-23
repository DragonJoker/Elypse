/*
See LICENSE file in root folder
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
		namespace multimap
		{
			template< typename T, typename U >
			inline void deleteAll( std::multimap< T, U * > & p_map )
			{
				for ( auto && i : p_map )
				{
					delete i.second;
				}

				p_map.clear();
			}

			template< typename T, typename U >
			inline bool hasCouple( std::multimap< T, U > const & p_map, T const & p_key, U const & p_subkey )
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
			inline uint32_t count( std::multimap< T, U > const & p_map, T const & p_key )
			{
				auto && i = p_map.find( p_key );
				auto && iend = p_map.cend();
				uint32_t l_count = 0;

				while ( i != iend && i->first == p_key )
				{
					++ i;
					++ l_count;
				}

				return l_count;
			}

			template< typename T, typename U >
			inline bool has( std::multimap< T, U > const & p_map, T const & p_key )
			{
				return p_map.find( p_key ) != p_map.cend();
			}

			template< typename T, typename U, typename Func, typename ... Params >
			inline void cycle( std::multimap< T, U * > const & p_map, Func p_func, Params && ... p_params )
			{
				for ( auto && i : p_map )
				{
					( i.second->* p_func )( std::forward< Params >( p_params )... );
				}
			}
		}

		namespace vector
		{
			template< typename T >
			inline void deleteAll( std::vector< T * > & p_vector )
			{
				for ( auto && i : p_vector )
				{
					delete i;
				}

				p_vector.clear();
			}

			template< typename T >
			inline void eraseValue( std::vector< T > & p_vector, T const & p_key )
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
			inline T * getOrNull( std::vector< T * > const & p_vector, size_t p_index )
			{
				T * l_return = nullptr;

				if ( p_index < p_vector.size() )
				{
					l_return = p_vector[p_index];
				}

				return l_return;
			}

			template< typename T >
			inline T * findOrNull( std::vector< T * > const & p_vector, T const * p_key )
			{
				T * l_return = nullptr;
				auto && l_it = std::find( p_vector.cbegin(), p_vector.cend(), p_key );

				if ( l_it != p_vector.end() )
				{
					l_return = ( *l_it );
				}

				return l_return;
			}

			template< typename T, typename Func >
			inline T * findOrNull( std::vector< T * > const & p_vector, Func p_func )
			{
				T * l_return = nullptr;
				auto && l_it = std::find_if( p_vector.cbegin(), p_vector.cend(), p_func );

				if ( l_it != p_vector.end() )
				{
					l_return = ( *l_it );
				}

				return l_return;
			}

			template< typename T >
			inline bool has( std::vector< T > const & p_vector, T const & p_key )
			{
				return std::find( p_vector.cbegin(), p_vector.cend(), p_key ) != p_vector.end();
			}

			template< typename T >
			inline size_t find( std::vector< T > const & p_vector, T const & p_key )
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
			inline void cycle( std::vector< T * > const & p_vector, Func p_func, Params && ... p_params )
			{
				for ( auto && l_element : p_vector )
				{
					( l_element->* p_func )( std::forward< Params >( p_params )... );
				}
			}
		}

		namespace list
		{
			template< typename T >
			inline void deleteAll( std::list< T * > & p_list )
			{
				for ( auto && i : p_list )
				{
					delete i;
				}

				p_list.clear();
			}
		}
	}
}

#endif
