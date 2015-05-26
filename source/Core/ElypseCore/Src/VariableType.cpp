#include "PrecompiledHeader.h"

#include "VariableType.h"
#include "Structure.h"

VariableTypeManager :: VariableTypeManager()
{
	_times( EMVT_NUM_TYPES )
	{
		_add( static_cast<VariableBaseType>( i ) );
	}
	_populateBaseTypeNames();
}

VariableTypeManager :: ~VariableTypeManager()
{
	General::Utils::map::deleteAll( m_types );
}

void VariableTypeManager :: _populateBaseTypeNames()
{
	m_baseTypeNames[EMVT_NULL]				 = "void";
	m_baseTypeNames[EMVT_INT]				 = "int";
	m_baseTypeNames[EMVT_REAL]				 = "real";
	m_baseTypeNames[EMVT_BOOL]				 = "boolean";
	m_baseTypeNames[EMVT_VECTOR3]			 = "vector3";
	m_baseTypeNames[EMVT_COLOUR]			 = "colour";
	m_baseTypeNames[EMVT_QUATERNION]		 = "quaternion";
	m_baseTypeNames[EMVT_CHAR]				 = "char";
	m_baseTypeNames[EMVT_STRING]			 = "string";
	m_baseTypeNames[EMVT_ENTITY]			 = "entity";
	m_baseTypeNames[EMVT_LIGHT]				 = "light";
	m_baseTypeNames[EMVT_CAMERA]			 = "camera";
	m_baseTypeNames[EMVT_NODE]				 = "scene_node";
	m_baseTypeNames[EMVT_OVERLAY]			 = "overlay";
	m_baseTypeNames[EMVT_ANI_GRP]			 = "animation_group";
	m_baseTypeNames[EMVT_ANI_OBJ]			 = "animated_object";
	m_baseTypeNames[EMVT_ANIMATION]			 = "animation";
	m_baseTypeNames[EMVT_PHY_OBJ]			 = "physics_object";
	m_baseTypeNames[EMVT_TIMER]				 = "timer";
	m_baseTypeNames[EMVT_CAMTEX]			 = "camtex";
	m_baseTypeNames[EMVT_PHY_MAT]			 = "physic_material";
	m_baseTypeNames[EMVT_OBJ_MAT]			 = "material";
	m_baseTypeNames[EMVT_ARRAY]				 = "array";
	m_baseTypeNames[EMVT_MAP]				 = "map";
	m_baseTypeNames[EMVT_INTMAP]			 = "int_map";
	m_baseTypeNames[EMVT_STRINGMAP]			 = "string_map";
	m_baseTypeNames[EMVT_REALMAP]			 = "real_map";
	m_baseTypeNames[EMVT_ITER]				 = "iterator";
	m_baseTypeNames[EMVT_SMITER]			 = "iterator";
	m_baseTypeNames[EMVT_RMITER]			 = "iterator";
	m_baseTypeNames[EMVT_IMITER]			 = "iterator";
	m_baseTypeNames[EMVT_PAIR]				 = "pair";
	m_baseTypeNames[EMVT_CODE]				 = "code";
	m_baseTypeNames[EMVT_NULLVALUE]			 = "NULL";
	m_baseTypeNames[EMVT_STRUCT]			 = "struct";
	m_baseTypeNames[EMVT_SUB1]				 = "*SUB1*";
	m_baseTypeNames[EMVT_SUB2]				 = "*SUB2*";
//	m_baseTypeNames[EMVT_SUB3]				 = "SUB3";
}

VariableType * VariableTypeManager :: _add( VariableBaseType p_type, VariableType * p_subType1, VariableType * p_subType2, const String & p_hash )
{
	VariableType * l_type = new VariableType( p_type, p_subType1, p_subType2 );
	const String & l_hash = ( p_hash.empty() ? _hash( p_type, p_subType1, p_subType2 ) : p_hash );
	m_types.insert( TypeMap::value_type( l_hash, l_type ) );
	return l_type;
}

String VariableTypeManager :: _hash( VariableBaseType p_type, VariableType * p_subType1, VariableType * p_subType2 )
{
	uint64_t l_hash[3];
	l_hash[0] = static_cast< uint64_t >( p_type );
	l_hash[1] = reinterpret_cast< uint64_t >( p_subType1 );
	l_hash[2] = reinterpret_cast< uint64_t >( p_subType2 );
	return	std::string( reinterpret_cast<const char *>( &l_hash[0] ), sizeof( l_hash ) );
}

VariableType * VariableTypeManager :: _get( VariableBaseType p_type, VariableType * p_subType1, VariableType * p_subType2 )
{
	const String & l_hash = _hash( p_type, p_subType1, p_subType2 );
	VariableType * l_vt = General::Utils::map::findOrNull( m_types, l_hash );

	if ( l_vt == NULL )
	{
		return _add( p_type, p_subType1, p_subType2, l_hash );
	}

	return l_vt;
}

String VariableType:: GetDesc()
{
	String l_desc = VariableTypeManager::GetBaseTypeName( m_toplevelType );

	if ( m_struct != NULL )
	{
		l_desc += " " + m_struct->GetName();
	}

	if ( ! m_subTypes.empty() )
	{
		l_desc += " <";
		_times( m_subTypes.size() )
		{
			l_desc += m_subTypes[i]->GetDesc() + ",";
		}
		l_desc.erase( l_desc.end() - 1 );
		l_desc += ">";
	}

	return l_desc;
}
