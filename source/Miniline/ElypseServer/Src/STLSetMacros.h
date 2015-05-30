#ifndef ___STL_MACROS_H___
#define ___STL_MACROS_H___

#include <set>

namespace General
{
	namespace Utils
	{
		class set
		{
		public:
			template <typename T>
			static inline void cycle( const std::set <T *> & p_set, void( T::* p_func )( void ) )
			{
				typename std::set <T *> ::const_iterator i = p_set.begin();
				const typename std::set <T *> ::const_iterator & iend = p_set.end();

				for ( ; i != iend; ++ i )
				{
					( * i->* p_func )();
				}
			}

			template <typename T, typename U>
			static inline void cycle( const std::set <T *> & p_set, void( T::* p_func )( const U & ), const U & p_param )
			{
				typename std::set <T *> ::const_iterator i = p_set.begin();
				const typename std::set <T *> ::const_iterator & iend = p_set.end();

				for ( ; i != iend; ++ i )
				{
					( * i->* p_func )( p_param );
				}
			}

			template <typename T, typename U>
			static inline void cycle( const std::set <T *> & p_set, void( T::* p_func )( U & ), U & p_param )
			{
				typename std::set <T *> ::const_iterator i = p_set.begin();
				const typename std::set <T *> ::const_iterator & iend = p_set.end();

				for ( ; i != iend; ++ i )
				{
					( * i->* p_func )( p_param );
				}
			}

			template <typename T, typename U>
			static inline void cycle( const std::set <T *> & p_set, void( T::* p_func )( U ), U p_param )
			{
				typename std::set <T *> ::const_iterator i = p_set.begin();
				const typename std::set <T *> ::const_iterator & iend = p_set.end();

				for ( ; i != iend; ++ i )
				{
					( * i->* p_func )( p_param );
				}
			}


			template <typename T>
			static inline void deleteAll( std::set <T *> & p_set )
			{
				typename std::set <T *> ::iterator i = p_set.begin();
				const typename std::set <T *> ::iterator & iend = p_set.end();

				for ( ; i != iend ; ++ i )
				{
					delete( * i );
				}

				p_set.clear();
			}

			template <typename T>
			static inline bool eraseValue( std::set <T> & p_set, const T & p_element )
			{
				const typename std::set <T> ::iterator ifind = p_set.find( p_element );

				if ( ifind != p_set.end() )
				{
					p_set.erase( ifind );
					return true;
				}

				return false;
			}

			template <typename T>
			static inline void eraseIterator( std::set <T> & p_set, typename std::set <T> ::iterator & p_where )
			{
				typename std::set <T> ::iterator l_erase = p_where;
				++ p_where;
				p_set.erase( l_erase );
			}

			template <typename T>
			static inline bool has( const std::set <T> & p_map, const T & p_key )
			{
				return p_map.find( p_key ) != p_map.end();
			}

			template <typename T>
			std::set <T> intersects( const std::set <T> & p_setA, const std::set <T> & p_setB )
			{
				std::set <T> l_intersection;

				std::insert_iterator <std::set <T> > l_insertIter( l_intersection, l_intersection.begin() );

				std::set_intersection( p_setA.begin(), p_setA.end(), p_setB.begin(), p_setB.end(), l_insertIter );

				return l_intersection;
			}

			template <typename T, typename U>
			std::set <T> createFromMapKey( const std::map <T, U> & p_map )
			{
				std::set <T> l_set;

				typename std::map <T, U> ::const_iterator i = p_map.begin();
				const typename std::map <T, U> ::const_iterator & iend = p_map.end();

				for ( ; i != iend ; ++ i )
				{
					l_set.insert( l_set.end(), i->first );
				}

				return l_set;
			}

			template <typename T, typename U>
			std::set <U> createFromMapData( const std::map <T, U> & p_map )
			{
				std::set <U> l_set;

				typename std::map <T, U> ::const_iterator i = p_map.begin();
				const typename std::map <T, U> ::const_iterator & iend = p_map.end();

				for ( ; i != iend ; ++ i )
				{
					l_set.insert( l_set.end(), i->second );
				}

				return l_set;
			}
		};
	}
}

#endif
