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
#ifndef ___SCRIPT_NODE_H___
#define ___SCRIPT_NODE_H___

#include "Module_Script.h"
#include "ScriptNodeValue.h"

#include "Module_Data.h"
#include "ConfigFile.h"

#include <Exception.h>

#include <OgreStringConverter.h>

namespace Elypse
{
	namespace Script
	{
		class d_dll_export ScriptNode : d_noncopyable
		{
			friend ScriptCompiler;
		public:
			static unsigned int sm_numNodes;

			NodeValueBase * m_value;
			RawFunction * m_function;
			ConfigFile * m_file;

			bool m_ref;

			VariableType * m_type;

			ScriptNodeArray m_childs;

			UserFunction * m_userFunction;

			unsigned int m_numUses;
			unsigned int m_createdAtLine;

		public:
			ScriptNode( ConfigFile * p_file = NULL, unsigned int p_lineNum = 0 );
			~ScriptNode();

			void _reinit();
		private:
			void _deleteDetails( ScriptNodeArray & p_callStack );
			void _use( ScriptNodeArray & p_callStack );
			void _delete();

		public:
			void Use();
			void Delete();
			void CopyNode( ScriptNode * p_node );

			void SetType( VariableType * p_type );
			void ExecChild( size_t p_index );
			void CopyValue( ScriptNode * p_originalNode );
			void CopyValue_Deep( ScriptNode * p_originalNode );
			void CopyValue_Ref( NodeValueBase * p_value );
			bool IsBaseType( VariableType * p_type )const;
			bool IsType( VariableType * p_type )const
			{
				return p_type == m_type;
			}

			static NodeValueBase * CreateNodeValue( VariableType * p_type );

		public:
			inline void SetRefType( VariableType * p_type )
			{
				m_value = NULL ;
				m_ref = true ;
				m_type = p_type;
			}
			inline void AddChild( ScriptNode * p_child )
			{
				m_childs.push_back( p_child );
			}
			inline void ReserveChilds( unsigned int p_numChilds )
			{
				m_childs.reserve( p_numChilds );
			}
			inline void SetFunction( RawFunction * p_function )
			{
				m_function = p_function;
			}
			inline bool HasFunction()const
			{
				return m_function != NULL;
			}
			inline void Execute()
			{
				m_function( this );
			}
			inline size_t GetNumChilds()
			{
				return m_childs.size();
			}
			inline bool IsRef()const
			{
				return m_ref;
			}
			inline void SetNullRef()
			{
				CopyValue_Ref( NULL );
			}



			inline VariableType * GetType()const
			{
				return m_type;
			}
			VariableBaseType GetBaseType()const;

			inline void SetOrigin( ConfigFile * p_file, unsigned int p_lineNum )
			{
				m_file = p_file;
				m_createdAtLine = p_lineNum;
			}

		protected:
			template <typename T>
			static inline NodeValueBase * _createValueBase( const T & p_value = T() )
			{
				NodeValue <T> * l_value = new NodeValue <T> ();
				l_value->m_value = p_value;
				return l_value;
			}

		public:
			template <typename T>
			inline void set( const T & p_value = T() )
			{
				if ( m_value == NULL )
				{
					m_value = _createValueBase( p_value );
				}
				else
				{
					if ( ! m_value->IsType <T> () )
					{
						GENLIB_EXCEPTION( "Runtime ScriptError : set value on an already existing value, of different type // file : " + ( m_file ? m_file->GetName() : String( "unknown" ) ) + " line " + StringConverter::toString( m_createdAtLine ) );
					}
					else
					{
						( static_cast <NodeValue <T> *>( m_value ) )->m_value = p_value;
					}
				}
			}

			template <typename T>
			inline T & get()
			{
				if ( m_value != NULL )
				{
					if ( ! m_value->IsType<T>() )
					{
						GENLIB_EXCEPTION( "Runtime ScriptError : get value, type mismatched // file : " + ( m_file ? m_file->GetName() : String( "unknown" ) ) + " line " + StringConverter::toString( m_createdAtLine ) );
					}
					else
					{
						return ( static_cast <NodeValue <T> *>( m_value )->m_value );
					}
				}
				else
				{
					GENLIB_EXCEPTION( "Runtime ScriptError : get value, no value to return // file : " + ( m_file ? m_file->GetName() : String( "unknown" ) ) + " line " + StringConverter::toString( m_createdAtLine ) );
				}
			}

			template<typename T>
			inline T & getAndExecChild( size_t p_index )
			{
				ScriptNode * l_node = m_childs[p_index];

				if ( l_node->HasFunction() )
				{
					l_node->Execute();
				}

				return l_node->get <T> ();
			}
		};
	}
}

#endif
