#include "PrecompiledHeader.h"

#include "Events.h"
#include "Sequence.h"
#include "ScriptNode.h"
#include "ScriptEngine.h"
#include "Context.h"
#include "EMuseInstance.h"
#include "EMOverlayGroup.h"
#include "EMOverlay.h"
#include <OgreLog.h>

void SceneNode_SetPosition :: Apply()
{
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	m_previousPosition = l_target->getPosition();
	l_target->setPosition( m_position );
}

void SceneNode_SetPosition :: Rollback()
{
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	l_target->setPosition( m_previousPosition );
}

void SceneNode_SetScale :: Apply()
{
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	m_previousScale = l_target->getScale();
	l_target->setScale( m_scale );
}

void SceneNode_SetScale :: Rollback()
{
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	l_target->setScale( m_previousScale );
}

void Overlay_Hide :: Apply()
{
	EMOverlayGroup * l_target = static_cast <EMOverlayGroup *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->SetVisible( false );
}

void Overlay_Hide :: Rollback()
{
	EMOverlayGroup * l_target = static_cast <EMOverlayGroup *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->SetVisible( true );
}

void Overlay_Show :: Apply()
{
	EMOverlayGroup * l_target = static_cast <EMOverlayGroup *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->SetVisible( true );
}

void Overlay_Show :: Rollback()
{
	EMOverlayGroup * l_target = static_cast <EMOverlayGroup *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->SetVisible( false );
}

void ScriptNode_Execute :: Apply()
{
	ScriptNode * l_target = static_cast <ScriptNode *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Execute();
}

void Sequence_Start :: Apply()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Start();
}

void Sequence_Start :: Rollback()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Stop();
}

void Sequence_Pause :: Apply()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Pause( true );
}

void Sequence_Pause :: Rollback()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Pause( false );
}

void Sequence_Unpause :: Apply()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Pause( false );
}

void Sequence_Unpause :: Rollback()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Pause( true );
}

void Sequence_Stop :: Apply()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Stop();
}

void Sequence_Stop :: Rollback()
{
	Sequence * l_target = static_cast <Sequence *>( m_target );

	if ( l_target == NULL )
	{
		return;
	}

	l_target->Start();
}

void SceneNode_Translate :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	l_target->setPosition( m_interpolator( m_frames, m_currentTime ) );
}

void SceneNode_Scale :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	l_target->setScale( m_interpolator( m_frames, m_currentTime ) );
}

void SceneNode_Rotate :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	SceneNode * l_target = static_cast <SceneNode *>( m_target );
	l_target->setOrientation( m_interpolator( m_frames, m_currentTime ) );
}

void Overlay_Translate :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	OverlayElement * l_target = static_cast <OverlayElement *>( m_target );
	Vector3 l_tmp = m_interpolator( m_frames, m_currentTime );
	l_target->setLeft( l_tmp.x );
	l_target->setTop( l_tmp.y );
}

void Overlay_Rotate :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	Overlay * l_target = static_cast <EMOverlayGroup *>( m_target )->GetOverlay();
	l_target->setRotate( Degree( m_interpolator( m_frames, m_currentTime ) ) );
}

void Overlay_Resize :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	EMOverlay * l_target = static_cast <EMOverlay *>( m_target );
	Vector3 l_tmp = m_interpolator( m_frames, m_currentTime );
	l_target->SetWidth( l_tmp.x );
	l_target->SetHeight( l_tmp.y );
}

void Overlay_TranslateRelative :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	EMOverlay * l_target = static_cast <EMOverlay *>( m_target );
	Vector3 l_tmp = m_interpolator( m_frames, m_currentTime );
	l_target->SetLeft( l_tmp.x * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
	l_target->SetTop( l_tmp.y * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
}

void Overlay_ResizeRelative :: Apply( Real p_time )
{
	_getRemainingTime( p_time );
	EMOverlay * l_target = static_cast <EMOverlay *>( m_target );
	Vector3 l_tmp = m_interpolator( m_frames, m_currentTime );
	l_target->SetWidth( l_tmp.x * ScriptEngine::GetContext()->mainViewport->getActualWidth() );
	l_target->SetHeight( l_tmp.y * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
}
