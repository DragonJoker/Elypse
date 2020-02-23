/*
See LICENSE file in root folder
*/
#ifndef ___EMUSE_SCRIPT_FUNCTION_H___
#define ___EMUSE_SCRIPT_FUNCTION_H___

#include "Module_Script.h"

namespace Elypse
{
	namespace Script
	{
		class d_dll_export Function : public named
		{
		public:
			RawFunction * m_function;
			VariableType * m_return;
			VariableTypeArray m_params;

		public:
			Function( String const & p_name )
				: named( p_name )
				, m_function( nullptr )
				, m_return( nullptr )
			{
			}
		};

		class d_dll_export OperatorFunction : public Function
		{
		public:
			bool m_unary;
			bool m_pre;

		public:
			OperatorFunction( String const & p_name )
				: Function( p_name )
				, m_unary( false )
				, m_pre( false )
			{
			}
		};

		class d_dll_export UserFunction : public Function
		{
		public:
			ScriptNode * m_node;
			ScriptNode * m_returnNode;
			ScriptNode * m_finalNode;

			ScriptNodeArray m_paramNodes;

			String m_name;
			bool m_initialised;
			ScriptNodeArrayArray m_deferredUses;
			ScriptNodeMap m_localVars;

		public:
			UserFunction( String const & p_name )
				: Function( p_name )
				, m_node( nullptr )
				, m_returnNode( nullptr )
				, m_finalNode( nullptr )
				, m_initialised( false )
			{}

			~UserFunction();

		public:
			void DeferUse( const ScriptNodeArray & p_array )
			{
				m_deferredUses.push_back( p_array );
			}
		};
	}
}

#endif
