#ifndef ___STL_MACROS3_H___
#define ___STL_MACROS3_H___

#include <map>
#include <set>
#include <vector>
#include <list>

namespace General
{
	namespace Utils
	{
		class multimap
		{
		public:
			template <typename T, typename U>
			static inline void deleteAll( std::multimap <T, U *> & p_map )
			{
				typename std::multimap <T, U *> ::iterator i = p_map.begin();
				const typename std::multimap <T, U *> ::iterator & iend = p_map.end();

				for ( ; i != iend ; ++ i )
				{
					delete i->second;
				}

				p_map.clear();
			}

			template <typename T, typename U>
			static inline bool hasCouple( const std::multimap <T, U> & p_map, const T & p_key, const U & p_subkey )
			{
				typename std::multimap <T, U> ::const_iterator i = p_map.find( p_key );
				const typename std::multimap <T, U> ::const_iterator & iend = p_map.end();

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

			template <typename T, typename U>
			static inline unsigned int count( const std::multimap <T, U> & p_map, const T & p_key )
			{
				typename std::multimap <T, U> ::const_iterator i = p_map.find( p_key );
				const typename std::multimap <T, U> ::const_iterator & iend = p_map.end();

				unsigned int l_count = 0;

				while ( i != iend && i->first == p_key )
				{
					++ i;
					++ l_count;
				}

				return l_count;
			}

			template <typename T, typename U>
			static inline bool has( const std::multimap <T, U> & p_map, const T & p_key )
			{
				return p_map.find( p_key ) != p_map.end();
			}

			template <typename T, typename U>
			static inline void cycle( const std::multimap <T, U *> & p_map, void( U::* p_func )( void ) )
			{
				typename std::multimap <T, U *> ::const_iterator i = p_map.begin();
				const typename std::multimap <T, U *> ::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )();
				}
			}

			template <typename T, typename U, typename Z>
			static inline void cycle( const std::multimap <T, U *> & p_map, void( U::* p_func )( const Z & ), const Z & p_param )
			{
				typename std::multimap <T, U *>::const_iterator i = p_map.begin();
				const typename std::multimap <T, U *>::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )( p_param );
				}
			}

			template <typename T, typename U, typename Z>
			static inline void cycle( const std::multimap <T, U *> & p_map, void( U::* p_func )( Z & ), Z & p_param )
			{
				typename std::multimap <T, U *> ::const_iterator i = p_map.begin();
				const typename std::multimap <T, U *> ::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )( p_param );
				}
			}

			template <typename T, typename U, typename Z>
			static inline void cycle( const std::multimap <T, U *> & p_map, void( U::* p_func )( Z ), const Z & p_param )
			{
				typename std::multimap <T, U *> ::const_iterator i = p_map.begin();
				const typename std::multimap <T, U *> ::const_iterator & iend = p_map.end();

				for ( ; i != iend; ++ i )
				{
					( i->second->* p_func )( p_param );
				}
			}
		};

		class vector
		{
		public:
			template <typename T>
			static inline void deleteAll( std::vector <T *> & p_vector )
			{
				size_t imax = p_vector.size();

				for ( size_t i = 0 ; i < imax ; i ++ )
				{
					delete p_vector[i];
				}

				p_vector.clear();
			}

			template <typename T>
			static inline void eraseValue( std::vector <T> & p_vector, const T & p_key )
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

			template <typename T>
			static inline bool has( std::vector <T> & p_vector, const T & p_key )
			{
				for ( size_t i = 0 ; i < p_vector.size() ; )
				{
					if ( p_vector[i] == p_key )
					{
						return true;
					}

					++ i;
				}

				return false;
			}

			template <typename T>
			static inline void cycle( const std::vector <T *> & p_vector, void( T::* p_func )( void ) )
			{
				size_t imax = p_vector.size();

				for ( size_t i = 0 ; i < imax ; i ++ )
				{
					( p_vector[i]->* p_func )();
				}
			}

			template <typename T, typename U>
			static inline void cycle( const std::vector <T *> & p_vector, void( T::* p_func )( const U & ), const U & p_param )
			{
				size_t imax = p_vector.size();

				for ( size_t i = 0 ; i < imax ; i ++ )
				{
					( p_vector[i]->* p_func )( p_param );
				}
			}

			template <typename T, typename U>
			static inline void cycle( const std::vector <T *> & p_vector, void( T::* p_func )( U & ), U & p_param )
			{
				size_t imax = p_vector.size();

				for ( size_t i = 0 ; i < imax ; i ++ )
				{
					( p_vector[i]->* p_func )( p_param );
				}
			}

			template <typename T, typename U>
			static inline void cycle( const std::vector <T *> & p_vector, void( T::* p_func )( U ), U p_param )
			{
				size_t imax = p_vector.size();

				for ( size_t i = 0 ; i < imax ; i ++ )
				{
					( p_vector[i]->* p_func )( p_param );
				}
			}
		};

		class list
		{
		public:
			template <typename T>
			static inline void deleteAll( std::list <T *> & p_list )
			{
				typename std::list <T *> ::iterator i = p_list.begin();
				const typename std::list <T *> ::iterator & iend = p_list.end();

				for ( ; i != iend ; ++ i )
				{
					delete( * i );
				}

				p_list.clear();
			}
		};
	}
}

#endif
