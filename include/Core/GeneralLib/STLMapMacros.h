/*
See LICENSE file in root folder
*/
#ifndef ___STL_MAP_MACROS_H___
#define ___STL_MAP_MACROS_H___

#include <map>
#include <memory>
#include <string>

namespace General
{
	namespace Utils
	{
		namespace map
		{
			template< typename T, typename U >
			inline void deleteAll( std::map< T, U * > & p_map )
			{
				for ( auto & i : p_map )
				{
					delete i.second;
					i.second = nullptr;
				}

				p_map.clear();
			}

			template< typename T, typename U >
			inline U * findOrNull( std::map< T, U * > const & p_map, T const & p_key )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					return ifind->second;
				}

				return nullptr;
			}

			template< typename T, typename U >
			inline U findOrNull( std::map< T, U > const & p_map, T const & p_key )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					return ifind->second;
				}

				return U();
			}

			template< typename T, typename U, typename D >
			inline U * findOrNull( std::map< T, std::unique_ptr< U, D > > const & p_map, T const & p_key )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind != p_map.cend() )
				{
					return ifind->second.get();
				}

				return nullptr;
			}

			template< typename T, typename U >
			inline bool has( std::map< T, U > const & p_map, T const & p_key )
			{
				return p_map.find( p_key ) != p_map.cend();
			}

			template< typename T, typename U >
			inline bool erase( std::map< T, U > const & p_map, T const & p_key )
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
			inline void eraseIterator( std::map< T, U > & p_map, typename std::map< T, U > ::iterator & p_where )
			{
				auto && l_erase = p_where;
				++ p_where;
				p_map.erase( l_erase );
			}

			template< typename T, typename U >
			inline bool deleteValue( std::map< T, U * > & p_map, T const & p_key )
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
			inline bool deleteValue( std::map< T *, U * > & p_map, T * p_key )
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
			inline bool deleteValue( std::map< std::string, U * > & p_map, std::string const & p_key )
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
			inline U * insert( std::map< T, U * > & p_map, T const & p_key, Params && ... p_params )
			{
				auto && ifind = p_map.find( p_key );
				U * l_object = nullptr;

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

			template< typename T, typename U, typename D, typename ... Params >
			inline U * insert( std::map< T, std::unique_ptr< U, D > > & p_map, T const & p_key, Params && ... p_params )
			{
				auto && ifind = p_map.find( p_key );

				if ( ifind == p_map.end() )
				{
					std::unique_ptr< U, D > l_object{ new U( p_params ... ) };
					ifind = p_map.emplace( p_key, std::move( l_object ) ).first;
				}

				return ifind->second.get();
			}

			template< typename T, typename U, typename Func, typename ... Params >
			inline void cycle( std::map <T, U *> const & p_map, Func p_func, Params && ... p_params )
			{
				for ( auto && i : p_map )
				{
					( i.second->* p_func )( std::forward< Params >( p_params )... );
				}
			}

			template< typename T, typename U >
			inline std::map< T, U > get_intersection( std::map< T, U > const & p_mapA, std::map< T, U > const & p_mapB )
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
		}
	}
}

#endif
