#ifndef ___GENERAL_PATH_H___
#define ___GENERAL_PATH_H___

#include <string>
#include "Macros.h"

#if GENLIB_WINDOWS
#	define d_path_slash	'\\'
#else
#	define d_path_slash '/'
#endif

namespace General
{
	namespace Utils
	{
		class Path : public std::string
		{
		public:
			inline Path()
			{}

			inline Path( const char * p_data )
				:	std::string( p_data )
			{}

			inline Path( const std::string & p_data )
				:	std::string( p_data )
			{}

			inline Path( const Path & p_data )
				:	std::string( p_data )
			{}

			inline const Path operator /( const Path & p_path )const
			{
				Path l_path( * this );
				l_path.push_back( d_path_slash );
				l_path.append( p_path );
				return l_path;
			}

			inline Path & operator /=( const Path & p_path )
			{
				push_back( d_path_slash );
				append( p_path );
				return *this;
			}

			inline const std::string GetPath()const
			{
				size_t l_index = find_last_of( d_path_slash );

				if ( l_index != std::string::npos )
				{
					return substr( 0, l_index );
				}

				return std::string();
			}

			inline const std::string GetLeaf()const
			{
				size_t l_index = find_last_of( d_path_slash );

				if ( l_index != std::string::npos )
				{
					return substr( l_index + 1, std::string::npos );
				}

				return std::string( *this );
			}
		};
	}
}

#endif
