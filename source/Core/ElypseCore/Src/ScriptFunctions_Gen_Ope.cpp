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

#include "ScriptFunctions.h"
#include "ScriptNode.h"
#include "ScriptEngine.h"
#include "ScriptCompiler.h"
#include "VariableType.h"
#include "Structure.h"
#include "MapIterator.h"

#include "Context.h"
#include "ElypseInstance.h"
#include "ElypseFrameListener.h"

#include "MuseFile.h"

#include "EMGui.h"

//#include "DownloadManager.h"

#include "SceneFileParser.h"

#include "ElypseLogs.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreResourceGroupManager.h>

//#include <Bezier.h>

#include "ObjectMaterial.h"


EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Miaou )
{
	VERBOSE_STARTFUNC( "T'occupe, c'est special" );
	ObjectMaterial * l_mat = new ObjectMaterial( "Miaou" );
	l_mat->CreateCopyFrom( "Dames/Joueur1" );
//	l_mat->DrawTriangle( 0, ColourValue::Red, Vector2( 0.0, 0.0), Vector2( 0.8, 0.2), Vector2( 0.5, 0.8));
	l_mat->DrawCircle( 0, ColourValue::Red, Vector2( 0.2f, 0.8f ), 0.2f );
	l_mat->DrawCircle( 0, ColourValue::Red, Vector2( 0.2f, 0.2f ), 0.2f );
//	l_mat->DrawLine( 0, ColourValue::Red, Vector2( 0.2, 0.2), Vector2( 0.4, 0.8), 0.05);
//	l_mat->CopyTexture( 0, "joueur1perdu.png");
	/*
	GET_AND_EXEC_PARAM( String, p_textureName);
	*/
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_IfBlock )
{
	VERBOSE_STARTFUNC( "if" );
	/*
	std::cout << "list childs : " << std::endl;
	_times( caller->m_childs.size())
	{
		std::cout << "child : " << i << " types : " << caller->m_childs[i]->GetType()->GetDesc() << std::endl;
	}
	*/
	size_t numParams = caller->GetNumChilds();
	size_t i = 0;
	size_t max = 0;
	bool useelse;
	bool finished = false;
	max = numParams;

	if ( numParams % 2 == 1 )
	{
		useelse = true;
		max--;
	}
	else
	{
		useelse = false;
	}

	max = ( max / 2 );

	for ( i = 0 ; i < max && finished == false ; i ++ )
	{
		bool l_condition = caller->getAndExecChild<bool>( 2 * i );

		if ( l_condition )
		{
			finished = true;

			try
			{
				caller->ExecChild( 2 * i + 1 );
			}
			catch ( GenException & p_exception )
			{
				EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (General) --> " + p_exception.GetDescription() );
			}
		}

		if ( ScriptEngine::GetContext()->m_breakOne || ScriptEngine::GetContext()->m_continue || ScriptEngine::GetContext()->m_return )
		{
			return;
		}
	}

	if ( useelse && ! finished )
	{
		caller->ExecChild( numParams - 1 );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_WhileLoop )
{
	VERBOSE_STARTFUNC( "while loop" );
	ScriptNode * cond, * code;
	cond = caller->m_childs[0];
	code = caller->m_childs[1];

	if ( cond->HasFunction() )
	{
		cond->Execute();
	}

	bool l_condition = cond->get<bool>();
	unsigned int i = 0;

	while ( l_condition )
	{
		i++;

		if ( i > 1000000 )
		{
			break;
		}

		if ( code->HasFunction() )
		{
			try
			{
				code->Execute();
			}
			catch ( GenException & p_exception )
			{
				EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (General) --> " + p_exception.GetDescription() );
			}
		}

		if ( ScriptEngine::GetContext()->m_breakOne )
		{
			ScriptEngine::GetContext()->m_breakOne = false;
			return;
		}

		if ( ScriptEngine::GetContext()->m_return )
		{
			return;
		}

		if ( ScriptEngine::GetContext()->m_continue )
		{
			ScriptEngine::GetContext()->m_continue = false;
		}

		if ( cond->HasFunction() )
		{
			try
			{
				cond->Execute();
			}
			catch ( GenException & p_exception )
			{
				EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (General) --> " + p_exception.GetDescription() );
			}

			l_condition = cond->get<bool>();
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_ForLoop )
{
	VERBOSE_STARTFUNC( "for loop" );
	ScriptNode * init, * cond, * code, * end;
	init = caller->m_childs[0];
	cond = caller->m_childs[1];
	end = caller->m_childs[2];
	code = caller->m_childs[3];

	if ( init->HasFunction() )
	{
		init->m_function( init );
	}

	if ( cond->HasFunction() )
	{
		cond->m_function( cond );
	}

	try
	{
		unsigned int i = 0 ;

		while ( cond->get<bool>() )
		{
			i++;

			if ( i > 1000000 )
			{
				break;
			}

			if ( code->HasFunction() )
			{
				code->Execute();
			}

			if ( end->HasFunction() )
			{
				end->Execute();
			}

			if ( cond->HasFunction() )
			{
				cond->Execute();
			}

			if ( ScriptEngine::GetContext()->m_breakOne )
			{
				ScriptEngine::GetContext()->m_breakOne = false;
				return;
			}

			if ( ScriptEngine::GetContext()->m_continue )
			{
				ScriptEngine::GetContext()->m_continue = false;
			}

			if ( ScriptEngine::GetContext()->m_return )
			{
				return;
			}
		}
	}
	catch ( GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (General) --> " + p_exception.GetDescription() );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_ExecuteFunction )
{
	VERBOSE_STARTFUNC( "personal function" );
	ScriptNode * l_node;
	size_t numParams, j;
	numParams = caller->GetNumChilds() - 3;
	numParams = numParams / 2;
	l_node = caller->m_childs[1];

	if ( numParams != static_cast <size_t>( l_node->get<int>() ) )
	{
		SCRIPT_ERROR( "Error @ ExecuteFunction, no correct numParams ! Real value : " + StringConverter::toString( numParams ) + " , expected : " + StringConverter::toString( l_node->get<int>() ) );
	}
	else
	{
		ScriptNode * l_currentFunction = ScriptEngine::GetContext()->m_currentFunction;
		ScriptEngine::GetContext()->m_currentFunction = caller;

		for ( j = 0 ; j < numParams ; j ++ )
		{
			ScriptNode * l_tempNode = caller->m_childs[3 + j + numParams];

			if ( l_tempNode->HasFunction() )
			{
				l_tempNode->Execute();
			}

			l_node = caller->m_childs[2 + j];

			if ( l_node->IsRef() )
			{
				l_node->CopyValue_Ref( l_tempNode->m_value );
			}
			else
			{
				l_node->CopyValue_Deep( l_tempNode );
			}
		}

		l_node = caller->m_childs[2 + numParams];

		if ( l_node->HasFunction() )
		{
			l_node->Execute();
		}

		caller->CopyValue_Ref( caller->m_childs[0]->m_value );
		ScriptEngine::GetContext()->m_currentFunction = l_currentFunction;
	}

	ScriptEngine::GetContext()->m_return = false;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_MultiLineFunction )
{
	size_t i, max;
	ScriptNode * l_sN;
	max = caller->GetNumChilds();

	for ( i = 0 ; i < max ; i ++ )
	{
		l_sN = caller->m_childs[i];

		try
		{
			if ( l_sN->HasFunction() )
			{
				l_sN->Execute();
			}
		}
		catch ( GenException & p_exception )
		{
			EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (General) --> " + p_exception.GetDescription() );
		}

		if ( ScriptEngine::GetContext()->m_breakOne || ScriptEngine::GetContext()->m_return || ScriptEngine::GetContext()->m_continue )
		{
			return;
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Echo )
{
	VERBOSE_STARTFUNC( "Echo" );
	GET_AND_EXEC_PARAM( String, l_string, 0 );
	EMUSE_MESSAGE_NORMAL( l_string );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_ClearScene )
{
	VERBOSE_STARTFUNC( "ClearScene" );
	ScriptEngine::GetContext()->sceneManager->clearScene();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Break )
{
	VERBOSE_STARTFUNC( "Gen_Break" );
	ScriptEngine::GetContext()->m_breakOne = true;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Continue )
{
	VERBOSE_STARTFUNC( "Gen_Continue" );
	ScriptEngine::GetContext()->m_continue = true;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Coucou )
{
	VERBOSE_STARTFUNC( "Gen_Coucou" );
	EMUSE_MESSAGE_NORMAL( "Coucou @ " + caller->m_file->GetName() + " line " + StringConverter::toString( caller->m_createdAtLine ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Describe )
{
	VERBOSE_STARTFUNC( "Gen_Describe" );
	RETURN_AS( String ) caller->m_childs[0]->GetType()->GetDesc();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_Return )
{
	VERBOSE_STARTFUNC( "Gen_Return" );

	if ( ScriptEngine::GetContext()->m_currentFunction != NULL )
	{
//		std::cout << "gen return ! " << std::endl;
		ScriptEngine::GetContext()->m_return = true;

		if ( caller->m_childs[0]->HasFunction() )
		{
			caller->m_childs[0]->Execute();
		}

		ScriptNode * l_function = ScriptEngine::GetContext()->m_currentFunction;

		if ( l_function->m_childs[0]->IsRef() )
		{
//			std::cout << "func, copy ref ? " << std::endl;
			l_function->m_childs[0]->CopyValue_Ref( caller->m_childs[0]->m_value );
		}
		else
		{
//			std::cout << "func, copy Deep ? " << std::endl;
			l_function->m_childs[0]->CopyValue_Deep( caller->m_childs[0] );
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_ReturnNull )
{
	VERBOSE_STARTFUNC( "Gen_ReturnNull" );
	ScriptEngine::GetContext()->m_return = true;
}

namespace Elypse
{
	namespace Script
	{
		void RemoveObjects( SceneNode * p_node )
		{
			unsigned short max, i;
			max = p_node->numChildren();

			for ( i = 0 ; i < max ; i ++ )
			{
				RemoveObjects( static_cast <SceneNode *>( p_node->getChild( i ) ) );
			}

			max = p_node->numAttachedObjects();

			for ( i = 0 ; i < max ; i ++ )
			{
				ScriptEngine::GetContext()->sceneManager->destroyMovableObject( p_node->detachObject( static_cast <unsigned short>( 0 ) ) );
			}
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_ClearSceneNode )
{
	VERBOSE_STARTFUNC( "ClearSceneNode" );
	GET_AND_EXEC_PARAM( SceneNode *, l_sceneNode, 0 );

	if ( l_sceneNode != NULL )
	{
		RemoveObjects( l_sceneNode );
		l_sceneNode->removeAndDestroyAllChildren();
		ScriptEngine::GetContext()->sceneManager->destroySceneNode( l_sceneNode->getName() );
	}

	l_sceneNode = NULL;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_ClearOverlays )
{
	VERBOSE_STARTFUNC( "ClearOverlays" );
	ScriptEngine::GetContext()->gui->ClearAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_BindKey )
{
	VERBOSE_STARTFUNC( "BindKey" );
	GET_AND_EXEC_TWO_PARAM( int, p_keyToBind, int, p_bindType );
	ScriptNode * l_code = caller->m_childs[2];

	if ( p_bindType >= 0 && p_bindType <= 3  && p_keyToBind >= 0 && p_keyToBind < NUM_KEYS && l_code != NULL )
	{
		if ( ScriptEngine::GetContext()->scriptEngine->m_compiler->m_keyboardBinds[p_bindType][p_keyToBind] != NULL )
		{
			ScriptEngine::GetContext()->scriptEngine->m_compiler->m_keyboardBinds[p_bindType][p_keyToBind]->Delete();
		}

		ScriptEngine::GetContext()->scriptEngine->m_compiler->m_keyboardBinds[p_bindType][p_keyToBind] = l_code;
		ScriptEngine::GetContext()->scriptEngine->m_compiler->m_keyboardBinds[p_bindType][p_keyToBind]->Use();
	}
	else
	{
		SCRIPT_ERROR( "Error @ GeneralCode_BindKey : Index out of bounds" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_StructGetMember )
{
	VERBOSE_STARTFUNC( "Ope_StructGetMember" );
	GET_AND_EXEC_TWO_PARAM( StructInstance *, l_struct, int, l_index );
	caller->CopyValue_Ref( l_struct->GetMember( l_index ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Add_Equal_Char_To_String )
{
	VERBOSE_STARTFUNC( "(String)+=(char)" );
	GET_AND_EXEC_TWO_PARAM( String, a, char, b );
	a.push_back( b );
	RETURN_AS( String ) a;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Multiply_Vector3_By_Quat )
{
	VERBOSE_STARTFUNC( "(Vector3)*(Quaternion)" );
	GET_AND_EXEC_TWO_PARAM( Vector3, a, Quaternion, b );
	RETURN_AS( Vector3 ) b * a;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Increment_Int )
{
	VERBOSE_STARTFUNC( "++(int)" );
	GET_AND_EXEC_PARAM( int, l_value, 0 );
	RETURN_AS( int ) ++l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Increment_Char )
{
	VERBOSE_STARTFUNC( "++(char)" );
	GET_AND_EXEC_PARAM( char, l_value, 0 );
	RETURN_AS( char ) ++l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Decrement_Int )
{
	VERBOSE_STARTFUNC( "--(int)" );
	GET_AND_EXEC_PARAM( int, l_value, 0 );
	RETURN_AS( int ) --l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Decrement_Char )
{
	VERBOSE_STARTFUNC( "--(char)" );
	GET_AND_EXEC_PARAM( char, l_value, 0 );
	RETURN_AS( char ) --l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Negate )
{
	VERBOSE_STARTFUNC( "!(unary)bool" );
	GET_AND_EXEC_PARAM( bool, l_value, 0 );
	RETURN_AS( bool ) ! l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_NegateReal )
{
	VERBOSE_STARTFUNC( "-(unary)real" );
	GET_AND_EXEC_PARAM( Real, l_value, 0 );
	RETURN_AS( Real ) - l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_NegateInt )
{
	VERBOSE_STARTFUNC( "-(unary)int" );
	GET_AND_EXEC_PARAM( int, l_value, 0 );
	RETURN_AS( int ) - l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_NegateVe3 )
{
	VERBOSE_STARTFUNC( "-(unary)Vector3" );
	GET_AND_EXEC_PARAM( Vector3, l_value, 0 );
	RETURN_AS( Vector3 ) - l_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_Or )
{
	VERBOSE_STARTFUNC( "or" );
	GET_AND_EXEC_PARAM( bool, a, 0 );

	if ( a )
	{
		RETURN_AS( bool ) true;
	}
	else
	{
		GET_AND_EXEC_PARAM( bool, b, 1 );
		RETURN_AS( bool ) b;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_ModuloInt )
{
	VERBOSE_STARTFUNC( "Modulo int" );
	GET_AND_EXEC_TWO_PARAM( int, a, int, b );
	RETURN_AS( int ) a % b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_And )
{
	VERBOSE_STARTFUNC( "and" );
	GET_AND_EXEC_PARAM( bool, a, 0 );

	if ( ! a )
	{
		RETURN_AS( bool ) false;
	}
	else
	{
		GET_AND_EXEC_PARAM( bool, b, 1 );
		RETURN_AS( bool ) b;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperior_Int )
{
	VERBOSE_STARTFUNC( ">(int)" );
	GET_AND_EXEC_TWO_PARAM( int, a, int, b );
	RETURN_AS( bool ) a > b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperior_Real )
{
	VERBOSE_STARTFUNC( ">(real)" );
	GET_AND_EXEC_TWO_PARAM( Real, a, Real, b );
	RETURN_AS( bool ) a > b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperior_Char )
{
	VERBOSE_STARTFUNC( ">(char)" );
	GET_AND_EXEC_TWO_PARAM( char, a, char, b );
	RETURN_AS( bool ) a > b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperior_String )
{
	VERBOSE_STARTFUNC( ">(string)" );
	GET_AND_EXEC_TWO_PARAM( String, a, String, b );
	RETURN_AS( bool ) a > b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperiorOrEqual_Int )
{
	VERBOSE_STARTFUNC( ">=(int)" );
	GET_AND_EXEC_TWO_PARAM( int, a, int, b );
	RETURN_AS( bool ) a >= b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperiorOrEqual_Real )
{
	VERBOSE_STARTFUNC( ">=(real)" );
	GET_AND_EXEC_TWO_PARAM( Real, a, Real, b );
	RETURN_AS( bool ) a >= b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsSuperiorOrEqual_Char )
{
	VERBOSE_STARTFUNC( ">=(char)" );
	GET_AND_EXEC_TWO_PARAM( char, a, char, b );
	RETURN_AS( bool ) a >= b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferior_Int )
{
	VERBOSE_STARTFUNC( "<(int)" );
	GET_AND_EXEC_TWO_PARAM( int, a, int, b );
	RETURN_AS( bool ) a < b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferior_Real )
{
	VERBOSE_STARTFUNC( "<(real)" );
	GET_AND_EXEC_TWO_PARAM( Real, a, Real, b );
	RETURN_AS( bool ) a < b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferior_Char )
{
	VERBOSE_STARTFUNC( "<(char)" );
	GET_AND_EXEC_TWO_PARAM( char, a, char, b );
	RETURN_AS( bool ) a < b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferior_String )
{
	VERBOSE_STARTFUNC( "<(string)" );
	GET_AND_EXEC_TWO_PARAM( String, a, String, b );
	RETURN_AS( bool ) a < b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferiorOrEqual_Int )
{
	VERBOSE_STARTFUNC( "<=(int)" );
	GET_AND_EXEC_TWO_PARAM( int, a, int, b );
	RETURN_AS( bool ) a <= b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferiorOrEqual_Real )
{
	VERBOSE_STARTFUNC( "<=(real)" );
	GET_AND_EXEC_TWO_PARAM( Real, a, Real, b );
	RETURN_AS( bool ) a <= b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ope_IsInferiorOrEqual_Char )
{
	VERBOSE_STARTFUNC( "<=(char)" );
	GET_AND_EXEC_TWO_PARAM( char, a, char, b );
	RETURN_AS( bool ) a <= b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseLDownScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_L_DOWN, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseMDownScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_M_DOWN, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseRDownScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_R_DOWN, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseLUpScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_L_UP, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseMUpScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_M_UP, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseRUpScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_R_UP, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseWheelUpScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_WHEEL_UP, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetMouseWheelDownScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( MOUSE_WHEEL_DOWN, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetKeyDownScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( KEY_CHAR_DOWN, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetKeyRepeatScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( KEY_CHAR_REPEAT, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Gen_SetNetworkScript )
{
	ScriptEngine::GetContext()->emuseInstance->GetFL()->SetCallback( NETWORK_MESSAGE, caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Real_To_Int )
{
	VERBOSE_STARTFUNC( "rtoi" );
	GET_AND_EXEC_PARAM( Real, val, 0 );
	RETURN_AS( int ) Math::IFloor( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Real_To_String )
{
	VERBOSE_STARTFUNC( "rtoa" );
	GET_AND_EXEC_PARAM( Real, val, 0 );
	RETURN_AS( String ) StringConverter::toString( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Int_To_Real )
{
	VERBOSE_STARTFUNC( "itor" );
	GET_AND_EXEC_PARAM( int, val, 0 );
	RETURN_AS( Real ) Real( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Int_To_Bool )
{
	VERBOSE_STARTFUNC( "itob" );
	GET_AND_EXEC_PARAM( int, val, 0 );
	RETURN_AS( bool ) val != 0;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Int_To_String )
{
	VERBOSE_STARTFUNC( "itoa" );
	GET_AND_EXEC_PARAM( int, val, 0 );
	RETURN_AS( String ) ToString( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Int_To_Char )
{
	VERBOSE_STARTFUNC( "itoc" );
	GET_AND_EXEC_PARAM( int, val, 0 );
	RETURN_AS( char ) val;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_String_To_Int )
{
	VERBOSE_STARTFUNC( "atoi" );
	GET_AND_EXEC_PARAM( String, val, 0 );
	RETURN_AS( int ) StringConverter::parseInt( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_String_To_Real )
{
	VERBOSE_STARTFUNC( "ator" );
	GET_AND_EXEC_PARAM( String, val, 0 );
	RETURN_AS( Real ) StringConverter::parseReal( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_String_To_Bool )
{
	VERBOSE_STARTFUNC( "atob" );
	GET_AND_EXEC_PARAM( String, val, 0 );
	RETURN_AS( bool ) StringConverter::parseBool( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_String_To_Char )
{
	VERBOSE_STARTFUNC( "atoc" );
	GET_AND_EXEC_PARAM( String, val, 0 );

	if ( ! val.empty() )
	{
		RETURN_AS( char ) val[0];
	}
	else
	{
		RETURN_AS( char ) '\0';
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Bool_To_Int )
{
	VERBOSE_STARTFUNC( "btoi" );
	GET_AND_EXEC_PARAM( bool, val, 0 );
	RETURN_AS( int ) static_cast< int >( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Bool_To_String )
{
	VERBOSE_STARTFUNC( "btoa" );
	GET_AND_EXEC_PARAM( bool, val, 0 );
	RETURN_AS( String ) StringConverter::toString( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Char_To_String )
{
	VERBOSE_STARTFUNC( "ctoa" );
	GET_AND_EXEC_PARAM( char, val, 0 );
	RETURN_AS( String ) val;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Char_To_Int )
{
	VERBOSE_STARTFUNC( "ctoi" );
	GET_AND_EXEC_PARAM( char, val, 0 );
	RETURN_AS( int ) static_cast< int >( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Vector3_To_String )
{
	VERBOSE_STARTFUNC( "vtoa" );
	GET_AND_EXEC_PARAM( Vector3, val, 0 );
	RETURN_AS( String ) StringConverter::toString( val );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cst_Quaternion_To_String )
{
	VERBOSE_STARTFUNC( "qtoa" );
	GET_AND_EXEC_PARAM( Quaternion, val, 0 );
	Vector3 l_axis;
	Radian l_angle;
	val.ToAngleAxis( l_angle , l_axis );
	RETURN_AS( String ) String( "Axis : " ) + StringConverter::toString( l_axis ) + String( " // Angle : " ) + StringConverter::toString( l_angle.valueDegrees() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_Set3 )
{
	VERBOSE_STARTFUNC( "Colour_Set3" );
	GET_AND_EXEC_THREE_PARAM( Real, r, Real, g, Real, b );
	RETURN_AS( ColourValue ) ColourValue( r, g, b );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_Set4 )
{
	VERBOSE_STARTFUNC( "Colour_Set4" );
	GET_AND_EXEC_FOUR_PARAM( Real, r, Real, g, Real, b, Real, a );
	RETURN_AS( ColourValue ) ColourValue( r, g, b, a );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_GetR )
{
	VERBOSE_STARTFUNC( "Colour_GetR" );
	GET_AND_EXEC_PARAM( ColourValue, p_colour, 0 );
	RETURN_AS( Real ) p_colour.r;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_GetG )
{
	VERBOSE_STARTFUNC( "Colour_GetG" );
	GET_AND_EXEC_PARAM( ColourValue, p_colour, 0 );
	RETURN_AS( Real ) p_colour.g;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_GetB )
{
	VERBOSE_STARTFUNC( "Colour_GetB" );
	GET_AND_EXEC_PARAM( ColourValue, p_colour, 0 );
	RETURN_AS( Real ) p_colour.b;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_GetA )
{
	VERBOSE_STARTFUNC( "Colour_GetA" );
	GET_AND_EXEC_PARAM( ColourValue, p_colour, 0 );
	RETURN_AS( Real ) p_colour.a;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_SetG )
{
	VERBOSE_STARTFUNC( "Colour_SetG" );
	GET_AND_EXEC_TWO_PARAM( ColourValue, p_colour, Real, p_value );
	p_colour.g = p_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_SetR )
{
	VERBOSE_STARTFUNC( "Colour_SetR" );
	GET_AND_EXEC_TWO_PARAM( ColourValue, p_colour, Real, p_value );
	p_colour.r = p_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_SetB )
{
	VERBOSE_STARTFUNC( "Colour_SetB" );
	GET_AND_EXEC_TWO_PARAM( ColourValue, p_colour, Real, p_value );
	p_colour.b = p_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Col_SetA )
{
	VERBOSE_STARTFUNC( "Colour_SetA" );
	GET_AND_EXEC_TWO_PARAM( ColourValue, p_colour, Real, p_value );
	p_colour.a = p_value;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_PackInt )
{
	GET_AND_EXEC_PARAM( int, p_value, 0 );
	String l_string;
	l_string.append( reinterpret_cast <const char *>( & p_value ), sizeof( int ) );
	RETURN_AS( String ) l_string;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_PackReal )
{
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	String l_string;
	l_string.append( reinterpret_cast <const char *>( & p_value ), sizeof( Real ) );
	RETURN_AS( String ) l_string;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_PackVector3 )
{
	GET_AND_EXEC_PARAM( Vector3, p_value, 0 );
	String l_string;
	l_string.append( reinterpret_cast <const char *>( & p_value ), sizeof( Vector3 ) );
	RETURN_AS( String ) l_string;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_PackQuaternion )
{
	GET_AND_EXEC_PARAM( Quaternion, p_value, 0 );
	String l_string;
	l_string.append( reinterpret_cast <const char *>( & p_value ), sizeof( Quaternion ) );
	RETURN_AS( String ) l_string;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_UnpackInt )
{
	GET_AND_EXEC_PARAM( String, p_value, 0 );

	if ( p_value.size() < sizeof( int ) )
	{
		RETURN_AS( int ) 0;
		return;
	}

	memcpy( & caller->get<int>(), p_value.c_str(), sizeof( int ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_UnpackReal )
{
	GET_AND_EXEC_PARAM( String, p_value, 0 );

	if ( p_value.size() < sizeof( Real ) )
	{
		RETURN_AS( Real ) 0.0;
		return;
	}

	memcpy( & caller->get<Real>(), p_value.c_str(), sizeof( Real ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_UnpackVector3 )
{
	GET_AND_EXEC_PARAM( String, p_value, 0 );

	if ( p_value.size() < sizeof( Vector3 ) )
	{
		RETURN_AS( Vector3 ) Vector3::ZERO;
		return;
	}

	memcpy( & caller->get<Vector3>(), p_value.c_str(), sizeof( Vector3 ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pck_UnpackQuaternion )
{
	GET_AND_EXEC_PARAM( String, p_value, 0 );

	if ( p_value.size() < sizeof( Quaternion ) )
	{
		RETURN_AS( Quaternion ) Quaternion::IDENTITY;
		return;
	}

	memcpy( & caller->get<Quaternion>(), p_value.c_str(), sizeof( Quaternion ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_IterBegin )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap, p_map, 0 );
	RETURN_AS( NodeValueBaseIMap::iterator ) p_map.begin();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_IterEnd )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap, p_map, 0 );
	RETURN_AS( NodeValueBaseIMap::iterator ) p_map.end();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_IterFind )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	RETURN_AS( NodeValueBaseIMap::iterator ) p_map.find( p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_IterBegin )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap, p_map, 0 );
	RETURN_AS( NodeValueBaseMap::iterator ) p_map.begin();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_IterEnd )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap, p_map, 0 );
	RETURN_AS( NodeValueBaseMap::iterator ) p_map.end();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_IterFind )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	RETURN_AS( NodeValueBaseMap::iterator ) p_map.find( p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_IterBegin )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap, p_map, 0 );
	RETURN_AS( NodeValueBaseRMap::iterator ) p_map.begin();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_IterEnd )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap, p_map, 0 );
	RETURN_AS( NodeValueBaseRMap::iterator ) p_map.end();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_IterFind )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, Real, p_key );
	RETURN_AS( NodeValueBaseRMap::iterator ) p_map.find( p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_EraseIter )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, NodeValueBaseIMap::iterator, p_iter );

	if ( p_iter->second != NULL )
	{
		delete p_iter->second;
	}

	p_map.erase( p_iter );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_EraseIter )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, NodeValueBaseMap::iterator, p_iter );

	if ( p_iter->second != NULL )
	{
		delete p_iter->second;
	}

	p_map.erase( p_iter );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_EraseIter )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, NodeValueBaseRMap::iterator, p_iter );

	if ( p_iter->second != NULL )
	{
		delete p_iter->second;
	}

	p_map.erase( p_iter );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Size )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap, p_map, 0 );
	RETURN_AS( int ) static_cast< int >( p_map.size() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Empty )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap, p_map, 0 );
	RETURN_AS( bool ) p_map.empty();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Has )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	RETURN_AS( bool ) General::Utils::map::has( p_map, p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Erase )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	RETURN_AS( bool ) General::Utils::map::deleteValue( p_map, p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Clear )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap, p_map, 0 );
	General::Utils::map::deleteAll( p_map );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Size )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap, p_map, 0 );
	RETURN_AS( int ) static_cast< int >( p_map.size() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Empty )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap, p_map, 0 );
	RETURN_AS( bool ) p_map.empty();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Has )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, Real, p_key );
	RETURN_AS( bool ) General::Utils::map::has( p_map, p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Erase )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, Real, p_key );
	RETURN_AS( bool ) General::Utils::map::deleteValue( p_map, p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Clear )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap, p_map, 0 );
	General::Utils::map::deleteAll( p_map );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Size )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap, p_map, 0 );
	RETURN_AS( int ) static_cast< int >( p_map.size() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Empty )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap, p_map, 0 );
	RETURN_AS( bool ) p_map.empty();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Has )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	RETURN_AS( bool ) General::Utils::map::has( p_map, p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Erase )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	RETURN_AS( bool ) General::Utils::map::deleteValue( p_map, p_key );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Clear )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap, p_map, 0 );
	General::Utils::map::deleteAll( p_map );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Add )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	ScriptNode * l_node = caller->m_childs[2];

	if ( l_node->HasFunction() )
	{
		l_node->Execute();
	}

	if ( l_node->m_value != NULL )
	{
		p_map.insert( std::make_pair( p_key, l_node->m_value->clone() ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_AddDef )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	NodeValueBase * l_node = ScriptNode::CreateNodeValue( caller->m_childs[0]->m_type->GetSubType( 0 ) );
	p_map.insert( std::make_pair( p_key, l_node ) );
	caller->CopyValue_Ref( l_node );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Add )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, Real, p_key );
	ScriptNode * l_node = caller->m_childs[2];

	if ( l_node->HasFunction() )
	{
		l_node->Execute();
	}

	if ( l_node->m_value != NULL )
	{
		p_map.insert( std::make_pair( p_key, l_node->m_value->clone() ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_AddDef )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, Real, p_key );
	NodeValueBase * l_node = ScriptNode::CreateNodeValue( caller->m_childs[0]->m_type->GetSubType( 0 ) );
	p_map.insert( std::make_pair( int( p_key ), l_node ) );
	caller->CopyValue_Ref( l_node );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Add )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	ScriptNode * l_node = caller->m_childs[2];

	if ( l_node->HasFunction() )
	{
		l_node->Execute();
	}

	if ( l_node->m_value != NULL )
	{
		p_map.insert( std::make_pair( p_key, l_node->m_value->clone() ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_AddDef )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	NodeValueBase * l_node = ScriptNode::CreateNodeValue( caller->m_childs[0]->m_type->GetSubType( 0 ) );
	p_map.insert( std::make_pair( p_key, l_node ) );
	caller->CopyValue_Ref( l_node );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Find )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	NodeValueBase * l_node = General::Utils::map::findOrNull( p_map, p_key );

	if ( l_node == NULL )
	{
		caller->SetNullRef();
		return SCRIPT_ERROR( "Error @ Map_Int_Find : item not found for key : " + StringConverter::toString( p_key ) );
	}

	caller->CopyValue_Ref( l_node );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Find )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, Real, p_key );
	NodeValueBase * l_node = General::Utils::map::findOrNull( p_map, p_key );

	if ( l_node == NULL )
	{
		caller->SetNullRef();
		return SCRIPT_ERROR( "Error @ Map_Real_Find : item not found for key : " + StringConverter::toString( p_key ) );
	}

	caller->CopyValue_Ref( l_node );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Find )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	NodeValueBase * l_node = General::Utils::map::findOrNull( p_map, p_key );

	if ( l_node == NULL )
	{
		caller->SetNullRef();
		return SCRIPT_ERROR( "Error @ Map_String_Find : item not found for key : " + p_key );
	}

	caller->CopyValue_Ref( l_node );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Int_Set )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseIMap, p_map, int, p_key );
	NodeValueBase * l_node = General::Utils::map::findOrNull( p_map, p_key );
	ScriptNode * l_value = caller->m_childs[2];

	if ( l_value->HasFunction() )
	{
		l_value->Execute();
	}

	if ( l_node == NULL )
	{
		return SCRIPT_ERROR( "Error @ Map_Int_Set : item not found for key : " + p_key );
	}

	l_node->CopyDeepValue( l_value->m_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Map_Real_Set )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseRMap, p_map, Real, p_key );
	NodeValueBase * l_node = General::Utils::map::findOrNull( p_map, p_key );
	ScriptNode * l_value = caller->m_childs[2];

	if ( l_value->HasFunction() )
	{
		l_value->Execute();
	}

	if ( l_node == NULL )
	{
		return SCRIPT_ERROR( "Error @ Map_Real_Set : item not found for key : " + StringConverter::toString( p_key ) );
	}

	l_node->CopyDeepValue( l_value->m_value );
}


EMUSE_SCRIPT_FUNCTION_DECLARE( Map_String_Set )
{
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseMap, p_map, String, p_key );
	NodeValueBase * l_node = General::Utils::map::findOrNull( p_map, p_key );
	ScriptNode * l_value = caller->m_childs[2];

	if ( l_value->HasFunction() )
	{
		l_value->Execute();
	}

	if ( l_node == NULL )
	{
		return SCRIPT_ERROR( "Error @ Map_String_Set : item not found for key : " + p_key );
	}

	l_node->CopyDeepValue( l_value->m_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Par_First )
{
	GET_AND_EXEC_PARAM( NodeValueBasePair, l_pair, 0 );
	caller->CopyValue_Ref( l_pair.first );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Par_Second )
{
	GET_AND_EXEC_PARAM( NodeValueBasePair, l_pair, 0 );
	caller->CopyValue_Ref( l_pair.second );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Par_Set )
{
	GET_AND_EXEC_PARAM( NodeValueBasePair, l_pair, 0 );

	if ( l_pair.first == NULL )
	{
		l_pair.first = caller->m_childs[1]->m_value->clone();
	}
	else
	{
		l_pair.first->CopyDeepValue( caller->m_childs[1]->m_value );
	}

	if ( l_pair.second == NULL )
	{
		l_pair.second = caller->m_childs[2]->m_value->clone();
	}
	else
	{
		l_pair.second->CopyDeepValue( caller->m_childs[2]->m_value );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sit_Next )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap::iterator, p_iter, 0 );
	++ p_iter;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Rit_Next )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap::iterator, p_iter, 0 );
	++ p_iter;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Iit_Next )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap::iterator, p_iter, 0 );
	++ p_iter;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sit_Prev )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap::iterator, p_iter, 0 );
	-- p_iter;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Rit_Prev )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap::iterator, p_iter, 0 );
	-- p_iter;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Iit_Prev )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap::iterator, p_iter, 0 );
	-- p_iter;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sit_Data )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap::iterator, p_iter, 0 );
	caller->CopyValue_Ref( p_iter->second );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Rit_Data )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap::iterator, p_iter, 0 );
	caller->CopyValue_Ref( p_iter->second );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Iit_Data )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap::iterator, p_iter, 0 );
	caller->CopyValue_Ref( p_iter->second );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sit_Key )
{
	GET_AND_EXEC_PARAM( NodeValueBaseMap::iterator, p_iter, 0 );
	RETURN_AS( String ) p_iter->first;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Rit_Key )
{
	GET_AND_EXEC_PARAM( NodeValueBaseRMap::iterator, p_iter, 0 );
	RETURN_AS( Real ) p_iter->first;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Iit_Key )
{
	GET_AND_EXEC_PARAM( NodeValueBaseIMap::iterator, p_iter, 0 );
	RETURN_AS( int ) p_iter->first;
}
