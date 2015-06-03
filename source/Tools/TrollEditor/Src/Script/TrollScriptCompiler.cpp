#include "PrecompiledHeader.h"

#include "TrollScriptCompiler.h"

#include "TrollScriptBlock.h"
#include "TrollScriptFunctions.h"
#include "GUI/MainFrame.h"
#include "Project/Scene.h"

#include <VariableType.h>

using namespace Troll::GUI;

TrollScriptCompiler::TrollScriptCompiler( const String & p_path, bool p_initTree )
	:	m_textCtrl( NULL ),
		m_currentCharIndex( 0 ),
		m_highlighting( true ),
		ScriptCompiler( p_path )
{
	m_trollBlockPool.Allocate( 10 );
	m_initTree = p_initTree;
}

TrollScriptCompiler::~TrollScriptCompiler()
{
}

void TrollScriptCompiler::Initialise( Troll::GUI::wxStcTextEditor * p_textCtrl )
{
	m_textCtrl = p_textCtrl;
	_initialiseVariableMap();
	_initialiseFunctionMap();
	_initialiseOperatorMap();
}

void TrollScriptCompiler::_log( const String & p_message )
{
	std::cout << p_message << std::endl;
	g_mainFrame->LogOutMessage( p_message );
}

ScriptNode * TrollScriptCompiler::CompileScript( const String & p_script )
{
	ScriptNode * l_return = ScriptCompiler::CompileScript( p_script );

	if ( m_textCtrl )
	{
		m_textCtrl->UpdatePrefs();
	}

	return l_return;
}

ScriptNode * TrollScriptCompiler::CompileScriptFile( ConfigFile * p_scriptFile )
{
	std::cout << "Compiling : " << p_scriptFile->GetName() << " from textctrl : " << ( m_textCtrl == NULL ? "NULL" : m_textCtrl->GetFilename().char_str() ) << std::endl;

	try
	{
		ScriptNode * l_return = ScriptCompiler::CompileScriptFile( p_scriptFile );

		if ( m_textCtrl )
		{
			m_textCtrl->UpdatePrefs();
		}

		return l_return;
	}
	catch ( ... )
	{
		return NULL;
	}
}

TrollScriptBlock * TrollScriptCompiler::_getBlock()
{
	TrollScriptBlock * l_block = m_trollBlockPool.Get();
	l_block->SetCompiler( this );
	return l_block;
}

void TrollScriptCompiler::_releaseBlock( ScriptBlock * p_block )
{
	p_block->Clear();
	m_trollBlockPool.Release( static_cast<TrollScriptBlock *>( p_block ) );
}

char TrollScriptCompiler::_getNextChar()
{
	m_currentCharIndex ++;

	if ( m_currentFileStream )
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
	}
	else
	{
		return ( ( *m_stringBuffer )[m_currentBufferIndex++] );
	}

	return '\0';
}

void TrollScriptCompiler::_putBack( char p_char )
{
	m_currentCharIndex --;

	if ( m_currentFileStream )
	{
		m_buffer[-- m_currentBufferIndex] = p_char;
	}
	else
	{
		m_currentBufferIndex--;
	}
}

void TrollScriptCompiler::_creaFunc( const String & p_functionName, RawFunction * p_function, VariableBaseType p_returnValue, ... )
{
//*******************
// Specific to Troll
	if ( m_initTree )
	{
		std::vector <VariableBaseType> l_params;
		va_list l_argsList;
		va_start( l_argsList, p_returnValue );
		VariableBaseType l_param = VariableBaseType( va_arg( l_argsList, int ) );

		while ( l_param != EMVT_NULL )
		{
			l_params.push_back( l_param );
			l_param = VariableBaseType( va_arg( l_argsList, int ) );
		}

		va_end( l_argsList );
		g_mainFrame->CreateFreeFunction( wxString( p_functionName.c_str(), wxConvLibc ), p_returnValue, l_params );
	}

	if ( m_textCtrl && m_textCtrl->GetContext() )
	{
		auto && l_it = m_textCtrl->GetContext()->Find( wxT( "emscript" ) );

		if ( l_it != m_textCtrl->GetContext()->End() )
		{
			( *l_it )->AddWord( eSTC_TYPE_WORD3, wxString( p_functionName.c_str(), wxConvLibc ) );
		}
	}

//*******************
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

void TrollScriptCompiler::_classFunc( const String & p_functionName, RawFunction * p_function, VariableBaseType p_returnValue, ... )
{
//*********************
// Specific to Troll
	if ( m_initTree )
	{
		std::vector <VariableBaseType> l_params;
		va_list l_argsList;
		va_start( l_argsList, p_returnValue );
		VariableBaseType l_param = VariableBaseType( va_arg( l_argsList, int ) );

		while ( l_param != EMVT_NULL )
		{
			l_params.push_back( l_param );
			l_param = VariableBaseType( va_arg( l_argsList, int ) );
		}

		va_end( l_argsList );
		g_mainFrame->CreateClassFunction( wxString( p_functionName.c_str(), wxConvLibc ), p_returnValue, l_params );
	}
	
	if ( m_textCtrl && m_textCtrl->GetContext() )
	{
		auto && l_it = m_textCtrl->GetContext()->Find( wxT( "emscript" ) );

		if ( l_it != m_textCtrl->GetContext()->End() )
		{
			( *l_it )->AddWord( eSTC_TYPE_WORD3, wxString( p_functionName.c_str(), wxConvLibc ) );
		}
	}

//*********************
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

ScriptNode * TrollScriptCompiler::_createConstant( VariableBaseType p_type, const String & p_name )
{
	genlib_assert( ! General::Utils::map::has( m_constants, p_name ) );

//*********************
// Specific to Troll

	if ( m_initTree )
	{
		g_mainFrame->CreateConstant( wxString( p_name.c_str(), wxConvLibc ), p_type );
	}

	if ( m_textCtrl && m_textCtrl->GetContext() )
	{
		auto && l_it = m_textCtrl->GetContext()->Find( wxT( "emscript" ) );

		if ( l_it != m_textCtrl->GetContext()->End() )
		{
			( *l_it )->AddWord( eSTC_TYPE_WORD2, wxString( p_name.c_str(), wxConvLibc ) );
		}
	}

//*********************
	return ScriptCompiler::_createConstant( p_type, p_name );
}

void TrollScriptCompiler::_functionGroup( const String & p_name )
{
	if ( m_initTree )
	{
		g_mainFrame->CreateFunctionsGroup( wxString( p_name.c_str(), wxConvLibc ) );
	}
}

void TrollScriptCompiler::_constantGroup( const String & p_name )
{
	if ( m_initTree )
	{
		g_mainFrame->CreateConstantsGroup( wxString( p_name.c_str(), wxConvLibc ) );
	}
}

void TrollScriptCompiler::_initialiseFunctionMap()
{
//	std::cout << "TrollScriptCompiler::_initialiseFunctionMap\n";
	ScriptCompiler::_initialiseFunctionMap();
	// Object
	_classFunc(	"SetMaterial"				, Troll_Ent_SetMaterial,				EMVT_NULL,		EMVT_ENTITY,	EMVT_STRING,								EMVT_NULL );
	// Overlays
	_classFunc(	"Resize"					, Troll_OvE_Resize,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"Translate"					, Troll_OvE_Translation,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"SetPosition"				, Troll_OvE_SetPosV3,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetLeft"					, Troll_OvE_SetPosX,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetTop"					, Troll_OvE_SetPosY,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetWidth"					, Troll_OvE_SetWidth,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetHeight"					, Troll_OvE_SetHeight,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetCaption"				, Troll_OvE_SetCaption,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetMaterial"				, Troll_OvE_SetMaterial,				EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetMouseOverMaterial"		, Troll_OvE_SetMouseOverMaterial,	EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetBorderMaterial"			, Troll_OvE_SetBorderMaterial,		EMVT_NULL,		EMVT_OVERLAY,	EMVT_STRING, 								EMVT_NULL );
	_classFunc(	"SetSize"					, Troll_OvE_SetSize,					EMVT_NULL,		EMVT_OVERLAY,	EMVT_REAL,		EMVT_REAL,					EMVT_NULL );
	// TrollScene Node
	_classFunc(	"Translate"					, Troll_ScN_Translate,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"TranslateLocal"			, Troll_ScN_TranslateLocal,			EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3, 								EMVT_NULL );
	_classFunc(	"RotateLocal"				, Troll_ScN_RotateLocal,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"Rotate"					, Troll_ScN_Rotate,					EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,	EMVT_REAL, 					EMVT_NULL );
	_classFunc(	"RotateAround"				, Troll_ScN_RotateAround,			EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,	EMVT_VECTOR3,	EMVT_REAL,	EMVT_NULL );
	_classFunc(	"RotateQuat"				, Troll_ScN_RotateQuat,				EMVT_NULL,		EMVT_NODE,		EMVT_QUATERNION,							EMVT_NULL );
	_classFunc(	"Scale"						, Troll_ScN_Scale,					EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetScale"					, Troll_ScN_SetScale,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,									EMVT_NULL );
	_classFunc(	"Yaw"						, Troll_ScN_Yaw,						EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"Pitch"						, Troll_ScN_Pitch,					EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetPosition"				, Troll_ScN_SetPosition,				EMVT_NULL,		EMVT_NODE,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetXPosition"				, Troll_ScN_SetXPosition,			EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetYPosition"				, Troll_ScN_SetYPosition,			EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"SetZPosition"				, Troll_ScN_SetZPosition,			EMVT_NULL,		EMVT_NODE,		EMVT_REAL, 									EMVT_NULL );
	_classFunc(	"Reset"						, Troll_ScN_Reset,					EMVT_NULL,		EMVT_NODE,													EMVT_NULL );
	_classFunc(	"SetOrientation"			, Troll_ScN_SetOrientation,			EMVT_NULL,		EMVT_NODE,		EMVT_QUATERNION,							EMVT_NULL );
	// Camera
	_classFunc(	"ModeWireFrame"				, Troll_Cam_ModeWireFrame,			EMVT_NULL,		EMVT_CAMERA,	EMVT_BOOL,									EMVT_NULL );
	_classFunc(	"SetPosition"				, Troll_Cam_SetPosition,				EMVT_NULL,		EMVT_CAMERA,	EMVT_VECTOR3,								EMVT_NULL );
	// Light
	_classFunc(	"SetDirection"				, Troll_Lgh_SetDirection,			EMVT_NULL,		EMVT_LIGHT,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetPosition"				, Troll_Lgh_SetPosition,				EMVT_NULL,		EMVT_LIGHT,		EMVT_VECTOR3,								EMVT_NULL );
	_classFunc(	"SetDiffuse"				, Troll_Lgh_SetDiffuse,				EMVT_NULL,		EMVT_LIGHT,		EMVT_COLOUR,								EMVT_NULL );
	_classFunc(	"SetSpecular"				, Troll_Lgh_SetSpecular,				EMVT_NULL,		EMVT_LIGHT,		EMVT_COLOUR,								EMVT_NULL );
	// Miscellaneous
	_creaFunc(	"SaveScript"				, Troll_Spc_SaveScript,				EMVT_NULL,		EMVT_STRING,												EMVT_NULL );
	_creaFunc(	"ImportMesh"				, Troll_Spc_ImportMesh,				EMVT_ENTITY,																EMVT_NULL );

	if ( m_initTree )
	{
		g_mainFrame->m_functionsList->CreateFunctionsTree();
		g_mainFrame->m_functionsList->CreateConstantsTree();
	}
}
