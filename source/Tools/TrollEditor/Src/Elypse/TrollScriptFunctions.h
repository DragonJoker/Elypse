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
#ifndef ___TROLL_SCRIPT_FUNCTIONS___
#define ___TROLL_SCRIPT_FUNCTIONS___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <ScriptFunctions.h>

BEGIN_TROLL_ELYPSE_NAMESPACE
{
#define TROLL_SCRIPT_FUNCTION_DEFINE( X) void d_fast_call X( Elypse::Script::ScriptNode * );

#define TROLL_SCRIPT_FUNCTION_DECLARE( X) void d_fast_call X( Elypse::Script::ScriptNode * caller)

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

	// Scene Node
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
END_TROLL_ELYPSE_NAMESPACE

#endif

