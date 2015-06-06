#ifndef ___STL_MACROS3_H___
#define ___STL_MACROS3_H___

#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>

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
