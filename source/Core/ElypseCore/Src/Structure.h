/*
See LICENSE file in root folder
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
			StructRow( String const & p_name, VariableType * p_type )
				: named( p_name )
				, m_type( p_type )
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
			Structure( String const & p_name );
			~Structure();

		public:
			void AddMember( String const & p_name, VariableType * p_type );
			void AddFunction( UserFunction * p_function );

			UserFunction * GetFunction( String const & p_name )
			{
				return General::Utils::map::findOrNull( m_classFunctions, p_name );
			}

			uint32_t FindMember( String const & p_name )const;



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

			NodeValueBase * GetMember( uint32_t p_index )const
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
