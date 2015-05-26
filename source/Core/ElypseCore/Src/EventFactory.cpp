#include "PrecompiledHeader.h"

#include "EventFactory.h"
#include "Events.h"

#include "Context.h"
#include "ScriptEngine.h"

BasePonctualEvent * SceneNode_SetPosition_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	if ( p_params.size() != 3 )
	{
		return NULL;
	}

	return new SceneNode_SetPosition( Vector3(	StringConverter::parseReal( p_params[0] ),
									  StringConverter::parseReal( p_params[1] ),
									  StringConverter::parseReal( p_params[2] ) ) );
}

BasePonctualEvent * SceneNode_SetScale_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	if ( p_params.size() != 3 )
	{
		return NULL;
	}

	return new SceneNode_SetScale( Vector3(	StringConverter::parseReal( p_params[0] ),
											StringConverter::parseReal( p_params[1] ),
											StringConverter::parseReal( p_params[2] ) ) );
}

BasePonctualEvent * Overlay_Hide_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new Overlay_Hide();
}

BasePonctualEvent * Overlay_Show_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new Overlay_Show();
}

BasePonctualEvent * ScriptNode_Execution_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new ScriptNode_Execute();
}

BasePonctualEvent * Sequence_Starting_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new Sequence_Start();
}

BasePonctualEvent * Sequence_Pausing_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new Sequence_Pause();
}

BasePonctualEvent * Sequence_Unpausing_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new Sequence_Unpause();
}

BasePonctualEvent * Sequence_Stopping_Factory :: CreateEvent( const EMuse::StringArray & p_params )
{
	return new Sequence_Stop();
}

BaseContinuousEvent * SceneNode_Translation_Factory :: CreateEvent()
{
	return new SceneNode_Translate();
}

BaseContinuousEvent * SceneNode_Rotation_Factory :: CreateEvent()
{
	return new SceneNode_Rotate();
}

BaseContinuousEvent * SceneNode_Scale_Factory :: CreateEvent()
{
	return new SceneNode_Scale();
}

BaseContinuousEvent * Overlay_Translation_Factory :: CreateEvent()
{
	return new Overlay_Translate();
}

BaseContinuousEvent * Overlay_Rotation_Factory :: CreateEvent()
{
	return new Overlay_Rotate();
}

BaseContinuousEvent * Overlay_Resizing_Factory :: CreateEvent()
{
	return new Overlay_Resize();
}

BaseContinuousEvent * Overlay_RelativeTranslation_Factory :: CreateEvent()
{
	return new Overlay_TranslateRelative();
}

BaseContinuousEvent * Overlay_RelativeResizing_Factory :: CreateEvent()
{
	return new Overlay_ResizeRelative();
}
