/*****************************************************************************

	Author : Marc BILLON
	Compagny : Fordev Studio - Copyright 2007

*****************************************************************************/

#ifndef ___MODULE_SCRIPT_H___
#define ___MODULE_SCRIPT_H___

#include "../EMuse.h"
#include <queue>

namespace EMuse
{
	namespace Script
	{
#define NUM_KEYS 255

		enum VariableType
		{
			EMVT_NULL							= 0x00000000,

			EMVT_INT							= 0x00000001,
			EMVT_REAL							= 0x00000002,

			EMVT_BOOL							= 0x00000010,
			EMVT_VECTOR3						= 0x00000020,
			EMVT_COLOUR							= 0x00000040,
			EMVT_QUATERNION						= 0x00000080,

			EMVT_CHAR							= 0x00000100,
			EMVT_STRING							= 0x00000200,

			EMVT_REF_ENTITY						= 0x00001000,
			EMVT_REF_LIGHT						= 0x00002000,
			EMVT_REF_CAMERA						= 0x00004000,
			EMVT_REF_SCENE_NODE					= 0x00008000,

//		EMVT_REF_OVERLAY					= 0x00010000,
			EMVT_REF_OVERLAY_ELEMENT			= 0x00020000,
//		EMVT_REF_OVERLAY_BORDERPANEL		= 0x00040000,
//		EMVT_REF_OVERLAY_TEXTAREA			= 0x00080000,

			EMVT_REF_ANIMATED_OBJECT_GROUP		= 0x00100000,
			EMVT_REF_ANIMATED_OBJECT			= 0x00200000,
			EMVT_REF_ANIMATION					= 0x00400000,
			EMVT_REF_PHYSICS_OBJECT				= 0x00800000,
			EMVT_REF_SCRIPT_TIMER				= 0x01000000,
			EMVT_REF_CAM_TEX					= 0x02000000,
			EMVT_REF_PHYSICS_MATERIAL			= 0x04000000,

			EMVT_ARRAY							= 0x10000000,

			EMVT_ANY_NUM						= 0x0000000F,
			EMVT_ANY_SPECIAL					= 0x000000F0,
			EMVT_ANY_ALPHA						= 0x00000F00,
			EMVT_ANY_PTR						= 0x0FFFF000,
			EMVT_ANY_OVERLAY					= 0x000F0000,
			EMVT_ANY							= 0xFFFFFFFF
		};

		enum KeyboardBindType
		{
			KBT_KEY_DOWN			= 0,
			KBT_KEY_UP				= 1,
			KBT_KEY_PRESSED			= 2,
			KBT_KEY_REPEAT			= 3
		};

		enum OperatorLevel
		{
			SO_MULTILINE,
			SO_EQUAL,
			SO_OR,
			SO_AND,
			SO_EQUALITY,
			SO_INFERIORITY,
			SO_ARRAY,
			SO_SUPERIORITY,
			SO_INCREMENT,
			SO_PLUSMINUS,
			SO_MULDIV,
			SO_MODULO,
			SO_NEGATE,
			SO_ARROW,
			SO_NONE
		};

		enum ScriptTimerType
		{
			EMTT_ONCE		= 0,
			EMTT_REPEAT		= 1,
			EMTT_CONTINUOUS	= 2,
			EMTT_PERMANENT	= 3
		};

		enum BlockType
		{
			BT_SEPARATOR		= 0x0001,
			BT_STRING			= 0x0002,
			BT_NUMERAL			= 0x0004,
			BT_OPERATOR			= 0x0008,
			BT_PARENTHESIS		= 0x0010,
			BT_BRACKETS			= 0x0020,
			BT_BRACES			= 0x0040,
			BT_SIMPLEQUOTE		= 0x0080,
			BT_DOUBLEQUOTE		= 0x0100,
			BT_INITIAL			= 0x0200,
			BT_VARIABLE_TYPE	= 0x0400,
			BT_IF_ELSE			= 0x0800
		};

		enum BlockSubType
		{
			BST_SEPARATOR_MAJOR,
			BST_SEPARATOR_MINOR,
			BST_FUNCTION,
			BST_VARIABLE,
			BST_KEYWORD_VTYPE,
			BST_KEYWORD_IFELSE,
			BST_NONE
		};

		class ScriptBlock;
		class ScriptEngine;
		class ScriptCompiler;
		class ScriptNode;
		class ScriptTimer;
		class ScriptTimerManager;

		class NodeValueBase;
		template<typename T>
		class NodeValue;

		class ScriptFunction;
		struct LineAnalysis;

		typedef void ( d_fast_call NodeFunction )( ScriptNode * );

		typedef std::map<String, ScriptFunction *>						ScriptFunctionMap;
		typedef std::multimap<String, ScriptFunction *>					ScriptFunctionMultiMap;
		typedef std::map<VariableType, ScriptFunctionMap>				ScriptClassFunctionMap;
		typedef std::map<OverlayElement *, ScriptNode *>					OverlayActionMap;
		typedef std::map<String, ScriptNode *>							ScriptNodeMap;
		typedef std::map<String, ScriptTimer *>							TimerMap;
		typedef std::vector<ScriptNode *>								ScriptNodeArray;
		typedef std::vector<VariableType>								VariableTypeArray;
		typedef std::vector<ScriptBlock *>								ScriptBlockArray;
		typedef std::queue<ScriptNode *>									ScriptNodeQueue;

		typedef std::vector<NodeValueBase *>								NodeValueBaseArray;

#define VERBOSE_COMPILATOR( p_string ) m_compiler->_log( p_string);
#define VERBOSE_COMPILATOR_INTERNAL( p_string ) _log( p_string);
		//EMUSE_LOG_MESSAGE_RELEASE( p_string )

		String ValueTypeToString( unsigned int p_type );
	}
}

#endif
