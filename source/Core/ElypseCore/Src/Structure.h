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
#ifndef ___EMUSE_SCRIPT_STRUCTURE_H___
#define ___EMUSE_SCRIPT_STRUCTURE_H___

#include "Module_Script.h"

namespace Elypse
{
	namespace Script
	{
		class StructRow : public named
		{
		protected:
			VariableType * m_type;

		public:
			StructRow( const String & p_name, VariableType * p_type )
				: named( p_name ),
					m_type( p_type )
			{
			}

		public:
			inline VariableType * GetType()const
			{
				return m_type;
			}
		};

		class Structure : public named
		{
		protected:
			StructRowArray m_members;
			VariableType * m_type;
			UserFunctionMap m_classFunctions;

		public:
			Structure( const String & p_name );
			~Structure();

		public:
			void AddMember( const String & p_name, VariableType * p_type );
			void AddFunction( UserFunction * p_function );

			UserFunction * GetFunction( const String & p_name )
			{
				return General::Utils::map::findOrNull( m_classFunctions, p_name );
			}

			unsigned int FindMember( const String & p_name )const;



			String GetDesc()const;

			inline VariableType * GetType()const
			{
				return m_type;
			}
		};

		class StructInstance
		{
		public:
			NodeValueBaseArray m_members;
			VariableType * m_definition;

		public:
			StructInstance( VariableType * p_def );
			~StructInstance();

			void Copy( StructInstance * p_origin );

			NodeValueBase * GetMember( unsigned int p_index )const
			{
				if ( m_members.size() > p_index )
				{
					return m_members[p_index];
				}

				std::cout << "OUT OF BOUNDS for get member : " << p_index << std::endl;
				return NULL;
			}
		};
	}
}

#endif
