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
				, m_function( NULL )
				, m_return( NULL )
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
				, m_node( NULL )
				, m_returnNode( NULL )
				, m_finalNode( NULL )
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
