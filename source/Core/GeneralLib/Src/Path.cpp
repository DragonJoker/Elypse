/*
See LICENSE file in root folder
*/
#include "Path.h"
#include "StringUtils.h"

namespace General
{
	namespace Utils
	{
		Path::Path()
		{
		}

		Path::Path( char const * const p_data )
			: std::string{ Normalise( p_data ) }
		{
		}

		Path::Path( std::string const & p_data )
			: std::string{ Normalise( p_data ) }
		{
		}

		Path::Path( const Path & p_data )
			: std::string{ Normalise( p_data ) }
		{
		}

		Path & Path::operator=( char const * const p_path )
		{
			std::string::operator=( Normalise( p_path ) );
			return *this;
		}

		Path & Path::operator=( std::string const & p_path )
		{
			std::string::operator=( Normalise( p_path ) );
			return *this;
		}

		Path & Path::operator=( const Path & p_path )
		{
			std::string::operator=( Normalise( p_path ) );
			return *this;
		}

		Path & Path::operator/=( const Path & p_path )
		{
			push_back( d_path_slash );
			append( p_path );
			return *this;
		}

		const Path Path::GetPath()const
		{
			size_t l_index = find_last_of( d_path_slash );

			if ( l_index != std::string::npos )
			{
				return substr( 0, l_index );
			}

			return Path();
		}

		const std::string Path::GetLeaf()const
		{
			size_t l_index = find_last_of( d_path_slash );

			if ( l_index != std::string::npos )
			{
				return substr( l_index + 1, std::string::npos );
			}

			return std::string( *this );
		}

		std::string Path::Normalise( std::string const & p_path )
		{
			std::string l_return = string::replace( p_path, '/', d_path_slash );
			l_return = string::replace( l_return, '\\', d_path_slash );
			return l_return;
		}

		Path operator/( const Path & p_a, const Path & p_b )
		{
			return Path( p_a ) /= p_b;
		}
	}
}
