#ifndef ___GENERAL_FUNCTION_TRACKER___
#define ___GENERAL_FUNCTION_TRACKER___

#include <iostream>

#define d_track General::Utils::FunctionTracker l_tracker ## __LINE__ ## __COUNTER__( __FUNCTION__, __FILE__, __LINE__)

namespace General
{
	namespace Utils
	{
		class FunctionTracker
		{
			const char * m_file;
			const char * m_function;
			const unsigned int m_line;
		public:
			FunctionTracker( const char * p_function, const char * p_file, unsigned int p_line )
				:	m_function( p_function ),
					m_file( p_file ),
					m_line( p_line )
			{
				std::cout << "Entered Function : " << m_function << " in " << m_file << ", line " << m_line << std::endl;
			}
			~FunctionTracker()
			{
				std::cout << "Exited Function : " << m_function << " in " << m_file << ", line " << m_line << std::endl;
			}
		};
	}
}

#endif
