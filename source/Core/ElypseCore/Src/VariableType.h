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
#ifndef ___EMUSE_VARIABLE_TYPE_H___
#define ___EMUSE_VARIABLE_TYPE_H___

#include "Module_Script.h"

namespace Elypse
{
	namespace Script
	{
		class d_dll_export VariableTypeManager : public AutoSingleton<VariableTypeManager>
		{
		protected:
			typedef std::map<String, VariableType *> TypeMap;

		protected:
			TypeMap m_types;
			std::map<VariableBaseType, std::string> m_baseTypeNames;

		public:
			VariableTypeManager();
			~VariableTypeManager();

		protected:
			String _hash( VariableBaseType p_type, VariableType * p_subType1 = NULL, VariableType * p_subType2 = NULL );

			VariableType * _add( VariableBaseType p_type, VariableType * p_subType1 = NULL, VariableType * p_subType2 = NULL, const String & p_hash = EMPTY_STRING );
			VariableType * _get( VariableBaseType p_type, VariableType * p_subType1, VariableType * p_subType2 );

			void _populateBaseTypeNames();

		public:
			static inline std::string GetBaseTypeName( VariableBaseType p_type )
			{
				return GetSingletonPtr()->m_baseTypeNames[p_type];
			}

			static inline VariableType * Get( VariableBaseType p_type, VariableType * p_subType1 = NULL, VariableType * p_subType2 = NULL )
			{
				return GetSingletonPtr()->_get( p_type, p_subType1, p_subType2 );
			}
		};

		class d_dll_export VariableType
		{
		public:
			VariableBaseType m_toplevelType;
			VariableTypeArray m_subTypes;
			Structure * m_struct;
			bool m_notConst;

		public:
			VariableType( VariableBaseType p_mainType, VariableType * p_subType1, VariableType * p_subType2 )
				: m_toplevelType( p_mainType ),
					m_struct( NULL )
			{
				if ( p_subType1 != NULL )
				{
					m_subTypes.push_back( p_subType1 );
				}

				if ( p_subType2 != NULL )
				{
					m_subTypes.push_back( p_subType2 );
				}
			}

			~VariableType()
			{
			}

		public:
			String GetDesc();
			void AddSubType( VariableType * p_type )
			{
				m_subTypes.push_back( p_type );
			}

		public:
			inline VariableBaseType GetBase()const
			{
				return m_toplevelType;
			}
			inline Structure * GetStruct()const
			{
				return m_struct;
			}
			inline VariableType * GetSubType( unsigned int p_index )const
			{
				if ( p_index < m_subTypes.size() )
				{
					return m_subTypes[p_index];
				}

				return NULL;
			}
		};
	}
}

#endif
