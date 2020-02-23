/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ScriptEngine.h"
#include "ScriptCompiler.h"
#include "ScriptNode.h"
#include "ScriptBlock.h"
#include "Structure.h"

#include "ScriptFunctions.h"
#include "VariableType.h"

#include "ElypseLogs.h"
#include "CompileException.h"

#define COMPILE_ERROR_IN_BLOCK( p_desc, p_block)												\
	_error();																					\
	COMPILE_EXCEPTION( "Compiler Error : [" + _getScriptFileName() + " @ L# "					\
						+ StringConverter::toString( p_block->m_lineNumBegin)					\
						+ " ] -> " + p_desc )

#define COMPILE_WARNING_IN_BLOCK( p_desc, p_block)												\
	_warning();																					\
	_log( "Compiler Warning [ " + _getScriptFileName()						\
							+ " @ L# " + StringConverter::toString( p_block->m_lineNumBegin)	\
							+ " ] -> " + p_desc );												\

UserFunction * Elypse::Script::ScriptCompiler::_compileUserFunction( ScriptBlockArray & p_childs, bool p_predeclareOnly )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileUserFunction" );
//	_printBlockArray( "_compileUserFunction", p_childs);
	ScriptBlockArray l_array;
	uint32_t imax = static_cast <uint32_t>( p_childs.size() );
	uint32_t l_current = 0;

	do
	{
		l_array.push_back( p_childs[l_current] );
		l_current ++;
	}
	while ( imax > l_current && p_childs[l_current]->m_type != BT_STRING );

	VariableType * l_type = _getVariableType( l_array );
	bool l_ref = false;

	if ( l_array.back()->m_contents == "&" )
	{
		l_ref = true;
	}

	l_array.clear();
	String const & l_name = p_childs[l_current]->m_contents;
	UserFunction * l_function = GetUserFunction( l_name );
	l_current ++;

	if ( l_function == nullptr )
	{
		l_function = _createUserFunction( l_name, l_type );
		m_currentUserFunction = l_function;

		if ( l_ref )
		{
			l_function->m_returnNode->CopyValue_Ref( nullptr );
		}

		ScriptBlock * l_params = p_childs[l_current];
		uint32_t imax = static_cast <uint32_t>( l_params->m_childs.size() );
		uint32_t j = 0;

		while ( j < imax )
		{
			do
			{
				l_array.push_back( l_params->m_childs[j] );
				j ++;
			}
			while ( imax > j && l_params->m_childs[j]->m_type != BT_STRING );

			if ( ! l_array.empty() )
			{
				l_type = _getVariableType( l_array );
				l_ref = false;

				if ( l_array.back()->m_contents == "&" )
				{
					l_ref = true;
				}

				l_function->m_params.push_back( l_type );

				if ( ! p_predeclareOnly && j < ( imax ) )
				{
					ScriptNode * l_userVar = _createUserVariable( l_params->m_childs[j]->m_contents, l_type, true );

					if ( l_ref )
					{
						l_userVar->CopyValue_Ref( nullptr );
					}

					l_function->m_paramNodes.push_back( l_userVar );
					//std::cout << "added param : " << l_params->m_childs[j]->m_contents << " typed : " << l_type->GetDesc() << std::endl;
				}

				j ++;
				l_array.clear();
			}
		}
	}

	m_currentUserFunction = l_function;
	l_current ++;

	if ( l_function->m_node->m_userFunction == nullptr
			&& p_childs.size() > l_current && p_childs[l_current]->m_type == BT_BRACES )
	{
		if ( p_childs[l_current]->m_compiledScript != nullptr )
		{
			l_function->m_node->m_userFunction = l_function;
			l_function->m_node->CopyNode( p_childs[l_current]->m_compiledScript );
		}
	}

	return l_function;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileSentence( ScriptBlockArray & p_childs )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileSentence" );

	if ( p_childs.empty() )
	{
		return nullptr;
	}

//	_printBlockArray( "_compileSentence", p_childs);

	if ( p_childs.size() == 1 && p_childs[0]->m_compiledScript != nullptr )
	{
		return p_childs[0]->m_compiledScript;
	}

	uint32_t l_typeList = _getTypeList( p_childs );

	try
	{
		if ( l_typeList & BT_VARIABLE_TYPE )
		{
			if ( l_typeList & BT_TYPEDEF )
			{
				_typedef( p_childs );
			}
			else if ( l_typeList & BT_PARENTHESIS && l_typeList & BT_BRACES && l_typeList & BT_STRING )
			{
				_compileUserFunction( p_childs, true );
				return nullptr;
			}
			else if ( l_typeList & BT_OPERATOR || l_typeList & BT_PARENTHESIS )
			{
				if ( l_typeList & BT_PARENTHESIS )
				{
					bool l_equals = false;

					for ( size_t i = 0; i < p_childs.size() && !l_equals; ++i )
					{
						l_equals = p_childs[i]->m_contents == "=";
					}

					if ( l_equals )
					{
						_declareVariable( p_childs );
					}
					else
					{
						_compileUserFunction( p_childs, true );
					}
				}
				else
				{
					_declareVariable( p_childs );
				}

				return nullptr;
			}
			else
			{
				_declareVariable( p_childs );
				return nullptr;
			}
		}
		else if ( l_typeList & BT_RETURN )
		{
			return _compileReturn( p_childs );
		}
		else if ( l_typeList & BT_OPERATOR )
		{
			return _compileOperatedSentence( p_childs );
		}
		else if ( l_typeList & BT_BRACKETS )
		{
			return _compileBrakets( p_childs );
		}
		else if ( l_typeList & BT_STRUCT )
		{
			_compileStructDeclatation( p_childs );
			return nullptr;
		}
		else if ( l_typeList & BT_IF_ELSE )
		{
			return _compileIfThenElse( p_childs );
		}
		else if ( l_typeList & BT_STRING && l_typeList & BT_PARENTHESIS )
		{
			return _compileFunctionUse( p_childs );
		}
		else if ( p_childs.size() == 1 && p_childs[0]->m_type == BT_STRING )
		{
			return p_childs[0]->_compileString();
		}
		else if ( p_childs.size() == 1 && p_childs[0]->m_type == BT_PARENTHESIS )
		{
			return _compileSentence( p_childs[0]->m_childs );
		}
		else
		{
			if ( p_childs[0]->m_compiledScript != nullptr )
			{
				return p_childs[0]->m_compiledScript;
			}
			else
			{
				COMPILE_ERROR_IN_BLOCK( "This line seems to have a problem. Unknown type during a variable declaration, perhaps ?", p_childs[0] );
			}
		}
	}
	catch ( const GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ScriptCompiler::_compileSentence : exception caught : " + p_exception.GetDescription() );
	}

	return nullptr;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileFunctionUse( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileFunctionUse" );
	ScriptNode * l_functionUseNode = nullptr;

	if ( p_blockArray.size() < 2 )
	{
		COMPILE_ERROR_IN_BLOCK( "Unknown error, wrong number of parameters for function _compileFunctionUse", p_blockArray[0] );
		return nullptr;
	}

	if ( p_blockArray[0]->m_type != BT_STRING || p_blockArray[1]->m_type != BT_PARENTHESIS )
	{
		COMPILE_ERROR_IN_BLOCK( "Unknown error, bad parameters type for function _compileFunctionUse", p_blockArray[0] );
		return nullptr;
	}

	{
		UserFunction * l_function = GetUserFunction( p_blockArray[0]->m_contents );

		if ( l_function != nullptr )
		{
			return _compileUserFunctionUse( p_blockArray, l_function );
		}
	}

	Function * l_function = _getFunction( p_blockArray[0]->m_contents );

	if ( l_function != nullptr )
	{
		l_functionUseNode = _compileFuncParams( p_blockArray, l_function );
		return l_functionUseNode;
	}

	COMPILE_ERROR_IN_BLOCK( "Function [" + p_blockArray[0]->m_contents + "] not found", p_blockArray[0] );
	return nullptr;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileOperatedSentence( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileOperatedSentence" );

	if ( p_blockArray.empty() )
	{
		return nullptr;
	}

	/*
		_printBlockArray( "_compileOperatedSentence", p_blockArray);
	*/
	ScriptBlock * l_block = _getHighestOperator( p_blockArray );

	if ( l_block == nullptr )
	{
		COMPILE_ERROR_IN_BLOCK( "No operators found for the block in _compileOperatedSentence", p_blockArray.back() );
	}

	ScriptBlockArray l_rightOperands, l_leftOperands;
	ScriptNode * l_rightNode = nullptr, * l_leftNode = nullptr;
	bool l_rightPart = false;
	uint32_t imax = static_cast <uint32_t>( p_blockArray.size() );

	for ( uint32_t i = 0 ; i < imax ; i ++ )
	{
		if ( l_rightPart )
		{
			l_rightOperands.push_back( p_blockArray[i] );
		}
		else
		{
			if ( p_blockArray[i] == l_block )
			{
				l_rightPart = true;
			}
			else
			{
				l_leftOperands.push_back( p_blockArray[i] );
			}
		}
	}

	if ( l_block->m_contents == "." )
	{
		return _compileStructMember( l_leftOperands, l_rightOperands, l_block );
	}
	else if ( l_block->m_contents == "->" )
	{
		if ( l_leftOperands.empty() || l_rightOperands.empty() )
		{
			COMPILE_ERROR_IN_BLOCK( "Operands missing for operator [->] ", l_block );
		}

		l_leftNode = _compileSentence( l_leftOperands );

		if ( l_leftNode == nullptr )
		{
			COMPILE_ERROR_IN_BLOCK( "Compiled left operand missing for operator [->] ", l_block );
		}

		if ( l_rightOperands.size() < 2 )
		{
			COMPILE_ERROR_IN_BLOCK( "Compiled right operand missing for operator [->] ", l_block );
		}

		if ( l_leftNode->GetType()->GetBase() == EMVT_STRUCT )
		{
			UserFunction * l_function = l_leftNode->GetType()->GetStruct()->GetFunction( l_rightOperands[0]->m_contents );
			VERBOSE_COMPILATOR_INTERNAL( "_compileUserFunctionUse" );

			if ( l_function == nullptr )
			{
				COMPILE_ERROR_IN_BLOCK( "Function : " + l_rightOperands[0]->m_contents + " doesn't exist for class " + l_leftNode->GetType()->GetStruct()->GetName(), p_blockArray[0] );
				return nullptr;
			}

			if ( l_function->m_node != nullptr )
			{
				ScriptNode * l_userFunctionNode = _compileFuncParams( l_rightOperands, l_function, l_leftNode );

				if ( l_userFunctionNode != nullptr )
				{
					l_userFunctionNode->SetFunction( Gen_ExecuteFunction );

					if ( ! l_function->m_params.empty() )
					{
						//TODO : user function use with params
						ScriptNode * l_returnValue = l_function->m_returnNode;
						ScriptNode * l_numParamsNode = CreateScriptNode( l_rightOperands[0]->m_lineNumBegin );
						uint32_t l_numParams = static_cast <uint32_t>( l_function->m_params.size() );
						l_numParamsNode->SetType( VariableTypeManager::Get( EMVT_INT ) );
						l_numParamsNode->set<int>( l_numParams );
						ScriptNodeArray l_childsNodes = l_userFunctionNode->m_childs;
						l_userFunctionNode->m_childs.clear();
						l_userFunctionNode->AddChild( l_returnValue );
						l_userFunctionNode->AddChild( l_numParamsNode );

						for ( uint32_t i = 0 ; i < l_numParams ; i ++ )
						{
							l_userFunctionNode->AddChild( l_function->m_paramNodes[i] );
						}

						l_userFunctionNode->AddChild( l_function->m_node );

						for ( uint32_t i = 0 ; i < l_numParams ; i ++ )
						{
							l_userFunctionNode->AddChild( l_childsNodes[i] );
						}
					}
					else
					{
						ScriptNode * l_numParamsNode = CreateScriptNode( p_blockArray[0]->m_lineNumBegin );
						l_numParamsNode->SetType( VariableTypeManager::Get( EMVT_INT ) );
						l_numParamsNode->set <int> ( 0 );
						l_userFunctionNode->AddChild( l_function->m_returnNode );
						l_userFunctionNode->AddChild( l_numParamsNode );
						l_userFunctionNode->AddChild( l_function->m_node );
					}
				}

				return l_userFunctionNode;
			}

			COMPILE_ERROR_IN_BLOCK( "Trying to use an empty / undeclared function", p_blockArray[0] );
			return nullptr;
//			return _compileUserFunctionUse();
		}

		Function * l_function = _getClassFunction( l_leftNode->GetType(), l_rightOperands[0]->m_contents );

		if ( l_function != nullptr )
		{
//			std::cout << "Left node of ->  : " << l_leftNode->GetType()->GetDesc() << std::endl;
			return _compileFuncParams( l_rightOperands, l_function, l_leftNode );
		}
		else
		{
//			std::cout << "operator -> : " << l_leftNode->GetType()->GetDesc() << std::endl;
			COMPILE_ERROR_IN_BLOCK( "Class function [" + l_rightOperands[0]->m_contents + "] of class "
									+ VariableTypeManager::GetBaseTypeName( l_leftNode->GetBaseType() ) + " not found", l_block );
		}
	}
	else
	{
		if ( ! l_rightOperands.empty() )
		{
			l_rightNode = _compileSentence( l_rightOperands );
		}

		if ( ! l_leftOperands.empty() )
		{
			l_leftNode = _compileSentence( l_leftOperands );
		}

		return _getOperator( l_block, l_leftNode, l_rightNode );
	}

	return nullptr;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileBrakets( ScriptBlockArray & p_blockArray )
{
	if ( p_blockArray.size() == 1 )
	{
		return _compileSentence( p_blockArray[0]->m_childs );
	}

	return nullptr;
}

ScriptBlock * Elypse::Script::ScriptCompiler::_getHighestOperator( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_getHighestOperator" );
	OperatorLevel l_level = SO_NONE;
	ScriptBlock * l_block = nullptr;
	uint32_t imax = static_cast <uint32_t>( p_blockArray.size() );

	for ( uint32_t i = 0 ; i < imax ; i ++ )
	{
		if ( p_blockArray[i]->m_type == BT_OPERATOR )
		{
			if ( l_level > p_blockArray[i]->m_operatorLevel )
			{
				l_level = p_blockArray[i]->m_operatorLevel;
				l_block = p_blockArray[i];
			}
			else if ( l_level == p_blockArray[i]->m_operatorLevel && p_blockArray[i]->m_operator_rightToLeft )
			{
				l_block = p_blockArray[i];
			}
		}
	}

	return l_block;
}

void Elypse::Script::ScriptCompiler::_compileFuncParamsWithinParenthesis( const ScriptBlockArray & p_blockArray, ScriptNodeArray & p_compiledNodes )
{
	ScriptBlockArray l_currentArray;
	size_t i = 0;

	for ( auto & l_block : p_blockArray )
	{
		if ( l_block->m_type == BT_SEPARATOR || i == ( p_blockArray.size() - 1 ) )
		{
			if ( i == ( p_blockArray.size() - 1 ) )
			{
				l_currentArray.push_back( l_block );
			}

			ScriptNode * l_node = _compileSentence( l_currentArray );
			l_currentArray.clear();

			if ( l_node != nullptr )
			{
				p_compiledNodes.push_back( l_node );
			}
		}
		else
		{
			l_currentArray.push_back( l_block );
		}

		++i;
	}
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileFuncParams( ScriptBlockArray & p_blockArray, Function * p_function , ScriptNode * p_classInstance )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileFunctionParameters" );
//	_printBlockArray( "_compileFunctionParameters for ( " + p_function->GetName() + " )", p_blockArray);
//	_printBlockArray( "_compileFunctionParameters", p_blockArray[1]->m_childs);
	ScriptNode * l_finalNode = CreateScriptNode( p_blockArray[0]->m_lineNumBegin );
	l_finalNode->SetFunction( p_function->m_function );
	VariableType * l_retType = _substituteVarType( p_function->m_return, ( p_classInstance != nullptr ? p_classInstance->GetType() : nullptr ) );
	l_finalNode->SetType( l_retType );
	uint32_t l_functionParamIndex = 0;

	if ( p_function->m_params.size() == 0 )
	{
		if ( ! p_blockArray.empty() )
		{
			if ( !( p_blockArray.size() > 1 && p_blockArray[1]->m_type == BT_PARENTHESIS && p_blockArray[1]->m_childs.size() == 0 ) )
			{
				COMPILE_ERROR_IN_BLOCK( "This function does not take any parameters : " + p_blockArray[0]->m_contents, p_blockArray[0] );
			}
		}

		return l_finalNode;
	}

	if ( p_classInstance != nullptr )
	{
		l_finalNode->AddChild( p_classInstance );
		l_functionParamIndex = 1;
	}

	uint32_t imax = static_cast <uint32_t>( p_blockArray.size() );

	for ( size_t i = 0; i < p_blockArray.size(); ++i )
	{
		ScriptBlock * l_block = p_blockArray[i];

		if ( l_block->m_type == BT_BRACES )
		{
			ScriptNode * l_node = l_block->m_compiledScript;

			if ( l_node == nullptr )
			{
				_releaseNode( l_finalNode );
//				delete l_finalNode;
				COMPILE_ERROR_IN_BLOCK( "Empty brace parameter for function : " + p_function->GetName(), l_block );
				return nullptr;
			}

			l_finalNode->AddChild( l_node );
			l_functionParamIndex ++;
			break; // Last param only.
		}

		if ( l_block->m_type == BT_PARENTHESIS )
		{
			ScriptNodeArray l_nodes;
			_compileFuncParamsWithinParenthesis( l_block->m_childs, l_nodes );

			for ( auto l_node : l_nodes )
			{
				if ( l_node != nullptr && p_function->m_params.size() > l_functionParamIndex )
				{
					VariableType * l_type = _substituteVarType( p_function->m_params[l_functionParamIndex], ( p_classInstance != nullptr ? p_classInstance->GetType() : ( !p_function->m_params.empty() ? p_function->m_params[0] : nullptr ) ) );

					if ( l_node->GetType() != l_type && l_type->GetBase() != EMVT_CODE )
					{
						_releaseNode( l_finalNode ); //delete l_finalNode;
						COMPILE_ERROR_IN_BLOCK( "Wrong parameter type for function : " + p_blockArray[0]->m_contents
												+ " -> expected : " + l_type->GetDesc()
												+ " but found : " + l_node->GetType()->GetDesc(), l_block );
						return nullptr;
					}

					l_finalNode->AddChild( l_node );
					l_functionParamIndex ++;
				}
			}
		}
	}

	if ( l_functionParamIndex != p_function->m_params.size() )
	{
		COMPILE_ERROR_IN_BLOCK( "Wrong number of params : found " + StringConverter::toString( l_functionParamIndex ) + " while expecting "
								+ StringConverter::toString( p_function->m_params.size() ) + " for function : "
								+ p_function->GetName(), p_blockArray[0] );
		return nullptr;
	}

	return l_finalNode;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileUserFunctionUse( ScriptBlockArray & p_blockArray, UserFunction * p_userFunction )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileUserFunctionUse" );

	if ( p_userFunction->m_node != nullptr )
	{
		ScriptNode * l_userFunctionNode = _compileFuncParams( p_blockArray, p_userFunction );

		if ( l_userFunctionNode != nullptr )
		{
			l_userFunctionNode->SetFunction( Gen_ExecuteFunction );

			if ( ! p_userFunction->m_params.empty() )
			{
				//TODO : user function use with params
				ScriptNode * l_returnValue = p_userFunction->m_returnNode;
				ScriptNode * l_numParamsNode = CreateScriptNode( p_blockArray[0]->m_lineNumBegin );
				uint32_t l_numParams = static_cast <uint32_t>( p_userFunction->m_params.size() );
				l_numParamsNode->SetType( VariableTypeManager::Get( EMVT_INT ) );
				l_numParamsNode->set<int>( l_numParams );
				ScriptNodeArray l_childsNodes = l_userFunctionNode->m_childs;
				l_userFunctionNode->m_childs.clear();
				l_userFunctionNode->AddChild( l_returnValue );
				l_userFunctionNode->AddChild( l_numParamsNode );

				for ( uint32_t i = 0 ; i < l_numParams ; i ++ )
				{
					l_userFunctionNode->AddChild( p_userFunction->m_paramNodes[i] );
				}

				l_userFunctionNode->AddChild( p_userFunction->m_node );

				for ( uint32_t i = 0 ; i < l_numParams ; i ++ )
				{
					l_userFunctionNode->AddChild( l_childsNodes[i] );
				}
			}
			else
			{
				ScriptNode * l_numParamsNode = CreateScriptNode( p_blockArray[0]->m_lineNumBegin );
				l_numParamsNode->SetType( VariableTypeManager::Get( EMVT_INT ) );
				l_numParamsNode->set <int> ( 0 );
				l_userFunctionNode->AddChild( p_userFunction->m_returnNode );
				l_userFunctionNode->AddChild( l_numParamsNode );
				l_userFunctionNode->AddChild( p_userFunction->m_node );
			}
		}

		return l_userFunctionNode;
	}

	COMPILE_ERROR_IN_BLOCK( "Trying to use an empty / undeclared function", p_blockArray[0] );
	return nullptr;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileIfThenElse( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileIfThenElse" );
	ScriptBlockArray l_array = p_blockArray;

	while ( l_array.back()->m_type == BT_SEPARATOR )
	{
		l_array.pop_back();
	}

	ScriptNode * l_finalNode = CreateScriptNode( p_blockArray[0]->m_lineNumBegin );
	ScriptNode * l_multiIfNode = nullptr;
	bool l_multipleif = false;
	bool l_if = false;
	l_finalNode->SetFunction( Gen_IfBlock );
	l_finalNode->SetType( VariableTypeManager::Get( EMVT_CODE ) );

	while ( ! l_array.empty() )
	{
		if ( l_array[0]->m_type != BT_IF_ELSE )
		{
			COMPILE_ERROR_IN_BLOCK( "Missing semi column", l_array[0] );
		}

		if ( l_array.size() == 2 )
		{
			ScriptNode * l_node = l_array[1]->Compile();

			if ( l_node )
			{
				l_finalNode->AddChild( l_node );
			}
			else
			{
				//WARNING : miaou ?
			}

			l_array.erase( l_array.begin(), l_array.begin() + 2 );
			//else
		}
		else	if ( l_array.size() >= 3
					 && l_array[0]->m_type == BT_IF_ELSE
					 && l_array[1]->m_type == BT_PARENTHESIS
					 && l_array[2]->m_type == BT_BRACES )
		{
			if ( l_if == false && l_array[0]->m_contents != "if" )
			{
				COMPILE_ERROR_IN_BLOCK( "Cannot start an if/elseif/else with anything other than an if statement", l_array[0] );
			}
			else if ( l_if && l_array[0]->m_contents == "if" )
			{
				if ( ! l_multipleif )
				{
					l_multiIfNode = CreateScriptNode( l_array[0]->m_lineNumBegin );
					l_multipleif = true;
					l_multiIfNode->SetFunction( Gen_MultiLineFunction );
					l_multiIfNode->SetType( VariableTypeManager::Get( EMVT_CODE ) );
				}

				l_finalNode = CreateScriptNode( l_array[0]->m_lineNumBegin );
				l_multiIfNode->AddChild( l_finalNode );
				l_finalNode->SetFunction( Gen_IfBlock );
				l_finalNode->SetType( VariableTypeManager::Get( EMVT_CODE ) );
			}
			else if ( l_array[0]->m_contents == "if" )
			{
				l_if = true;
			}

			ScriptNode * l_node = l_array[1]->Compile();

			if ( l_node != nullptr )
			{
				if ( l_node->GetBaseType() == EMVT_BOOL )
				{
					ScriptNode * l_node2 = l_array[2]->Compile();

					if ( l_node2 != nullptr )
					{
						l_finalNode->AddChild( l_node );
						l_finalNode->AddChild( l_node2 );
					}
					else
					{
						COMPILE_ERROR_IN_BLOCK( "No code block compiled for the if/elseif statement", l_array[0] );
					}
				}
				else
				{
					COMPILE_ERROR_IN_BLOCK( "The condition within an if statement must be boolean", l_array[0] );
				}
			}
			else
			{
				COMPILE_ERROR_IN_BLOCK( "No condition node found for the if/elseif statement", l_array[0] );
			}

			l_array.erase( l_array.begin(), l_array.begin() + 3 );
		}
		else
		{
			COMPILE_ERROR_IN_BLOCK( "If/elseif/else statement with wrong / empty parameters / code blocks", l_array[0] );
			l_array.clear();
		}
	}

	VERBOSE_COMPILATOR_INTERNAL( "_compileIfThenElse END" );

	if ( l_multipleif )
	{
		l_multiIfNode->AddChild( l_finalNode );
		return l_multiIfNode;
	}

	return l_finalNode;
}

void Elypse::Script::ScriptCompiler::_declareVariable( ScriptBlockArray & p_childs )
{
	VERBOSE_COMPILATOR_INTERNAL( "_declareVariable" );
	ScriptBlockArray l_array;
	uint32_t imax = static_cast <uint32_t>( p_childs.size() );
	uint32_t l_current = 0;

	do
	{
		l_array.push_back( p_childs[l_current] );
		l_current ++;
	}
	while ( imax > l_current && p_childs[l_current]->m_type != BT_STRING );

	VariableType * l_type = _getVariableType( l_array );
	l_array.clear();

	for ( uint32_t i = l_current ; i < imax ; i ++ )
	{
		if ( p_childs[i]->m_type == BT_SEPARATOR )
		{
			_declareVariableDetail( l_type, l_array );
			l_array.clear();
		}
		else
		{
			l_array.push_back( p_childs[i] );
		}
	}

	if ( ! l_array.empty() )
	{
		_declareVariableDetail( l_type, l_array );
	}
}

void Elypse::Script::ScriptCompiler::_declareVariableDetail( VariableType * p_type, ScriptBlockArray & p_array )
{
	VERBOSE_COMPILATOR_INTERNAL( "_declareVariableDetail" );
	genlib_assert( p_array.size() > 0 );
	String const & l_variableName = p_array[0]->m_contents;
	ScriptNode * l_node = _getUserVariable( l_variableName );

	if ( l_node != nullptr && ( l_node->GetType() != p_type ) )
	{
		COMPILE_ERROR_IN_BLOCK( "Error while declaring variable " + l_variableName + ", it has already been declared with another type", p_array[0] );
		return;
	}

	if ( l_node != nullptr )
	{
		//TODO double definition of a variable. allowed ?
		return;
	}

	/*
	if (p_array.size() == 2 && p_array[1]->m_type == BT_OPERATOR && p_array[1]->m_contents == "*")
	{
		l_node = _createUserVariable( l_variableName, VariableTypeManager::Get( EMVT_ARRAY, p_type));
		return;
	}
	*/
	l_node = _createUserVariable( l_variableName, p_type );

	if ( p_array.size() > 2 && p_array[1]->m_type == BT_OPERATOR && p_array[1]->m_contents == "=" )
	{
		ScriptBlockArray l_array( p_array.begin() + 2, p_array.end() );
		ScriptNode * l_valNode = _compileSentence( l_array );

		if ( l_valNode != nullptr )
		{
			l_valNode->Use();

			if ( l_valNode->GetType() != p_type )
			{
				COMPILE_ERROR_IN_BLOCK( "Variable definition for [" + l_variableName
										+ "] failed : initialisation between different types / subtypes", l_array[0] );
				return;
			}
			else
			{
				if ( l_valNode->HasFunction() )
				{
					l_valNode->Execute();
				}

				l_node->CopyValue( l_valNode );
			}

			l_valNode->Delete();
		}
	}
}

ScriptNode * Elypse::Script::ScriptCompiler::_getOperator( ScriptBlock * p_operator, ScriptNode * p_leftOperand, ScriptNode * p_rightOperand )
{
	VERBOSE_COMPILATOR_INTERNAL( "_getOperator" );
	ScriptNode * l_returnValue = nullptr;
	OperatorFunctionMultiMap::iterator iter = m_operators.find( p_operator->m_contents );

	if ( iter == m_operators.end() )
	{
		COMPILE_ERROR_IN_BLOCK( "Operator not recognised : " + p_operator->m_contents, p_operator );
		return nullptr;
	}

	const OperatorFunctionMultiMap::iterator & iend = m_operators.end();
	bool l_found = false;
	OperatorFunction * l_function;

	while ( iter != iend && iter->first == p_operator->m_contents )
	{
		l_function = iter->second;

		if ( l_function->m_unary )
		{
			if ( l_function->m_pre )
			{
				if ( p_rightOperand != nullptr && p_rightOperand->IsBaseType( l_function->m_params[0] ) )
				{
					l_found = true;
					break;
				}
			}
			else
			{
				if ( p_leftOperand != nullptr && p_leftOperand->IsBaseType( l_function->m_params[0] ) )
				{
					l_found = true;
					break;
				}
			}
		}
		else
		{
			if ( p_leftOperand && p_rightOperand )
			{
				if ( p_leftOperand->IsBaseType( l_function->m_params[0] )
						&&	p_rightOperand->IsBaseType( l_function->m_params[1] ) )
				{
					l_found = true;
					break;
				}
			}
		}

		++ iter;
	}

	if ( l_found )
	{
		l_returnValue = CreateScriptNode( p_operator->m_lineNumBegin );
		l_returnValue->SetType( _substituteVarType( l_function->m_return, ( p_leftOperand != nullptr ? p_leftOperand->GetType() : nullptr ) ) );

		if ( l_function->m_unary )
		{
			if ( l_function->m_pre )
			{
				l_returnValue->AddChild( p_rightOperand );
			}
			else
			{
				l_returnValue->AddChild( p_leftOperand );
			}
		}
		else
		{
			l_returnValue->AddChild( p_leftOperand );
			l_returnValue->AddChild( p_rightOperand );
		}

		l_returnValue->SetFunction( l_function->m_function );
	}
	else
	{
		String l_error;
		l_error = "Overload not found for operator : ";

		if ( p_leftOperand != nullptr )
		{
			l_error += " [" + p_leftOperand->GetType()->GetDesc() + "] ";
		}

		l_error += p_operator->m_contents;

		if ( p_rightOperand != nullptr )
		{
			l_error += " [" + p_rightOperand->GetType()->GetDesc() + "] ";
		}

		COMPILE_ERROR_IN_BLOCK( l_error, p_operator );
	}

	if ( p_leftOperand != nullptr && p_rightOperand != nullptr
			&&	p_leftOperand->GetType()->GetBase() == p_rightOperand->GetType()->GetBase()
			&&	p_leftOperand->GetType() != p_rightOperand->GetType()
	   )
	{
		String l_error;
		l_error = "Problematic parameters for operator " + p_operator->m_contents + " : ";
		l_error += " [" + p_leftOperand->GetType()->GetDesc() + "] against";
		l_error += " [" + p_rightOperand->GetType()->GetDesc() + "] ";
		COMPILE_ERROR_IN_BLOCK( l_error, p_operator );
	}

	return l_returnValue;
}

VariableType * Elypse::Script::ScriptCompiler::_getVariableType( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_getVariableType" );

	if ( p_blockArray.size() == 1 ) //simple types are most likely
	{
		return p_blockArray[0]->m_variableType;
	}

	ScriptBlockArray l_array;

	for ( auto & l_block : p_blockArray )
	{
		if ( l_block->m_type == BT_VARIABLE_TYPE )
		{
			l_array.push_back( l_block );
		}
	}

	uint32_t l_index = 0;
	return _getVariableTypeRecus( l_array, l_index );
}

VariableType * Elypse::Script::ScriptCompiler::_getVariableTypeRecus( ScriptBlockArray & p_blockArray, uint32_t & p_recursIndex )
{
	VERBOSE_COMPILATOR_INTERNAL( "_getVariableTypeRecus" );

	if ( p_blockArray.empty() )
	{
		return nullptr;
	}

	if ( p_recursIndex >= p_blockArray.size() )
	{
		COMPILE_ERROR_IN_BLOCK( "Incomplete type declaration", p_blockArray[0] );
		return nullptr;
	}

	VariableType * l_type = p_blockArray[p_recursIndex]->m_variableType;
	VariableType * l_sub1 = l_type->GetSubType( 0 ), *l_sub2 = l_type->GetSubType( 1 );
	VariableBaseType l_btype = l_type->GetBase();
	p_recursIndex ++;

	if ( l_btype == EMVT_MAP )
	{
		l_sub1 = _getVariableTypeRecus( p_blockArray, p_recursIndex );
		l_sub2 = _getVariableTypeRecus( p_blockArray, p_recursIndex );

		if ( l_sub1 != nullptr && l_sub1->GetBase() == EMVT_STRING )
		{
			l_btype = EMVT_STRINGMAP;
		}
		else if ( l_sub1 != nullptr && l_sub1->GetBase() == EMVT_INT )
		{
			l_btype = EMVT_INTMAP;
		}
		else if ( l_sub1 != nullptr && l_sub1->GetBase() == EMVT_REAL )
		{
			l_btype = EMVT_REALMAP;
		}

		l_sub1 = l_sub2;
		l_sub2 = nullptr;
	}
	else if ( l_btype == EMVT_ARRAY
			  || l_btype == EMVT_INTMAP
			  || l_btype == EMVT_REALMAP
			  || l_btype == EMVT_STRINGMAP )
	{
		if ( l_sub1 == nullptr )
		{
			l_sub1 = _getVariableTypeRecus( p_blockArray, p_recursIndex );
		}
	}
	else if ( l_btype == EMVT_PAIR )
	{
		if ( l_sub1 == nullptr || l_sub2 == nullptr )
		{
			l_sub1 = _getVariableTypeRecus( p_blockArray, p_recursIndex );
			l_sub2 = _getVariableTypeRecus( p_blockArray, p_recursIndex );
		}
	}
	else if ( l_btype == EMVT_ITER && l_sub1 == nullptr )
	{
		l_sub1 = _getVariableTypeRecus( p_blockArray, p_recursIndex );

		if ( l_sub1->GetBase() == EMVT_INTMAP )
		{
			l_btype = EMVT_IMITER;
			l_sub2 = l_sub1->GetSubType( 0 );
		}
		else if ( l_sub1->GetBase() == EMVT_REALMAP )
		{
			l_btype = EMVT_RMITER;
			l_sub2 = l_sub1->GetSubType( 0 );
		}
		else if ( l_sub1->GetBase() == EMVT_STRINGMAP )
		{
			l_btype = EMVT_SMITER;
			l_sub2 = l_sub1->GetSubType( 0 );
		}
		else
		{
			COMPILE_ERROR_IN_BLOCK( "Can only declare iterators on maps", p_blockArray[0] );
		}
	}

	if ( l_btype != EMVT_STRUCT )
	{
		l_type = VariableTypeManager::Get( l_btype, l_sub1, l_sub2 );
	}

	return l_type;
}

void Elypse::Script::ScriptCompiler::_compileStructDeclatation( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileStructDeclatation" );
//	_printBlockArray( "_compileStructDeclatation", p_blockArray);

	if ( p_blockArray.size() != 3 )
	{
		COMPILE_ERROR_IN_BLOCK( "problem while declaring struct, need 3 blocks, found " + StringConverter::toString( p_blockArray.size() ), p_blockArray[0] );
//		std::cout << "not normal, sized : " << p_blockArray.size() << std::endl;
		return;
	}

	String const & l_name = p_blockArray[1]->m_contents;
//	genlib_assert( ! General::Utils::map::has( m_structures, l_name));
	Structure * l_struct = General::Utils::map::findOrNull( m_structures, l_name );

	if ( l_struct == nullptr )
	{
		l_struct = new Structure( l_name );
	}

//	std::cout << "Struct named : " << l_name << std::endl;
	ScriptBlock * l_block = p_blockArray[2];
	ScriptBlockArray l_array;

	for ( auto & l_child : l_block->m_childs )
	{
		if ( l_child->m_type == BT_SEPARATOR )
		{
			_addStructMember( l_struct, l_array );
			l_array.clear();
		}
		else
		{
			l_array.push_back( l_child );
		}
	}

	if ( ! l_array.empty() )
	{
		//Error, missing ; somewhere within that shit;
	}

	m_typedefs.insert( std::make_pair( l_name, l_struct->GetType() ) );
	m_structures.insert( std::make_pair( l_name, l_struct ) );
}

void Elypse::Script::ScriptCompiler::_addStructMember( Structure * p_struct, ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_addStructMember" );
	ScriptBlockArray l_array;
	std::size_t l_current = 0;
	std::size_t imax = p_blockArray.size();

	do
	{
		l_array.push_back( p_blockArray[l_current] );
		l_current ++;
	}
	while ( imax > l_current && p_blockArray[l_current]->m_type != BT_STRING );

	if ( l_current >= imax )
	{
		//Error, no var name ?
	}

	VariableType * l_type = _getVariableType( l_array );
	p_struct->AddMember( p_blockArray[l_current]->m_contents, l_type );
}

void Elypse::Script::ScriptCompiler::_typedef( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_typedef" );

	if ( p_blockArray.size() < 3 )
	{
		return;
	}

	String const & l_name = p_blockArray[p_blockArray.size() - 1]->m_contents;
	ScriptBlockArray l_array( p_blockArray.begin() + 1, p_blockArray.begin() + p_blockArray.size() - 2 );
	VariableType * l_type = _getVariableType( l_array );
	m_typedefs.insert( std::make_pair( l_name, l_type ) );
}

ScriptNode  * Elypse::Script::ScriptCompiler::_compileStructMember( ScriptBlockArray & p_left, ScriptBlockArray & p_right, ScriptBlock * p_block )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileStructMember" );
//	_printBlockArray( "_compileStructMember - Left", p_left);
//	_printBlockArray( "_compileStructMember - Right", p_right);

	if ( p_left.empty() || p_right.empty() )
	{
		COMPILE_ERROR_IN_BLOCK( "Operands missing for operator [.] ", p_block );
		return nullptr;
	}

	ScriptNode * l_leftNode = _compileSentence( p_left );

	if ( l_leftNode == nullptr )
	{
		COMPILE_ERROR_IN_BLOCK( "Compiled left operand missing for operator [.] ", p_block );
		return nullptr;
	}

	if ( l_leftNode->GetBaseType() != EMVT_STRUCT || l_leftNode->GetType()->GetStruct() == nullptr )
	{
		COMPILE_ERROR_IN_BLOCK( "Operator [.] only applies to instances of a structure", p_block );
		return nullptr;
	}

	Structure * l_struct = l_leftNode->GetType()->GetStruct();

	if ( p_right.size() != 1 )
	{
		COMPILE_ERROR_IN_BLOCK( "Compiled right operand for operator [.] is too complex. usage : struct.member ", p_block );
		return nullptr;
	}

	uint32_t l_index = l_struct->FindMember( p_right[0]->m_contents );

	if ( l_index == String::npos )
	{
		COMPILE_ERROR_IN_BLOCK( "Member [" + p_right[0]->m_contents + "] is not recognized within the struct[" + l_struct->GetName() + "]", p_block );
		return nullptr;
	}

	ScriptNode * l_finalNode = CreateScriptNode( p_block->m_lineNumBegin );
	l_finalNode->SetRefType( l_struct->GetType()->GetSubType( l_index ) );
	l_finalNode->SetFunction( Ope_StructGetMember );
	l_finalNode->AddChild( l_leftNode );
	l_finalNode->AddChild( GetFlyweight( static_cast< int >( l_index ) ) );
	return l_finalNode;
}

ScriptNode * Elypse::Script::ScriptCompiler::_compileReturn( ScriptBlockArray & p_childs )
{
	VERBOSE_COMPILATOR_INTERNAL( "_compileReturn" );

	if ( m_currentUserFunction == nullptr )
	{
		COMPILE_ERROR_IN_BLOCK( "return operator not allowed outside a function declaration", p_childs[0] );
		return nullptr;
	}

	if ( p_childs.size() < 2 )
	{
		ScriptNode * l_node = CreateScriptNode();
		l_node->SetType( VariableTypeManager::Get( EMVT_NULL ) );
		l_node->SetFunction( Gen_ReturnNull );
		return l_node;
	}

	if ( p_childs[0]->m_type != BT_RETURN )
	{
		COMPILE_ERROR_IN_BLOCK( "return operator must be used in the beginning of a sentence ...", p_childs[0] );
		return nullptr;
	}

	ScriptBlockArray l_array( p_childs.begin() + 1, p_childs.end() );
	ScriptNode * l_node = _compileSentence( l_array );

	if ( m_currentUserFunction->m_returnNode->GetType() != l_node->GetType() )
	{
		COMPILE_ERROR_IN_BLOCK( "return operator : expected " + m_currentUserFunction->m_returnNode->GetType()->GetDesc()
								+ "  but found " + l_node->GetType()->GetDesc(), p_childs[0] );
		return nullptr;
	}

	ScriptNode * l_finalNode = CreateScriptNode();
	l_finalNode->SetType( VariableTypeManager::Get( EMVT_NULL ) );
	l_finalNode->SetFunction( Gen_Return );
	l_finalNode->AddChild( l_node );
	return l_finalNode;
}

VariableType * Elypse::Script::ScriptCompiler::_substituteVarType( VariableType * p_base, VariableType * p_type )
{
	if ( p_type != nullptr )
	{
		if ( p_base->GetBase() == EMVT_SUB1 )
		{
			return p_type->GetSubType( 0 );
		}

		if ( p_base->GetBase() == EMVT_SUB2 )
		{
			return p_type->GetSubType( 1 );
		}

		if ( p_base->GetBase() == EMVT_ITER )
		{
			if ( p_type->GetBase() == EMVT_STRINGMAP )
			{
				return VariableTypeManager::Get( EMVT_SMITER, p_type, p_type->GetSubType( 0 ) );
			}

			if ( p_type->GetBase() == EMVT_INTMAP )
			{
				return VariableTypeManager::Get( EMVT_IMITER, p_type, p_type->GetSubType( 0 ) );
			}

			if ( p_type->GetBase() == EMVT_REALMAP )
			{
				return VariableTypeManager::Get( EMVT_RMITER, p_type, p_type->GetSubType( 0 ) );
			}
		}

		if ( p_base->GetBase() == EMVT_ARRAYS1 )
		{
			return ( VariableTypeManager::Get( EMVT_ARRAY, p_type ) );
		}
	}

	return p_base;
}

void Elypse::Script::ScriptCompiler::_declareStruct( ScriptBlockArray & p_blockArray )
{
	VERBOSE_COMPILATOR_INTERNAL( "_declareStruct" );

	if ( p_blockArray.size() != 2 || p_blockArray[1]->m_type != BT_STRING )
	{
		COMPILE_ERROR_IN_BLOCK( "Struct/Class declaration work as such : struct A { ... }", p_blockArray[0] );
		return;
	}

	String const & l_name = p_blockArray[1]->m_contents;
	Structure * l_struct = General::Utils::map::findOrNull( m_structures, l_name );

	if ( l_struct == nullptr )
	{
		l_struct = new Structure( l_name );
		m_structures.insert( std::make_pair( l_name, l_struct ) );
		m_typedefs.insert( std::make_pair( l_name, l_struct->GetType() ) );
	}

	m_currentStructure = l_struct;
}

void Elypse::Script::ScriptCompiler::_printBlockArray( String const & p_where, const ScriptBlockArray & p_childs )
{
	std::cout << p_where << " : " << p_childs.size() << "\n{" << std::endl;
	size_t i = 0;

	for ( auto & l_child : p_childs )
	{
		std::cout << "\tcontains : " << i++ << " - " << l_child->m_contents << " @ " << l_child->m_type << std::endl;
	}

	std::cout << "}" << std::endl;
}
