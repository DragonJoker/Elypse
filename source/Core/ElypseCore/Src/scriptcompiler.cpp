#include "PrecompiledHeader.h"

#include "ScriptEngine.h"
#include "ScriptCompiler.h"
#include "ScriptNode.h"
#include "ScriptBlock.h"
#include "VariableType.h"
#include "Structure.h"

#include "ScriptFunctions.h"
#include "ScriptFunctionOperators.h"

#include "EMusePlugin.h"

#include "ConfigFile.h"
#include "MuseFile.h"

#include <OgreDataStream.h>
#include <OgreLog.h>

#include <Exception.h>

#include "EMuseLogs.h"

#define COMPILE_ERROR_IN_BLOCK( p_desc, p_block)												\
	_error();																					\
	GENLIB_EXCEPTION(	"Compiler Error : [" + _getScriptFileName() + " @ L# "					\
						+ StringConverter::toString( p_block->m_lineNumBegin)					\
						+ " ] -> " + p_desc )

#define COMPILE_WARNING_IN_BLOCK( p_desc, p_block)												\
	_warning();																					\
	_log(	"Compiler Warning [ " + _getScriptFileName()										\
							+ " @ L# " + StringConverter::toString( p_block->m_lineNumBegin)	\
							+ " ] -> " + p_desc );												\
 

GENLIB_INIT_SINGLETON( EMuse::Script::ScriptCompiler );

EMuse::Script::ScriptCompiler :: ScriptCompiler( const Path & p_path )
	:	m_currentLine( 0 ),
		m_path( p_path ),
		m_currentFileStream( NULL ),
		m_currentUserFunction( NULL ),
		m_currentStructure( NULL ),
		m_numWarnings( 0 ),
		m_numErrors( 0 )
{
	m_nodePool.Allocate( 200 );
	m_blockPool.Allocate( 16 );
	GENLIB_SET_SINGLETON();

	m_keyboardBinds[0] = new ScriptNode * [NUM_KEYS];
	m_keyboardBinds[1] = new ScriptNode * [NUM_KEYS];
	m_keyboardBinds[2] = new ScriptNode * [NUM_KEYS];
	m_keyboardBinds[3] = new ScriptNode * [NUM_KEYS];

	for ( unsigned int i = 0; i < NUM_KEYS; ++i )
	{
		m_keyboardBinds[0][i] = NULL;
		m_keyboardBinds[1][i] = NULL;
		m_keyboardBinds[2][i] = NULL;
		m_keyboardBinds[3][i] = NULL;
	}

	m_buffer = new char[1024];
	m_currentBufferIndex = 0;
	m_currentMaxIndex = 0;
}

EMuse::Script::ScriptCompiler :: ~ScriptCompiler()
{
	delete [] m_buffer;

	for ( unsigned int j = 0 ; j < NUM_KEYS ; j ++ )
	{
		if ( m_keyboardBinds[0][j] )
		{
			m_keyboardBinds[0][j]->Delete();
		}

		if ( m_keyboardBinds[1][j] )
		{
			m_keyboardBinds[1][j]->Delete();
		}

		if ( m_keyboardBinds[2][j] )
		{
			m_keyboardBinds[2][j]->Delete();
		}

		if ( m_keyboardBinds[2][j] )
		{
			m_keyboardBinds[3][j]->Delete();
		}
	}

	delete [] m_keyboardBinds[0];
	delete [] m_keyboardBinds[1];
	delete [] m_keyboardBinds[2];
	delete [] m_keyboardBinds[3];

	General::Utils::map::cycle( m_constants, & ScriptNode::Delete );
	m_constants.clear();
	General::Utils::map::cycle( m_userVariables, & ScriptNode::Delete );
	m_userVariables.clear();
	General::Utils::map::deleteAll( m_userFunctions );
	General::Utils::map::deleteAll( m_functions );
	{
		ClassFunctionMap::iterator iter = m_classFunctions.begin();
		const ClassFunctionMap::iterator & iend = m_classFunctions.end();

		for ( ; iter != iend ; ++ iter )
		{
			General::Utils::map::deleteAll( iter->second );
		}

		m_classFunctions.clear();
	}
	multimap::deleteAll( m_operators );
	General::Utils::map::deleteAll( m_structures );
	General::Utils::map::cycle( m_charFlyweight, &ScriptNode::Delete );
	General::Utils::map::cycle( m_intFlyweight, &ScriptNode::Delete );
	General::Utils::map::cycle( m_stringFlyweight, &ScriptNode::Delete );
	General::Utils::map::cycle( m_realFlyweight, &ScriptNode::Delete );
}

ScriptNode * EMuse::Script::ScriptCompiler :: GetUsableFunctionNode( const String & p_functionName )const
{
	UserFunction * l_function = GetUserFunction( p_functionName );

	if ( l_function == NULL )
	{
//		l_function = _createUserFunction( p_functionName, EMVT_INT);
		return NULL;
	}

	if ( l_function->m_finalNode != NULL )
	{
		return l_function->m_finalNode;
	}

	ScriptNode * l_functionNode = new ScriptNode( NULL, 0 );
	ScriptNode * l_returnValue = new ScriptNode( NULL, 0 );
	ScriptNode * l_numParams = new ScriptNode( NULL, 0 );
	l_functionNode->SetFunction( Gen_ExecuteFunction );
	l_numParams->SetType( VariableTypeManager::Get( EMVT_INT ) );
	l_functionNode->AddChild( l_returnValue );
	l_functionNode->AddChild( l_numParams );
	l_functionNode->AddChild( l_function->m_node );
	l_functionNode->Use();
	l_function->m_finalNode = l_functionNode;
	return l_functionNode;
}

void EMuse::Script::ScriptCompiler :: Initialise()
{
	_initialiseVariableMap();
	_initialiseFunctionMap();
	_initialiseOperatorMap();
}

void EMuse::Script::ScriptCompiler :: _log( const String & p_message )
{
	EMUSE_LOG_MESSAGE_RELEASE( p_message );
}

const String & EMuse::Script::ScriptCompiler :: _getScriptFileName()const
{
	if ( m_currentScriptFile != NULL )
	{
		return m_currentScriptFile->GetName();
	}

	return EMPTY_STRING;
}

ScriptNode * EMuse::Script::ScriptCompiler :: GetProgramConstant( const String & p_variableName )const
{
	ScriptNode * l_node = General::Utils::map::findOrNull( m_constants, p_variableName );

	if ( l_node != NULL )
	{
		return l_node;
	}

	if ( m_currentUserFunction != NULL )
	{
		l_node = General::Utils::map::findOrNull( m_currentUserFunction->m_localVars, p_variableName );

		if ( l_node != NULL )
		{
			return l_node;
		}
	}

	return General::Utils::map::findOrNull( m_userVariables, p_variableName );
}

char EMuse::Script::ScriptCompiler :: _getNextChar()
{
	if ( m_currentFileStream != NULL )
	{
		if ( m_currentBufferIndex < m_currentMaxIndex )
		{
			return m_buffer[m_currentBufferIndex ++];
		}

		_readFile();

		if ( m_currentBufferIndex < m_currentMaxIndex )
		{
			return m_buffer[m_currentBufferIndex ++];
		}

		return '\0';
	}

	return ( ( * m_stringBuffer )[m_currentBufferIndex ++] );
}

bool EMuse::Script::ScriptCompiler :: _eof()
{
	if ( m_currentFileStream != NULL )
	{
		return ( m_currentFileStream->eof() && m_currentBufferIndex >= m_currentMaxIndex );
	}

	return m_stringBuffer->size() == m_currentBufferIndex;
}

void EMuse::Script::ScriptCompiler :: _readFile()
{
	m_currentBufferIndex = 0;
	m_currentFileStream->read( m_buffer, 1024 );
	m_currentMaxIndex = uint32_t( m_currentFileStream->gcount() );
}

void EMuse::Script::ScriptCompiler :: _putBack( char p_char )
{
	if ( m_currentFileStream != NULL )
	{
		m_buffer[-- m_currentBufferIndex] = p_char;
	}
	else
	{
		m_currentBufferIndex --;
	}
}

ScriptNode * EMuse::Script::ScriptCompiler :: CompileScript( const String & p_script )
{
	m_currentScriptFile = NULL;
	auto l_line = m_currentLine;
	m_currentLine = 1;
	ScriptBlock * l_initialBlock = new ScriptBlock;
	l_initialBlock->_initialise( this, BT_INITIAL, 0, 0, NULL );
	m_currentFileStream = NULL;
	m_currentBufferIndex = 0;
	m_currentMaxIndex = 0;
	m_stringBuffer = & p_script;

	try
	{
		l_initialBlock->Parse();
	}
	catch ( GenException & p_except )
	{
		_log( "Critical failure : " + p_except.GetDescription() );
	}

	m_currentFileStream = NULL;
	ScriptNode * l_scriptNode = l_initialBlock->GetScriptNode();
	delete l_initialBlock;
	m_currentLine = l_line;
	return l_scriptNode;
}

ScriptNode * EMuse::Script::ScriptCompiler :: CompileScriptFile( ConfigFile * p_scriptFile )
{
	m_currentScriptFile = p_scriptFile;
	String line;
	const String & l_fileDescName = p_scriptFile->GetDescriptiveName();
	m_currentLine = 1;
	m_numWarnings = 0;
	m_numErrors = 0;
	ScriptNode * l_scriptNode = NULL;
	String l_script;
	std::ifstream * origStream;
	Ogre::DataStreamPtr stream;
	int numDeleted;
	_log( "Compiling from compiler 4.0 file : " + l_fileDescName );
	const Path & l_path = p_scriptFile->GetOwner()->GetCompletePath() / p_scriptFile->GetName();
	origStream = new std::ifstream;
	origStream->open( l_path.c_str(), std::ios::in | std::ios::binary );

	if ( origStream->fail() )
	{
		_log( "Compilation failed : file " + l_fileDescName + " does not exist (1)" );
		delete origStream ;
	}
	else
	{
		if ( origStream->eof() || origStream->bad() )
		{
			delete origStream;
			_log( "Compilation failed : file " + l_fileDescName + " does not exist(2)" );
		}
		else
		{
			ScriptBlock * l_initialBlock = new ScriptBlock;
			l_initialBlock->_initialise( this, BT_INITIAL, 0, 0, NULL );
			m_currentFileStream = origStream;
			m_currentBufferIndex = 0;
			m_currentMaxIndex = 0;

			try
			{
				l_initialBlock->Parse();
			}
			catch ( GenException & p_e )
			{
				_log( "Compilation failed : file " + l_fileDescName + " gave an exception" + p_e.GetDescription() );
			}

			m_currentFileStream = NULL;
			l_scriptNode = l_initialBlock->GetScriptNode();
			delete l_initialBlock;
			numDeleted = 0;
			delete origStream ;

			if ( m_numWarnings > 0 || m_numErrors > 0 )
			{
				_log( "Compilation Result for file : "
					  + l_fileDescName + " : "
					  + StringConverter::toString( m_numErrors ) + " error(s) , "
					  + StringConverter::toString( m_numWarnings ) + " warning(s)" );
			}
			else
			{
				_log( "Compilation success : " + l_fileDescName );
			}
		}
	}

	m_currentFileStream = NULL;
	return l_scriptNode;
}

ScriptNode * EMuse::Script::ScriptCompiler :: _createConstant( VariableBaseType p_type, const String & p_name )
{
	genlib_assert( ! General::Utils::map::has( m_constants, p_name ) );
	ScriptNode * l_node = CreateScriptNode(); //new ScriptNode( NULL, 0);
	l_node->SetType( VariableTypeManager::Get( p_type ) );
	l_node->Use();
	m_constants.insert( ScriptNodeMap::value_type( p_name, l_node ) );
	return l_node;
}

ScriptBlock * EMuse::Script::ScriptCompiler :: _getBlock()
{
	ScriptBlock * l_block = m_blockPool.Get();
	l_block->SetCompiler( this );
	return l_block;
}

void EMuse::Script::ScriptCompiler :: _releaseBlock( ScriptBlock * p_block )
{
	p_block->Clear();
	m_blockPool.Release( p_block );
}

void EMuse::Script::ScriptCompiler :: _leaveUserFunction()
{
//	m_withinUserFunction = false;
	m_currentUserFunction = NULL;
	/*
	General::Utils::map::cycle( m_userFuncVariables.back(), & ScriptNode::Delete);
	m_userFuncVariables.clear();
	*/
}

ScriptNode * EMuse::Script::ScriptCompiler :: _createUserVariable( const String & p_variableName, VariableType * p_variableType, bool p_functionParam )
{
	if ( p_variableType == NULL )
	{
		return NULL;
	}

	ScriptNode * l_node = General::Utils::map::findOrNull( m_userVariables, p_variableName );

	if ( l_node != NULL )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Warning : variable " + p_variableName + " is being redeclared : .\
									Second declaration @ line " + StringConverter::toString( _getCurrentLine() ) );
		return l_node;
	}

	if ( _isInStructDecla() && !_isInUserFunction() )
	{
		m_currentStructure->AddMember( p_variableName, p_variableType );
		return NULL;
	}

	l_node = CreateScriptNode();
	l_node->SetType( p_variableType );
	l_node->Use();

	if ( _isInUserFunction() )
	{
		genlib_assert( m_currentUserFunction != NULL );
		m_currentUserFunction->m_localVars.insert( ScriptNodeMap::value_type( p_variableName, l_node ) );
	}
	else
	{
		m_userVariables.insert( ScriptNodeMap::value_type( p_variableName, l_node ) );
	}

//	std::cout << "Created user var : " << (_isInUserFunction() ? " in user func " : "") << p_variableName << " of type : " << p_variableType->GetDesc() << std::endl;
	return l_node;
}

UserFunction  * EMuse::Script::ScriptCompiler :: _createUserFunction( const String & p_functionName, VariableType * p_functionReturnType )
{
	if ( p_functionReturnType == NULL )
	{
		return NULL;
	}

	//TODOCOMPILER : check & error message if overwritten
//	General::Utils::map::deleteValue( m_userFunctions, p_functionName);
	UserFunction * l_function = new UserFunction( p_functionName );
	l_function->m_return = p_functionReturnType;
	l_function->m_returnNode = CreateScriptNode();
	l_function->m_returnNode->SetType( p_functionReturnType );
	l_function->m_returnNode->Use();
	l_function->m_node = CreateScriptNode();
//	l_function->m_node->m_userFunction = l_function;
	l_function->m_node->Use();

	if ( _isInStructDecla() )
	{
		m_currentStructure->AddFunction( l_function );
	}
	else
	{
		m_userFunctions.insert( UserFunctionMap::value_type( p_functionName, l_function ) );
	}

	return l_function;
}

void EMuse::Script::ScriptCompiler :: _creaFunc( const String & p_functionName, RawFunction * p_function, VariableBaseType p_returnValue, ... )
{
	General::Utils::map::deleteValue( m_functions, p_functionName );
	Function * l_scriptFunction = new Function( p_functionName );
	l_scriptFunction->m_function = p_function;
	l_scriptFunction->m_return = VariableTypeManager::Get( p_returnValue );
	va_list l_vl;
	va_start( l_vl, p_returnValue );
	VariableBaseType l_val = VariableBaseType( va_arg( l_vl, int ) );

	while ( l_val != EMVT_NULL )
	{
		l_scriptFunction->m_params.push_back( VariableTypeManager::Get( l_val ) );
		l_val = VariableBaseType( va_arg( l_vl, int ) );
	}

	va_end( l_vl );
	m_functions.insert( FunctionMap::value_type( p_functionName, l_scriptFunction ) );
}

void EMuse::Script::ScriptCompiler :: _classFunc( const String & p_functionName, RawFunction * p_function, VariableBaseType p_returnValue, ... )
{
	va_list l_vl;
	va_start( l_vl, p_returnValue );
	VariableBaseType l_val;
	VariableBaseType l_param1;
	l_param1 = VariableBaseType( va_arg( l_vl, int ) );
	General::Utils::map::deleteValue( m_classFunctions[l_param1], p_functionName );
	Function * l_scriptFunction = new Function( p_functionName );
	l_scriptFunction->m_function = p_function;
	l_scriptFunction->m_return = VariableTypeManager::Get( p_returnValue );
	l_val = l_param1;

	while ( l_val != EMVT_NULL )
	{
		l_scriptFunction->m_params.push_back( VariableTypeManager::Get( l_val ) );
		l_val = VariableBaseType( va_arg( l_vl, int ) );
	}

	va_end( l_vl );
	m_classFunctions[l_param1].insert( FunctionMap::value_type( p_functionName, l_scriptFunction ) );
}

void EMuse::Script::ScriptCompiler :: _createOperator( const String & p_name, RawFunction * p_func, VariableBaseType p_returnType,
		VariableBaseType p_left, VariableBaseType p_right )
{
	OperatorFunction * l_scriptFunction = new OperatorFunction( p_name );
	l_scriptFunction->m_function = p_func;
	l_scriptFunction->m_return = VariableTypeManager::Get( p_returnType );

	if ( p_left != EMVT_NULL && p_right != EMVT_NULL )
	{
		l_scriptFunction->m_unary = false;
		l_scriptFunction->m_params.push_back( VariableTypeManager::Get( p_left ) );
		l_scriptFunction->m_params.push_back( VariableTypeManager::Get( p_right ) );
	}
	else
	{
		l_scriptFunction->m_unary = true;

		if ( p_left != EMVT_NULL )
		{
			l_scriptFunction->m_pre = false;
			l_scriptFunction->m_params.push_back( VariableTypeManager::Get( p_left ) );
		}
		else
		{
			l_scriptFunction->m_pre = true;
			l_scriptFunction->m_params.push_back( VariableTypeManager::Get( p_right ) );
		}
	}

	m_operators.insert( OperatorFunctionMultiMap::value_type( p_name, l_scriptFunction ) );
}

void EMuse::Script::ScriptCompiler :: _initialiseOperatorMap()
{
	_createOperator( "+",	Ope_Add<Real, int, Real>, 				EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "+",	Ope_Add<Real, Real, int>, 				EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "+",	Ope_Add<int>,							EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "+",	Ope_Add<char>, 							EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "+",	Ope_Add<Real>, 							EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "+",	Ope_Add<String>,						EMVT_STRING,		EMVT_STRING,		EMVT_STRING	);
	_createOperator( "+",	Ope_Add<String, char, String>,			EMVT_STRING,		EMVT_CHAR,			EMVT_STRING	);
	_createOperator( "+",	Ope_Add<String, String, char>,			EMVT_STRING,		EMVT_STRING,		EMVT_CHAR	);
	_createOperator( "+",	Ope_Add<Vector3>,						EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "+",	Ope_Add<ColourValue>,					EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "+",	Ope_Add<Quaternion>,					EMVT_QUATERNION,	EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( "=",	Ope_Set<int>,							EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "=",	Ope_Set<char>,							EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "=",	Ope_Set<bool>,							EMVT_BOOL,			EMVT_BOOL,			EMVT_BOOL	);
	_createOperator( "=",	Ope_Set<Real>,							EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "=",	Ope_Set<String>,						EMVT_STRING,		EMVT_STRING,		EMVT_STRING	);
	_createOperator( "=",	Ope_Set<Quaternion>,					EMVT_QUATERNION,	EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( "=",	Ope_Set<ColourValue>,					EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "=",	Ope_Set<Vector3>,						EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "=",	Ope_Set<NodeValueBaseArray>,			EMVT_ARRAY,			EMVT_ARRAY,			EMVT_ARRAY	);
	_createOperator( "=",	Ope_Set<NodeValueBaseMap>,				EMVT_STRINGMAP,		EMVT_STRINGMAP,		EMVT_STRINGMAP	);
	_createOperator( "=",	Ope_Set<NodeValueBaseIMap>,				EMVT_INTMAP,		EMVT_INTMAP,		EMVT_INTMAP	);
	_createOperator( "=",	Ope_Set<NodeValueBaseRMap>,				EMVT_REALMAP,		EMVT_REALMAP,		EMVT_REALMAP	);
	_createOperator( "=",	Ope_Set<CamTex *>,						EMVT_CAMTEX,		EMVT_CAMTEX,		EMVT_CAMTEX	);
	_createOperator( "=",	Ope_Set<ScriptTimer *>,					EMVT_TIMER,			EMVT_TIMER,			EMVT_TIMER	);
	_createOperator( "=",	Ope_Set<PhysicsObject *>,				EMVT_PHY_OBJ,		EMVT_PHY_OBJ,		EMVT_PHY_OBJ	);
	_createOperator( "=",	Ope_Set<EMAnimation *>,					EMVT_ANIMATION,		EMVT_ANIMATION,		EMVT_ANIMATION	);
	_createOperator( "=",	Ope_SetNull<StructInstance *>,			EMVT_STRUCT,		EMVT_STRUCT,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_Set<AnimatedObject *>,				EMVT_ANI_OBJ,		EMVT_ANI_OBJ,		EMVT_ANI_OBJ	);
	_createOperator( "=",	Ope_Set<AnimatedObjectGroup *>,			EMVT_ANI_GRP,		EMVT_ANI_GRP,		EMVT_ANI_GRP	);
	_createOperator( "=",	Ope_Set<EMOverlay *>,					EMVT_OVERLAY,		EMVT_OVERLAY,		EMVT_OVERLAY	);
	_createOperator( "=",	Ope_Set<PhysicsMaterial *>,				EMVT_PHY_MAT,		EMVT_PHY_MAT,		EMVT_PHY_MAT	);
	_createOperator( "=",	Ope_Set<Material *>,						EMVT_OBJ_MAT,		EMVT_OBJ_MAT,		EMVT_OBJ_MAT	);
	_createOperator( "=",	Ope_Set<SceneNode *>,					EMVT_NODE,			EMVT_NODE,			EMVT_NODE	);
	_createOperator( "=",	Ope_Set<Camera *>,						EMVT_CAMERA,		EMVT_CAMERA,		EMVT_CAMERA	);
	_createOperator( "=",	Ope_Set<Light *>,						EMVT_LIGHT,			EMVT_LIGHT,			EMVT_LIGHT	);
	_createOperator( "=",	Ope_Set<Entity *>,						EMVT_ENTITY,		EMVT_ENTITY,		EMVT_ENTITY	);
	_createOperator( "=",	Ope_Set<StructInstance *>,				EMVT_STRUCT,		EMVT_STRUCT,		EMVT_STRUCT	);
	_createOperator( "=",	Ope_Set<NodeValueBaseMap::iterator>,	EMVT_SMITER,		EMVT_SMITER,		EMVT_SMITER	);
	_createOperator( "=",	Ope_Set<NodeValueBaseIMap::iterator>,	EMVT_IMITER,		EMVT_IMITER,		EMVT_IMITER	);
	_createOperator( "=",	Ope_Set<NodeValueBaseRMap::iterator>,	EMVT_RMITER,		EMVT_RMITER,		EMVT_RMITER	);
	_createOperator( "=",	Ope_SetNull<CamTex *>,					EMVT_CAMTEX,		EMVT_CAMTEX,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<ScriptTimer *>,				EMVT_TIMER,			EMVT_TIMER,			EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<PhysicsObject *>,			EMVT_PHY_OBJ,		EMVT_PHY_OBJ,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<EMAnimation *>,				EMVT_ANIMATION,		EMVT_ANIMATION,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<AnimatedObject *>,			EMVT_ANI_OBJ,		EMVT_ANI_OBJ,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<AnimatedObjectGroup *>,		EMVT_ANI_GRP,		EMVT_ANI_GRP,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<EMOverlay *>,				EMVT_OVERLAY,		EMVT_OVERLAY,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<PhysicsMaterial *>,			EMVT_PHY_MAT,		EMVT_PHY_MAT,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<Material *>,					EMVT_OBJ_MAT,		EMVT_OBJ_MAT,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<SceneNode *>,				EMVT_NODE,			EMVT_NODE,			EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<Camera *>,					EMVT_CAMERA,		EMVT_CAMERA,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<Light *>,					EMVT_LIGHT,			EMVT_LIGHT,			EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<Entity *>,					EMVT_ENTITY,		EMVT_ENTITY,		EMVT_NULLVALUE	);
	_createOperator( "=",	Ope_SetNull<StructInstance *>,			EMVT_STRUCT,		EMVT_STRUCT,		EMVT_NULLVALUE	);
	_createOperator( ":=",	Ope_Assign<int>,						EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( ":=",	Ope_Assign<char>,						EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( ":=",	Ope_Assign<bool>,						EMVT_BOOL,			EMVT_BOOL,			EMVT_BOOL	);
	_createOperator( ":=",	Ope_Assign<Real>,						EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( ":=",	Ope_Assign<String>,						EMVT_STRING,		EMVT_STRING,		EMVT_STRING	);
	_createOperator( ":=",	Ope_Assign<Quaternion>,					EMVT_QUATERNION,	EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( ":=",	Ope_Assign<ColourValue>,				EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( ":=",	Ope_Assign<Vector3>,					EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( ":=",	Ope_Assign<NodeValueBaseArray>,			EMVT_ARRAY,			EMVT_ARRAY,			EMVT_ARRAY	);
	_createOperator( ":=",	Ope_Assign<NodeValueBaseMap>,			EMVT_STRINGMAP,		EMVT_STRINGMAP,		EMVT_STRINGMAP	);
	_createOperator( ":=",	Ope_Assign<NodeValueBaseIMap>,			EMVT_INTMAP,		EMVT_INTMAP,		EMVT_INTMAP	);
	_createOperator( ":=",	Ope_Assign<NodeValueBaseRMap>,			EMVT_REALMAP,		EMVT_REALMAP,		EMVT_REALMAP	);
	_createOperator( ":=",	Ope_Assign<CamTex *>,					EMVT_CAMTEX,		EMVT_CAMTEX,		EMVT_CAMTEX	);
	_createOperator( ":=",	Ope_Assign<ScriptTimer *>,				EMVT_TIMER,			EMVT_TIMER,			EMVT_TIMER	);
	_createOperator( ":=",	Ope_Assign<PhysicsObject *>,				EMVT_PHY_OBJ,		EMVT_PHY_OBJ,		EMVT_PHY_OBJ	);
	_createOperator( ":=",	Ope_Assign<EMAnimation *>,				EMVT_ANIMATION,		EMVT_ANIMATION,		EMVT_ANIMATION	);
	_createOperator( ":=",	Ope_Assign<AnimatedObject *>,			EMVT_ANI_OBJ,		EMVT_ANI_OBJ,		EMVT_ANI_OBJ	);
	_createOperator( ":=",	Ope_Assign<AnimatedObjectGroup *>,		EMVT_ANI_GRP,		EMVT_ANI_GRP,		EMVT_ANI_GRP	);
	_createOperator( ":=",	Ope_Assign<EMOverlay *>,					EMVT_OVERLAY,		EMVT_OVERLAY,		EMVT_OVERLAY	);
	_createOperator( ":=",	Ope_Assign<PhysicsMaterial *>,			EMVT_PHY_MAT,		EMVT_PHY_MAT,		EMVT_PHY_MAT	);
	_createOperator( ":=",	Ope_Assign<Material *>,					EMVT_OBJ_MAT,		EMVT_OBJ_MAT,		EMVT_OBJ_MAT	);
	_createOperator( ":=",	Ope_Assign<SceneNode *>,					EMVT_NODE,			EMVT_NODE,			EMVT_NODE	);
	_createOperator( ":=",	Ope_Assign<Camera *>,					EMVT_CAMERA,		EMVT_CAMERA,		EMVT_CAMERA	);
	_createOperator( ":=",	Ope_Assign<Light *>,						EMVT_LIGHT,			EMVT_LIGHT,			EMVT_LIGHT	);
	_createOperator( ":=",	Ope_Assign<Entity *>,					EMVT_ENTITY,		EMVT_ENTITY,		EMVT_ENTITY	);
	_createOperator( ":=",	Ope_Assign<StructInstance *>,			EMVT_STRUCT,		EMVT_STRUCT,		EMVT_STRUCT	);
	_createOperator( "==",	Ope_Compare<int>,						EMVT_BOOL,			EMVT_INT,			EMVT_INT	);
	_createOperator( "==",	Ope_Compare<char>,						EMVT_BOOL,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "==",	Ope_Compare<bool>,						EMVT_BOOL,			EMVT_BOOL,			EMVT_BOOL	);
	_createOperator( "==",	Ope_Compare<Real>,						EMVT_BOOL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "==",	Ope_Compare<String>,					EMVT_BOOL,			EMVT_STRING,		EMVT_STRING	);
	_createOperator( "==",	Ope_Compare<Quaternion>,				EMVT_BOOL,			EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( "==",	Ope_Compare<ColourValue>,				EMVT_BOOL,			EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "==",	Ope_Compare<Vector3>,					EMVT_BOOL,			EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseArray>,		EMVT_BOOL,			EMVT_ARRAY,			EMVT_ARRAY	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseMap>,			EMVT_BOOL,			EMVT_STRINGMAP,		EMVT_STRINGMAP	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseIMap>,			EMVT_BOOL,			EMVT_INTMAP,		EMVT_INTMAP	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseRMap>,			EMVT_BOOL,			EMVT_REALMAP,		EMVT_REALMAP	);
	_createOperator( "==",	Ope_Compare<CamTex *>,					EMVT_BOOL,			EMVT_CAMTEX,		EMVT_CAMTEX	);
	_createOperator( "==",	Ope_Compare<ScriptTimer *>,				EMVT_BOOL,			EMVT_TIMER,			EMVT_TIMER	);
	_createOperator( "==",	Ope_Compare<PhysicsObject *>,			EMVT_BOOL,			EMVT_PHY_OBJ,		EMVT_PHY_OBJ	);
	_createOperator( "==",	Ope_Compare<EMAnimation *>,				EMVT_BOOL,			EMVT_ANIMATION,		EMVT_ANIMATION	);
	_createOperator( "==",	Ope_Compare<AnimatedObject *>,			EMVT_BOOL,			EMVT_ANI_OBJ,		EMVT_ANI_OBJ	);
	_createOperator( "==",	Ope_Compare<AnimatedObjectGroup *>,		EMVT_BOOL,			EMVT_ANI_GRP,		EMVT_ANI_GRP	);
	_createOperator( "==",	Ope_Compare<EMOverlay *>,				EMVT_BOOL,			EMVT_OVERLAY,		EMVT_OVERLAY	);
	_createOperator( "==",	Ope_Compare<PhysicsMaterial *>,			EMVT_BOOL,			EMVT_PHY_MAT,		EMVT_PHY_MAT	);
	_createOperator( "==",	Ope_Compare<Material *>,					EMVT_BOOL,			EMVT_OBJ_MAT,		EMVT_OBJ_MAT	);
	_createOperator( "==",	Ope_Compare<SceneNode *>,				EMVT_BOOL,			EMVT_NODE,			EMVT_NODE	);
	_createOperator( "==",	Ope_Compare<Camera *>,					EMVT_BOOL,			EMVT_CAMERA,		EMVT_CAMERA	);
	_createOperator( "==",	Ope_Compare<Light *>,					EMVT_BOOL,			EMVT_LIGHT,			EMVT_LIGHT	);
	_createOperator( "==",	Ope_Compare<Entity *>,					EMVT_BOOL,			EMVT_ENTITY,		EMVT_ENTITY	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseMap::iterator>,	EMVT_BOOL,		EMVT_SMITER,		EMVT_SMITER	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseIMap::iterator>,	EMVT_BOOL,		EMVT_IMITER,		EMVT_IMITER	);
	_createOperator( "==",	Ope_Compare<NodeValueBaseRMap::iterator>,	EMVT_BOOL,		EMVT_RMITER,		EMVT_RMITER	);
	_createOperator( "==",	Ope_CompareNull<CamTex *>,				EMVT_BOOL,			EMVT_CAMTEX,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<ScriptTimer *>,			EMVT_BOOL,			EMVT_TIMER,			EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<PhysicsObject *>,		EMVT_BOOL,			EMVT_PHY_OBJ,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<EMAnimation *>,			EMVT_BOOL,			EMVT_ANIMATION,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<AnimatedObject *>,		EMVT_BOOL,			EMVT_ANI_OBJ,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<AnimatedObjectGroup *>,	EMVT_BOOL,			EMVT_ANI_GRP,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<EMOverlay *>,			EMVT_BOOL,			EMVT_OVERLAY,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<PhysicsMaterial *>,		EMVT_BOOL,			EMVT_PHY_MAT,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<Material *>,				EMVT_BOOL,			EMVT_OBJ_MAT,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<SceneNode *>,			EMVT_BOOL,			EMVT_NODE,			EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<Camera *>,				EMVT_BOOL,			EMVT_CAMERA,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<Light *>,				EMVT_BOOL,			EMVT_LIGHT,			EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<Entity *>,				EMVT_BOOL,			EMVT_ENTITY,		EMVT_NULLVALUE	);
	_createOperator( "==",	Ope_CompareNull<StructInstance *>,		EMVT_BOOL,			EMVT_STRUCT,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiff<int>,						EMVT_BOOL,			EMVT_INT,			EMVT_INT	);
	_createOperator( "!=",	Ope_IsDiff<char>,						EMVT_BOOL,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "!=",	Ope_IsDiff<bool>,						EMVT_BOOL,			EMVT_BOOL,			EMVT_BOOL	);
	_createOperator( "!=",	Ope_IsDiff<Real>,						EMVT_BOOL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "!=",	Ope_IsDiff<String>,						EMVT_BOOL,			EMVT_STRING,		EMVT_STRING	);
	_createOperator( "!=",	Ope_IsDiff<Quaternion>,					EMVT_BOOL,			EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( "!=",	Ope_IsDiff<ColourValue>,				EMVT_BOOL,			EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "!=",	Ope_IsDiff<Vector3>,					EMVT_BOOL,			EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseArray>,			EMVT_BOOL,			EMVT_ARRAY,			EMVT_ARRAY	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseMap>,			EMVT_BOOL,			EMVT_STRINGMAP,		EMVT_STRINGMAP	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseIMap>,			EMVT_BOOL,			EMVT_INTMAP,		EMVT_INTMAP	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseRMap>,			EMVT_BOOL,			EMVT_REALMAP,		EMVT_REALMAP	);
	_createOperator( "!=",	Ope_IsDiff<CamTex *>,					EMVT_BOOL,			EMVT_CAMTEX,		EMVT_CAMTEX	);
	_createOperator( "!=",	Ope_IsDiff<ScriptTimer *>,				EMVT_BOOL,			EMVT_TIMER,			EMVT_TIMER	);
	_createOperator( "!=",	Ope_IsDiff<PhysicsObject *>,				EMVT_BOOL,			EMVT_PHY_OBJ,		EMVT_PHY_OBJ	);
	_createOperator( "!=",	Ope_IsDiff<EMAnimation *>,				EMVT_BOOL,			EMVT_ANIMATION,		EMVT_ANIMATION	);
	_createOperator( "!=",	Ope_IsDiff<AnimatedObject *>,			EMVT_BOOL,			EMVT_ANI_OBJ,		EMVT_ANI_OBJ	);
	_createOperator( "!=",	Ope_IsDiff<AnimatedObjectGroup *>,		EMVT_BOOL,			EMVT_ANI_GRP,		EMVT_ANI_GRP	);
	_createOperator( "!=",	Ope_IsDiff<EMOverlay *>,					EMVT_BOOL,			EMVT_OVERLAY,		EMVT_OVERLAY	);
	_createOperator( "!=",	Ope_IsDiff<PhysicsMaterial *>,			EMVT_BOOL,			EMVT_PHY_MAT,		EMVT_PHY_MAT	);
	_createOperator( "!=",	Ope_IsDiff<Material *>,					EMVT_BOOL,			EMVT_OBJ_MAT,		EMVT_OBJ_MAT	);
	_createOperator( "!=",	Ope_IsDiff<SceneNode *>,					EMVT_BOOL,			EMVT_NODE,			EMVT_NODE	);
	_createOperator( "!=",	Ope_IsDiff<Camera *>,					EMVT_BOOL,			EMVT_CAMERA,		EMVT_CAMERA	);
	_createOperator( "!=",	Ope_IsDiff<Light *>,						EMVT_BOOL,			EMVT_LIGHT,			EMVT_LIGHT	);
	_createOperator( "!=",	Ope_IsDiff<Entity *>,					EMVT_BOOL,			EMVT_ENTITY,		EMVT_ENTITY	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseMap::iterator>,	EMVT_BOOL,			EMVT_SMITER,		EMVT_SMITER	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseIMap::iterator>, EMVT_BOOL,			EMVT_IMITER,		EMVT_IMITER	);
	_createOperator( "!=",	Ope_IsDiff<NodeValueBaseRMap::iterator>, EMVT_BOOL,			EMVT_RMITER,		EMVT_RMITER	);
	_createOperator( "!=",	Ope_IsDiffNull<CamTex *>,				EMVT_BOOL,			EMVT_CAMTEX,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<ScriptTimer *>,			EMVT_BOOL,			EMVT_TIMER,			EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<PhysicsObject *>,			EMVT_BOOL,			EMVT_PHY_OBJ,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<EMAnimation *>,			EMVT_BOOL,			EMVT_ANIMATION,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<AnimatedObject *>,		EMVT_BOOL,			EMVT_ANI_OBJ,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<AnimatedObjectGroup *>,	EMVT_BOOL,			EMVT_ANI_GRP,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<EMOverlay *>,				EMVT_BOOL,			EMVT_OVERLAY,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<PhysicsMaterial *>,		EMVT_BOOL,			EMVT_PHY_MAT,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<Material *>,				EMVT_BOOL,			EMVT_OBJ_MAT,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<SceneNode *>,				EMVT_BOOL,			EMVT_NODE,			EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<Camera *>,				EMVT_BOOL,			EMVT_CAMERA,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<Light *>,					EMVT_BOOL,			EMVT_LIGHT,			EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<Entity *>,				EMVT_BOOL,			EMVT_ENTITY,		EMVT_NULLVALUE	);
	_createOperator( "!=",	Ope_IsDiffNull<StructInstance *>,		EMVT_BOOL,			EMVT_STRUCT,		EMVT_NULLVALUE	);
	_createOperator( "+=",	Ope_AddEqual<int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "+=",	Ope_AddEqual<Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "+=",	Ope_AddEqual<int, int>,					EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "+=",	Ope_AddEqual<char, char>,				EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "+=",	Ope_AddEqual<Real, Real>,				EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "+=",	Ope_AddEqual<String, String>,			EMVT_STRING,		EMVT_STRING,		EMVT_STRING	);
	_createOperator( "+=",	Ope_AddEqual<Vector3, Vector3>,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "+=",	Ope_AddEqual<ColourValue, ColourValue>,	EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
//	_createOperator( "+",	Ope_Add_Char_To_String,					EMVT_STRING,		EMVT_STRING,		EMVT_CHAR		);
	_createOperator( "+=",	Ope_Add_Equal_Char_To_String,			EMVT_STRING,		EMVT_STRING,		EMVT_CHAR	);
	_createOperator( "-",	Ope_Sub<Real, int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "-",	Ope_Sub<Real, Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "-",	Ope_Sub<int>,							EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "-",	Ope_Sub<char>,							EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "-",	Ope_Sub<Real>,							EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "-",	Ope_Sub<Vector3>, 						EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "-",	Ope_Sub<ColourValue>,					EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "-",	Ope_Sub<Quaternion>,					EMVT_QUATERNION,	EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( "-=",	Ope_SubEqual<int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "-=",	Ope_SubEqual<Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "-=",	Ope_SubEqual<int, int>,					EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "-=",	Ope_SubEqual<char, char>,				EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "-=",	Ope_SubEqual<Real, Real>,				EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "-=",	Ope_SubEqual<Vector3, Vector3>,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "-=",	Ope_SubEqual<ColourValue, ColourValue>,	EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "/",	Ope_Div<Real, int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "/",	Ope_Div<Real, Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "/",	Ope_Div<int>,							EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "/",	Ope_Div<Real>,							EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "/",	Ope_Div<ColourValue>,					EMVT_COLOUR, 		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "/",	Ope_Div<ColourValue, ColourValue, Real>,	EMVT_COLOUR, 		EMVT_COLOUR,		EMVT_REAL	);
	_createOperator( "/",	Ope_Div<Vector3, Vector3, Real>,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_REAL	);
	_createOperator( "/=",	Ope_DivEqual<int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "/=",	Ope_DivEqual<Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "/=",	Ope_DivEqual<int, int>,					EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "/=",	Ope_DivEqual<char, char>,				EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "/=",	Ope_DivEqual<Real, Real>,				EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "/=",	Ope_DivEqual<Vector3, Vector3>,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "/=",	Ope_DivEqual<Vector3, Real>,				EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_REAL	);
	_createOperator( "/=",	Ope_DivEqual<ColourValue, Real>,			EMVT_COLOUR,		EMVT_COLOUR,		EMVT_REAL	);
	_createOperator( "*",	Ope_Mul<Real, int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "*",	Ope_Mul<Real, Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "*",	Ope_Mul<int>,							EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "*",	Ope_Mul<Real>,							EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "*",	Ope_Mul<ColourValue>,					EMVT_COLOUR,		EMVT_COLOUR,		EMVT_COLOUR	);
	_createOperator( "*",	Ope_Mul<Vector3>,						EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "*",	Ope_Mul<Vector3, Vector3, Real>,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_REAL	);
	_createOperator( "*",	Ope_Mul<Vector3, Quaternion, Vector3>,	EMVT_VECTOR3,		EMVT_QUATERNION,	EMVT_VECTOR3	);
	_createOperator( "*",	Ope_Mul<Vector3, Real, Vector3>,			EMVT_VECTOR3,		EMVT_REAL,			EMVT_VECTOR3	);
	_createOperator( "*",	Ope_Mul<Quaternion>,					EMVT_QUATERNION,	EMVT_QUATERNION,	EMVT_QUATERNION	);
	_createOperator( "*",	Ope_Multiply_Vector3_By_Quat,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_QUATERNION	);
	_createOperator( "*=",	Ope_MulEqual<int, Real>,					EMVT_REAL,			EMVT_INT,			EMVT_REAL	);
	_createOperator( "*=",	Ope_MulEqual<Real, int>,					EMVT_REAL,			EMVT_REAL,			EMVT_INT	);
	_createOperator( "*=",	Ope_MulEqual<int, int>,					EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( "*=",	Ope_MulEqual<char, char>,				EMVT_CHAR,			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "*=",	Ope_MulEqual<Real, Real>,				EMVT_REAL,			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "*=",	Ope_MulEqual<Vector3, Vector3>,			EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_VECTOR3	);
	_createOperator( "*=",	Ope_MulEqual<Vector3, Real>,				EMVT_VECTOR3,		EMVT_VECTOR3,		EMVT_REAL	);
	_createOperator( "*=",	Ope_MulEqual<ColourValue, Real>,			EMVT_COLOUR,		EMVT_COLOUR,		EMVT_REAL	);
	_createOperator( "||",	Ope_Or,									EMVT_BOOL, 			EMVT_BOOL,			EMVT_BOOL	);
	_createOperator( "&&",	Ope_And,								EMVT_BOOL, 			EMVT_BOOL,			EMVT_BOOL	);
	_createOperator( "!",	Ope_Negate,								EMVT_BOOL, 			EMVT_NULL,			EMVT_BOOL	);
	_createOperator( "++",	Ope_Increment_Int,						EMVT_INT,			EMVT_INT,			EMVT_NULL	);
	_createOperator( "++",	Ope_Increment_Char,						EMVT_CHAR,			EMVT_CHAR,			EMVT_NULL	);
	_createOperator( "--",	Ope_Decrement_Int,						EMVT_INT,			EMVT_INT,			EMVT_NULL	);
	_createOperator( "--",	Ope_Decrement_Char,						EMVT_CHAR,			EMVT_CHAR,			EMVT_NULL	);
	_createOperator( "++",	Rit_Next,								EMVT_NULL,			EMVT_RMITER,		EMVT_NULL	);
	_createOperator( "++",	Sit_Next,								EMVT_NULL,			EMVT_SMITER,		EMVT_NULL	);
	_createOperator( "++",	Iit_Next,								EMVT_NULL,			EMVT_IMITER,		EMVT_NULL	);
	_createOperator( "++",	Rit_Next,								EMVT_NULL,			EMVT_NULL,			EMVT_RMITER	);
	_createOperator( "++",	Sit_Next,								EMVT_NULL,			EMVT_NULL,			EMVT_SMITER	);
	_createOperator( "++",	Iit_Next,								EMVT_NULL,			EMVT_NULL,			EMVT_IMITER	);
	_createOperator( "--",	Rit_Prev,								EMVT_NULL,			EMVT_RMITER,		EMVT_NULL	);
	_createOperator( "--",	Sit_Prev,								EMVT_NULL,			EMVT_SMITER,		EMVT_NULL	);
	_createOperator( "--",	Iit_Prev,								EMVT_NULL,			EMVT_IMITER,		EMVT_NULL	);
	_createOperator( "--",	Rit_Prev,								EMVT_NULL,			EMVT_NULL,			EMVT_RMITER	);
	_createOperator( "--",	Sit_Prev,								EMVT_NULL,			EMVT_NULL,			EMVT_SMITER	);
	_createOperator( "--",	Iit_Prev,								EMVT_NULL,			EMVT_NULL,			EMVT_IMITER	);
	_createOperator( "++",	Ope_Increment_Int,						EMVT_INT,			EMVT_NULL,			EMVT_INT	);
	_createOperator( "++",	Ope_Increment_Char,						EMVT_CHAR,			EMVT_NULL,			EMVT_CHAR	);
	_createOperator( "--",	Ope_Decrement_Int,						EMVT_INT,			EMVT_NULL,			EMVT_INT	);
	_createOperator( "--",	Ope_Decrement_Char,						EMVT_CHAR,			EMVT_NULL,			EMVT_CHAR	);
	_createOperator( "-",	Ope_NegateReal,							EMVT_REAL,			EMVT_NULL,			EMVT_REAL	);
	_createOperator( "-",	Ope_NegateInt,							EMVT_INT,			EMVT_NULL,			EMVT_INT	);
	_createOperator( "-",	Ope_NegateVe3,							EMVT_VECTOR3,		EMVT_NULL,			EMVT_VECTOR3	);
	_createOperator( "%",	Ope_ModuloInt,							EMVT_INT,			EMVT_INT,			EMVT_INT	);
	_createOperator( ">",	Ope_IsSuperior_Int,						EMVT_BOOL, 			EMVT_INT,			EMVT_INT	);
	_createOperator( ">",	Ope_IsSuperior_Char,					EMVT_BOOL, 			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( ">",	Ope_IsSuperior_String,					EMVT_BOOL, 			EMVT_STRING,		EMVT_STRING	);
	_createOperator( ">",	Ope_IsSuperior_Real,					EMVT_BOOL, 			EMVT_REAL,			EMVT_REAL	);
	_createOperator( ">=",	Ope_IsSuperiorOrEqual_Int,				EMVT_BOOL, 			EMVT_INT,			EMVT_INT	);
	_createOperator( ">=",	Ope_IsSuperiorOrEqual_Char,				EMVT_BOOL, 			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( ">=",	Ope_IsSuperiorOrEqual_Real,				EMVT_BOOL, 			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "<",	Ope_IsInferior_Int,						EMVT_BOOL, 			EMVT_INT,			EMVT_INT	);
	_createOperator( "<",	Ope_IsInferior_Char,					EMVT_BOOL, 			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "<",	Ope_IsInferior_String,					EMVT_BOOL, 			EMVT_STRING,		EMVT_STRING	);
	_createOperator( "<",	Ope_IsInferior_Real,					EMVT_BOOL, 			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "<=",	Ope_IsInferiorOrEqual_Int,				EMVT_BOOL, 			EMVT_INT,			EMVT_INT	);
	_createOperator( "<=",	Ope_IsInferiorOrEqual_Char,				EMVT_BOOL, 			EMVT_CHAR,			EMVT_CHAR	);
	_createOperator( "<=",	Ope_IsInferiorOrEqual_Real,				EMVT_BOOL, 			EMVT_REAL,			EMVT_REAL	);
	_createOperator( "[]",	Arr_Get,								EMVT_SUB1, 			EMVT_ARRAY,			EMVT_INT	);
	_createOperator( "[]",	Str_GetChar,							EMVT_CHAR, 			EMVT_STRING,		EMVT_INT	);
	_createOperator( "[]",	Ve3_OperatorArray,						EMVT_REAL, 			EMVT_VECTOR3,		EMVT_INT	);
}

void EMuse::Script::ScriptCompiler :: _initialiseVariableMap()
{
	_constantGroup(	"General" );
	_createConstant(	EMVT_REAL,				"GENERAL_ELAPSED_TIME"	);
	_createConstant(	EMVT_REAL,				"GENERAL_SCREEN_WIDTH"	);
	_createConstant(	EMVT_REAL,				"GENERAL_SCREEN_HEIGHT"	);
	_constantGroup(	"Mouse" );
	_createConstant(	EMVT_REAL,				"MOUSE_POSITION_X"	);
	_createConstant(	EMVT_REAL,				"MOUSE_POSITION_Y"	);
	_createConstant(	EMVT_REAL,				"MOUSE_DIFFERENCE_X"	);
	_createConstant(	EMVT_REAL,				"MOUSE_DIFFERENCE_Y"	);
	_createConstant(	EMVT_INT,				"MOUSE_CURSOR_TEXT"	)->set<int>( static_cast <int>( TextCursor ) );
	_createConstant(	EMVT_INT,				"MOUSE_CURSOR_HAND"	)->set<int>( static_cast <int>( HandCursor ) );
	_createConstant(	EMVT_INT,				"MOUSE_CURSOR_ARROW"	)->set<int>( static_cast <int>( ArrowCursor ) );
	_createConstant(	EMVT_INT,				"MOUSE_CURSOR_SIZENS"	)->set<int>( static_cast <int>( SizeNSCursor ) );
	_createConstant(	EMVT_INT,				"MOUSE_CURSOR_SIZEWE"	)->set<int>( static_cast <int>( SizeWECursor ) );
	_constantGroup(	"Network" );
	_createConstant(	EMVT_STRING,			"NETWORK_CURRENT_MESSAGE"	);
	_constantGroup(	"Math" );
	_createConstant(	EMVT_REAL,				"PI"	)->set<Real>( Math::PI );
	_createConstant(	EMVT_BOOL,				"true"	)->set( true );
	_createConstant(	EMVT_BOOL,				"false"	)->set( false );
	_createConstant(	EMVT_NULLVALUE,			"NULL"	);
	_constantGroup(	"Timer" );
	_createConstant(	EMVT_INT,				"TIMERTYPE_ONCE"	)->set<int>( 0 );
	_createConstant(	EMVT_INT,				"TIMERTYPE_REPEAT"	)->set<int>( 1 );
	_createConstant(	EMVT_INT,				"TIMERTYPE_CONTINUOUS"	)->set<int>( 2 );
	_createConstant(	EMVT_INT,				"TIMERTYPE_PERMANENT"	)->set<int>( 3 );
	_createConstant(	EMVT_REAL,				"CURRENTTIMER_TIME_LEFT"	);
	_createConstant(	EMVT_REAL,				"CURRENTTIMER_TIME_BASE"	);
	_createConstant(	EMVT_REAL,				"CURRENTTIMER_TIME_ELAPSED"	);
	_createConstant(	EMVT_INT,				"CURRENTTIMER_NUMEXECS"	);
	_createConstant(	EMVT_TIMER,				"CURRENTTIMER_SELF"	);
	_constantGroup(	"Keyboard" );
	_createConstant(	EMVT_CHAR,				"KEY_CURRENT"	)->set<char>( '\0' );
	_createConstant(	EMVT_INT,				"BINDKEY_DOWN"	)->set<int>( 0 );
	_createConstant(	EMVT_INT,				"BINDKEY_UP"	)->set<int>( 1 );
	_createConstant(	EMVT_INT,				"BINDKEY_PRESSED"	)->set<int>( 2 );
	_createConstant(	EMVT_INT,				"BINDKEY_REPEATED"	)->set<int>( 3 );
	_constantGroup(	"Web" );
	_createConstant(	EMVT_BOOL,				"HTTP_METHOD_POST"	)->set( true );
	_createConstant(	EMVT_BOOL,				"HTTP_METHOD_GET"	)->set( false );
	_constantGroup(	"Colour" );
	_createConstant(	EMVT_COLOUR,			"COLOUR_RED"	)->set( ColourValue::Red );
	_createConstant(	EMVT_COLOUR,			"COLOUR_GREEN"	)->set( ColourValue::Green );
	_createConstant(	EMVT_COLOUR,			"COLOUR_BLUE"	)->set( ColourValue::Blue );
	_createConstant(	EMVT_COLOUR,			"COLOUR_BLACK"	)->set( ColourValue::Black );
	_createConstant(	EMVT_COLOUR,			"COLOUR_WHITE"	)->set( ColourValue::White );
	_constantGroup(	"Vector3" );
	_createConstant(	EMVT_VECTOR3,			"NEGATIVE_UNIT_X"	)->set(	Vector3::NEGATIVE_UNIT_X );
	_createConstant(	EMVT_VECTOR3,			"NEGATIVE_UNIT_Y"	)->set(	Vector3::NEGATIVE_UNIT_Y );
	_createConstant(	EMVT_VECTOR3,			"NEGATIVE_UNIT_Z"	)->set(	Vector3::NEGATIVE_UNIT_Z );
	_createConstant(	EMVT_VECTOR3,			"UNIT_X"	)->set(	Vector3::UNIT_X );
	_createConstant(	EMVT_VECTOR3,			"UNIT_Y"	)->set(	Vector3::UNIT_Y );
	_createConstant(	EMVT_VECTOR3,			"UNIT_Z"	)->set(	Vector3::UNIT_Z );
	_createConstant(	EMVT_VECTOR3,			"VECTOR3_ZERO"	)->set<Vector3>( Vector3::ZERO );
	_constantGroup(	"String" );
	_createConstant(	EMVT_STRING,			"endl"	)->set<String>( "\n" );
	_createConstant(	EMVT_STRING,			"EMPTY_STRING"	)->set<String>( EMPTY_STRING );
	_createConstant(	EMVT_INT,				"npos"	)->set<int>( static_cast<int>( String::npos ) );
	_constantGroup(	"Keycode" );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_0"	)->set<int>( 0x60 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_1"	)->set<int>( 0x61 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_2"	)->set<int>( 0x62 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_3"	)->set<int>( 0x63 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_4"	)->set<int>( 0x64 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_5"	)->set<int>( 0x65 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_6"	)->set<int>( 0x66 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_7"	)->set<int>( 0x67 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_8"	)->set<int>( 0x68 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_9"	)->set<int>( 0x69 );
	_createConstant(	EMVT_INT,				"KEYCODE_0"	)->set<int>( 0x30 );
	_createConstant(	EMVT_INT,				"KEYCODE_1"	)->set<int>( 0x31 );
	_createConstant(	EMVT_INT,				"KEYCODE_2"	)->set<int>( 0x32 );
	_createConstant(	EMVT_INT,				"KEYCODE_3"	)->set<int>( 0x33 );
	_createConstant(	EMVT_INT,				"KEYCODE_4"	)->set<int>( 0x34 );
	_createConstant(	EMVT_INT,				"KEYCODE_5"	)->set<int>( 0x35 );
	_createConstant(	EMVT_INT,				"KEYCODE_6"	)->set<int>( 0x36 );
	_createConstant(	EMVT_INT,				"KEYCODE_7"	)->set<int>( 0x37 );
	_createConstant(	EMVT_INT,				"KEYCODE_8"	)->set<int>( 0x38 );
	_createConstant(	EMVT_INT,				"KEYCODE_9"	)->set<int>( 0x39 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_STAR"	)->set<int>( 106 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_PLUS"	)->set<int>( 107 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_MINUS"	)->set<int>( 109 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_SLASH"	)->set<int>( 111 );
	_createConstant(	EMVT_INT,				"KEYCODE_NP_DOT"	)->set<int>( 110 );
	_createConstant(	EMVT_INT,				"KEYCODE_ENTER"	)->set<int>( 13 );
	_createConstant(	EMVT_INT,				"KEYCODE_SHIFT"	)->set<int>( 16 );
	_createConstant(	EMVT_INT,				"KEYCODE_CTRL"	)->set<int>( 17 );
	_createConstant(	EMVT_INT,				"KEYCODE_ALT"	)->set<int>( 18 );
	_createConstant(	EMVT_INT,				"KEYCODE_CAPSLOCK"	)->set<int>( 20 );
	_createConstant(	EMVT_INT,				"KEYCODE_TAB"	)->set<int>( 9 );
	_createConstant(	EMVT_INT,				"KEYCODE_PERCENT"	)->set<int>( 192 );
	_createConstant(	EMVT_INT,				"KEYCODE_MU"	)->set<int>( 220 );
	_createConstant(	EMVT_INT,				"KEYCODE_QUESTION"	)->set<int>( 188 );
	_createConstant(	EMVT_INT,				"KEYCODE_DOT"	)->set<int>( 190 );
	_createConstant(	EMVT_INT,				"KEYCODE_SLASH"	)->set<int>( 191 );
	_createConstant(	EMVT_INT,				"KEYCODE_PARAGRAPH"	)->set<int>( 223 );
	_createConstant(	EMVT_INT,				"KEYCODE_SUPERIOR"	)->set<int>( 226 );
	_createConstant(	EMVT_INT,				"KEYCODE_SCROLLLOCK"	)->set<int>( 145 );
	_createConstant(	EMVT_INT,				"KEYCODE_PAUSE"	)->set<int>( 19 );
	_createConstant(	EMVT_INT,				"KEYCODE_ESCAPE"	)->set<int>( 27 );
	_createConstant(	EMVT_INT,				"KEYCODE_PAGEUP"	)->set<int>( 33 );
	_createConstant(	EMVT_INT,				"KEYCODE_PAGEDOWN"	)->set<int>( 34 );
	_createConstant(	EMVT_INT,				"KEYCODE_END"	)->set<int>( 35 );
	_createConstant(	EMVT_INT,				"KEYCODE_HOME"	)->set<int>( 36 );
	_createConstant(	EMVT_INT,				"KEYCODE_LEFT"	)->set<int>( 37 );
	_createConstant(	EMVT_INT,				"KEYCODE_UP"	)->set<int>( 38 );
	_createConstant(	EMVT_INT,				"KEYCODE_RIGHT"	)->set<int>( 39 );
	_createConstant(	EMVT_INT,				"KEYCODE_DOWN"	)->set<int>( 40 );
	_createConstant(	EMVT_INT,				"KEYCODE_SQUARE"	)->set<int>( 222 );
	_createConstant(	EMVT_INT,				"KEYCODE_DEGREE"	)->set<int>( 219 );
	_createConstant(	EMVT_INT,				"KEYCODE_PLUS"	)->set<int>( 187 );
	_createConstant(	EMVT_INT,				"KEYCODE_TREMA"	)->set<int>( 221 );
	_createConstant(	EMVT_INT,				"KEYCODE_POUND"	)->set<int>( 186 );
	_createConstant(	EMVT_INT,				"KEYCODE_BACKSPACE"	)->set<int>( 8 );
	_createConstant(	EMVT_INT,				"KEYCODE_DEL"	)->set<int>( 46 );
	_createConstant(	EMVT_INT,				"KEYCODE_INSERT"	)->set<int>( 45 );
	_createConstant(	EMVT_INT,				"KEYCODE_NUMLOCK"	)->set<int>( 144 );
	_createConstant(	EMVT_INT,				"KEYCODE_SPACE"	)->set<int>( 32 );
	_createConstant(	EMVT_INT,				"KEYCODE_F1"	)->set<int>( 112 );
	_createConstant(	EMVT_INT,				"KEYCODE_F2"	)->set<int>( 113 );
	_createConstant(	EMVT_INT,				"KEYCODE_F3"	)->set<int>( 114 );
	_createConstant(	EMVT_INT,				"KEYCODE_F4"	)->set<int>( 115 );
	_createConstant(	EMVT_INT,				"KEYCODE_F5"	)->set<int>( 116 );
	_createConstant(	EMVT_INT,				"KEYCODE_F6"	)->set<int>( 117 );
	_createConstant(	EMVT_INT,				"KEYCODE_F7"	)->set<int>( 118 );
	_createConstant(	EMVT_INT,				"KEYCODE_F8"	)->set<int>( 119 );
	_createConstant(	EMVT_INT,				"KEYCODE_F9"	)->set<int>( 120 );
	_createConstant(	EMVT_INT,				"KEYCODE_F10"	)->set<int>( 121 );
	_createConstant(	EMVT_INT,				"KEYCODE_F11"	)->set<int>( 122 );
	_createConstant(	EMVT_INT,				"KEYCODE_F12"	)->set<int>( 123 );

	for ( unsigned int i = 0 ; i < 26 ; i ++ )
	{
		_createConstant( EMVT_INT, String( "KEYCODE_" ) + static_cast <char>( 65 + i ) )->set<int>( 65 + i );
	}
}

void EMuse::Script::ScriptCompiler :: _initialiseFunctionMap()
{
	//Private function, for tests.
//		_creaFunc(	"Miaou"							,Gen_Miaou,						EMVT_NULL,		EMVT_NULL,		EMVT_NULL,									EMVT_NULL);
	_functionGroup( "General" );
	_creaFunc(	"for",							Gen_ForLoop,					EMVT_NULL, 		EMVT_CODE,		EMVT_BOOL,		EMVT_CODE,		EMVT_CODE,	EMVT_NULL );
	_creaFunc(	"while",						Gen_WhileLoop,					EMVT_NULL, 		EMVT_BOOL,		EMVT_CODE,									EMVT_NULL );
	_creaFunc(	"Echo",							Gen_Echo,						EMVT_NULL, 		EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"ClearScene",					Gen_ClearScene,					EMVT_NULL, 																	EMVT_NULL );
	_creaFunc(	"ClearSceneNode",				Gen_ClearSceneNode,				EMVT_NULL, 		EMVT_NODE,													EMVT_NULL );
	_creaFunc(	"ClearOverlays",				Gen_ClearOverlays,				EMVT_NULL, 																	EMVT_NULL );
	_creaFunc(	"BindKey",						Gen_BindKey,					EMVT_NULL, 		EMVT_INT,		EMVT_INT,		EMVT_CODE,					EMVT_NULL );
	_creaFunc(	"break",						Gen_Break,						EMVT_NULL, 																	EMVT_NULL );
	_creaFunc(	"continue",						Gen_Continue,					EMVT_NULL, 																	EMVT_NULL );
//		_creaFunc(	"return",						Gen_Return,						EMVT_NULL, 		EMVT_CODE, 													EMVT_NULL);
	_creaFunc(	"d_coucou",						Gen_Coucou,						EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"DescribeVar",					Gen_Describe,					EMVT_STRING,	EMVT_CODE,													EMVT_NULL );
	_creaFunc( "SetMouseLDownScript",			Gen_SetMouseLDownScript,		EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseLUpScript",				Gen_SetMouseLUpScript,			EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseRDownScript",			Gen_SetMouseRDownScript,		EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseRUpScript",				Gen_SetMouseRUpScript,			EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseMDownScript",			Gen_SetMouseMDownScript,		EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseMUpScript",				Gen_SetMouseMUpScript,			EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseWheelUpScript",			Gen_SetMouseWheelUpScript,		EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetMouseWheelDownScript",		Gen_SetMouseWheelDownScript,	EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetCharDownScript",				Gen_SetKeyDownScript,			EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetCharRepeatScript",			Gen_SetKeyRepeatScript,			EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_creaFunc( "SetNetworkScript",				Gen_SetNetworkScript,			EMVT_NULL,		EMVT_CODE, 													EMVT_NULL );
	_functionGroup( "Object" );
	_creaFunc(	"Object_GetByName",				Ent_GetByName,					EMVT_ENTITY,	EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Object_Create",				Ent_Create,						EMVT_ENTITY,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Object_CreateWithSkeleton",	Ent_CreateWithSkeleton,			EMVT_ENTITY,	EMVT_STRING,	EMVT_STRING,	EMVT_STRING,				EMVT_NULL );
	_creaFunc(	"Object_Destroy",				Ent_Destroy,					EMVT_NULL,		EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"SetMaterial",					Ent_SetMaterial,				EMVT_NULL,		EMVT_ENTITY,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"SetSubMaterial",				Ent_SetSubMaterial,				EMVT_NULL,		EMVT_ENTITY,	EMVT_INT,		EMVT_STRING,				EMVT_NULL );
	_classFunc(	"GetParent",					Ent_GetParent,					EMVT_NODE,		EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"GetBoundingRadius",			Ent_GetBoundingRadius,			EMVT_REAL,		EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"ShowBoundingBox",				Ent_ShowBoundingBox,			EMVT_NULL,		EMVT_ENTITY,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"Show",							Ent_Show,						EMVT_NULL,		EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"Hide",							Ent_Hide,						EMVT_NULL,		EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"SetVisible",					Ent_SetVisible,					EMVT_NULL,		EMVT_ENTITY,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"IsVisible",					Ent_IsVisible,					EMVT_BOOL,		EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"GetName",						Ent_GetName,					EMVT_STRING,	EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"GetMeshName",					Ent_GetMeshName,				EMVT_STRING,	EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"GetMaterialName",				Ent_GetMaterialName,			EMVT_STRING,	EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"SetParent",					Ent_SetParent,					EMVT_NULL,		EMVT_ENTITY,	EMVT_NODE,									EMVT_NULL );
	_classFunc(	"GetSubMaterialName",			Ent_GetSubMaterialName,			EMVT_STRING,	EMVT_ENTITY,	EMVT_INT,									EMVT_NULL );
	_classFunc(	"GetPhysicsObject",				Ent_GetPhysicsObject,			EMVT_PHY_OBJ,	EMVT_ENTITY,												EMVT_NULL );
	_classFunc(	"CopyPhysicsObject",			Ent_CopyPhysicsObject,			EMVT_PHY_OBJ,	EMVT_ENTITY,	EMVT_PHY_OBJ,								EMVT_NULL );
	_classFunc(	"GetNumSubMaterials",			Ent_GetNumSubMaterials,			EMVT_INT,		EMVT_ENTITY,												EMVT_NULL );
	_functionGroup( "Timer" );
	_creaFunc(	"Timer_Kill",					Tmr_Kill,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Timer_Shift",					Tmr_Shift,						EMVT_NULL,		EMVT_STRING,	EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"Timer_GetByName",				Tmr_GetByName,					EMVT_TIMER,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Timer_SetBaseTime",			Tmr_SetBaseTime,				EMVT_NULL,		EMVT_TIMER,		EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"Timer_Exists",					Tmr_Exists,						EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"timer",						Tmr_Create,						EMVT_TIMER,		EMVT_STRING,	EMVT_INT,		EMVT_REAL,		EMVT_CODE,	EMVT_NULL );
	_creaFunc(	"ContinuousTimer",				Tmr_CreateContinuous,			EMVT_TIMER,		EMVT_STRING,	EMVT_REAL,		EMVT_CODE,		EMVT_CODE,	EMVT_NULL );
	_creaFunc(	"permanent_timer",				Tmr_CreatePermanent,			EMVT_TIMER,		EMVT_STRING,	EMVT_CODE,									EMVT_NULL );
	_creaFunc(	"Timer_Pause",					Tmr_Pause,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Timer_Play",					Tmr_Play,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Timer_PauseAll",				Tmr_PauseAll,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Timer_KillAll",				Tmr_KillAll,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Timer_PlayAll",				Tmr_PlayAll,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Timeline_Pause",				Tml_Pause,						EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Timeline_Play",				Tml_Play,						EMVT_NULL,																	EMVT_NULL );
	_classFunc(	"SetBaseTime",					Tmr_SetBaseTime,				EMVT_NULL,		EMVT_TIMER,		EMVT_REAL,									EMVT_NULL );
	_classFunc(	"GetNumExecs",					Tmr_GetNumExecs,				EMVT_INT,		EMVT_TIMER,													EMVT_NULL );
	_classFunc(	"GetTimeLeft",					Tmr_GetTimeLeft,				EMVT_REAL,		EMVT_TIMER,													EMVT_NULL );
	_classFunc(	"GetTimeElapsed",				Tmr_GetTimeElapsed,				EMVT_REAL,		EMVT_TIMER,													EMVT_NULL );
	_classFunc(	"GetTimeBase",					Tmr_GetTimeBase,				EMVT_REAL,		EMVT_TIMER,													EMVT_NULL );
	_classFunc(	"GetName",						Tmr_GetName,					EMVT_STRING,	EMVT_TIMER,													EMVT_NULL );
	_functionGroup( "Vector" );
	_creaFunc(	"SetVector3",					Ve3_Set,						EMVT_VECTOR3,	EMVT_REAL,		EMVT_REAL,		EMVT_REAL, 					EMVT_NULL );
	_creaFunc(	"V3_CubicInterpolation",		Ve3_CubicInterpolation,			EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_creaFunc(	"V3_LinearInterpolation",		Ve3_LinearInterpolation,		EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_creaFunc(	"V3_SquaredInterpolation",		Ve3_SquaredInterpolation,		EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_creaFunc(	"V3_SinusInterpolation",		Ve3_SinusInterpolation,			EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_creaFunc(	"V3_BezierInterpolation",		Ve3_BezierInterpolation,		EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL,	EMVT_NULL );
	_creaFunc(	"V3_BezierGetLength",			Ve3_BezierGetLength,			EMVT_REAL,		EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_INT,	EMVT_NULL );
	_creaFunc(	"V3_GetLength",					Ve3_GetLength,					EMVT_REAL,		EMVT_VECTOR3,												EMVT_NULL );
	_creaFunc(	"V3_OperatorArray",				Ve3_OperatorArray,				EMVT_REAL,		EMVT_VECTOR3,	EMVT_INT,									EMVT_NULL );
	_creaFunc(	"V3_SnapCoords",				Ve3_SnapCoords,					EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"V3_ProjectOnScreen",			Ve3_ProjectOnScreen,			EMVT_VECTOR3,	EMVT_VECTOR3,												EMVT_NULL );
	_creaFunc(	"V3_DotProduct",				Ve3_DotProduct,					EMVT_REAL,		EMVT_VECTOR3,	EMVT_VECTOR3,								EMVT_NULL );
	_creaFunc(	"V3_CrossProduct",				Ve3_CrossProduct,				EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3,								EMVT_NULL );
	_creaFunc(	"V3_GetRotationTo",				Ve3_GetRotationTo,				EMVT_QUATERNION, EMVT_VECTOR3,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"Length",						Ve3_GetLength,					EMVT_REAL,		EMVT_VECTOR3, 												EMVT_NULL );
	_classFunc(	"Normalise",					Ve3_Normalise,					EMVT_NULL,		EMVT_VECTOR3, 												EMVT_NULL );
	_classFunc(	"NormalisedCopy",				Ve3_NormalisedCopy,				EMVT_VECTOR3,	EMVT_VECTOR3, 												EMVT_NULL );
	_classFunc(	"Dot",							Ve3_DotProduct,					EMVT_REAL,		EMVT_VECTOR3,	EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"Cross",						Ve3_CrossProduct,				EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_VECTOR3, 								EMVT_NULL );
	_functionGroup( "Quaternion" );
	_creaFunc(	"SetQuaternion",				Qua_FromAxisAngle,				EMVT_QUATERNION, EMVT_VECTOR3,	EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"Quat_Slerp",					Qua_Slerp,						EMVT_QUATERNION, EMVT_QUATERNION, EMVT_QUATERNION, EMVT_REAL,					EMVT_NULL );
	_classFunc(	"GetAngle",						Qua_GetAngle,					EMVT_REAL,		EMVT_QUATERNION, 											EMVT_NULL );
	_classFunc(	"GetAxis",						Qua_GetAxis,					EMVT_VECTOR3,	EMVT_QUATERNION, 											EMVT_NULL );
	_functionGroup( "Overlay Element" );
	_creaFunc(	"OverlayElement_Exists",		OvE_Exists,						EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"OverlayElement_Create",		OvE_Create,						EMVT_OVERLAY,	EMVT_STRING,	EMVT_STRING,	EMVT_STRING,				EMVT_NULL );
	_creaFunc(	"OverlayElement_Destroy",		OvE_Destroy,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"OverlayElement_MousePickup",	OvE_MousePickup,				EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"OverlayElement_GetByName",		OvE_GetByName,					EMVT_OVERLAY,	EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Overlay_SetAction",			OvE_SetAction,					EMVT_NULL,		EMVT_STRING,	EMVT_CODE,									EMVT_NULL );
	_classFunc(	"GetPosition",					OvE_GetPosition,				EMVT_VECTOR3,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetLeft",						OvE_GetLeft,					EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetTop",						OvE_GetTop,						EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetAbsolutePosition",			OvE_GetAbsolutePosition,		EMVT_VECTOR3,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetAbsoluteLeft",				OvE_GetAbsoluteLeft,			EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetAbsoluteTop",				OvE_GetAbsoluteTop,				EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"SetPosition",					OvE_SetPosition,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetLeft",						OvE_SetLeft,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetTop",						OvE_SetTop,						EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetWidth",						OvE_SetWidth,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetHeight",					OvE_SetHeight,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"GetName",						OvE_GetName,					EMVT_STRING,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"SetTextColour",				OvE_SetTextColour,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_COLOUR,								EMVT_NULL );
	_classFunc(	"GetTextColour",				OvE_GetTextColour,				EMVT_COLOUR,	EMVT_OVERLAY,												EMVT_NULL );
	_classFunc(	"Translate",					OvE_Translation,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"Resize",						OvE_Resize,						EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"SetCaption",					OvE_SetCaption,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetMaterial",					OvE_SetMaterial,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetMouseOverMaterial",			OvE_SetMouseOverMaterial,		EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetBorderMaterial",			OvE_SetBorderMaterial,			EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetSize",						OvE_SetSize,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetLRBorderSize",				OvE_SetLRBorderSize,			EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetTBBorderSize",				OvE_SetTBBorderSize,			EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_classFunc(	"Show",							OvE_Show,						EMVT_NULL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"Hide",							OvE_Hide,						EMVT_NULL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"SetVisible",					OvE_SetVisible,					EMVT_NULL,		EMVT_OVERLAY, 	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"IsVisible",					OvE_IsVisible,					EMVT_BOOL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"TextureLookUp",				OvE_TextureLookUp,				EMVT_COLOUR,	EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_classFunc(	"GetWidth",						OvE_GetWidth,					EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetHeight",					OvE_GetHeight,					EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetParent",					OvE_GetParent,					EMVT_OVERLAY,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"Clone",						OvE_Clone,						EMVT_OVERLAY,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetCaption",					OvE_GetCaption,					EMVT_STRING,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"GetMaterialName",				OvE_GetMaterialName,			EMVT_STRING,	EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"LimitToParent",				OvE_LimitToParent,				EMVT_NULL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"SetFontSize",					OvE_SetFontSize,				EMVT_NULL,		EMVT_OVERLAY, 	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"GetFontSize",					OvE_GetFontSize,				EMVT_REAL,		EMVT_OVERLAY, 												EMVT_NULL );
	_classFunc(	"SetAlpha",						OvE_SetAlpha,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_classFunc(	"ScrollUV",						OvE_ScrollUV,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc(	"SetScrollUV",					OvE_SetScrollUV,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc(	"ScaleUV",						OvE_ScaleUV,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc(	"SetScaleUV",					OvE_SetScaleUV,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc(	"RotateUV",						OvE_RotateUV,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetRotateUV",					OvE_SetRotateUV,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_functionGroup( "Overlay" );
	_creaFunc(	"Overlay_Show",					OvL_Show,						EMVT_NULL,		EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"Overlay_Hide",					OvL_Hide,						EMVT_NULL,		EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"Overlay_HideAll",				OvL_HideAll,					EMVT_NULL,					 												EMVT_NULL );
	_creaFunc(	"Overlay_SetVisible",			OvL_SetVisible,					EMVT_NULL,		EMVT_STRING, 	EMVT_BOOL,									EMVT_NULL );
	_creaFunc(	"Overlay_IsVisible",			OvL_IsVisible,					EMVT_BOOL,		EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"Overlay_SetRotate",			OvL_SetRotate,					EMVT_NULL,		EMVT_STRING,	EMVT_REAL, 									EMVT_NULL );
	_creaFunc(	"Overlay_Rotate",				OvL_Rotate,						EMVT_NULL,		EMVT_STRING,	EMVT_REAL, 									EMVT_NULL );
	_functionGroup( "Scene node" );
	_creaFunc(	"SceneNode_GetByName",			ScN_GetByName,					EMVT_NODE,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"SceneNode_Exists",				ScN_Exists,						EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"SceneNode_Create",				ScN_Create,						EMVT_NODE,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"SceneNode_Destroy",			ScN_Delete,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_classFunc(	"Translate",					ScN_Translate,					EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"TranslateLocal",				ScN_TranslateLocal,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"RotateLocal",					ScN_RotateLocal,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"Rotate",						ScN_Rotate,						EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"RotateAround",					ScN_RotateAround,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL,	EMVT_NULL );
	_classFunc(	"RotateQuat",					ScN_RotateQuat,					EMVT_NULL,		EMVT_NODE,		EMVT_QUATERNION,							EMVT_NULL );
	_classFunc(	"Scale",						ScN_Scale,						EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"SetScale",						ScN_SetScale,					EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"Yaw",							ScN_Yaw,						EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"GetYaw",						ScN_GetYaw,						EMVT_REAL,		EMVT_NODE, 													EMVT_NULL );
	_classFunc(	"Roll",							ScN_Roll,						EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"GetRoll",						ScN_GetRoll,					EMVT_REAL,		EMVT_NODE, 													EMVT_NULL );
	_classFunc(	"Pitch",						ScN_Pitch,						EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"GetPitch",						ScN_GetPitch,					EMVT_REAL,		EMVT_NODE, 													EMVT_NULL );
	_classFunc(	"GetPosition",					ScN_GetPosition,				EMVT_VECTOR3,	EMVT_NODE, 													EMVT_NULL );
	_classFunc(	"GetAbsolutePosition",			ScN_GetDerivedPosition,			EMVT_VECTOR3,	EMVT_NODE, 													EMVT_NULL );
	_classFunc(	"SetPosition",					ScN_SetPosition,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetXPosition",					ScN_SetXPosition,				EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetYPosition",					ScN_SetYPosition,				EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetZPosition",					ScN_SetZPosition,				EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"GetName",						ScN_GetName,					EMVT_STRING,	EMVT_NODE,													EMVT_NULL );
	_classFunc(	"Reset",						ScN_Reset,						EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"Group",						ScN_Group,						EMVT_NULL,		EMVT_NODE,		EMVT_NODE,									EMVT_NULL );
	_classFunc(	"Ungroup",						ScN_Ungroup,					EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"GetParent",					ScN_GetParent,					EMVT_NODE,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"UngroupAll",					ScN_UngroupAll,					EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"Recenter",						ScN_Recenter,					EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"SetParent",					ScN_AttachTo,					EMVT_NULL,		EMVT_NODE,		EMVT_NODE,									EMVT_NULL );
	_classFunc(	"GetOrientation",				ScN_GetOrientation,				EMVT_QUATERNION, EMVT_NODE,													EMVT_NULL );
	_classFunc(	"SetOrientation",				ScN_SetOrientation,				EMVT_NULL,		EMVT_NODE,		EMVT_QUATERNION,							EMVT_NULL );
	_classFunc(	"SetVisible",					ScN_SetVisible,					EMVT_NULL,		EMVT_NODE,		EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"Show",							ScN_Show,						EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"Hide",							ScN_Hide,						EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"SetInheritOrientation",		ScN_SetInheritOrientation,		EMVT_NULL,		EMVT_NODE,		EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"SetInheritScale",				ScN_SetInheritScale,			EMVT_NULL,		EMVT_NODE,		EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"GetInheritOrientation",		ScN_GetInheritOrientation,		EMVT_BOOL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"GetInheritScale",				ScN_GetInheritScale,			EMVT_BOOL,		EMVT_NODE,													EMVT_NULL );
	_functionGroup( "Camera" );
	_creaFunc(	"Camera_GetByName",				Cam_GetByName,					EMVT_CAMERA,	EMVT_STRING, 												EMVT_NULL );
	_classFunc(	"GetParent",					Cam_GetParent,					EMVT_NODE,		EMVT_CAMERA, 												EMVT_NULL );
	_classFunc(	"ModeWireFrame",				Cam_ModeWireFrame,				EMVT_NULL,		EMVT_CAMERA,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"GetLocalXAxis",				Cam_GetLocalXAxis,				EMVT_VECTOR3,	EMVT_CAMERA, 												EMVT_NULL );
	_classFunc(	"GetLocalYAxis",				Cam_GetLocalYAxis,				EMVT_VECTOR3,	EMVT_CAMERA, 												EMVT_NULL );
	_classFunc(	"GetLocalZAxis",				Cam_GetLocalZAxis,				EMVT_VECTOR3,	EMVT_CAMERA, 												EMVT_NULL );
	_classFunc(	"AttachTo",						Cam_AttachTo,					EMVT_NULL,		EMVT_CAMERA,	EMVT_NODE,									EMVT_NULL );
	_classFunc(	"GetPosition",					Cam_GetPosition,				EMVT_VECTOR3,	EMVT_CAMERA,												EMVT_NULL );
	_classFunc(	"SetPosition",					Cam_SetPosition,				EMVT_NULL,		EMVT_CAMERA,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetAutoTrack",					Cam_SetAutoTrack,				EMVT_NULL,		EMVT_CAMERA,	EMVT_NODE,									EMVT_NULL );
	_classFunc( "Clamp",						Cam_Clamp,						EMVT_NULL,		EMVT_CAMERA,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_classFunc( "GetScreenCoords",				Cam_GetScreenCoords,			EMVT_VECTOR3,	EMVT_CAMERA,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc( "IsPositionVisible",			Cam_IsPositionVisible,			EMVT_BOOL,		EMVT_CAMERA,	EMVT_VECTOR3,								EMVT_NULL );
	_functionGroup( "CamTex" );
	_creaFunc(	"CamTex_GetByName",				CTx_GetByName,					EMVT_CAMTEX,	EMVT_STRING,												EMVT_NULL );
	_classFunc(	"Refresh",						CTx_Refresh,					EMVT_NULL,		EMVT_CAMTEX,												EMVT_NULL );
	_classFunc(	"GetCamera",					CTx_GetCamera,					EMVT_CAMERA,	EMVT_CAMTEX,												EMVT_NULL );
	_classFunc(	"SetMain",						CTx_SetMain,					EMVT_NULL,		EMVT_CAMTEX,												EMVT_NULL );
	_classFunc(	"SetBackgroundColour",			CTx_SetBackgroundColour,		EMVT_NULL,		EMVT_CAMTEX,	EMVT_COLOUR,								EMVT_NULL );
	_functionGroup( "Light" );
	_creaFunc(	"Light_GetByName",				Lgh_GetByName,					EMVT_LIGHT,		EMVT_STRING,												EMVT_NULL );
	_classFunc(	"SetDirection",					Lgh_SetDirection,				EMVT_NULL,		EMVT_LIGHT,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetPosition",					Lgh_SetPosition,				EMVT_NULL,		EMVT_LIGHT,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetDiffuse",					Lgh_SetDiffuse,					EMVT_NULL,		EMVT_LIGHT,		EMVT_COLOUR,								EMVT_NULL );
	_classFunc(	"SetSpecular",					Lgh_SetSpecular,				EMVT_NULL,		EMVT_LIGHT,		EMVT_COLOUR,								EMVT_NULL );
	_classFunc(	"SetVisible",					Lgh_SetVisible,					EMVT_NULL,		EMVT_LIGHT,		EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"IsVisible",					Lgh_IsVisible,					EMVT_BOOL,		EMVT_LIGHT,													EMVT_NULL );
	_classFunc(	"Show",							Lgh_Show,						EMVT_NULL,		EMVT_LIGHT,													EMVT_NULL );
	_classFunc(	"Hide",							Lgh_Hide,						EMVT_NULL,		EMVT_LIGHT,													EMVT_NULL );
	_functionGroup( "Array" );
	_classFunc(	"Add",							Arr_Add,						EMVT_NULL,		EMVT_ARRAY,		EMVT_SUB1,									EMVT_NULL );
	_classFunc(	"AddDefault",					Arr_AddDefault,					EMVT_SUB1,		EMVT_ARRAY,											EMVT_NULL );
	_classFunc(	"Erase",						Arr_RemoveIndex,				EMVT_NULL,		EMVT_ARRAY,		EMVT_INT,									EMVT_NULL );
	_classFunc(	"Size",							Arr_GetSize,					EMVT_INT,		EMVT_ARRAY,													EMVT_NULL );
	_classFunc(	"Clear",						Arr_Clear,						EMVT_NULL,		EMVT_ARRAY,													EMVT_NULL );
	_classFunc(	"Set",							Arr_Set,						EMVT_NULL,		EMVT_ARRAY,		EMVT_INT,	EMVT_SUB1,						EMVT_NULL );
	_classFunc(	"Get",							Arr_Get,						EMVT_SUB1,		EMVT_ARRAY,		EMVT_INT,									EMVT_NULL );
	_functionGroup( "Int Maps" );
	_classFunc(	"Add",							Map_Int_Add,					EMVT_NULL,		EMVT_INTMAP,	EMVT_INT,	EMVT_SUB1,						EMVT_NULL );
	_classFunc(	"AddDefault",					Map_Int_AddDef,					EMVT_SUB1,		EMVT_INTMAP,	EMVT_INT,									EMVT_NULL );
	_classFunc(	"Size",							Map_Int_Size,					EMVT_INT,		EMVT_INTMAP,												EMVT_NULL );
	_classFunc(	"Has",							Map_Int_Has,					EMVT_BOOL,		EMVT_INTMAP,	EMVT_INT,									EMVT_NULL );
	_classFunc(	"Set",							Map_Int_Set,					EMVT_BOOL,		EMVT_INTMAP,	EMVT_INT,	EMVT_SUB1,						EMVT_NULL );
	_classFunc(	"Find",							Map_Int_Find,					EMVT_SUB1,		EMVT_INTMAP,	EMVT_INT,									EMVT_NULL );
	_classFunc(	"Clear",						Map_Int_Clear,					EMVT_NULL,		EMVT_INTMAP,												EMVT_NULL );
	_classFunc(	"IsEmpty",						Map_Int_Empty,					EMVT_NULL,		EMVT_INTMAP,												EMVT_NULL );
	_classFunc(	"Erase",						Map_Int_Erase,					EMVT_BOOL,		EMVT_INTMAP,	EMVT_INT,									EMVT_NULL );
	_classFunc(	"Begin",						Map_Int_IterBegin,				EMVT_ITER,		EMVT_INTMAP,												EMVT_NULL );
	_classFunc(	"Find_Iter",					Map_Int_IterFind,				EMVT_ITER,		EMVT_INTMAP,	EMVT_INT,									EMVT_NULL );
	_classFunc(	"EraseIter",					Map_Int_EraseIter,				EMVT_NULL,		EMVT_INTMAP,	EMVT_IMITER,								EMVT_NULL );
	_classFunc(	"End",							Map_Int_IterEnd,				EMVT_ITER,		EMVT_INTMAP,												EMVT_NULL );
	_functionGroup( "String Maps" );
	_classFunc(	"Add",							Map_String_Add,					EMVT_NULL,		EMVT_STRINGMAP,	EMVT_STRING,	EMVT_SUB1,					EMVT_NULL );
	_classFunc(	"AddDefault",					Map_String_AddDef,				EMVT_SUB1,		EMVT_STRINGMAP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"Size",							Map_String_Size,				EMVT_INT,		EMVT_STRINGMAP,												EMVT_NULL );
	_classFunc(	"Has",							Map_String_Has,					EMVT_BOOL,		EMVT_STRINGMAP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"Set",							Map_String_Set,					EMVT_BOOL,		EMVT_STRINGMAP,	EMVT_STRING,	EMVT_SUB1,					EMVT_NULL );
	_classFunc(	"Find",							Map_String_Find,				EMVT_SUB1,		EMVT_STRINGMAP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"Clear",						Map_String_Clear,				EMVT_NULL,		EMVT_STRINGMAP,												EMVT_NULL );
	_classFunc(	"IsEmpty",						Map_String_Empty,				EMVT_NULL,		EMVT_STRINGMAP,												EMVT_NULL );
	_classFunc(	"Erase",						Map_String_Erase,				EMVT_BOOL,		EMVT_STRINGMAP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"Begin",						Map_String_IterBegin,			EMVT_ITER,		EMVT_STRINGMAP,												EMVT_NULL );
	_classFunc(	"Find_Iter",					Map_String_IterFind,			EMVT_ITER,		EMVT_STRINGMAP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"EraseIter",					Map_String_EraseIter,			EMVT_NULL,		EMVT_STRINGMAP,	EMVT_SMITER,								EMVT_NULL );
	_classFunc(	"End",							Map_String_IterEnd,				EMVT_ITER,		EMVT_STRINGMAP,												EMVT_NULL );
	_functionGroup( "Real Maps" );
	_classFunc(	"Add",							Map_Real_Add,					EMVT_NULL,		EMVT_REALMAP,	EMVT_REAL,	EMVT_SUB1,						EMVT_NULL );
	_classFunc(	"AddDefault",					Map_Real_AddDef,				EMVT_SUB1,		EMVT_REALMAP,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"Size",							Map_Real_Size,					EMVT_INT,		EMVT_REALMAP,												EMVT_NULL );
	_classFunc(	"Has",							Map_Real_Has,					EMVT_BOOL,		EMVT_REALMAP,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"Set",							Map_Real_Set,					EMVT_BOOL,		EMVT_REALMAP,	EMVT_REAL,	EMVT_SUB1,						EMVT_NULL );
	_classFunc(	"Find",							Map_Real_Find,					EMVT_SUB1,		EMVT_REALMAP,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"Clear",						Map_Real_Clear,					EMVT_NULL,		EMVT_REALMAP,												EMVT_NULL );
	_classFunc(	"IsEmpty",						Map_Real_Empty,					EMVT_NULL,		EMVT_REALMAP,												EMVT_NULL );
	_classFunc(	"Erase",						Map_Real_Erase,					EMVT_BOOL,		EMVT_REALMAP,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"Begin",						Map_Real_IterBegin,				EMVT_ITER,		EMVT_REALMAP,												EMVT_NULL );
	_classFunc(	"Find_Iter",					Map_Real_IterFind,				EMVT_ITER,		EMVT_REALMAP,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"EraseIter",					Map_Real_EraseIter,				EMVT_NULL,		EMVT_REALMAP,	EMVT_RMITER,								EMVT_NULL );
	_classFunc(	"End",							Map_Real_IterEnd,				EMVT_ITER,		EMVT_REALMAP,												EMVT_NULL );
	_functionGroup( "MapIterators" );
	_classFunc(	"Next",							Sit_Next,						EMVT_NULL,		EMVT_SMITER,												EMVT_NULL );
	_classFunc(	"Prev",							Sit_Prev,						EMVT_NULL,		EMVT_SMITER,												EMVT_NULL );
	_classFunc(	"Key",							Sit_Key,						EMVT_STRING,	EMVT_SMITER,												EMVT_NULL );
	_classFunc(	"Data",							Sit_Data,						EMVT_SUB2,		EMVT_SMITER,												EMVT_NULL );
	_classFunc(	"Next",							Iit_Next,						EMVT_NULL,		EMVT_IMITER,												EMVT_NULL );
	_classFunc(	"Prev",							Iit_Prev,						EMVT_NULL,		EMVT_IMITER,												EMVT_NULL );
	_classFunc(	"Key",							Iit_Key,						EMVT_INT,		EMVT_IMITER,												EMVT_NULL );
	_classFunc(	"Data",							Iit_Data,						EMVT_SUB2,		EMVT_IMITER,												EMVT_NULL );
	_classFunc(	"Next",							Rit_Next,						EMVT_NULL,		EMVT_RMITER,												EMVT_NULL );
	_classFunc(	"Prev",							Rit_Prev,						EMVT_NULL,		EMVT_RMITER,												EMVT_NULL );
	_classFunc(	"Key",							Rit_Key,						EMVT_REAL,		EMVT_RMITER,												EMVT_NULL );
	_classFunc(	"Data",							Rit_Data,						EMVT_SUB2,		EMVT_RMITER,												EMVT_NULL );
	_functionGroup( "Pair" );
	_classFunc(	"First",						Par_First,						EMVT_SUB1,		EMVT_PAIR,													EMVT_NULL );
	_classFunc(	"Second",						Par_Second,						EMVT_SUB2,		EMVT_PAIR,													EMVT_NULL );
	_classFunc(	"Set",							Par_Set,						EMVT_NULL,		EMVT_PAIR,		EMVT_SUB1,	EMVT_SUB2,						EMVT_NULL );
	_functionGroup( "Colour" );
	_creaFunc(	"SetColourRGB",					Col_Set3,						EMVT_COLOUR,	EMVT_REAL,		EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_creaFunc(	"SetColourRGBA",				Col_Set4,						EMVT_COLOUR,	EMVT_REAL,		EMVT_REAL,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc(	"R",							Col_GetR,						EMVT_REAL,		EMVT_COLOUR,												EMVT_NULL );
	_classFunc(	"G",							Col_GetG,						EMVT_REAL,		EMVT_COLOUR,												EMVT_NULL );
	_classFunc(	"B",							Col_GetB,						EMVT_REAL,		EMVT_COLOUR,												EMVT_NULL );
	_classFunc(	"A",							Col_GetB,						EMVT_REAL,		EMVT_COLOUR,												EMVT_NULL );
	_classFunc(	"SetR",							Col_SetR,						EMVT_NULL,		EMVT_COLOUR,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"SetG",							Col_SetG,						EMVT_NULL,		EMVT_COLOUR,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"SetB",							Col_SetB,						EMVT_NULL,		EMVT_COLOUR,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"SetA",							Col_SetB,						EMVT_NULL,		EMVT_COLOUR,	EMVT_REAL,									EMVT_NULL );
	_functionGroup( "Sound" );
	_creaFunc(	"Sound_Play",					Snd_Play,						EMVT_NULL, 		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Sound_Play_Global",			Snd_Play_Global,				EMVT_NULL, 		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Sound_Play_All",				Snd_Play_All,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Sound_Pause",					Snd_Pause,						EMVT_NULL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Sound_Pause_Global",			Snd_Pause_Global,				EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Sound_Pause_All",				Snd_Pause_All,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Sound_Stop",					Snd_Stop,						EMVT_NULL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Sound_Stop_Global",			Snd_Stop_Global,				EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Sound_Stop_All",				Snd_Stop_All,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"Sound_SetVolume",				Snd_SetVolume,					EMVT_NULL, 		EMVT_STRING,	EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"Sound_SetSFXVolume",			Snd_SetSFXVolume,				EMVT_NULL, 		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"Sound_SetMusicVolume",			Snd_SetMusicVolume,				EMVT_NULL, 		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"Sound_MuteSFX",				Snd_MuteSFX,					EMVT_NULL, 		EMVT_BOOL,													EMVT_NULL );
	_creaFunc(	"Sound_MuteMusic",				Snd_MuteMusic,					EMVT_NULL, 		EMVT_BOOL,													EMVT_NULL );
	_functionGroup( "Playlist" );
	_creaFunc(	"Playlist_Start",				Pll_Start,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Playlist_Pause",				Pll_Pause,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Playlist_Stop",				Pll_Stop,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_functionGroup(	"Portal" );
	_creaFunc(	"Portal_GetTargetName",			Por_GetTargetName,				EMVT_STRING,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Portal_GetTargetZone",			Por_GetTargetZone,				EMVT_STRING,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_functionGroup( "Video" );
	_creaFunc(	"Video_Create",					Vid_Create,						EMVT_NULL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Video_CreateInstance",			Vid_CreateInstance,				EMVT_NULL,		EMVT_STRING,	EMVT_ENTITY,								EMVT_NULL );
	_creaFunc(	"Video_Destroy",				Vid_Destroy,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Video_DestroyInstance",		Vid_DestroyInstance,			EMVT_NULL,		EMVT_STRING,	EMVT_ENTITY,								EMVT_NULL );
	_creaFunc(	"Video_Play",					Vid_Play,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Video_Pause",					Vid_Pause,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Video_Stop",					Vid_Stop,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Video_SetVolume",				Vid_SetVolume,					EMVT_NULL,		EMVT_INT,													EMVT_NULL );
	_creaFunc(	"Video_Mute",					Vid_Mute,						EMVT_NULL,		EMVT_BOOL,													EMVT_NULL );
	_creaFunc(	"Video_SetLooped",				Vid_SetLooped,					EMVT_NULL,		EMVT_STRING,	EMVT_BOOL,									EMVT_NULL );
	_creaFunc(	"Video_ChangeVideo",			Vid_ChangeVideo,				EMVT_NULL,		EMVT_STRING,	EMVT_ENTITY,								EMVT_NULL );
	_creaFunc(	"Video_Initialise",				Vid_Initialise,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Video_Reset",					Vid_Reset,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "VideoObject_Delay",			Vid_DelayObject,				EMVT_NULL,		EMVT_STRING,	EMVT_ENTITY,								EMVT_NULL );
	_creaFunc( "VideoOverlay_Delay",			Vid_DelayOverlay,				EMVT_NULL,		EMVT_STRING,	EMVT_OVERLAY,								EMVT_NULL );
	_creaFunc( "Video_InitialiseObject",		Vid_InitialiseObject,			EMVT_NULL,		EMVT_STRING,	EMVT_ENTITY,								EMVT_NULL );
	_creaFunc( "Video_InitialiseOverlay",		Vid_InitialiseOverlay,			EMVT_NULL,		EMVT_STRING,	EMVT_OVERLAY,								EMVT_NULL );
	_creaFunc( "Video_SetVolumePercent",		Vid_SetVolumePercent,			EMVT_NULL,		EMVT_STRING,	EMVT_REAL,									EMVT_NULL );
	_functionGroup( "Miscellaneous" );
	_creaFunc(	"GetCurrentSecond",				Tim_GetCurrentSecond,			EMVT_INT,																	EMVT_NULL );
	_creaFunc(	"GetCurrentMinute",				Tim_GetCurrentMinute,			EMVT_INT,																	EMVT_NULL );
	_creaFunc(	"GetCurrentHour",				Tim_GetCurrentHour,				EMVT_INT,																	EMVT_NULL );
	_creaFunc(	"StartDownloadingMuseFile",		Spc_Download,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "GetMD5Hash",					Spc_GetMD5Hash,					EMVT_STRING,	EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"SaveScript",					Spc_SaveScript,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"ObjectDuplicate",				Spc_ObjectDuplicate,			EMVT_ENTITY,	EMVT_ENTITY,												EMVT_NULL );
	_creaFunc(	"ObjectDelete",					Spc_ObjectDelete,				EMVT_NULL,		EMVT_ENTITY,												EMVT_NULL );
	_creaFunc(	"GetGroundCoords",				Spc_GetGroundCoords,			EMVT_VECTOR3,	EMVT_REAL,		EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"MousePickupObject",			Spc_MousePickupObject,			EMVT_ENTITY,	EMVT_REAL,		EMVT_REAL,		EMVT_BOOL,					EMVT_NULL );
	_creaFunc(	"ImportMesh",					Spc_ImportMesh,					EMVT_ENTITY,																EMVT_NULL );
	_creaFunc(	"StringFromUrl",				Spc_StringFromUrl,				EMVT_STRING,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"ThreadedStringFromUrl",		Spc_ThreadedStringFromUrl,		EMVT_NULL,		EMVT_STRING,	EMVT_STRING,	EMVT_STRING,	EMVT_CODE,	EMVT_NULL );
	_creaFunc(	"SetSessionCookie",				Spc_SetSessionCookie,			EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"GetWebFileType",				Spc_GetWebFileType,				EMVT_STRING,	EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"OpenUrlInWindow",				Spc_OpenUrl,					EMVT_BOOL,		EMVT_STRING,	EMVT_BOOL,									EMVT_NULL );
	_creaFunc(	"ExecuteJavaScript",			Spc_ExecuteJavaScript,			EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Quit",							Spc_Quit,						EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"ClosestObject",				Spc_ClosestObject,				EMVT_ENTITY,	EMVT_VECTOR3,												EMVT_NULL );
	_creaFunc(	"TestAllEntities",				Spc_TestAllEntities,			EMVT_NULL,		EMVT_NULL,													EMVT_NULL );
	_creaFunc(	"GetCharWidth",					Spc_GetCharWidth,				EMVT_REAL,		EMVT_CHAR,		EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_creaFunc(	"GetLineWidth",					Spc_GetLineWidth,				EMVT_REAL,		EMVT_STRING,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_creaFunc(	"GetMuseFileDirectory",			Spc_GetMuseDirectory,			EMVT_STRING,																EMVT_NULL );
	_creaFunc(	"GetCurrentDirectory",			Spc_GetCurrentDirectory,		EMVT_STRING,																EMVT_NULL );
	_creaFunc(	"SetCursor",					Spc_SetCursor,					EMVT_NULL,		EMVT_INT,													EMVT_NULL );
	_functionGroup( "Cast" );
	_creaFunc(	"rtoi",							Cst_Real_To_Int,				EMVT_INT,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"rtoa",							Cst_Real_To_String,				EMVT_STRING,	EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"itor",							Cst_Int_To_Real,				EMVT_REAL,		EMVT_INT,													EMVT_NULL );
	_creaFunc(	"itob",							Cst_Int_To_Bool,				EMVT_BOOL,		EMVT_INT,													EMVT_NULL );
	_creaFunc(	"itoa",							Cst_Int_To_String,				EMVT_STRING,	EMVT_INT,													EMVT_NULL );
	_creaFunc(	"itoc",							Cst_Int_To_Char,				EMVT_CHAR,		EMVT_INT,													EMVT_NULL );
	_creaFunc(	"atoi",							Cst_String_To_Int,				EMVT_INT,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"ator",							Cst_String_To_Real,				EMVT_REAL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"atob",							Cst_String_To_Bool,				EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"atoc",							Cst_String_To_Char,				EMVT_CHAR,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"btoi",							Cst_Bool_To_Int,				EMVT_INT,		EMVT_BOOL,													EMVT_NULL );
	_creaFunc(	"btoa",							Cst_Bool_To_String,				EMVT_STRING,	EMVT_BOOL,													EMVT_NULL );
	_creaFunc(	"ctoa",							Cst_Char_To_String,				EMVT_STRING,	EMVT_CHAR,													EMVT_NULL );
	_creaFunc(	"ctoi",							Cst_Char_To_Int,				EMVT_INT,		EMVT_CHAR,													EMVT_NULL );
	_creaFunc(	"vtoa",							Cst_Vector3_To_String,			EMVT_STRING,	EMVT_VECTOR3,												EMVT_NULL );
	_creaFunc(	"qtoa",							Cst_Quaternion_To_String,		EMVT_STRING,	EMVT_QUATERNION,											EMVT_NULL );
	_functionGroup( "Math" );
	_creaFunc(	"rand",							Mth_Rand,						EMVT_REAL,		EMVT_REAL,		EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"abs",							Mth_Abs,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"floor",						Mth_Floor,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"ceil",							Mth_Ceil,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"round",						Mth_Round,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"max",							Mth_Min,						EMVT_REAL,		EMVT_REAL,		EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"min",							Mth_Max,						EMVT_REAL,		EMVT_REAL,		EMVT_REAL,									EMVT_NULL );
	_creaFunc(	"minmax",						Mth_Minmax,						EMVT_REAL,		EMVT_REAL,		EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_creaFunc(	"sin",							Mth_Sin,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"cos",							Mth_Cos,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"tan",							Mth_Tan,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"asin",							Mth_ASin,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"acos",							Mth_ACos,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"atan",							Mth_ATan,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"sqr",							Mth_Sqr,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"sqrt",							Mth_Sqrt,						EMVT_REAL,		EMVT_REAL,													EMVT_NULL );
	_functionGroup( "String" );
	_creaFunc(	"substr",						Str_Substr,						EMVT_STRING,	EMVT_STRING,	EMVT_INT,		EMVT_INT,					EMVT_NULL );
	_creaFunc(	"strlen",						Str_Strlen,						EMVT_INT,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"strhas",						Str_Contains,					EMVT_BOOL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"strtrm",						Str_Trim,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"strspl",						Str_Split,						EMVT_NULL,		EMVT_STRING,	EMVT_STRING,	EMVT_ARRAYS1,				EMVT_NULL );
	_creaFunc(	"strSplitOne",					Str_SplitOne,					EMVT_STRING,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"strFind",						Str_Find,						EMVT_INT,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"strGetChar",					Str_GetChar,					EMVT_CHAR,		EMVT_STRING,	EMVT_INT,									EMVT_NULL );
	_creaFunc(	"strSetChar",					Str_SetChar,					EMVT_NULL,		EMVT_STRING,	EMVT_INT,		EMVT_CHAR,					EMVT_NULL );
	_creaFunc(	"str_replace",					Str_Replace,					EMVT_STRING,	EMVT_STRING,	EMVT_STRING,	EMVT_STRING,				EMVT_NULL );
	_classFunc(	"clear",						Str_Clear,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_classFunc(	"empty",						Str_Empty,						EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_classFunc(	"replace",						Str_Replace,					EMVT_STRING,	EMVT_STRING,	EMVT_STRING,	EMVT_STRING,				EMVT_NULL );
	_classFunc(	"erase",						Str_Erase,						EMVT_NULL,		EMVT_STRING,	EMVT_INT,		EMVT_INT,					EMVT_NULL );
	_classFunc(	"length",						Str_Strlen,						EMVT_INT,		EMVT_STRING,												EMVT_NULL );
	_classFunc(	"find",							Str_Find,						EMVT_INT,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_classFunc( "lowerCase",					Str_Lower,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_classFunc( "upperCase",					Str_Upper,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Char" );
	_creaFunc(	"isLower",						Chr_IsLower,					EMVT_BOOL,		EMVT_CHAR,													EMVT_NULL );
	_creaFunc(	"isUpper",						Chr_IsUpper,					EMVT_BOOL,		EMVT_CHAR,													EMVT_NULL );
	_creaFunc(	"isAlpha",						Chr_IsAlpha,					EMVT_BOOL,		EMVT_CHAR,													EMVT_NULL );
	_creaFunc(	"isNumeral",					Chr_IsNumeral,					EMVT_BOOL,		EMVT_CHAR,													EMVT_NULL );
	_functionGroup( "Network" );
	_creaFunc( "Network_CreateClient",			Nwk_CreateClient,				EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Network_ConnectClient",		Nwk_ConnectClient,				EMVT_NULL,		EMVT_STRING,	EMVT_STRING,	EMVT_INT,					EMVT_NULL );
	_creaFunc( "Network_CloseClient",			Nwk_CloseClient,				EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Network_ClientSendMessage",	Nwk_ClientSendMessage,			EMVT_NULL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_functionGroup( "Mirror" );
	_creaFunc(	"Mirror_SetAlpha",				Mir_SetAlpha,					EMVT_NULL,		EMVT_STRING,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_creaFunc(	"Mirror_Update",				Mir_Update,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Post" );
	_creaFunc(	"PostEffect_Enable",			PEf_Enable,						EMVT_NULL,		EMVT_STRING,	EMVT_BOOL,									EMVT_NULL );
	_functionGroup( "WebImage" );
	_creaFunc(	"WebImage_Create",				Wim_Create,						EMVT_NULL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"WebImage_Destroy",				Wim_Delete,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"WebImage_Reload",				Wim_Reload,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Pub" );
	_creaFunc(	"Pub_Reload",					Pub_Reload,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Scene" );
	_creaFunc(	"Scene_Load",					Sce_Load,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Scene_Unload",					Sce_Unload,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Scene_SetShadows",				Sce_Shadows,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Animation Group" );
	_creaFunc(	"AnimationGroup_Create",		AoG_Create,						EMVT_ANI_GRP,	EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"AnimationGroup_GetByName",		AoG_GetByName,					EMVT_ANI_GRP,	EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"AnimationGroup_Exists",		AoG_Exists,						EMVT_BOOL,		EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"AnimationGroup_Destroy",		AoG_Destroy,					EMVT_NULL,		EMVT_STRING, 												EMVT_NULL );
	_creaFunc(	"AnimationGroup_DestroyAll",	AoG_DestroyAll,					EMVT_NULL,		 															EMVT_NULL );
	_classFunc(	"GetObject",					AoG_GetObject,					EMVT_ANI_OBJ,	EMVT_ANI_GRP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"AddObject",					AoG_AddObject,					EMVT_BOOL,		EMVT_ANI_GRP,	EMVT_ENTITY,								EMVT_NULL );
	_classFunc(	"RemoveObject",					AoG_RemObject,					EMVT_BOOL,		EMVT_ANI_GRP,	EMVT_STRING,								EMVT_NULL );
	_classFunc( "ChainAllAnimations",			AoG_ChainAll,					EMVT_NULL,		EMVT_ANI_GRP,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"StartAnimation",				AoG_StartAnimation,				EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"PauseAnimation",				AoG_PauseAnimation,				EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"StopAnimation",				AoG_StopAnimation,				EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"SetTimeAnimation",				AoG_SetTimeAnimation,			EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetAnimationTimeScale",		AoG_SetAnimationTimeScale,		EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_classFunc(	"RestartAnimation",				AoG_RestartAnimation,			EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"SetAnimationWeight",			AoG_SetAnimationWeight,			EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetAnimationTimeOut",			AoG_SetAnimationTimeOut,		EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetAnimationTimeIn",			AoG_SetAnimationTimeIn,			EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_REAL,					EMVT_NULL );
	_classFunc(	"SetAnimationLooped",			AoG_SetAnimationLooped,			EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_BOOL,					EMVT_NULL );
	_classFunc(	"SetAnimationReversed",			AoG_SetAnimationReversed,		EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_BOOL,					EMVT_NULL );
	_classFunc(	"StopAllAnimations",			AoG_StopAllAnimations,			EMVT_NULL,		EMVT_ANI_GRP,												EMVT_NULL );
	_classFunc(	"PauseAllAnimations",			AoG_PauseAllAnimations,			EMVT_NULL,		EMVT_ANI_GRP,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"ChainAnimation",				AoG_ChainAnimation,				EMVT_NULL,		EMVT_ANI_GRP,	EMVT_STRING,	EMVT_STRING,				EMVT_NULL );
	_functionGroup( "Animated Object" );
	_classFunc(	"GetAnimation",					AOb_GetAnimation,				EMVT_ANIMATION,	EMVT_ANI_OBJ,	EMVT_STRING,								EMVT_NULL );
	_classFunc(	"GetNumPlayingAnimations",		AOb_GetNumPlayingAnimations,	EMVT_INT,		EMVT_ANI_OBJ,												EMVT_NULL );
	_classFunc(	"GetPlayingAnimationName",		AOb_GetPlayingAnimationName,	EMVT_STRING,	EMVT_ANI_OBJ,	EMVT_INT,									EMVT_NULL );
	_functionGroup( "Animation" );
	_classFunc(	"Start",						Ani_Start,						EMVT_NULL,		EMVT_ANIMATION, 											EMVT_NULL );
	_classFunc(	"Stop",							Ani_Stop,						EMVT_NULL,		EMVT_ANIMATION, 											EMVT_NULL );
	_classFunc(	"Pause",						Ani_Pause,						EMVT_NULL,		EMVT_ANIMATION, 											EMVT_NULL );
	_classFunc(	"Restart",						Ani_Restart,					EMVT_NULL,		EMVT_ANIMATION, 											EMVT_NULL );
	_classFunc(	"SetTime",						Ani_SetTime,					EMVT_NULL,		EMVT_ANIMATION,	EMVT_REAL,									EMVT_NULL );
	_classFunc(	"GetLength",					Ani_GetLength,					EMVT_REAL,		EMVT_ANIMATION,												EMVT_NULL );
	_classFunc( "SetNextAnimation",				Ani_SetNext,					EMVT_NULL,		EMVT_ANIMATION,	EMVT_ANIMATION,								EMVT_NULL );
	_classFunc( "SetLooped",					Ani_SetLooped,					EMVT_NULL,		EMVT_ANIMATION,	EMVT_BOOL,									EMVT_NULL );
	_classFunc( "SetReversed",					Ani_SetReversed,				EMVT_NULL,		EMVT_ANIMATION,	EMVT_BOOL,									EMVT_NULL );
	_functionGroup( "Physics Simulation" );
	_creaFunc(	"Simulation_ClearObjects",		PhS_ClearObjects,				EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Simulation_Destroy",			PhS_Destroy,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Simulation_SetCurrent",		PhS_SetCurrentSimulation,		EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"Simulation_SetGravity",		PhS_SetGravity,					EMVT_NULL,		EMVT_VECTOR3,												EMVT_NULL );
	_creaFunc(	"Simulation_GetGravity",		PhS_GetGravity,					EMVT_VECTOR3,	EMVT_NULL, 													EMVT_NULL );
	_creaFunc(	"Simulation_SetTimescale",		PhS_SetTimescale,				EMVT_NULL,		EMVT_REAL, 													EMVT_NULL );
	_creaFunc(	"Simulation_GetTimescale",		PhS_GetTimescale,				EMVT_REAL,		EMVT_NULL, 													EMVT_NULL );
	_creaFunc(	"Simulation_SetSteptime",		PhS_SetSteptime,				EMVT_NULL,		EMVT_REAL, 													EMVT_NULL );
	_creaFunc(	"Simulation_GetSteptime",		PhS_GetSteptime,				EMVT_REAL,		EMVT_NULL, 													EMVT_NULL );
	_creaFunc(	"Simulation_SetSteptimeLimit",	PhS_GetSteptimeLimit,			EMVT_NULL,		EMVT_REAL, 													EMVT_NULL );
	_creaFunc(	"Simulation_SetErp",			PhS_SetErp,						EMVT_NULL,		EMVT_REAL, 													EMVT_NULL );
	_creaFunc(	"Simulation_SetCfm",			PhS_SetCfm,						EMVT_NULL,		EMVT_REAL, 													EMVT_NULL );
	_creaFunc(	"Simulation_SetAutoUpdate",		PhS_SetAutoUpdate,				EMVT_NULL,		EMVT_BOOL, 													EMVT_NULL );
	_creaFunc(	"Simulation_Update",			PhS_Update,						EMVT_NULL,		EMVT_NULL, 													EMVT_NULL );
	_creaFunc(	"Simulation_Raytrace",			PhS_Raytrace,					EMVT_PHY_OBJ,	EMVT_VECTOR3,	EMVT_VECTOR3,								EMVT_NULL );
	_creaFunc(	"Simulation_SetUpdateScript",	PhS_SetUpdateScript,			EMVT_NULL,		EMVT_CODE,													EMVT_NULL );
	_functionGroup( "Physics Object" );
	_creaFunc(	"PhysicsObject_GetByName",		POb_GetByName,					EMVT_PHY_OBJ,	EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"PhysicsObject_Create",			POb_Create,						EMVT_PHY_OBJ,	EMVT_ENTITY,	EMVT_BOOL,		EMVT_BOOL,					EMVT_NULL );
	_creaFunc(	"PhysicsObject_Destroy",		POb_Destroy,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_classFunc(	"GetName",						POb_GetName,					EMVT_STRING,	EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"GetSpeed",						POb_GetSpeed,					EMVT_VECTOR3,	EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"SetSpeed",						POb_SetSpeed,					EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"GetAngularSpeed",				POb_GetAngularSpeed,			EMVT_VECTOR3,	EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"SetAngularSpeed",				POb_SetAngularSpeed,			EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"AddForce",						POb_AddForce,					EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"AddTorque",					POb_AddTorque,					EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetForce",						POb_SetForce,					EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetTorque",					POb_SetTorque,					EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"GetIgnoreGravity",				POb_GetIgnoreGravity,			EMVT_BOOL,		EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"SetIgnoreGravity",				POb_SetIgnoreGravity,			EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"SetEnabled",					POb_SetEnabled,					EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"CheckAllCollisions",			POb_CheckAllCollisions,			EMVT_INT,		EMVT_PHY_OBJ,	EMVT_BOOL,		EMVT_BOOL,		EMVT_BOOL,	EMVT_NULL );
	_classFunc(	"IsEnabled",					POb_IsEnabled,					EMVT_BOOL,		EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"CollisionCheck",				POb_CollisionCheck,				EMVT_BOOL,		EMVT_PHY_OBJ,	EMVT_PHY_OBJ,	EMVT_BOOL,					EMVT_NULL );
	_classFunc(	"ReassertPosition",				POb_ReassertPosition,			EMVT_NULL,		EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"GetNumCollisions",				POb_GetNumCollisions,			EMVT_INT,		EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"GetNumCollisionsWith",			POb_GetNumCollisionsWith,		EMVT_INT,		EMVT_PHY_OBJ,	EMVT_PHY_OBJ,	EMVT_BOOL,					EMVT_NULL );
	_classFunc(	"Reset",						POb_Reset,						EMVT_NULL,		EMVT_PHY_OBJ,												EMVT_NULL );
	_classFunc(	"AddBoundingBox",				POb_AddBoundingBox,				EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_VECTOR3,	EMVT_VECTOR3,				EMVT_NULL );
	_classFunc(	"AddBoudingSphere",				POb_AddBoundingSphere,			EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_REAL,		EMVT_VECTOR3,				EMVT_NULL );
	_classFunc(	"AddBoundingMesh",				POb_AddBoundingMesh,			EMVT_NULL,		EMVT_PHY_OBJ,	EMVT_STRING,	EMVT_VECTOR3,				EMVT_NULL );
	_functionGroup( "Universe" );
	_creaFunc(	"Universe_GetSceneNodeByName",	Uni_GetSceneNodeByName,			EMVT_NODE,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Universe_GetObjectByName",		Uni_GetEntityByName,			EMVT_ENTITY,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Universe_GetLightByName",		Uni_GetLightByName,				EMVT_LIGHT,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Universe_GetCameraByName",		Uni_GetCameraByName,			EMVT_CAMERA,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc(	"Universe_GetUniverseList",		Uni_GetUniverseList,			EMVT_ARRAY,																	EMVT_NULL );
	_creaFunc(	"Universe_GetSceneList",		Uni_GetSceneList,				EMVT_ARRAY,		EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Binary Pack/Unpack" );
	_creaFunc(	"pack_int",						Pck_PackInt,					EMVT_STRING,	EMVT_INT,													EMVT_NULL );
	_creaFunc(	"pack_real",					Pck_PackReal,					EMVT_STRING,	EMVT_REAL,													EMVT_NULL );
	_creaFunc(	"pack_vector3",					Pck_PackVector3,				EMVT_STRING,	EMVT_VECTOR3,												EMVT_NULL );
	_creaFunc(	"pack_quaternion",				Pck_PackQuaternion,				EMVT_STRING,	EMVT_QUATERNION,											EMVT_NULL );
	_creaFunc(	"unpack_int",					Pck_UnpackInt,					EMVT_INT,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"unpack_real",					Pck_UnpackReal,					EMVT_REAL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"unpack_vector3",				Pck_UnpackVector3,				EMVT_VECTOR3,	EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"unpack_quaternion",			Pck_UnpackQuaternion,			EMVT_QUATERNION, EMVT_STRING,												EMVT_NULL );
	_functionGroup( "Material" );
	_creaFunc( "Material_GetByName",			Mat_GetByName,					EMVT_OBJ_MAT,	EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Material_Exists",				Mat_Exists,						EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Material_Create",				Mat_Create,						EMVT_OBJ_MAT,	EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Material_Destroy",				Mat_Destroy,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Material_CreateCopy",			Mat_CreateCopy,					EMVT_OBJ_MAT,	EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_classFunc( "ScrollUV",					Mat_ScrollUV,					EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc( "SetScrollUV",					Mat_SetScrollUV,				EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_classFunc( "RotateUV",					Mat_RotateUV,					EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_classFunc( "SetRotateUV",					Mat_SetRotateUV,				EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_classFunc( "ScaleUV",						Mat_ScaleUV,					EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_classFunc( "SetScaleUV",					Mat_SetScaleUV,					EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_REAL,					EMVT_NULL );
	_classFunc( "CreateTextureUnit",			Mat_CreateTextureUnit,			EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_STRING,								EMVT_NULL );
	_classFunc( "ChangeImage",					Mat_ChangeImage,				EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_INT,		EMVT_STRING,				EMVT_NULL );
	_classFunc( "GetImageName",				Mat_GetImageName,				EMVT_STRING,	EMVT_OBJ_MAT,	EMVT_INT,									EMVT_NULL );
	_classFunc( "CreateCopy",					Mat_CreateMatCopy,				EMVT_NULL,		EMVT_OBJ_MAT,	EMVT_STRING,								EMVT_NULL );
	_functionGroup(	"ConfigFile" );
	_creaFunc(	"OpenForRead",					Con_OpenForRead,				EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"CloseForRead",					Con_CloseForRead,				EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"DestroyConfigFile",			Con_Destroy,					EMVT_NULL,																	EMVT_NULL );
	_creaFunc(	"WriteLine",					Con_WriteLine,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"WriteBuffer",					Con_WriteBuffer,				EMVT_NULL,		EMVT_STRING,	EMVT_INT,									EMVT_NULL );
	_creaFunc(	"ReadLine",						Con_ReadLine,					EMVT_STRING,																EMVT_NULL );
	_creaFunc(	"ReadBuffer",					Con_ReadBuffer,					EMVT_STRING,	EMVT_INT,													EMVT_NULL );
	_functionGroup( "Sequence" );
	_creaFunc( "Sequence_Start",				Seq_Start,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Sequence_Stop",					Seq_Stop,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Sequence_Pause",				Seq_Pause,						EMVT_NULL,		EMVT_STRING,	EMVT_BOOL,									EMVT_NULL );
	_functionGroup( "Billboard" );
	_creaFunc( "Billboard_Create",				Bil_Create,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Billboard_Exists",				Bil_Exists,						EMVT_BOOL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Billboard_SetMaterial",			Bil_SetMaterial,				EMVT_NULL,		EMVT_STRING,	EMVT_STRING,								EMVT_NULL );
	_creaFunc( "Billboard_SetParent",			Bil_SetParent,					EMVT_NULL,		EMVT_STRING,	EMVT_NODE,									EMVT_NULL );
	_creaFunc( "Billboard_SetDefaultSize",		Bil_SetDefaultSize,				EMVT_NULL,		EMVT_STRING,	EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	_creaFunc( "Billboard_Clear",				Bil_Clear,						EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc( "Billboard_AddElementAt",		Bil_AddElement,					EMVT_NULL,		EMVT_STRING,	EMVT_VECTOR3,								EMVT_NULL );
	_creaFunc( "Billboard_AddSizedElementAt",	Bil_AddSizedElementAt,			EMVT_NULL,		EMVT_STRING,	EMVT_VECTOR3,	EMVT_REAL,		EMVT_REAL,	EMVT_NULL );
	_creaFunc( "Billboard_RemoveIndex",			Bil_RemoveIndex,				EMVT_NULL,		EMVT_STRING,	EMVT_INT,									EMVT_NULL );
	_creaFunc( "Billboard_Destroy",				Bil_Destroy,					EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
}

Function * EMuse::Script::ScriptCompiler :: _getClassFunction( VariableType * p_class, const String & p_functionName )const
{
	VariableBaseType l_base = p_class->GetBase();
	const ClassFunctionMap::const_iterator & l_classMapIter = m_classFunctions.find( l_base );

	if ( l_classMapIter != m_classFunctions.end() )
	{
		return General::Utils::map::findOrNull( l_classMapIter->second, p_functionName );
	}

	return NULL;
}

unsigned int EMuse::Script::ScriptCompiler :: _getTypeList( ScriptBlockArray & p_childs ) const
{
	unsigned int l_typeList = 0;
	unsigned int imax = static_cast <unsigned int>( p_childs.size() );

	for ( unsigned int i = 0 ; i < imax ; i ++ )
	{
		l_typeList |= p_childs[i]->m_type;
	}

	return l_typeList;
}

ScriptNode * EMuse::Script::ScriptCompiler :: CreateScriptNode()
{
	ScriptNode * l_node = m_nodePool.Get();
	l_node->_reinit();
	l_node->m_file = _getCurrentConfigFile();
	l_node->m_createdAtLine = _getCurrentLine();
	return l_node;
}

ScriptNode * EMuse::Script::ScriptCompiler :: CreateScriptNode( unsigned int p_lineNum )
{
	ScriptNode * l_node = m_nodePool.Get();
	l_node->_reinit();
	l_node->m_file = _getCurrentConfigFile();
	l_node->m_createdAtLine = p_lineNum;
	return l_node;
}

VariableType * EMuse::Script::ScriptCompiler :: FindType( const String & p_name )const
{
	VariableType * l_type = General::Utils::map::findOrNull( m_typedefs, p_name );

	if ( l_type != NULL )
	{
		return l_type;
	}

	return NULL;
}

ScriptNode * EMuse::Script::ScriptCompiler :: GetFlyweight( char p_value )
{
	ScriptNode * l_node = General::Utils::map::findOrNull( m_charFlyweight, p_value ); ;

	if ( l_node != NULL )
	{
		return l_node;
	}

	l_node = CreateScriptNode();
	l_node->Use();
	l_node->SetType( VariableTypeManager::Get( EMVT_CHAR ) );
	l_node->set<char>( p_value );
	m_charFlyweight.insert( std::map< char, ScriptNode *>::value_type( p_value, l_node ) );
	return l_node;
}

ScriptNode * EMuse::Script::ScriptCompiler :: GetFlyweight( const String & p_value )
{
	ScriptNode * l_node = General::Utils::map::findOrNull( m_stringFlyweight, p_value ); ;

	if ( l_node != NULL )
	{
		return l_node;
	}

	l_node = CreateScriptNode();
	l_node->Use();
	l_node->SetType( VariableTypeManager::Get( EMVT_STRING ) );
	l_node->set<String>( p_value );
	m_stringFlyweight.insert( std::map< String, ScriptNode *>::value_type( p_value, l_node ) );
	return l_node;
}

ScriptNode * EMuse::Script::ScriptCompiler :: GetFlyweight( int p_value )
{
	ScriptNode * l_node = General::Utils::map::findOrNull( m_intFlyweight, p_value ); ;

	if ( l_node != NULL )
	{
		return l_node;
	}

	l_node = CreateScriptNode();
	l_node->Use();
	l_node->SetType( VariableTypeManager::Get( EMVT_INT ) );
	l_node->set<int>( p_value );
	m_intFlyweight.insert( std::map< int, ScriptNode *>::value_type( p_value, l_node ) );
	return l_node;
}

ScriptNode * EMuse::Script::ScriptCompiler :: GetFlyweight( Real p_value )
{
	ScriptNode * l_node = General::Utils::map::findOrNull( m_realFlyweight, p_value ); ;

	if ( l_node != NULL )
	{
		return l_node;
	}

	l_node = CreateScriptNode();
	l_node->Use();
	l_node->SetType( VariableTypeManager::Get( EMVT_REAL ) );
	l_node->set<Real>( p_value );
	m_realFlyweight.insert( std::map< Real, ScriptNode *>::value_type( p_value, l_node ) );
	return l_node;
}

void EMuse::Script::ScriptCompiler :: _releaseNode( ScriptNode * p_node )
{
	p_node->_delete();
	m_nodePool.Release( p_node );
}

ScriptNode * EMuse::Script::ScriptCompiler :: _getUserVariable( const String & p_variableName )
{
	if ( m_currentUserFunction != NULL )
	{
		ScriptNode * l_node = General::Utils::map::findOrNull( m_currentUserFunction->m_localVars, p_variableName );

		if ( l_node != NULL )
		{
			return l_node;
		}

		if ( _isInStructDecla() )
		{
//			std::cout << "get user var ? : " << p_variableName << "//?-->" << std::endl;
			uint32_t l_index = m_currentStructure->FindMember( p_variableName );

			if ( l_index != String::npos )
			{
				ScriptNode * l_finalNode = CreateScriptNode();
				l_finalNode->SetRefType( m_currentStructure->GetType()->GetSubType( l_index ) );
				l_finalNode->SetFunction( Ope_StructGetMember );
				ScriptNode * l_node = General::Utils::map::findOrNull( m_currentUserFunction->m_localVars, String( "this" ) );
				l_finalNode->AddChild( l_node );
				l_finalNode->AddChild( GetFlyweight( static_cast <int>( l_index ) ) );
				return l_finalNode;
			}
		}
	}

	return General::Utils::map::findOrNull( m_userVariables, p_variableName );
}

UserFunction :: ~UserFunction()
{
	General::Utils::map::cycle( m_localVars, &ScriptNode::Delete );
	m_localVars.clear();

	if ( m_node != NULL )
	{
		m_node->Delete();
	}

	if ( m_returnNode != NULL )
	{
		m_returnNode->Delete();
	}
}

void EMuse::Script::ScriptCompiler :: ReleaseScriptNode( ScriptNode * p_node )
{
	sm_singleton->_releaseNode( p_node );
}


UserFunction * EMuse::Script::ScriptCompiler :: GetUserFunction( const String & p_functionName )const
{
	if ( _isInStructDecla() )
	{
		UserFunction * l_function = m_currentStructure->GetFunction( p_functionName );

		if ( l_function != NULL )
		{
			return l_function;
		}
	}

	return General::Utils::map::findOrNull( m_userFunctions, p_functionName );
}
