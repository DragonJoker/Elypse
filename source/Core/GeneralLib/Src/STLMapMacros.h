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
			template <typename T, typename U>
			static inline void deleteAll( std::map <T, U *> & p_map )
			{
				typename std::map <T, U *> ::iterator i = p_map.begin();
				const typename std::map <T, U *> ::iterator & iend = p_map.end();

				for ( ; i != iend ; ++ i )
				{
					delete i->second;
				}

				p_map.clear();
			}

			template <typename T, typename U>
			static inline U * findOrNull( const std::map <T, U *> & p_map, const T & p_key )
			{
				const typename std::map <T, U *> ::const_iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					return ifind->second;
				}

				return NULL;
			}

			template <typename T, typename U>
			static inline U findOrNull( const std::map <T, U> & p_map, const T & p_key )
			{
				const typename std::map <T, U> ::const_iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					return ifind->second;
				}

				return U();
			}

			template <typename T, typename U>
			static inline bool has( const std::map <T, U> & p_map, const T & p_key )
			{
				return p_map.find( p_key ) != p_map.end();
			}

			template <typename T, typename U>
			static inline bool erase( const std::map <T, U> & p_map, const T & p_key )
			{
				const typename std::map <T, U> ::const_iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					p_map.erase( ifind );
					return true;
				}

				return false;
			}

			template <typename T, typename U>
			static inline void eraseIterator( std::map <T, U> & p_map, typename std::map <T, U> ::iterator & p_where )
			{
				typename std::map <T, U> ::iterator l_erase = p_where;
				++ p_where;
				p_map.erase( l_erase );
			}

			template <typename T, typename U>
			static inline bool deleteValue( std::map <T, U *> & p_map, const T & p_key )
			{
				const typename std::map <T, U *> ::iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					delete ifind->second;
					p_map.erase( ifind );
					return true;
				}

				return false;
			}

			template <typename T, typename U>
			static inline bool deleteValue( std::map <T *, U *> & p_map, T * p_key )
			{
				if ( p_key == NULL )
				{
					return false;
				}

				const typename std::map <T *, U *> ::iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					delete ifind->second;
					p_map.erase( ifind );
					return true;
				}

				return false;
			}

			template <typename U>
			static inline bool deleteValue( std::map <std::string, U *> & p_map, const std::string & p_key )
			{
				if ( p_key.empty() )
				{
					return false;
				}

				const typename std::map <std::string, U *> ::iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					delete ifind->second;
					p_map.erase( ifind );
					return true;
				}

				return false;
			}

			template <typename T, typename U>
			static inline U * insert( std::map <T, U *> & p_map, const T & p_key )
			{
				const typename std::map <T, U *> ::iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					return ifind->second;
				}

				U * l_object = new U();
				p_map.insert( typename std::map <T, U *> ::value_type( p_key, l_object ) );
				return l_object;
			}

			template <typename T, typename U, typename V>
			static inline U * insert( std::map <T, U *> & p_map, const T & p_key, const V & p_param )
			{
				const typename std::map <T, U *> ::iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					return ifind->second;
				}

				U * l_object = new U( p_param );
				p_map.insert( typename std::map <T, U *> ::value_type( p_key, l_object ) );
				return l_object;
			}

			template <typename T, typename U, typename V, typename W>
			static inline U * insert( std::map <T, U *> & p_map, const T & p_key, const V & p_param, const W & p_param2 )
			{
				const typename std::map <T, U *> ::iterator & ifind = p_map.find( p_key );

				if ( ifind != p_map.end() )
				{
					return ifind->second;
				}

				U * l_object = new U( p_param, p_param2 );
				p_map.insert( typename std::map <T, U *> ::value_type( p_key, l_object ) );
				return l_object;
			}

			template <typename T, typename U>
			static inline void cycle( const std::map <T, U *> & p_map, void( U::* p_func )( void ) )
			{
				typename std::map <T, U *> ::const_iterator i = p_map.begin();
				const typename std::map <T, U *> ::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )();
				}
			}

			template <typename T, typename U, typename Z>
			static inline void cycle( const std::map <T, U *> & p_map, void( U::* p_func )( const Z & ), const Z & p_param )
			{
				typename std::map <T, U *>::const_iterator i = p_map.begin();
				const typename std::map <T, U *>::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )( p_param );
				}
			}

			template <typename T, typename U, typename V, typename W>
			static inline void cycle( const std::map <T, U *> & p_map, void( U::* p_func )( const V &, const W & ), const V & p_param, const W & p_param2 )
			{
				typename std::map <T, U *>::const_iterator i = p_map.begin();
				const typename std::map <T, U *>::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )( p_param, p_param2 );
				}
			}

			template <typename T, typename U, typename Z>
			static inline void cycle( const std::map <T, U *> & p_map, void( U::* p_func )( Z ), const Z & p_param )
			{
				typename std::map <T, U *> ::const_iterator i = p_map.begin();
				const typename std::map <T, U *> ::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )( p_param );
				}
			}

			template <typename T, typename U>
			static inline std::map <T, U> intersects( const std::map <T, U> & p_mapA, const std::map <T, U> & p_mapB )
			{
				std::map <T, U> l_map;

				if ( p_mapA.empty() || p_mapB.empty() )
				{
					return l_map;
				}

				typename std::map <T, U> ::const_iterator i = p_mapA.begin();
				typename std::map <T, U> ::const_iterator j = p_mapB.begin();
				const typename std::map <T, U> ::const_iterator & iend = p_mapA.end();
				const typename std::map <T, U> ::const_iterator & jend = p_mapB.end();

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
