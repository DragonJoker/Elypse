/*
See LICENSE file in root folder
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
