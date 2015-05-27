
#ifndef ___TROLL_SCRIPT_FUNCTIONS___
#define ___TROLL_SCRIPT_FUNCTIONS___

#include <ScriptFunctions.h>

namespace Troll
{
	namespace Script
	{
#define TROLL_SCRIPT_FUNCTION_DEFINE( X) void d_fast_call X( EMuse::Script::ScriptNode * );

#define TROLL_SCRIPT_FUNCTION_DECLARE( X) void d_fast_call Troll::Script::X( EMuse::Script::ScriptNode * caller)

		// Entity
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Ent_SetMaterial )

		// Overlay
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_Resize )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_Translation )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetSize )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetPosV3 )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetPosX )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetPosY )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetHeight )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetWidth )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetCaption )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_GetCaption )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetMaterial )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetBorderMaterial )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetMouseOverMaterial )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_OvE_SetTextColour )

		// TrollScene Node
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_RotateAround )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_Rotate )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_RotateLocal )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_RotateQuat )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_Pitch )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_Yaw )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_Translate )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_TranslateLocal )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_Scale )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_SetScale )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_SetPosition )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_SetXPosition )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_SetYPosition )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_SetZPosition )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_Reset )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_ScN_SetOrientation )

		// Camera
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Cam_SetPosition )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Cam_ModeWireFrame )

		// Light
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Lgh_SetDirection )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Lgh_SetPosition )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Lgh_SetDiffuse )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Lgh_SetSpecular )

		// Miscellaneous
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Spc_SaveScript )
		TROLL_SCRIPT_FUNCTION_DEFINE( Troll_Spc_ImportMesh )
	}
}

#endif

