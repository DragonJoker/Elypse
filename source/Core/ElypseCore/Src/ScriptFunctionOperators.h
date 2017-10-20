/*
See LICENSE file in root folder
*/
#ifndef ___EMUSE_SCRIPT_FUNCTIONS_OPERATORS___
#define ___EMUSE_SCRIPT_FUNCTIONS_OPERATORS___


#include "Module_Script.h"
#include "Module_Main.h"
#include "ScriptFunctions.h"

namespace Elypse
{
	namespace Script
	{
		template<typename T, typename U>
		void Ope_SubEqual( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) -= *std::get< 1 >( ret ) );
		}

		template<>
		void Ope_SubEqual< int, Real >( ScriptNode * caller )
		{
			std::tuple< int *, Real * > ret = GetAndExecParams< int, Real >( caller );
			ReturnAs< int >( caller, *std::get< 0 >( ret ) = int( *std::get< 0 >( ret ) - *std::get< 1 >( ret ) ) );
		}

		template<typename T, typename U>
		void Ope_MulEqual( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) *= *std::get< 1 >( ret ) );
		}

		template<>
		void Ope_MulEqual< int, Real >( ScriptNode * caller )
		{
			std::tuple< int *, Real * > ret = GetAndExecParams< int, Real >( caller );
			ReturnAs< int >( caller, *std::get< 0 >( ret ) = int( *std::get< 0 >( ret ) ** std::get< 1 >( ret ) ) );
		}

		template<typename T, typename U>
		void Ope_AddEqual( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) += *std::get< 1 >( ret ) );
		}

		template<>
		void Ope_AddEqual< int, Real >( ScriptNode * caller )
		{
			std::tuple< int *, Real * > ret = GetAndExecParams< int, Real >( caller );
			ReturnAs< int >( caller, *std::get< 0 >( ret ) = int( *std::get< 0 >( ret ) + *std::get< 1 >( ret ) ) );
		}

		template<typename T, typename U>
		void Ope_DivEqual( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) /= *std::get< 1 >( ret ) );
		}

		template<>
		void Ope_DivEqual< int, Real >( ScriptNode * caller )
		{
			std::tuple< int *, Real * > ret = GetAndExecParams< int, Real >( caller );
			ReturnAs< int >( caller, *std::get< 0 >( ret ) /= int( *std::get< 0 >( ret ) / *std::get< 1 >( ret ) ) );
		}

		template<typename T>
		void Ope_Set( ScriptNode * caller )
		{
			GetAndExecParams< T, T >( caller ); // Necessary call, to evaluate caller->m_childs[0] and caller->m_childs[1]
			caller->m_childs[0]->CopyValue_Deep( caller->m_childs[1] );
			caller->CopyValue_Ref( caller->m_childs[0]->m_value );
		}

		template<typename T>
		void Ope_Assign( ScriptNode * caller )
		{
			if ( caller->m_childs[0]->HasFunction() )
			{
				caller->m_childs[0]->Execute();
			}

			if ( caller->m_childs[1]->HasFunction() )
			{
				caller->m_childs[1]->Execute();
			}

			caller->m_childs[0]->CopyValue_Ref( caller->m_childs[1]->m_value );
			caller->CopyValue_Ref( caller->m_childs[0]->m_value );
		}


		template<typename T>
		void Ope_SetNull( ScriptNode * caller )
		{
			std::tuple< T * > ret = GetAndExecParams< T >( caller );
			ReturnAs< T >( caller, T( NULL ) );
		}

		template<typename T>
		void Ope_Compare( ScriptNode * caller )
		{
			std::tuple< T *, T * > ret = GetAndExecParams< T, T >( caller );
			ReturnAs< bool >( caller, *std::get< 0 >( ret ) == *std::get< 1 >( ret ) );
		}

		template<typename T>
		void Ope_CompareNull( ScriptNode * caller )
		{
			std::tuple< T * > ret = GetAndExecParams< T >( caller );
			ReturnAs< bool >( caller, *std::get< 0 >( ret ) == T( NULL ) );
		}

		template<typename T>
		void Ope_IsDiff( ScriptNode * caller )
		{
			std::tuple< T *, T * > ret = GetAndExecParams< T, T >( caller );
			ReturnAs< bool >( caller, *std::get< 0 >( ret ) != *std::get< 1 >( ret ) );
		}
		template<typename T>
		void Ope_IsDiffNull( ScriptNode * caller )
		{
			std::tuple< T * > ret = GetAndExecParams< T >( caller );
			ReturnAs< bool >( caller, *std::get< 0 >( ret ) != T( NULL ) );
		}

		template<typename R, typename T, typename U>
		void Ope_Add( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< R >( caller, *std::get< 0 >( ret ) + *std::get< 1 >( ret ) );
		}

		template<typename T>
		void Ope_Add( ScriptNode * caller )
		{
			std::tuple< T *, T * > ret = GetAndExecParams< T, T >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) + *std::get< 1 >( ret ) );
		}

		template<typename R, typename T, typename U>
		void Ope_Mul( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< R >( caller, *std::get< 0 >( ret ) ** std::get< 1 >( ret ) );
		}

		template<typename T>
		void Ope_Mul( ScriptNode * caller )
		{
			std::tuple< T *, T * > ret = GetAndExecParams< T, T >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) ** std::get< 1 >( ret ) );
		}

		template<typename R, typename T, typename U>
		void Ope_Div( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< R >( caller, *std::get< 0 >( ret ) / *std::get< 1 >( ret ) );
		}
		template<typename T>
		void Ope_Div( ScriptNode * caller )
		{
			std::tuple< T *, T * > ret = GetAndExecParams< T, T >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) / *std::get< 1 >( ret ) );
		}

		template<typename R, typename T, typename U>
		void Ope_Sub( ScriptNode * caller )
		{
			std::tuple< T *, U * > ret = GetAndExecParams< T, U >( caller );
			ReturnAs< R >( caller, *std::get< 0 >( ret ) - *std::get< 1 >( ret ) );
		}
		template<typename T>
		void Ope_Sub( ScriptNode * caller )
		{
			std::tuple< T *, T * > ret = GetAndExecParams< T, T >( caller );
			ReturnAs< T >( caller, *std::get< 0 >( ret ) - *std::get< 1 >( ret ) );
		}

	}
}

#endif
