#include "MD5.h"
#include <cstring>
#include <cstdio>

#pragma warning( disable:4996 )

//Magic initialization constants
#define MD5_INIT_STATE_0 0x67452301
#define MD5_INIT_STATE_1 0xefcdab89
#define MD5_INIT_STATE_2 0x98badcfe
#define MD5_INIT_STATE_3 0x10325476

//Constants for Transform routine.
#define MD5_S11  7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21  5
#define MD5_S22  9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31  4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41  6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

//Transformation Constants - Round 1
#define MD5_T01  0xd76aa478 //Transformation Constant 1
#define MD5_T02  0xe8c7b756 //Transformation Constant 2
#define MD5_T03  0x242070db //Transformation Constant 3
#define MD5_T04  0xc1bdceee //Transformation Constant 4
#define MD5_T05  0xf57c0faf //Transformation Constant 5
#define MD5_T06  0x4787c62a //Transformation Constant 6
#define MD5_T07  0xa8304613 //Transformation Constant 7
#define MD5_T08  0xfd469501 //Transformation Constant 8
#define MD5_T09  0x698098d8 //Transformation Constant 9
#define MD5_T10  0x8b44f7af //Transformation Constant 10
#define MD5_T11  0xffff5bb1 //Transformation Constant 11
#define MD5_T12  0x895cd7be //Transformation Constant 12
#define MD5_T13  0x6b901122 //Transformation Constant 13
#define MD5_T14  0xfd987193 //Transformation Constant 14
#define MD5_T15  0xa679438e //Transformation Constant 15
#define MD5_T16  0x49b40821 //Transformation Constant 16

//Transformation Constants - Round 2
#define MD5_T17  0xf61e2562 //Transformation Constant 17
#define MD5_T18  0xc040b340 //Transformation Constant 18
#define MD5_T19  0x265e5a51 //Transformation Constant 19
#define MD5_T20  0xe9b6c7aa //Transformation Constant 20
#define MD5_T21  0xd62f105d //Transformation Constant 21
#define MD5_T22  0x02441453 //Transformation Constant 22
#define MD5_T23  0xd8a1e681 //Transformation Constant 23
#define MD5_T24  0xe7d3fbc8 //Transformation Constant 24
#define MD5_T25  0x21e1cde6 //Transformation Constant 25
#define MD5_T26  0xc33707d6 //Transformation Constant 26
#define MD5_T27  0xf4d50d87 //Transformation Constant 27
#define MD5_T28  0x455a14ed //Transformation Constant 28
#define MD5_T29  0xa9e3e905 //Transformation Constant 29
#define MD5_T30  0xfcefa3f8 //Transformation Constant 30
#define MD5_T31  0x676f02d9 //Transformation Constant 31
#define MD5_T32  0x8d2a4c8a //Transformation Constant 32

//Transformation Constants - Round 3
#define MD5_T33  0xfffa3942 //Transformation Constant 33
#define MD5_T34  0x8771f681 //Transformation Constant 34
#define MD5_T35  0x6d9d6122 //Transformation Constant 35
#define MD5_T36  0xfde5380c //Transformation Constant 36
#define MD5_T37  0xa4beea44 //Transformation Constant 37
#define MD5_T38  0x4bdecfa9 //Transformation Constant 38
#define MD5_T39  0xf6bb4b60 //Transformation Constant 39
#define MD5_T40  0xbebfbc70 //Transformation Constant 40
#define MD5_T41  0x289b7ec6 //Transformation Constant 41
#define MD5_T42  0xeaa127fa //Transformation Constant 42
#define MD5_T43  0xd4ef3085 //Transformation Constant 43
#define MD5_T44  0x04881d05 //Transformation Constant 44
#define MD5_T45  0xd9d4d039 //Transformation Constant 45
#define MD5_T46  0xe6db99e5 //Transformation Constant 46
#define MD5_T47  0x1fa27cf8 //Transformation Constant 47
#define MD5_T48  0xc4ac5665 //Transformation Constant 48

//Transformation Constants - Round 4
#define MD5_T49  0xf4292244 //Transformation Constant 49
#define MD5_T50  0x432aff97 //Transformation Constant 50
#define MD5_T51  0xab9423a7 //Transformation Constant 51
#define MD5_T52  0xfc93a039 //Transformation Constant 52
#define MD5_T53  0x655b59c3 //Transformation Constant 53
#define MD5_T54  0x8f0ccc92 //Transformation Constant 54
#define MD5_T55  0xffeff47d //Transformation Constant 55
#define MD5_T56  0x85845dd1 //Transformation Constant 56
#define MD5_T57  0x6fa87e4f //Transformation Constant 57
#define MD5_T58  0xfe2ce6e0 //Transformation Constant 58
#define MD5_T59  0xa3014314 //Transformation Constant 59
#define MD5_T60  0x4e0811a1 //Transformation Constant 60
#define MD5_T61  0xf7537e82 //Transformation Constant 61
#define MD5_T62  0xbd3af235 //Transformation Constant 62
#define MD5_T63  0x2ad7d2bb //Transformation Constant 63
#define MD5_T64  0xeb86d391 //Transformation Constant 64

//Null data (except for first BYTE) used to finalise the checksum calculation
static unsigned char PADDING[64] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

using namespace General::Utils;

std::string MD5::HashFile( const std::string & p_filePath )
{
	//open the file as a binary file in readonly mode, denying write access
	FILE * l_file = fopen( p_filePath.c_str(), "rb" );

	if ( l_file == NULL )
	{
		return std::string();
	}

	//the file has been successfully opened, so now get and return its checksum
	const std::string & l_returnValue = HashFile( l_file );
	fclose( l_file );
	return l_returnValue;
}

std::string MD5::HashFile( FILE * p_file )
{
	if ( p_file == NULL )
	{
		return std::string();
	}

	try
	{
		MD5 l_MD5Checksum;				//checksum object
		int l_len = 0;							//number of bytes read from the file
		const int l_bufferSize = 1024;			//checksum the file in blocks of 1024 bytes
		unsigned char l_buffer[l_bufferSize];	//buffer for data read from the file

		//checksum the file in blocks of 1024 bytes
		while ( ( l_len = static_cast <int>( fread( l_buffer, sizeof( char ), l_bufferSize, p_file ) ) ) > 0 )
		{
			l_MD5Checksum._update( l_buffer, l_len );
		}

		//finalise the checksum and return it
		return l_MD5Checksum._final();
	}
	catch ( ... )
	{
		return std::string();
	}
}

std::string  MD5::Hash( const unsigned char * p_buffer, unsigned int p_len )
{
	MD5 l_MD5Checksum;
	l_MD5Checksum._update( p_buffer, p_len );
	return l_MD5Checksum._final();
}

std::string  MD5::Hash( const std::string & p_string )
{
	return MD5::Hash( reinterpret_cast<const unsigned char * >( p_string.c_str() ), ( unsigned int )( p_string.length() ) );
}

unsigned long MD5::_rotateLeft( unsigned long p_x, int p_n )
{
	//rotate and return x
	return ( ( p_x << p_n ) | ( p_x >> ( 32 - p_n ) ) );
}

void MD5::FF( unsigned long & A,
			  unsigned long B, unsigned long C, unsigned long D,
			  unsigned long X, unsigned long S, unsigned long T )
{
	unsigned long F = ( ( B & C ) | ( ~B & D ) );
	A += F + X + T;
	A = _rotateLeft( A, S );
	A += B;
}

void MD5::GG( unsigned long & A,
			  unsigned long B, unsigned long C, unsigned long D,
			  unsigned long X, unsigned long S, unsigned long T )
{
	unsigned long G = ( ( B & D ) | ( C & ~D ) );
	A += G + X + T;
	A = _rotateLeft( A, S );
	A += B;
}

void MD5::HH( unsigned long & A,
			  unsigned long B, unsigned long C, unsigned long D,
			  unsigned long X, unsigned long S, unsigned long T )
{
	unsigned long H = ( B ^ C ^ D );
	A += H + X + T;
	A = _rotateLeft( A, S );
	A += B;
}

void MD5::II( unsigned long & A,
			  unsigned long B, unsigned long C, unsigned long D,
			  unsigned long X, unsigned long S, unsigned long T )
{
	unsigned long I = ( C ^ ( B | ~D ) );
	A += I + X + T;
	A = _rotateLeft( A, S );
	A += B;
}

void MD5::ByteToDWord( unsigned long * p_output, const unsigned char * p_input, unsigned int p_len )
{
	unsigned int i = 0; //index to p_output array
	unsigned int j = 0; //index to p_input array

	//transfer the data by shifting and copying
	for ( ; j < p_len ; i ++, j += 4 )
	{
		p_output[i] = static_cast <unsigned long>( p_input[j] )	|
					  static_cast <unsigned long>( p_input[j + 1] ) << 8		|
					  static_cast <unsigned long>( p_input[j + 2] ) << 16		|
					  static_cast <unsigned long>( p_input[j + 3] ) << 24;
	}
}

void MD5::_transform( const unsigned char p_block[64] )
{
	//initialise local data with current checksum
	unsigned long a = m_MD5[0];
	unsigned long b = m_MD5[1];
	unsigned long c = m_MD5[2];
	unsigned long d = m_MD5[3];
	//copy BYTES from input 'Block' to an array of ULONGS 'X'
	unsigned long X[16];
	ByteToDWord( X, p_block, 64 );
	//Perform Round 1 of the transformation
	FF( a, b, c, d, X[ 0], MD5_S11, MD5_T01 );
	FF( d, a, b, c, X[ 1], MD5_S12, MD5_T02 );
	FF( c, d, a, b, X[ 2], MD5_S13, MD5_T03 );
	FF( b, c, d, a, X[ 3], MD5_S14, MD5_T04 );
	FF( a, b, c, d, X[ 4], MD5_S11, MD5_T05 );
	FF( d, a, b, c, X[ 5], MD5_S12, MD5_T06 );
	FF( c, d, a, b, X[ 6], MD5_S13, MD5_T07 );
	FF( b, c, d, a, X[ 7], MD5_S14, MD5_T08 );
	FF( a, b, c, d, X[ 8], MD5_S11, MD5_T09 );
	FF( d, a, b, c, X[ 9], MD5_S12, MD5_T10 );
	FF( c, d, a, b, X[10], MD5_S13, MD5_T11 );
	FF( b, c, d, a, X[11], MD5_S14, MD5_T12 );
	FF( a, b, c, d, X[12], MD5_S11, MD5_T13 );
	FF( d, a, b, c, X[13], MD5_S12, MD5_T14 );
	FF( c, d, a, b, X[14], MD5_S13, MD5_T15 );
	FF( b, c, d, a, X[15], MD5_S14, MD5_T16 );
	//Perform Round 2 of the transformation
	GG( a, b, c, d, X[ 1], MD5_S21, MD5_T17 );
	GG( d, a, b, c, X[ 6], MD5_S22, MD5_T18 );
	GG( c, d, a, b, X[11], MD5_S23, MD5_T19 );
	GG( b, c, d, a, X[ 0], MD5_S24, MD5_T20 );
	GG( a, b, c, d, X[ 5], MD5_S21, MD5_T21 );
	GG( d, a, b, c, X[10], MD5_S22, MD5_T22 );
	GG( c, d, a, b, X[15], MD5_S23, MD5_T23 );
	GG( b, c, d, a, X[ 4], MD5_S24, MD5_T24 );
	GG( a, b, c, d, X[ 9], MD5_S21, MD5_T25 );
	GG( d, a, b, c, X[14], MD5_S22, MD5_T26 );
	GG( c, d, a, b, X[ 3], MD5_S23, MD5_T27 );
	GG( b, c, d, a, X[ 8], MD5_S24, MD5_T28 );
	GG( a, b, c, d, X[13], MD5_S21, MD5_T29 );
	GG( d, a, b, c, X[ 2], MD5_S22, MD5_T30 );
	GG( c, d, a, b, X[ 7], MD5_S23, MD5_T31 );
	GG( b, c, d, a, X[12], MD5_S24, MD5_T32 );
	//Perform Round 3 of the transformation
	HH( a, b, c, d, X[ 5], MD5_S31, MD5_T33 );
	HH( d, a, b, c, X[ 8], MD5_S32, MD5_T34 );
	HH( c, d, a, b, X[11], MD5_S33, MD5_T35 );
	HH( b, c, d, a, X[14], MD5_S34, MD5_T36 );
	HH( a, b, c, d, X[ 1], MD5_S31, MD5_T37 );
	HH( d, a, b, c, X[ 4], MD5_S32, MD5_T38 );
	HH( c, d, a, b, X[ 7], MD5_S33, MD5_T39 );
	HH( b, c, d, a, X[10], MD5_S34, MD5_T40 );
	HH( a, b, c, d, X[13], MD5_S31, MD5_T41 );
	HH( d, a, b, c, X[ 0], MD5_S32, MD5_T42 );
	HH( c, d, a, b, X[ 3], MD5_S33, MD5_T43 );
	HH( b, c, d, a, X[ 6], MD5_S34, MD5_T44 );
	HH( a, b, c, d, X[ 9], MD5_S31, MD5_T45 );
	HH( d, a, b, c, X[12], MD5_S32, MD5_T46 );
	HH( c, d, a, b, X[15], MD5_S33, MD5_T47 );
	HH( b, c, d, a, X[ 2], MD5_S34, MD5_T48 );
	//Perform Round 4 of the transformation
	II( a, b, c, d, X[ 0], MD5_S41, MD5_T49 );
	II( d, a, b, c, X[ 7], MD5_S42, MD5_T50 );
	II( c, d, a, b, X[14], MD5_S43, MD5_T51 );
	II( b, c, d, a, X[ 5], MD5_S44, MD5_T52 );
	II( a, b, c, d, X[12], MD5_S41, MD5_T53 );
	II( d, a, b, c, X[ 3], MD5_S42, MD5_T54 );
	II( c, d, a, b, X[10], MD5_S43, MD5_T55 );
	II( b, c, d, a, X[ 1], MD5_S44, MD5_T56 );
	II( a, b, c, d, X[ 8], MD5_S41, MD5_T57 );
	II( d, a, b, c, X[15], MD5_S42, MD5_T58 );
	II( c, d, a, b, X[ 6], MD5_S43, MD5_T59 );
	II( b, c, d, a, X[13], MD5_S44, MD5_T60 );
	II( a, b, c, d, X[ 4], MD5_S41, MD5_T61 );
	II( d, a, b, c, X[11], MD5_S42, MD5_T62 );
	II( c, d, a, b, X[ 2], MD5_S43, MD5_T63 );
	II( b, c, d, a, X[ 9], MD5_S44, MD5_T64 );
	//add the transformed values to the current checksum
	m_MD5[0] += a;
	m_MD5[1] += b;
	m_MD5[2] += c;
	m_MD5[3] += d;
}

MD5::MD5()
{
	std::memset( m_buffer, 0, 64 );
	m_count[0] = m_count[1] = 0;
	m_MD5[0] = MD5_INIT_STATE_0;
	m_MD5[1] = MD5_INIT_STATE_1;
	m_MD5[2] = MD5_INIT_STATE_2;
	m_MD5[3] = MD5_INIT_STATE_3;
}

void MD5::DWordToByte( unsigned char * p_output, unsigned long * p_input, unsigned int p_len )
{
	//transfer the data by shifting and copying
	unsigned int i = 0;
	unsigned int j = 0;

	for ( ; j < p_len; i++, j += 4 )
	{
		p_output[j]		= static_cast <unsigned char>( p_input[i] & 0xff );
		p_output[j + 1] = static_cast <unsigned char>( ( p_input[i] >> 8 ) & 0xff );
		p_output[j + 2] = static_cast <unsigned char>( ( p_input[i] >> 16 ) & 0xff );
		p_output[j + 3] = static_cast <unsigned char>( ( p_input[i] >> 24 ) & 0xff );
	}
}

std::string MD5::_final()
{
	//Save number of bits
	unsigned char l_bits[8];
	DWordToByte( l_bits, m_count, 8 );
	//Pad out to 56 mod 64.
	unsigned int l_index = static_cast <unsigned int>( ( m_count[0] >> 3 ) & 0x3f );
	unsigned int l_padLen = ( l_index < 56 ) ? ( 56 - l_index ) : ( 120 - l_index );
	_update( PADDING, l_padLen );
	//Append length (before padding)
	_update( l_bits, 8 );
	//Store final state in 'lpszMD5'
	const int l_MD5Size = 16;
	unsigned char l_MD5[l_MD5Size];
	DWordToByte( l_MD5, m_MD5, l_MD5Size );
	//Convert the hexadecimal checksum to a String
	std::string l_strMD5;

	for ( int i = 0 ; i < l_MD5Size ; i ++ )
	{
		char l_tmp[3];

		if ( l_MD5[i] == 0 )
		{
			l_strMD5 += "00";
			continue;
		}

		if ( l_MD5[i] <= 15 )
		{
			sprintf( l_tmp, "0%x", l_MD5[i] );

			for ( size_t l = 0 ; l < 2 ; l ++ )
			{
				l_strMD5 += l_tmp[l];
			}

			continue;
		}

		sprintf( l_tmp, "%x", l_MD5[i] );

		for ( size_t l = 0 ; l < 2 ; l ++ )
		{
			l_strMD5 += l_tmp[l];
		}
	}

	return l_strMD5;
}

void MD5::_update( const unsigned char * p_input, unsigned long p_len )
{
	//Compute number of bytes mod 64
	unsigned int l_index = static_cast <unsigned int>( ( m_count[0] >> 3 ) & 0x3F );

	//Update number of bits
	if ( ( m_count[0] += p_len << 3 )  < ( p_len << 3 ) )
	{
		m_count[1] ++;
	}

	m_count[1] += ( p_len >> 29 );
	//Transform as many times as possible.
	unsigned int i = 0;
	unsigned int l_partLen = 64 - l_index;

	if ( p_len >= l_partLen )
	{
		memcpy( & m_buffer[l_index], p_input, l_partLen );
		_transform( m_buffer );

		for ( i = l_partLen; i + 63 < p_len; i += 64 )
		{
			_transform( & p_input[i] );
		}

		l_index = 0;
	}
	else
	{
		i = 0;
	}

	// Buffer remaining input
	memcpy( & m_buffer[l_index], & p_input[i], p_len - i );
}
