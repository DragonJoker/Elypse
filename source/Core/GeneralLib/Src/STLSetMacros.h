/*
See LICENSE file in root folder
*/
#ifndef ___STL_MACROS_H___
#define ___STL_MACROS_H___

#include <set>
#include <map>
#include <algorithm>

namespace General
{
	namespace Utils
	{
		namespace set
		{
			template< typename T, typename Func, typename ... Params >
			static inline void cycle( const std::set< T * > & p_set, Func p_func, Params && ... p_params )
			{
				for ( auto && l_element : p_set )
				{
					( l_element->* p_func )( std::forward< Params >( p_params )... );
				}
			}

			template< typename T >
			static inline void deleteAll( std::set< T * > & p_set )
			{
				for ( auto && l_element : p_set )
				{
					delete l_element;
				}

				p_set.clear();
			}

			template< typename T >
			static inline bool eraseValue( std::set< T > & p_set, T const & p_element )
			{
				auto && ifind = p_set.find( p_element );

				if ( ifind != p_set.end() )
				{
					p_set.erase( ifind );
					return true;
				}

				return false;
			}

			template< typename T >
			static inline void eraseIterator( std::set< T > & p_set, typename std::set< T > ::iterator & p_where )
			{
				auto && l_erase = p_where;
				++ p_where;
				p_set.erase( l_erase );
			}

			template< typename T >
			static inline bool has( std::set< T > const & p_map, T const & p_key )
			{
				return p_map.find( p_key ) != p_map.end();
			}

			template< typename T >
			static std::set< T > get_intersection( std::set< T > const & p_setA, std::set< T > const & p_setB )
			{
				std::set< T > l_intersection;
				std::insert_iterator <std::set< T > > l_insertIter( l_intersection, l_intersection.begin() );
				std::set_intersection( p_setA.begin(), p_setA.end(), p_setB.begin(), p_setB.end(), l_insertIter );
				return l_intersection;
			}

			template< typename T, typename U >
			static std::set< T > createFromMapKey( std::map< T, U > const & p_map )
			{
				std::set< T > l_set;

				for ( auto && i : p_map )
				{
					l_set.insert( l_set.end(), i.first );
				}

				return l_set;
			}

			template< typename T, typename U >
			static std::set< U > createFromMapData( std::map< T, U > const & p_map )
			{
				std::set< U > l_set;

				for ( auto && i : p_map )
				{
					l_set.insert( l_set.end(), i.second );
				}

				return l_set;
			}
		}
	}
}

#endif
