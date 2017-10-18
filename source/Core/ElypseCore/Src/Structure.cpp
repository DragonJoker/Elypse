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
#include "PrecompiledHeader.h"
#include "Structure.h"
#include "VariableType.h"
#include "ScriptNodeValue.h"
#include "ScriptNode.h"
#include "ScriptCompiler.h"
#include "Function.h"


Structure::Structure( String const & p_name )
	: named( p_name )
{
	m_type = new VariableType( EMVT_STRUCT, NULL, NULL );
	m_type->m_struct = this;
}

Structure::~Structure()
{
	General::Utils::vector::deleteAll( m_members );
	General::Utils::map::deleteAll( m_classFunctions );
	delete m_type;
}

void Structure::AddMember( String const & p_name, VariableType * p_type )
{
	m_members.push_back( new StructRow( p_name, p_type ) );
	m_type->AddSubType( p_type );
}

void Structure::AddFunction( UserFunction * p_function )
{
	m_classFunctions.insert( std::make_pair( p_function->GetName(), p_function ) );
	p_function->m_params.push_back( m_type );
	ScriptNode * l_node = ScriptCompiler::GetSingletonPtr()->CreateScriptNode();
	l_node->SetType( m_type );
	l_node->Use();
	l_node->CopyValue_Ref( NULL );
	p_function->m_paramNodes.push_back( l_node );
	p_function->m_localVars.insert( std::make_pair( "this", l_node ) );
}

String Structure::GetDesc() const
{
	String l_desc = "struct " + m_name + "\n{\n";

	for ( auto & l_member : m_members )
	{
		l_desc += l_member->GetType()->GetDesc() + " " + l_member->GetName() + "\n";
	}

	l_desc += "}\n";
	return l_desc;
}

uint32_t Structure::FindMember( String const & p_name )const
{
	uint32_t i = 0;

	for ( auto & l_member : m_members )
	{
		if ( l_member->GetName() == p_name )
		{
			return i;
		}

		i++;
	}

	return -1;
}

StructInstance::StructInstance( VariableType * p_def )
	: m_definition( p_def )
{
	for ( auto & l_type : m_definition->m_subTypes )
	{
		m_members.push_back( ScriptNode::CreateNodeValue( l_type ) );
	}
}

StructInstance::~StructInstance()
{
	General::Utils::vector::deleteAll( m_members );
}

void StructInstance::Copy( StructInstance * p_origin )
{
	m_definition = p_origin->m_definition;
	General::Utils::vector::deleteAll( m_members );

	for ( auto & l_member : p_origin->m_members )
	{
		m_members.push_back( l_member->clone() );
	}
}
