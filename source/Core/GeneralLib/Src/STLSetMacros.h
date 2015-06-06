#ifndef ___STL_MACROS_H___
#define ___STL_MACROS_H___

#include <set>
#include <map>

namespace General
{
	namespace Utils
	{
		class set
		{
		public:
			template< typename T, typename Func, typename ... Params >
			static inline void cycle( const std::set< T * > & p_set, Func p_func, Params && ... p_params )
			{
				for ( auto && l_element : p_set )
				{
					( l_element->* p_func )( p_params... );
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
			static inline bool eraseValue( std::set< T > & p_set, const T & p_element )
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
			static inline bool has( const std::set< T > & p_map, const T & p_key )
			{
				return p_map.find( p_key ) != p_map.end();
			}

			template< typename T >
			static std::set< T > get_intersection( const std::set< T > & p_setA, const std::set< T > & p_setB )
			{
				std::set< T > l_intersection;
				std::insert_iterator <std::set< T > > l_insertIter( l_intersection, l_intersection.begin() );
				std::set_intersection( p_setA.begin(), p_setA.end(), p_setB.begin(), p_setB.end(), l_insertIter );
				return l_intersection;
			}

			template< typename T, typename U >
			static std::set< T > createFromMapKey( const std::map <T, U> & p_map )
			{
				std::set< T > l_set;

				for ( auto && i : p_map )
				{
					l_set.insert( l_set.end(), i.first );
				}

				return l_set;
			}

			template< typename T, typename U >
			static std::set< U > createFromMapData( const std::map< T, U > & p_map )
			{
				std::set< U > l_set;

				for ( auto && i : p_map )
				{
					l_set.insert( l_set.end(), i.second );
				}

				return l_set;
			}
		};
	}
}

#endif
