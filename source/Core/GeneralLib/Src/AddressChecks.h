#ifndef ___ADDRESS_CHECK_H___
#define ___ADDRESS_CHECK_H___

#include <map>
#include <string>

#include <iostream>

#include "Macros.h"
#include "AutoSingleton.h"

namespace General
{
	namespace Utils
	{
		struct StaticPointerStatus
		{
			char * m_function;
			char * m_file;
			unsigned int m_line;
			void * m_address;

			StaticPointerStatus(	void * p_address, char * p_funcName,
									char * p_fileName, unsigned int p_lineNum )
				:	m_address( p_address ),
					m_function( p_funcName ),
					m_file( p_fileName ),
					m_line( p_lineNum )
			{}
		};

		class AddressChecker : public General::Theory::AutoSingleton <AddressChecker>
		{
		private:
			typedef std::map <std::string, StaticPointerStatus *> PtrMap;

		private:
			PtrMap m_variableMap;

		public:
			void RegisterVariable(	char * p_varName, void * p_address,
									char * p_functionName, char * p_filename,
									unsigned int p_lineNum )
			{
				std::string l_name = p_varName;
				PtrMap::iterator ifind = m_variableMap.find( l_name );

				if ( ifind == m_variableMap.end() )
				{
					m_variableMap.insert( PtrMap::value_type( l_name, new StaticPointerStatus(	p_address, p_functionName,
										  p_filename, p_lineNum ) ) );
				}
			}

			bool CheckVariable( char * p_varName, void * p_address,
								char * p_functionName, char * p_filename,
								unsigned int p_lineNum )
			{
				std::string l_name = std::string( p_varName );
				PtrMap::iterator ifind = m_variableMap.find( l_name );

				if ( ifind != m_variableMap.end() )
				{
					StaticPointerStatus * l_status = ifind->second;

					if ( l_status->m_address == p_address )
					{
						l_status->m_file = p_filename;
						l_status->m_line = p_lineNum;
						l_status->m_function = p_functionName;
						return true;
					}

					std::cout << "Possible Memory Corruption on " << l_name << " : old address : " << l_status->m_address << " became " << p_address << "\n";
					std::cout << "Between : " << l_status->m_function << "() in " << l_status->m_file << " @ line " << l_status->m_line << " --and-- ";
					std::cout << p_functionName << " in " << p_filename << "() @ line " << p_lineNum << "\n\n";
					l_status->m_address = p_address;
					l_status->m_file = p_filename;
					l_status->m_line = p_lineNum;
					l_status->m_function = p_functionName;
					return false;
				}

				return false;
			}

			void SetVariable(	char * p_varName, void * p_address,
								char * p_functionName, char * p_filename,
								unsigned int p_lineNum )
			{
				std::string l_name = std::string( p_varName );
				PtrMap::iterator ifind = m_variableMap.find( l_name );

				if ( ifind != m_variableMap.end() )
				{
					StaticPointerStatus * l_status = ifind->second;
					l_status->m_address = p_address;
					l_status->m_file = p_filename;
					l_status->m_line = p_lineNum;
					l_status->m_function = p_functionName;
				}
			}
		};
	}
}

#define EMVC_REGISTER_GLOBAL( p_var)		AddressChecker::GetSingletonPtr()->RegisterVariable( #p_var, p_var, __FUNCTION__, __FILE__, __LINE__)
#define EMVC_REGISTER_NAMED( p_var, p_name)	AddressChecker::GetSingletonPtr()->RegisterVariable( p_name, p_var, __FUNCTION__, __FILE__, __LINE__)
#define EMVC_CHECK_GLOBAL( p_var)			AddressChecker::GetSingletonPtr()->CheckVariable( #p_var, p_var, __FUNCTION__, __FILE__, __LINE__)
#define EMVC_CHECK_NAMED( p_var, p_name)	AddressChecker::GetSingletonPtr()->CheckVariable( p_name, p_var, __FUNCTION__, __FILE__, __LINE__)
#define EMVC_SET_GLOBAL( p_var)				AddressChecker::GetSingletonPtr()->SetVariable( #p_var, p_var, __FUNCTION__, __FILE__, __LINE__)
#define EMVC_SET_NAMED( p_var, p_name)		AddressChecker::GetSingletonPtr()->SetVariable( p_name, p_var, __FUNCTION__, __FILE__, __LINE__)

#endif
