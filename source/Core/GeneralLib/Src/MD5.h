/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENLIB_MD5_H___
#define ___GENLIB_MD5_H___

#include <string>
#include <stdio.h>

namespace General
{
	namespace Utils
	{
		class MD5
		{
		private:
			unsigned char m_buffer[64];
			unsigned long m_count[2];
			unsigned long m_MD5[4];

		public:
			MD5();
			~MD5() {};
		public:
			static std::string Hash( std::string const & p_string );
			static std::string Hash( const unsigned char * p_buffer, uint32_t p_len );
			static std::string HashFile( FILE * p_file );
			static std::string HashFile( std::string const & p_filePath );

		private:
			void _transform( const unsigned char p_block[64] );
			void _update( const unsigned char * p_input, unsigned long p_len );
			std::string _final();

		private:
			inline unsigned long _rotateLeft( unsigned long p_x, int p_n );
			inline void FF( unsigned long & A,
							unsigned long B, unsigned long C, unsigned long D,
							unsigned long X, unsigned long S, unsigned long T );
			inline void GG( unsigned long & A,
							unsigned long B, unsigned long C, unsigned long D,
							unsigned long X, unsigned long S, unsigned long T );
			inline void HH( unsigned long & A,
							unsigned long B, unsigned long C, unsigned long D,
							unsigned long X, unsigned long S, unsigned long T );
			inline void II( unsigned long & A,
							unsigned long B, unsigned long C, unsigned long D,
							unsigned long X, unsigned long S, unsigned long T );
			void DWordToByte( unsigned char * p_output, unsigned long * p_input, uint32_t p_len );
			void ByteToDWord( unsigned long * p_output, const unsigned char * p_input, uint32_t p_len );

		};
	}
}

#endif
