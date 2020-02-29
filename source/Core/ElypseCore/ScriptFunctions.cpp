/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ScriptFunctions.h"

#if ELYPSE_WINDOWS
#	define _WINGDI_
#elif ELYPSE_LINUX
#	include <sys/stat.h>
#elif ELYPSE_MACOS

#endif

#include "ScriptEngine.h"
#include "VariableType.h"

#include "ScriptNode.h"
#include "ScriptNodeValue.h"
#include "ScriptCompiler.h"
#include "ScriptTimerManager.h"
#include "ScriptTimer.h"
#include "Portal.h"
#include "ElypseResourceGroupManager.h"

#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreLog.h>
#include <OgreEntity.h>
#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreCamera.h>
#include <OgreStringConverter.h>
#include <OgreOverlayManager.h>
#include <OgreSubEntity.h>
#include <OgreViewport.h>
#include <OgreRenderTarget.h>
#include <OgreFontManager.h>
#include <OgreTechnique.h>

#include <time.h>

#include "ElypseInstance.h"
#include "ElypseFrameListener.h"
#include "ElypsePlugin.h"

#include "SoundManager.h"
#include "SoundPlaylist.h"
#include "SoundObject.h"
#include "SoundInstance.h"
#include "VideoManager.h"
#include "VideoObject.h"
#include "VideoInstance.h"
#include "VideoOverlay.h"

#include "DownloadManager.h"
#include "MuseDownloader.h"
#include "MuseFile.h"

#include "SceneFileSaver.h"
#include "SceneFileParser.h"

#include "AnimatedObjectGroup.h"
#include "AnimatedObject.h"
#include "Animation.h"
#include "AnimationManager.h"

#include "Mirror.h"
#include "MirrorManager.h"
#include "CamTex.h"
#include "PostEffect.h"

#include "WebImageManager.h"
#include "WebImage.h"
#include "MaterialTarget.h"

#include "Zone.h"
#include "Universe.h"

#include "EMOverlayGroup.h"
#include "EMOverlay.h"
#include "EMGui.h"

#include "PubManager.h"
#include "PubObject.h"

#include "PhysicsEngine.h"
#include "PhysicsSimulation.h"
#include "PhysicsObject.h"
#include "BoundingMesh.h"
#include "SimpleBoundingShapes.h"

#include "ElypsePlugin.h"
#include "Context.h"

#include "NetworkManager.h"

#include <Manager.h>
#include <FastMath.h>
#include <File.h>
#include <Bezier.h>
#include <NeoCurl.h>
#include <Buffer.h>
#include <MD5.h>
#include <StringUtils.h>

#include "ElypseLogs.h"

#pragma warning( disable:4996 )

using namespace General::Math;
using namespace Ogre;

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_Create )
{
	VERBOSE_STARTFUNC( " Timer_Create " );
	GET_AND_EXEC_THREE_PARAM( String, l_name, int, l_type, Real, l_time );
	ScriptNode * l_sCode = caller->m_childs[3];

	if ( ( ! l_name.empty() ) && ( l_type >= 0 && l_type <= 3 ) )
	{
		RETURN_AS( ScriptTimer * ) ScriptEngine::GetContext()->timerManager->AddTimer( l_name, l_time, l_sCode, static_cast <ScriptTimerType>( l_type ) );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Timer_Create : wrong params, name null or wrong type ( 0 to 3 normally, here " + StringConverter::toString( l_type ) + " )" );
		RETURN_AS( ScriptTimer * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_CreatePermanent )
{
	VERBOSE_STARTFUNC( "Timer_CreatePermanent" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	ScriptNode * l_code = caller->m_childs[1];

	if ( ! l_name.empty() )
	{
		RETURN_AS( ScriptTimer * ) ScriptEngine::GetContext()->timerManager->AddTimer( l_name, 0.0, l_code, EMTT_PERMANENT, nullptr );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Timer_CreateContinuous : timer name null" );
		RETURN_AS( ScriptTimer * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_CreateContinuous )
{
	VERBOSE_STARTFUNC( " Timer_CreateContinuous " );
	GET_AND_EXEC_TWO_PARAM( String, l_sName, Real, l_sTime );
	ScriptNode * l_sFinalCode = caller->m_childs[2];
	ScriptNode * l_sCode = caller->m_childs[3];

	if ( ! l_sName.empty() )
	{
		RETURN_AS( ScriptTimer * ) ScriptEngine::GetContext()->timerManager->AddTimer( l_sName, l_sTime, l_sCode, EMTT_CONTINUOUS, l_sFinalCode );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Timer_CreateContinuous : timer name null" );
		RETURN_AS( ScriptTimer * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_Kill )
{
	VERBOSE_STARTFUNC( " Timer_Kill " );
	GET_AND_EXEC_PARAM( String, l_timerName, 0 );
	ScriptEngine::GetContext()->timerManager->DestroyTimer( l_timerName );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_Shift )
{
	VERBOSE_STARTFUNC( " Timer_Shift " );
	GET_AND_EXEC_TWO_PARAM( String, l_timerName, Real, l_shift );
	ScriptTimer * l_timer = ScriptEngine::GetContext()->timerManager->GetByName( l_timerName );

	if ( l_timer != nullptr )
	{
		l_timer->Shift( l_shift );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_GetByName )
{
	VERBOSE_STARTFUNC( " Timer_GetByName " );
	GET_AND_EXEC_PARAM( String, l_timerName, 0 );
	RETURN_AS( ScriptTimer * ) ScriptEngine::GetContext()->timerManager->GetByName( l_timerName );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_SetBaseTime )
{
	VERBOSE_STARTFUNC( " Timer_SetBaseTime " );
	GET_AND_EXEC_TWO_PARAM( ScriptTimer *, l_timer, Real, l_time );

	if ( l_timer != nullptr )
	{
		l_timer->SetBaseTime( l_time );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Timer_SetBaseTime -> no timer" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_Exists )
{
	VERBOSE_STARTFUNC( " Timer_Exists " );
	GET_AND_EXEC_PARAM( String, l_timerName, 0 );
	RETURN_AS( bool ) ScriptEngine::GetContext()->timerManager->HasTimer( l_timerName );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_Pause )
{
	VERBOSE_STARTFUNC( " Timer_Pause " );
	GET_AND_EXEC_PARAM( String, l_timerName, 0 );

	if ( ScriptEngine::GetContext()->timerManager->HasTimer( l_timerName ) )
	{
		ScriptEngine::GetContext()->timerManager->GetByName( l_timerName )->Pause();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_Play )
{
	VERBOSE_STARTFUNC( " Timer_Play " );
	GET_AND_EXEC_PARAM( String, l_timerName, 0 );

	if ( ScriptEngine::GetContext()->timerManager->HasTimer( l_timerName ) )
	{
		ScriptEngine::GetContext()->timerManager->GetByName( l_timerName )->Play();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_GetNumExecs )
{
	VERBOSE_STARTFUNC( "Timer_GetNumExecs" );
	GET_AND_EXEC_PARAM( ScriptTimer *, l_timer, 0 );

	if ( l_timer != nullptr )
	{
		RETURN_AS( int ) l_timer->m_numExecs;
	}
	else
	{
		RETURN_AS( int ) 0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_GetTimeLeft )
{
	VERBOSE_STARTFUNC( "Timer_GetTimeLeft" );
	GET_AND_EXEC_PARAM( ScriptTimer *, l_timer, 0 );

	if ( l_timer != nullptr )
	{
		RETURN_AS( int ) int( l_timer->m_leftTime );
	}
	else
	{
		RETURN_AS( int ) 0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_GetTimeElapsed )
{
	VERBOSE_STARTFUNC( "Timer_GetTimeLeft" );
	GET_AND_EXEC_PARAM( ScriptTimer *, l_timer, 0 );

	if ( l_timer != nullptr )
	{
		RETURN_AS( Real ) l_timer->m_baseTime - l_timer->m_leftTime;
	}
	else
	{
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_GetTimeBase )
{
	VERBOSE_STARTFUNC( "Timer_GetTimeLeft" );
	GET_AND_EXEC_PARAM( ScriptTimer *, l_timer, 0 );

	if ( l_timer != nullptr )
	{
		RETURN_AS( Real ) l_timer->m_baseTime;
	}
	else
	{
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_GetName )
{
	VERBOSE_STARTFUNC( "Timer_GetName" );
	GET_AND_EXEC_PARAM( ScriptTimer *, l_timer, 0 );

	if ( l_timer != nullptr )
	{
		RETURN_AS( String ) l_timer->GetName();
	}
	else
	{
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_PauseAll )
{
	VERBOSE_STARTFUNC( " Timer_PauseAll " );
	ScriptEngine::GetContext()->timerManager->PauseAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_KillAll )
{
	VERBOSE_STARTFUNC( " Timer_KillAll " );
	ScriptEngine::GetContext()->timerManager->KillAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tmr_PlayAll )
{
	VERBOSE_STARTFUNC( " Timer_PlayAll " );
	ScriptEngine::GetContext()->timerManager->PlayAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tml_Pause )
{
	VERBOSE_STARTFUNC( " Timeline_Pause " );
	ScriptEngine::GetContext()->timerManager->Pause();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tml_Play )
{
	VERBOSE_STARTFUNC( " Timeline_Play " );
	ScriptEngine::GetContext()->timerManager->Play();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_Set )
{
	VERBOSE_STARTFUNC( "Vector3" );
	GET_AND_EXEC_THREE_PARAM( Real, x, Real, y, Real, z );
	RETURN_AS( Vector3 ) Vector3( x, y , z );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_LinearInterpolation )
{
	VERBOSE_STARTFUNC( "V3_LinearInterpolation" );
	GET_AND_EXEC_THREE_PARAM( Vector3, l_start, Vector3, l_end, Real, l_ratio );
	RETURN_AS( Vector3 ) Vector3( l_end * ( 1 - l_ratio ) + ( l_start * l_ratio ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_SquaredInterpolation )
{
	VERBOSE_STARTFUNC( "V3_SquaredInterpolation" );
	GET_AND_EXEC_THREE_PARAM( Vector3, l_start, Vector3, l_end, Real, l_ratio );
	RETURN_AS( Vector3 ) Vector3( l_end * ( 1 - l_ratio * l_ratio * l_ratio ) + ( l_start * l_ratio * l_ratio * l_ratio ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_SinusInterpolation )
{
	VERBOSE_STARTFUNC( "V3_SinusInterpolation" );
	GET_AND_EXEC_THREE_PARAM( Vector3, l_start, Vector3, l_end, Real, l_ratio );
	Radian l_angle = Radian( -Math::HALF_PI + Math::PI * l_ratio );
	Real l_interpolatedValue = Real( ( Math::Sin( l_angle ) + 1.0 ) / 2.0 );
	RETURN_AS( Vector3 ) Vector3( l_end * ( 1 - l_interpolatedValue ) + ( l_start * l_interpolatedValue ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_CubicInterpolation )
{
	VERBOSE_STARTFUNC( "V3_CubicInterpolation" );
	GET_AND_EXEC_THREE_PARAM( Vector3, l_start, Vector3, l_end, Real, l_ratio );
	RETURN_AS( Vector3 ) Vector3( l_end * ( 2 * l_ratio * l_ratio * l_ratio - 3 * l_ratio * l_ratio + 1 ) + l_start * ( 3 * l_ratio * l_ratio - 2 * l_ratio * l_ratio * l_ratio ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_BezierInterpolation )
{
	VERBOSE_STARTFUNC( "Ve3_BezierInterpolation" );
	GET_AND_EXEC_FOUR_PARAM( Vector3, l_start, Vector3, l_middle, Vector3, l_end, Real, l_ratio );
	Real const * l_factors = Bezier<3>::GetSingletonPtr()->Get( l_ratio );
	RETURN_AS( Vector3 ) Vector3( l_start * l_factors[0] + l_middle * l_factors[1] + l_end * l_factors[2] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_BezierGetLength )
{
	VERBOSE_STARTFUNC( "Ve3_BezierGetLength" );
	GET_AND_EXEC_FOUR_PARAM( Vector3, l_start, Vector3, l_middle, Vector3, l_end, int, p_num );
	Vector3 l_point = l_start;
	Real l_length = 0.0;

	for ( int i = 0 ; i < p_num; i ++ )
	{
		Real const * l_factors = Bezier<3>::GetSingletonPtr()->Get( ( i + 1 ) / static_cast<Real>( p_num ) );
		Vector3 l_pointB( l_start * l_factors[0] + l_middle * l_factors[1] + l_end * l_factors[2] );
		l_length += l_point.distance( l_pointB );
		l_point = l_pointB;
	}

	RETURN_AS( Real ) l_length;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_OperatorArray )
{
	VERBOSE_STARTFUNC( " [] (Vector3)" );
	GET_AND_EXEC_TWO_PARAM( Vector3, l_vector, int, l_index );

	if ( l_index >= 0 && l_index < 3 )
	{
		RETURN_AS( Real ) l_vector.ptr()[l_index];
	}
	else
	{
		SCRIPT_ERROR( "Error @ Ve3_OperatorArray : index out of bounds" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_SnapCoords )
{
	VERBOSE_STARTFUNC( "V3_SnapCoords" );
	GET_AND_EXEC_TWO_PARAM( Vector3, l_originalVector, Real, l_clamp );
	Vector3 l_vector = l_originalVector;
	Real l_temp;

	for ( uint32_t i = 0 ; i < 3 ; i ++ )
	{
		l_temp = ( l_vector.ptr()[i] ) / l_clamp - Math::Floor( l_vector.ptr()[i] / l_clamp );

		if ( l_temp < 0.5 )
		{
			l_vector.ptr()[i] -= l_temp * l_clamp;
		}
		else
		{
			l_vector.ptr()[i] += ( 1 - l_temp ) * l_clamp;
		}
	}

	RETURN_AS( Vector3 ) l_vector;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_GetLength )
{
	VERBOSE_STARTFUNC( "V3_GetLength" );
	GET_AND_EXEC_PARAM( Vector3, l_vector, 0 );
	RETURN_AS( Real ) l_vector.length();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_ProjectOnScreen )
{
	VERBOSE_STARTFUNC( "V3_ProjectOnScreen" );
	GET_AND_EXEC_PARAM( Vector3, l_originalVector, 0 );
	Vector3 l_vector = l_originalVector;
	l_vector = ScriptEngine::GetContext()->mainCamera->getProjectionMatrix() * ScriptEngine::GetContext()->mainCamera->getViewMatrix() * l_vector;
	l_vector.y = -l_vector.y;
	l_vector += 1.0;
	l_vector.x *= ScriptEngine::GetContext()->mainViewport->getActualWidth();
	l_vector.y *= ScriptEngine::GetContext()->mainViewport->getActualHeight();
	l_vector.z = 0.0;
	l_vector /= 2.0;
	RETURN_AS( Vector3 ) l_vector;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_DotProduct )
{
	VERBOSE_STARTFUNC( "V3_DotProduct" );
	GET_AND_EXEC_TWO_PARAM( Vector3, a, Vector3, b );
	RETURN_AS( Real ) a.dotProduct( b );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_CrossProduct )
{
	VERBOSE_STARTFUNC( "V3_CrossProduct" );
	GET_AND_EXEC_TWO_PARAM( Vector3, a, Vector3, b );
	RETURN_AS( Vector3 ) a.crossProduct( b );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_GetRotationTo )
{
	VERBOSE_STARTFUNC( "V3_GetRotationTo" );
	GET_AND_EXEC_TWO_PARAM( Vector3, a, Vector3, b );
	RETURN_AS( Quaternion ) a.normalisedCopy().getRotationTo( b.normalisedCopy() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_Normalise )
{
	VERBOSE_STARTFUNC( "V3_Normalise" );
	GET_AND_EXEC_PARAM( Vector3, l_vector, 0 );
	l_vector.normalise();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ve3_NormalisedCopy )
{
	VERBOSE_STARTFUNC( "V3_NormalisedCopy" );
	GET_AND_EXEC_PARAM( Vector3, l_vector, 0 );
	RETURN_AS( Vector3 ) l_vector.normalisedCopy();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetByName )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetByName" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	EMOverlay * l_overlay = ScriptEngine::GetContext()->gui->GetElementByName( l_name );

	if ( l_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetByName -> OverlayElement : " + l_name + " does not exist" );
	}

	RETURN_AS( EMOverlay * ) l_overlay;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Exists )
{
	VERBOSE_STARTFUNC( "OverlayElement_Exists" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	EMOverlay * l_overlay = ScriptEngine::GetContext()->gui->GetElementByName( l_name );
	RETURN_AS( bool )( l_overlay != nullptr );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Create )
{
	VERBOSE_STARTFUNC( "OverlayElement_Create" );
	GET_AND_EXEC_THREE_PARAM( String, l_name, String, l_template, String, l_parent );

	if ( ScriptEngine::GetContext()->gui->GetElementByName( l_name ) == nullptr )
	{
		EMOverlayGroup * l_overlayGroup;
		EMOverlay * l_overlay = nullptr;
		EMOverlay * l_parentOverlay = ScriptEngine::GetContext()->gui->GetElementByName( l_parent );

		if ( l_parentOverlay == nullptr )
		{
			SCRIPT_ERROR( "Error @ OverlayElement_GetByName -> Parent OverlayElement : " + l_parent + " does not exist" );
		}
		else
		{
			l_overlayGroup = l_parentOverlay->GetOwner();
			l_overlay = l_parentOverlay->CreateChild( l_name );

			if ( l_overlay == nullptr )
			{
				SCRIPT_ERROR( "Error @ OverlayElement_Create -> Can't create OverlayElement " + l_name );
			}
			else
			{
				if ( ! l_overlay->Initialise( l_template ) )
				{
					SCRIPT_ERROR( "Warning @ OverlayElement_Create -> Template " + l_template + " doesn't exist" );
				}

				/*
								else
								{
									l_overlay->SetLeft( 0.0);
									l_overlay->SetTop( 0.0);
									l_overlay->SetWidth( 256.0);
									l_overlay->SetHeight( 256.0);

									if (l_overlay->GetOgreOverlayElement()->getTypeName() == "BorderPanel")
									{
										BorderPanelOverlayElement * l_borderOV = static_cast <BorderPanelOverlayElement *> (l_overlay->GetOgreOverlayElement());
										SCRIPT_ERROR( StringConverter::toString( l_borderOV->getLeftBorderSize()) + "," + StringConverter::toString( l_borderOV->getRightBorderSize()) +
													  "," + StringConverter::toString( l_borderOV->getTopBorderSize()) + "," + StringConverter::toString( l_borderOV->getBottomBorderSize()));
									}
									else if (l_overlay->GetOgreOverlayElement()->getTypeName() == "TextArea")
									{
										SCRIPT_ERROR( "Info - " + l_name + "'s CharHeight : " + StringConverter::toString( static_cast <TextAreaOverlayElement *> (l_overlay->GetOgreOverlayElement())->getCharHeight() * ScriptEngine::GetContext()->mainViewport->getActualHeight()));
									}
								}
				*/
			}
		}

		RETURN_AS( EMOverlay * ) l_overlay;
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Create -> OverlayElement " + l_name + " already exists" );
		RETURN_AS( EMOverlay * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Destroy )
{
	VERBOSE_STARTFUNC( "OverlayElement_Destroy" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	EMOverlay * l_overlay = ScriptEngine::GetContext()->gui->GetElementByName( l_name );

	if ( l_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Destroy -> OverlayElement : " + l_name + " does not exist" );
		return;
	}

	if ( l_overlay->GetParent() != nullptr )
	{
		l_overlay->GetParent()->DestroyChild( l_overlay );
	}
	else
	{
		delete l_overlay;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_MousePickup )
{
	VERBOSE_STARTFUNC( "OverlayElement_MousePickup" );
	GET_AND_EXEC_TWO_PARAM( Real, x, Real, y );
	RETURN_AS( EMOverlay * ) ScriptEngine::GetContext()->gui->GetTopmostAt( x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Translation )
{
	VERBOSE_STARTFUNC( "OverlayElement_Translation" );
	GET_AND_EXEC_THREE_PARAM( EMOverlay *, p_overlay, Real, p_x, Real, p_y );

	if ( p_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Translation : overlay null" );
		return;
	}

	p_overlay->GetOgreOverlayElement()->setPosition( p_overlay->GetOgreOverlayElement()->getLeft() + p_x / ScriptEngine::GetContext()->mainViewport->getActualWidth(),
			p_overlay->GetOgreOverlayElement()->getTop() + p_y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Show )
{
	VERBOSE_STARTFUNC( "OverlayElement_Show" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		l_overlay->SetVisible( true );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Show : OverlayElement null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Hide )
{
	VERBOSE_STARTFUNC( "OverlayElement_Hide" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		l_overlay->SetVisible( false );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Hide : OverlayElement null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetVisible )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetVisible" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, l_overlay, bool, p_visible );

	if ( l_overlay != nullptr )
	{
		l_overlay->SetVisible( p_visible );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Hide : OverlayElement null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_IsVisible )
{
	VERBOSE_STARTFUNC( "OverlayElement_IsVisible" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( bool ) l_overlay->GetOgreOverlayElement()->isVisible();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Hide : OverlayElement null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Resize )
{
	VERBOSE_STARTFUNC( "OverlayElement_Resize" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( Real, x, 1 );
		GET_AND_EXEC_PARAM( Real, y, 2 );
		l_overlay->GetOgreOverlayElement()->setDimensions( l_overlay->GetOgreOverlayElement()->getWidth() + x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), l_overlay->GetOgreOverlayElement()->getHeight() + y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Resize : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetSize )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetSize" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( Real, x, 1 );
		GET_AND_EXEC_PARAM( Real, y, 2 );
		l_overlay->GetOgreOverlayElement()->setDimensions( x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_Resize : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetLRBorderSize )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetLRBorderSize" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( Real, l, 1 );
		GET_AND_EXEC_PARAM( Real, r, 2 );
		OverlayElement * l_ov = l_overlay->GetOgreOverlayElement();

		if ( l_ov->getTypeName() != "BorderPanel" )
		{
			SCRIPT_ERROR( "Error @ OverlayElement_SetLRBorderSize : overlay is not a BorderPanel" );
		}
		else
		{
			BorderPanelOverlayElement * l_bov = static_cast <BorderPanelOverlayElement *>( l_ov );
			Real b = l_bov->getBottomBorderSize();
			Real t = l_bov->getTopBorderSize();
			l_bov->setBorderSize( l, r, t, b );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetLRBorderSize : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetTBBorderSize )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetTBBorderSize" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( Real, t, 1 );
		GET_AND_EXEC_PARAM( Real, b, 2 );
		OverlayElement * l_ov = l_overlay->GetOgreOverlayElement();

		if ( l_ov->getTypeName() != "BorderPanel" )
		{
			SCRIPT_ERROR( "Error @ OverlayElement_SetTBBorderSize : overlay is not a BorderPanel" );
		}
		else
		{
			BorderPanelOverlayElement * l_bov = static_cast <BorderPanelOverlayElement *>( l_ov );
			Real l = l_bov->getLeftBorderSize();
			Real r = l_bov->getRightBorderSize();
			l_bov->setBorderSize( l, r, t, b );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetTBBorderSize : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetHeight )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetHeight" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, l_height );

	if ( p_overlay != nullptr )
	{
		p_overlay->GetOgreOverlayElement()->setHeight( l_height / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetHeight : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetWidth )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetWidth" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, l_width );

	if ( p_overlay != nullptr )
	{
		p_overlay->GetOgreOverlayElement()->setWidth( l_width / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetWidth : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetCaption )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetCaption" );
	GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

	if ( p_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( String, l_caption, 1 );
		//TODO : check if that's enough // text area overlay element
		p_overlay->SetCaption( l_caption );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetCaption : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetCaption )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetCaption" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( String ) l_overlay->GetOgreOverlayElement()->getCaption();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetCaption : overlay null" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetMouseOverMaterial )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetMaterial" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( String, l_materialName, 1 );

		if ( MaterialManager::getSingletonPtr()->getByName( l_materialName ) )
		{
			l_overlay->SetMouseOverMaterial( l_materialName );
		}
		else
		{
			SCRIPT_ERROR( "Error @ OverlayElement_SetMouseOverMaterial : material " + l_materialName + "does not exist" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetMouseOverMaterial : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetMaterial )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetMaterial" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( String, p_materialName, 1 );

		if ( MaterialManager::getSingletonPtr()->getByName( p_materialName ) )
		{
			WebImage * l_image = WebImageManager::GetSingletonPtr()->GetWebImage( p_materialName );

			if ( l_image == nullptr || l_image->IsReady() )
			{
				l_overlay->SetBaseMaterial( p_materialName );
			}
			else
			{
				l_image->AddTarget( new MaterialTarget_Overlay( l_overlay ) );
			}

//			l_overlay->SetBaseMaterial( p_materialName);
		}
		else
		{
			SCRIPT_ERROR( "Error @ OverlayElement_SetMaterial : material " + p_materialName + "does not exist" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetMaterial : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetBorderMaterial )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetBorderMaterial" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		GET_AND_EXEC_PARAM( String, l_materialName, 1 );

		if ( MaterialManager::getSingletonPtr()->getByName( l_materialName ) )
		{
			( static_cast <BorderPanelOverlayElement *>( l_overlay->GetOgreOverlayElement() ) )->setBorderMaterialName( l_materialName );
		}
		else
		{
			SCRIPT_ERROR( "Error @ OverlayElement_SetBorderMaterial : material [" + l_materialName + "] does not exist" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetBorderMaterial : overlay null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetTextColour )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetTextColour" );
	GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

	if ( p_overlay != nullptr )
	{
		OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();

		if ( l_overlay->getTypeName() == "TextArea" )
		{
			GET_AND_EXEC_PARAM( ColourValue, l_colour, 1 );
			TextAreaOverlayElement * l_over = static_cast <TextAreaOverlayElement *>( l_overlay );
			l_over->setColourBottom( l_colour );
			l_over->setColourTop( l_colour );
		}
		else
		{
			SCRIPT_ERROR( "Error @ OverlayElement_SetTextColour : OverlayElement [" + l_overlay->getName() + "] is not a TextArea" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetTextColour : OverlayElement null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetTextColour )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetTextColour" );
	GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

	if ( p_overlay != nullptr )
	{
		OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();

		if ( l_overlay->getTypeName() == "TextArea" )
		{
			TextAreaOverlayElement * l_over = static_cast <TextAreaOverlayElement *>( l_overlay );
			RETURN_AS( ColourValue ) l_over->getColour();
		}
		else
		{
			SCRIPT_ERROR( "Error @ OverlayElement_GetTextColour : OverlayElement [" + l_overlay->getName() + "] is not a TextArea" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetTextColour : OverlayElement null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetPosition )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetPosition" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Vector3 ) Vector3( l_overlay->GetOgreOverlayElement()->_getLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth(),
									  l_overlay->GetOgreOverlayElement()->_getTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight(),
									  0.0 );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetPosV3 : OverlayElement null" );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetLeft )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetLeft" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Real ) l_overlay->GetOgreOverlayElement()->_getLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetPosX : OverlayElement null" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetTop )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetTop" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Real ) l_overlay->GetOgreOverlayElement()->_getTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetPosY : OverlayElement null" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetAbsolutePosition )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetAbsolutePosition" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Vector3 ) Vector3( l_overlay->GetOgreOverlayElement()->_getDerivedLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth(),
									  l_overlay->GetOgreOverlayElement()->_getDerivedTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight(),
									  0.0 );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetPosV3 : OverlayElement null" );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetAbsoluteLeft )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetAbsoluteLeft" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Real ) l_overlay->GetOgreOverlayElement()->_getDerivedLeft() * ScriptEngine::GetContext()->mainViewport->getActualWidth();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetPosX : OverlayElement null" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetAbsoluteTop )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetAbsoluteTop" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Real ) l_overlay->GetOgreOverlayElement()->_getDerivedTop() * ScriptEngine::GetContext()->mainViewport->getActualHeight();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetPosY : OverlayElement null" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetPosition )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetPosition" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Vector3, l_vpos );

	if ( p_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetPosV3 : overlay null" );
		return;
	}

	p_overlay->GetOgreOverlayElement()->setPosition( l_vpos.x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), l_vpos.y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetLeft )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetLeft" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, posX );

	if ( p_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetPosV3 : overlay null" );
		return;
	}

	p_overlay->GetOgreOverlayElement()->setLeft( posX / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetTop )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetTop" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, posY );

	if ( p_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetPosV3 : overlay null" );
		return;
	}

	p_overlay->GetOgreOverlayElement()->setTop( posY / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetWidth )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetWidth" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Real ) l_overlay->GetOgreOverlayElement()->getWidth() * ScriptEngine::GetContext()->mainViewport->getActualWidth();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetWidth : OverlayElement null" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetHeight )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetHeight" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( Real ) l_overlay->GetOgreOverlayElement()->getHeight() * ScriptEngine::GetContext()->mainViewport->getActualHeight();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetWidth : OverlayElement null" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetParent )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetParent" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( EMOverlay * ) l_overlay->GetParent();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetWidth : OverlayElement null" );
		RETURN_AS( EMOverlay * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetName )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetName" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( String ) l_overlay->GetName();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetName : OverlayElement null" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetMaterialName )
{
	VERBOSE_STARTFUNC( "OverlayElement_GetMaterialName" );
	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

	if ( l_overlay != nullptr )
	{
		RETURN_AS( String ) l_overlay->GetOgreOverlayElement()->getMaterialName();
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_GetMaterialName : OverlayElement null" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_TextureLookUp )
{
	/*
	VERBOSE_STARTFUNC( "OverlayElement_TextureLookUp" )
	HardwarePixelBufferSharedPtr  texturePixBuff;
	ColourValue *cv;
	ScriptNode *sn_overlay,*sn_x,*sn_y;
	Real x,y;
	OverlayElement *over;
	size_t width,height;
	Image::Box box,getBox;

	GET_AND_EXEC_PARAM(sn_overlay,0);
	GET_AND_EXEC_PARAM(sn_x,0);
	GET_AND_EXEC_PARAM(sn_y,0);

	over = ((OverlayElement*)sn_overlay->m_ref);

	x = sn_x->m_real;
	y = sn_y->m_real;

	cv = new ColourValue;

	texturePixBuff = ((Ogre::TexturePtr)TextureManager::getSingletonPtr()->getByName(over->getTechnique()->getPass(0)->getTextureUnitState(0)->getTextureName()))->getBuffer();

	width = texturePixBuff->getWidth();
	height = texturePixBuff->getHeight();
	box = Box(0, 0, width, height);
	if(x>= 0 && x<1 && y>= 0 && y<1)
	{
	getBox = Box(x*(width-1), y*(height-1), x*(width-1), y*(height-1));

	const PixelBox& pbox = texturePixBuff->lock(box, HardwareBuffer::HBL_DISCARD);
	PixelUtil::unpackColour(cv,pbox.format,pbox.getSubVolume(getBox).data);
	texturePixBuff->unlock();

	}
	delete cv;
	*/
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetAction )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetAction" );
	GET_AND_EXEC_PARAM( String, l_overlayName, 0 );
	ScriptNode * l_sCode = caller->m_childs[1];
	EMOverlay * over = ScriptEngine::GetContext()->gui->GetElementByName( l_overlayName );

	if ( over != nullptr )
	{
		over->SetClickedScript( l_sCode );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_SetAction : OverlayElement " + l_overlayName + " not found" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_LimitToParent )
{
	VERBOSE_STARTFUNC( "OverlayElement_LimitToParent" );
	GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

	if ( p_overlay != nullptr )
	{
		OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();

		if ( l_overlay->getParent() != nullptr )
		{
			l_overlay->setTop( minmax<Real>( 0, l_overlay->getTop(), l_overlay->getParent()->getHeight() - l_overlay->getHeight() ) );
			l_overlay->setLeft( minmax<Real>( 0, l_overlay->getLeft(), l_overlay->getParent()->getWidth() - l_overlay->getWidth() ) );
		}
		else
		{
			l_overlay->setTop( minmax<Real>( 0, l_overlay->getTop(), 1 - l_overlay->getHeight() ) );
			l_overlay->setLeft( minmax<Real>( 0, l_overlay->getLeft(), 1 - l_overlay->getWidth() ) );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_LimitToParent : OverlayElement is null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetFontSize )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetFontSize" );
	GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, p_size );

	if ( p_overlay != nullptr && p_overlay->GetOgreOverlayElement() != nullptr && p_overlay->GetOgreOverlayElement()->getTypeName() == "TextArea" )
	{
		( static_cast<TextAreaOverlayElement *>( p_overlay->GetOgreOverlayElement() ) )->setCharHeight( p_size  / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	}
	else
	{
		SCRIPT_ERROR( "Error @ OverlayElement_LimitToParent : OverlayElement is null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_GetFontSize )
{
	VERBOSE_STARTFUNC( "OverlayElement_SetFontSize" );
	GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

	if ( p_overlay != nullptr && p_overlay->GetOgreOverlayElement() != nullptr && p_overlay->GetOgreOverlayElement()->getTypeName() == "TextArea" )
	{
		RETURN_AS( Real )( static_cast<TextAreaOverlayElement *>( p_overlay->GetOgreOverlayElement() ) )->getCharHeight();
	}
	else
	{
		RETURN_AS( Real ) 0.0;
		SCRIPT_ERROR( "Error @ OverlayElement_LimitToParent : OverlayElement is null" );
	}
}




EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_ScrollUV )
{
	VERBOSE_STARTFUNC( "OvE_ScrollUV" );
	GET_AND_EXEC_FOUR_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_x, Real, p_y );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setTextureScroll( p_x + l_tex->getTextureUScroll(), p_y + l_tex->getTextureVScroll() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetScrollUV )
{
	VERBOSE_STARTFUNC( "OvE_ScrollUV" );
	GET_AND_EXEC_FOUR_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_x, Real, p_y );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setTextureScroll( p_x, p_y );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_RotateUV )
{
	VERBOSE_STARTFUNC( "OvE_RotateUV" );
	GET_AND_EXEC_THREE_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_angle );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_RotateUV : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setTextureRotate( l_tex->getTextureRotate() + Degree( p_angle ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetRotateUV )
{
	VERBOSE_STARTFUNC( "OvE_SetRotateUV" );
	GET_AND_EXEC_THREE_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_angle );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_SetRotateUV : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setTextureRotate( Degree( p_angle ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_ScaleUV )
{
	VERBOSE_STARTFUNC( "OvE_ScaleUV" );
	GET_AND_EXEC_FOUR_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_x, Real, p_y );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScaleUV : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setTextureScale( l_tex->getTextureUScale() * p_x, l_tex->getTextureVScale() * p_y );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetScaleUV )
{
	VERBOSE_STARTFUNC( "OvE_SetScaleUV" );
	GET_AND_EXEC_FOUR_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_x, Real, p_y );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_SetScaleUV : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setTextureScale( p_x, p_y );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_SetAlpha )
{
	VERBOSE_STARTFUNC( "OvE_SetAlpha" );
	GET_AND_EXEC_THREE_PARAM( EMOverlay *, p_overlay, int, p_index, Real, p_alpha );

	if ( p_overlay == nullptr )
	{
		return SCRIPT_ERROR( "Error @ OvE_SetAlpha : OverlayElement is null" );
	}

	Material * l_mat = p_overlay->GetOrCopyMaterial();
	Pass * l_pass = l_mat->getBestTechnique()->getPass( 0 );

	if ( p_index >= l_pass->getNumTextureUnitStates() )
	{
		return SCRIPT_ERROR( "Error @ OvE_ScrollUV : Index out of bounds" );
	}

	TextureUnitState * l_tex = l_pass->getTextureUnitState( uint16_t( p_index ) );
	l_tex->setColourOperationEx( LBX_BLEND_MANUAL, LBS_TEXTURE, LBS_CURRENT, ColourValue::White, ColourValue::White, p_alpha );
//	l_tex->setAlphaOperation( LBX_SOURCE1, LBS_MANUAL, LBS_CURRENT, 1.0, 1.0, p_alpha);
	l_mat->getBestTechnique()->getPass( 0 )->setSceneBlending( SBT_TRANSPARENT_ALPHA );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvE_Clone )
{
	/*
	VERBOSE_STARTFUNC( "OverlayElement_Clone");

	GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0)

	if (l_overlay != nullptr)
	{
		int l_num = 1 ;

		EMOverlay * over, * currentOver;

		currentOver = ScriptEngine::GetContext()->gui->GetElementByName( l_overlay->getName());

		over = ScriptEngine::GetContext()->gui->GetElementByName( l_overlay->getName() + "_" + StringConverter::toString( l_num));

		while( over != nullptr )
		{
			l_num++;
			over = ScriptEngine::GetContext()->gui->GetElementByName( l_overlay->getName() + "_" + StringConverter::toString( l_num));
		}

		OverlayElement * l_overElement = l_overlay->clone( l_overlay->getName() + "_" + StringConverter::toString( l_num));

		over = new EMOverlay( currentOver->GetGroup()->m_overlay, l_overElement, currentOver->GetParent());

		static_cast <OverlayContainer *> (currentOver->GetParent()->GetOverlay())->addChild( l_overElement);

		currentOver->GetGroup()->AddElement( over);

		ScriptEngine::GetContext()->gui->AddElement( over);

		RETURN_AS( OverlayElement *) l_overElement;
	}
	else
	{
		RETURN_AS( OverlayElement *) nullptr;
		SCRIPT_ERROR( "Error @ OverlayElement_Clone : OverlayElement null");
	}
	*/
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_Hide )
{
	VERBOSE_STARTFUNC( "Overlay_Hide" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	EMOverlayGroup * over = ScriptEngine::GetContext()->gui->GetOverlayGroup( l_name );

	if ( over != nullptr )
	{
		over->SetVisible( false );
		over->ShowImmediate( false );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Overlay_Hide : Overlay (" + l_name + ") does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_HideAll )
{
	VERBOSE_STARTFUNC( "Overlay_HideAll" );
	OverlayGroupMap l_overlays = ScriptEngine::GetContext()->gui->GetAllOverlays();
	OverlayGroupMap::iterator l_it = l_overlays.begin();

	while ( l_it != l_overlays.end() )
	{
		l_it->second->SetVisible( false );
		l_it->second->ShowImmediate( false );
		++l_it;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_Show )
{
	VERBOSE_STARTFUNC( "Overlay_Show" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	EMOverlayGroup * over = ScriptEngine::GetContext()->gui->GetOverlayGroup( l_name );

	if ( over != nullptr )
	{
		over->SetVisible( true );
		over->ShowImmediate( true );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Overlay_Show : Overlay (" + l_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_SetVisible )
{
	VERBOSE_STARTFUNC( "Overlay_SetVisible" );
	GET_AND_EXEC_TWO_PARAM( String, l_name, bool, p_visible );
	EMOverlayGroup * over = ScriptEngine::GetContext()->gui->GetOverlayGroup( l_name );

	if ( over != nullptr )
	{
		over->SetVisible( p_visible );
		over->ShowImmediate( p_visible );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Overlay_SetVisible : Overlay (" + l_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_IsVisible )
{
	VERBOSE_STARTFUNC( "Overlay_IsVisible" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	EMOverlayGroup * over = ScriptEngine::GetContext()->gui->GetOverlayGroup( l_name );

	if ( over != nullptr )
	{
		RETURN_AS( bool ) over->IsVisible();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Overlay_IsVisible : Overlay (" + l_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_SetRotate )
{
	VERBOSE_STARTFUNC( "Overlay_SetRotate" );
	GET_AND_EXEC_TWO_PARAM( String, l_name, Real, l_angle );
	EMOverlayGroup * over = ScriptEngine::GetContext()->gui->GetOverlayGroup( l_name );

	if ( over != nullptr )
	{
		over->GetOverlay()->setRotate( Degree( l_angle ) );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Overlay_SetRotate : Overlay (" + l_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( OvL_Rotate )
{
	VERBOSE_STARTFUNC( "Overlay_Rotate" );
	GET_AND_EXEC_TWO_PARAM( String, l_name, Real, l_angle );
	EMOverlayGroup * over = ScriptEngine::GetContext()->gui->GetOverlayGroup( l_name );

	if ( over != nullptr )
	{
		Degree l_dangle = over->GetOverlay()->getRotate() + Degree( l_angle );
		over->GetOverlay()->setScale( 1, Real( 1.33 - 0.33 * Math::Abs( Math::Sin( l_dangle ) ) ) );
		over->GetOverlay()->setRotate( l_dangle );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Overlay_SetRotate : Overlay (" + l_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( CTx_GetByName )
{
	VERBOSE_STARTFUNC( "CamTex_GetByName" );
	GET_AND_EXEC_PARAM( String, l_camTexName, 0 );
	auto l_camTex = ScriptEngine::GetContext()->camTexManager->GetElementByName( l_camTexName );

	if ( !l_camTex )
	{
		SCRIPT_ERROR( "Error @ CamTex_GetByName : CamTex [" + l_camTexName + "] does not exist." );
	}

	RETURN_AS( CamTex * ) l_camTex.get();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( CTx_Refresh )
{
	VERBOSE_STARTFUNC( "CamTex_Refresh" );
	GET_AND_EXEC_PARAM( CamTex *, l_camTex, 0 );

	if ( l_camTex != nullptr )
	{
		l_camTex->Update();
	}
	else
	{
		SCRIPT_ERROR( "Error @ CamTex_Refresh : no camTex." );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( CTx_SetMain )
{
	VERBOSE_STARTFUNC( "CamTex_SetMain" );
	GET_AND_EXEC_PARAM( CamTex *, l_camTex, 0 );

	if ( l_camTex != nullptr )
	{
		Camera * cam = l_camTex->GetCamera();
		Viewport * view = ScriptEngine::GetContext()->mainViewport;
		l_camTex->SetCamera( ScriptEngine::GetContext()->mainCamera );
		view->setCamera( cam );
		ScriptEngine::GetContext()->mainCamera = cam;
		cam->setAspectRatio( Real( view->getActualWidth() ) / Real( view->getActualHeight() ) );
	}
	else
	{
		SCRIPT_ERROR( "Error @ CamTex_SetMain : no camTex." );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( CTx_SetBackgroundColour )
{
	VERBOSE_STARTFUNC( "CTx_SetBackgroundColour" );
	GET_AND_EXEC_TWO_PARAM( CamTex *, l_camTex, ColourValue, l_colour );

	if ( l_camTex != nullptr )
	{
		Camera * cam = l_camTex->GetCamera();
		Viewport * view = cam->getViewport();
		view->setBackgroundColour( l_colour );
	}
	else
	{
		SCRIPT_ERROR( "Error @ CamTex_SetMain : no camTex." );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( CTx_GetCamera )
{
	VERBOSE_STARTFUNC( "CamTex_GetCamera" );
	GET_AND_EXEC_PARAM( CamTex *, l_camTex, 0 );

	if ( l_camTex != nullptr )
	{
		RETURN_AS( Camera * ) l_camTex->GetCamera();
	}
	else
	{
		RETURN_AS( Camera * ) nullptr;
		SCRIPT_ERROR( "Error @ CamTex_GetCamera : no camTex." );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_Add )
{
	VERBOSE_STARTFUNC( "Array_Add" );
	GET_AND_EXEC_PARAM( NodeValueBaseArray, arr, 0 );
	ScriptNode * l_node = caller->m_childs[1];

	if ( l_node->HasFunction() )
	{
		l_node->Execute();
	}

	arr.push_back( l_node->m_value->clone() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_AddDefault )
{
	VERBOSE_STARTFUNC( "Arr_AddDefault" );
	GET_AND_EXEC_PARAM( NodeValueBaseArray, arr, 0 );
	NodeValueBase * l_node = ScriptNode::CreateNodeValue( caller->m_childs[0]->m_type->GetSubType( 0 ) );
	arr.push_back( l_node );
	caller->CopyValue_Ref( l_node );
}


EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_RemoveIndex )
{
	VERBOSE_STARTFUNC( "Array_RemoveIndex" );
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseArray, arr, int, index );

	if ( index >= 0 && static_cast <size_t>( index ) < arr.size() )
	{
		delete arr[index];
		arr.erase( arr.begin() + index );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Array_RemoveIndex : index out of bounds" );
	}
}
EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_Set )
{
	VERBOSE_STARTFUNC( "Arr_Set" );
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseArray, arr, int, l_index );
	ScriptNode * l_param = caller->m_childs[2];

	if ( l_param->HasFunction() )
	{
		l_param->Execute();
	}

	if ( l_index < 0 || static_cast <size_t>( l_index ) >= arr.size() )
	{
		return SCRIPT_ERROR( "Error @ Array_Set : index out of bounds" );
	}

	NodeValueBase * l_value = arr[l_index];

	if ( l_value->m_type != l_param->m_value->m_type )
	{
		return SCRIPT_ERROR( "Error @ Array_Set : type mismatch" );
	}

	l_value->CopyDeepValue( l_param->m_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_GetSize )
{
	VERBOSE_STARTFUNC( "Array_GetSize" );
	GET_AND_EXEC_PARAM( NodeValueBaseArray, arr, 0 );
	RETURN_AS( int ) static_cast< int >( arr.size() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_Clear )
{
	VERBOSE_STARTFUNC( "Array_Clear" );
	GET_AND_EXEC_PARAM( NodeValueBaseArray, arr, 0 );
	General::Utils::vector::deleteAll( arr );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_IsEmpty )
{
	VERBOSE_STARTFUNC( "Array_IsEmpty" );
	GET_AND_EXEC_PARAM( NodeValueBaseArray, arr, 0 );
	RETURN_AS( bool ) arr.empty();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Arr_Get )
{
	VERBOSE_STARTFUNC( "Arr_Get" );
	GET_AND_EXEC_TWO_PARAM( NodeValueBaseArray, arr, int, l_index );

	if ( l_index >= 0 && l_index < int( arr.size() ) )
	{
		caller->CopyValue_Ref( arr[l_index] );
	}
	else
	{
		caller->SetNullRef();
		SCRIPT_ERROR( "Arr_Get : index out of bounds( " + StringConverter::toString( l_index ) + " out of " + StringConverter::toString( arr.size() ) + ")" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Play )
{
	VERBOSE_STARTFUNC( "Sound_Play" );
	GET_AND_EXEC_TWO_PARAM( String, l_soundName, String, l_nodeName );
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( l_soundName );

	if ( l_sound == nullptr )
	{
		SCRIPT_ERROR( "Sound_Play : cannot find sound[ " + l_soundName + "]" );
		return;
	}

	SoundInstance * l_instance = l_sound->GetSoundInstance( l_nodeName );

	if ( l_instance == nullptr )
	{
		SCRIPT_ERROR( "Sound_Play : cannot find instance named [ " + l_soundName + "] for sound " + l_soundName );
		return;
	}

	l_instance->Play();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Play_Global )
{
	VERBOSE_STARTFUNC( "Sound_Play_Global" );
	GET_AND_EXEC_PARAM( String, l_soundName, 0 );
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( l_soundName );

	if ( l_sound == nullptr )
	{
		SCRIPT_ERROR( "Sound_Play_Global : cannot find sound[ " + l_soundName + "]" );
		return;
	}

	if ( ! l_sound->IsGlobal() )
	{
		SCRIPT_ERROR( "Sound_Play_Global : sound[ " + l_soundName + "] is not global" );
		return;
	}

	l_sound->GetGlobal()->Play();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Play_All )
{
	VERBOSE_STARTFUNC( "Sound_Play_All" );
	SoundManager::GetSingletonPtr()->StartAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Pause )
{
	VERBOSE_STARTFUNC( "Sound_Pause" );
	GET_AND_EXEC_TWO_PARAM( String, l_soundName, String, l_nodeName );
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( l_soundName );

	if ( l_sound == nullptr )
	{
		SCRIPT_ERROR( "Sound_Pause : cannot find sound[ " + l_soundName + "]" );
		return;
	}

	SoundInstance * l_instance = l_sound->GetSoundInstance( l_nodeName );

	if ( l_instance == nullptr )
	{
		SCRIPT_ERROR( "Sound_Pause : cannot find instance named [ " + l_soundName + "] for sound " + l_soundName );
		return;
	}

	l_instance->Pause();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Pause_Global )
{
	VERBOSE_STARTFUNC( "Sound_Pause_Global" );
	GET_AND_EXEC_PARAM( String, l_soundName, 0 );
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( l_soundName );

	if ( l_sound == nullptr )
	{
		SCRIPT_ERROR( "Sound_Pause_Global : cannot find sound[ " + l_soundName + "]" );
		return;
	}

	if ( ! l_sound->IsGlobal() )
	{
		SCRIPT_ERROR( "Sound_Pause_Global : sound[ " + l_soundName + "] is not global" );
		return;
	}

	l_sound->GetGlobal()->Pause();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Pause_All )
{
	VERBOSE_STARTFUNC( "Sound_Pause_All" );
	SoundManager::GetSingletonPtr()->PauseAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Stop )
{
	VERBOSE_STARTFUNC( "Sound_Stop" );
	GET_AND_EXEC_TWO_PARAM( String, l_soundName, String, l_nodeName );
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( l_soundName );

	if ( l_sound == nullptr )
	{
		SCRIPT_ERROR( "Sound_Stop : cannot find sound[ " + l_soundName + "]" );
		return;
	}

	SoundInstance * l_instance = l_sound->GetSoundInstance( l_nodeName );

	if ( l_instance == nullptr )
	{
		SCRIPT_ERROR( "Sound_Stop : cannot find instance named [ " + l_soundName + "] for sound " + l_soundName );
		return;
	}

	l_instance->Stop();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Stop_Global )
{
	VERBOSE_STARTFUNC( "Sound_Stop_Global" );
	GET_AND_EXEC_PARAM( String, l_soundName, 0 );
	SoundObject * l_sound = SoundManager::GetSingletonPtr()->GetSoundObject( l_soundName );

	if ( l_sound == nullptr )
	{
		SCRIPT_ERROR( "Sound_Stop_Global : cannot find sound[ " + l_soundName + "]" );
		return;
	}

	if ( ! l_sound->IsGlobal() )
	{
		SCRIPT_ERROR( "Sound_Stop_Global : sound[ " + l_soundName + "] is not global" );
		return;
	}

	l_sound->GetGlobal()->Stop();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_Stop_All )
{
	VERBOSE_STARTFUNC( "Sound_Stop_All" );
	SoundManager::GetSingletonPtr()->StopAll();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_SetVolume )
{
	VERBOSE_STARTFUNC( "Sound_SetVolume" );
	GET_AND_EXEC_TWO_PARAM( String, l_sSoundName, Real, l_sVolume );
	SoundManager::GetSingletonPtr()->SetVolumePercent( l_sSoundName, l_sVolume );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_SetSFXVolume )
{
	VERBOSE_STARTFUNC( "Sound_SetSFXVolume" );
	GET_AND_EXEC_PARAM( Real, l_sVolume, 0 );
	SoundManager::GetSingletonPtr()->SetVolume( l_sVolume, EM_SFX );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_SetMusicVolume )
{
	VERBOSE_STARTFUNC( "Sound_SetMusicVolume" );
	GET_AND_EXEC_PARAM( Real, l_sVolume, 0 );
	SoundManager::GetSingletonPtr()->SetVolume( l_sVolume, EM_MUSIC );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_MuteSFX )
{
	VERBOSE_STARTFUNC( "Sound_MuteSFX" );
	GET_AND_EXEC_PARAM( bool, l_muted, 0 );
	SoundManager::GetSingletonPtr()->Mute( l_muted, EM_SFX );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Snd_MuteMusic )
{
	VERBOSE_STARTFUNC( "Sound_MuteMusic" );
	GET_AND_EXEC_PARAM( bool, l_muted, 0 );
	SoundManager::GetSingletonPtr()->Mute( l_muted, EM_MUSIC );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pll_Start )
{
	VERBOSE_STARTFUNC( "Playlist_Start" );
	GET_AND_EXEC_PARAM( String, l_snName, 0 );
	SoundPlaylist * l_pll = SoundManager::GetSingletonPtr()->GetSoundPlaylist( l_snName );

	if ( l_pll )
	{
		l_pll->Play();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pll_Pause )
{
	VERBOSE_STARTFUNC( "Playlist_Pause" );
	GET_AND_EXEC_PARAM( String, l_snName, 0 );
	SoundPlaylist * l_pll = SoundManager::GetSingletonPtr()->GetSoundPlaylist( l_snName );

	if ( l_pll )
	{
		l_pll->Pause();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pll_Stop )
{
	VERBOSE_STARTFUNC( "Playlist_Stop" );
	GET_AND_EXEC_PARAM( String, l_snName, 0 );
	SoundPlaylist * l_pll = SoundManager::GetSingletonPtr()->GetSoundPlaylist( l_snName );

	if ( l_pll )
	{
		l_pll->Stop();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Por_GetTargetName )
{
	VERBOSE_STARTFUNC( "Portal_GetTargetName" );
	GET_AND_EXEC_PARAM( String, l_portalName, 0 );
	Portal * l_portal = ScriptEngine::GetContext()->scene->GetPortal( l_portalName );

	if ( l_portal != nullptr )
	{
		RETURN_AS( String ) l_portal->GetTargetName();
	}
	else
	{
		SCRIPT_ERROR( "Portal_GetTargetZone : portal [ " + l_portalName + "] doesn't exist" );
		RETURN_AS( String ) "";
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Por_GetTargetZone )
{
	VERBOSE_STARTFUNC( "Portal_GetTargetZone" );
	GET_AND_EXEC_TWO_PARAM( String, l_sceneName, String, l_portalName );
	Zone * l_scene = ScriptEngine::GetContext()->universe->GetZone( l_sceneName );

	if ( l_scene != nullptr )
	{
		Portal * l_portal = l_scene->GetPortal( l_portalName );

		if ( l_portal != nullptr )
		{
			Zone * l_zone = l_portal->GetTargetZone();

			if ( l_zone != nullptr )
			{
				RETURN_AS( String ) l_zone->GetName();
			}
			else
			{
				SCRIPT_ERROR( "Portal_GetTargetZone : portal [" + l_portalName + "] has no target zone" );
				RETURN_AS( String ) "";
			}
		}
		else
		{
			SCRIPT_ERROR( "Portal_GetTargetZone : portal [ " + l_portalName + "] doesn't exist" );
			RETURN_AS( String ) "";
		}
	}
	else
	{
		SCRIPT_ERROR( "Portal_GetTargetZone : scene [ " + l_sceneName + "] doesn't exist" );
		RETURN_AS( String ) "";
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Create )
{
	VERBOSE_STARTFUNC( "Video_Create" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, String, p_url );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->CreateVideoObject( p_name, ScriptEngine::GetContext()->plugin );
	ScriptEngine::GetContext()->scene->AddZoneObject( l_object );
	l_object->SetUrl( p_url );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_CreateInstance )
{
	VERBOSE_STARTFUNC( "Video_CreateInstance" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, Entity *, p_entity );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_object != nullptr && p_entity != nullptr )
	{
		l_object->CreateInstance( p_entity );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Destroy )
{
	VERBOSE_STARTFUNC( "Video_Destroy" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	VideoManager::GetSingletonPtr()->DeleteVideoObject( p_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_DestroyInstance )
{
	VERBOSE_STARTFUNC( "Video_DestroyInstance" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, Entity *, p_entity );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_object != nullptr && p_entity != nullptr )
	{
		l_object->DeleteInstance( p_entity );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Play )
{
	VERBOSE_STARTFUNC( "Video_Play" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_object == nullptr )
	{
		SCRIPT_ERROR( "Error @ Vid_Play : Video " + p_name + " does not exist" );
		return;
	}

	l_object->Start();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Pause )
{
	VERBOSE_STARTFUNC( "Video_Pause" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_object == nullptr )
	{
		SCRIPT_ERROR( "Error @ Vid_Play : Video " + p_name + " does not exist" );
		return;
	}

	l_object->Pause();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Stop )
{
	VERBOSE_STARTFUNC( "Video_Stop" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_object == nullptr )
	{
		SCRIPT_ERROR( "Error @ Vid_Play : Video " + p_name + " does not exist" );
		return;
	}

	l_object->Stop();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_SetVolume )
{
	VERBOSE_STARTFUNC( "Video_SetVolume" );
	GET_AND_EXEC_PARAM( int, l_sVolume, 0 );
	VideoManager::GetSingletonPtr()->SetVolume( l_sVolume );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_SetVolumePercent )
{
	VERBOSE_STARTFUNC( "Video_SetVolumePercent" );
	GET_AND_EXEC_TWO_PARAM( String, p_videoName, Real, l_sVolumePercent );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_videoName );

	if ( l_object == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_SetVolumePercent : Video " + p_videoName + " does not exist" );
		return;
	}

	l_object->SetVolumePercent( l_sVolumePercent );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_SetLooped )
{
	VERBOSE_STARTFUNC( "Video_SetLooped" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, bool, p_looped );
	VideoObject * l_object = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_object != nullptr )
	{
		l_object->SetLooped( p_looped );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Mute )
{
	VERBOSE_STARTFUNC( "Video_Mute" );
	GET_AND_EXEC_PARAM( bool, l_sMuted, 0 );
	VideoManager::GetSingletonPtr()->Mute( l_sMuted );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_ChangeVideo )
{
	VERBOSE_STARTFUNC( "Video_ChangeVideo" );
	GET_AND_EXEC_TWO_PARAM( String, l_sVideoObjectName, Entity *, l_sEnt );

	if ( l_sEnt == nullptr
			|| VideoManager::GetSingletonPtr()->GetVideoObject( l_sVideoObjectName ) == nullptr )
	{
		return;
	}

	l_sEnt->setMaterialName( l_sVideoObjectName );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Initialise )
{
	VERBOSE_STARTFUNC( "Video_Initialise" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	VideoObject * l_obj = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_obj == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_Initialise : video not found : " + p_name );
		return;
	}

	l_obj->SetBasePath( ScriptEngine::GetContext()->baseDirectory );
	l_obj->Initialise();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_Reset )
{
	VERBOSE_STARTFUNC( "Video_Reset" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	VideoObject * l_obj = VideoManager::GetSingletonPtr()->GetVideoObject( p_name );

	if ( l_obj == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_Reset : video not found : " + p_name );
		return;
	}

	l_obj->Reset();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_DelayObject )
{
	VERBOSE_STARTFUNC( "Video_ObjectDelay" );
	GET_AND_EXEC_TWO_PARAM( String, p_objname, Entity *, p_instance );
	VideoObject * l_obj = VideoManager::GetSingletonPtr()->GetVideoObject( p_objname );

	if ( l_obj == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_ObjectDelay : video not found : " + p_objname );
		return;
	}

	VideoInstance * l_instance = l_obj->GetInstance( p_instance );

	if ( l_instance != nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_ObjectDelay : instance not found for video " + p_objname );
		return;
	}

	l_instance->DelayLoad();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_DelayOverlay )
{
	VERBOSE_STARTFUNC( "Video_OverlayDelay" );
	GET_AND_EXEC_TWO_PARAM( String, p_ovName, EMOverlay *, p_overlay );
	VideoObject * l_obj = VideoManager::GetSingletonPtr()->GetVideoObject( p_ovName );

	if ( l_obj == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_OverlayDelay : video not found : " + p_ovName );
		return;
	}

	VideoOverlay * l_overlay = l_obj->GetOverlay( p_overlay );

	if ( l_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_OverlayDelay : overlay not found for video " + p_ovName );
		return;
	}

	l_overlay->DelayLoad();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_InitialiseObject )
{
	VERBOSE_STARTFUNC( "Video_ObjectInitialise" );
	GET_AND_EXEC_TWO_PARAM( String, p_objname, Entity *, p_instance );
	VideoObject * l_obj = VideoManager::GetSingletonPtr()->GetVideoObject( p_objname );

	if ( l_obj == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_ObjectInitialise : video not found : " + p_objname );
		return;
	}

	VideoInstance * l_instance = l_obj->GetInstance( p_instance );

	if ( l_instance != nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_ObjectInitialise : instance not found for video " + p_objname );
		return;
	}

	l_instance->Initialise( p_objname );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Vid_InitialiseOverlay )
{
	VERBOSE_STARTFUNC( "Video_OverlayInitialise" );
	GET_AND_EXEC_TWO_PARAM( String, p_ovName, EMOverlay *, p_overlay );
	VideoObject * l_obj = VideoManager::GetSingletonPtr()->GetVideoObject( p_ovName );

	if ( l_obj == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_OverlayInitialise : video not found : " + p_ovName );
		return;
	}

	VideoOverlay * l_overlay = l_obj->GetOverlay( p_overlay );

	if ( l_overlay == nullptr )
	{
		SCRIPT_ERROR( "Error @ Video_OverlayInitialise : overlay not found for video " + p_ovName );
		return;
	}

	l_overlay->Initialise( p_ovName );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mus_IsDownloaded )
{
	VERBOSE_STARTFUNC( "Muse_IsDownloaded" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( !p_name.empty() )
	{
		MuseFile * l_file = ScriptEngine::GetContext()->downloadManager->GetMuseFile( p_name );

		if ( l_file == nullptr )
		{
			RETURN_AS( bool ) false;
		}
		else
		{
			RETURN_AS( bool ) l_file->GetDownloader().IsDownloaded();
		}
	}
	else
	{
		RETURN_AS( bool ) false;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mus_StartDownload )
{
	VERBOSE_STARTFUNC( "Muse_StartDownloading" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	ScriptNode * l_sCode = caller->m_childs[1];

	if ( !p_name.empty() )
	{
		MuseFile * l_file = ScriptEngine::GetContext()->downloadManager->GetMuseFile( p_name );

		if ( l_file == nullptr )
		{
			return SCRIPT_ERROR( "Muse_StartDownloading : muse file named [" + p_name + "] does not exist" );
		}

		l_file->GetDownloader().StartDownload();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mus_DownloadSeparable )
{
	VERBOSE_STARTFUNC( "Muse_DownloadSeparate" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	ScriptNode * l_sCode = caller->m_childs[1];

	if ( !p_name.empty() )
	{
		auto l_context = ScriptEngine::GetContext();
		Url l_dir{ l_context->baseURL.GetProtocol() };
		l_dir /= l_context->baseURL.GetBaseSite();
		l_dir /= l_context->baseURL.GetDirectory();
		MuseFile * l_file = l_context->downloadManager->StartDownloadFile( l_dir / p_name, false );

		if ( l_file == nullptr )
		{
			return SCRIPT_ERROR( "Muse_DownloadSeparate : muse file named [" + p_name + "] was not found" );
		}

		
		l_file->GetDownloader().SetDownloadFiles( true );
		l_file->GetDownloader().StartDownload( [l_sCode]( MuseFile * museFile )
			{
				auto l_emcfg = museFile->GetConfigFile( "main.emcfg" );

				if ( l_emcfg )
				{
					ScriptEngine::GetContext()->sceneFileParser->ParseScript( l_emcfg );
				}

				l_sCode->Execute();
			} );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mus_GetDirectory )
{
	VERBOSE_STARTFUNC( "Muse_GetDirectory" );
	RETURN_AS( String ) ScriptEngine::GetContext()->baseURL.GetDirectory();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_ObjectDuplicate )
{
	VERBOSE_STARTFUNC( "Special_ObjectDuplicate" );
	int num;
	GET_AND_EXEC_PARAM( Entity *, olde, 0 );

	if ( olde != nullptr )
	{
		SceneNode * olds = olde->getParentSceneNode();
		String name = olds->getName();

		if ( name[name.size() - 2] == '_' )
		{
			num = name[name.size() - 1] - 48;
			name.erase( name.size() - 1 );
			name.erase( name.size() - 1 );
		}
		else
		{
			num = 1;
		}

		while ( ScriptEngine::GetContext()->sceneManager->hasEntity( name + String( "_" ) + StringConverter::toString( num ) ) )
		{
			num ++;
		}

		name += String( "_" ) + StringConverter::toString( num );
		SceneNode * news = olds->getParentSceneNode()->createChildSceneNode( name, olds->getPosition(), olds->getOrientation() );
		news->setInheritOrientation( false );
		news->setInheritScale( false );
		Entity * newe = olde->clone( name );
		news->setScale( olds->getScale() );
		news->attachObject( newe );
		RETURN_AS( Entity * ) newe;
	}
	else
	{
		SCRIPT_ERROR( "Error @ Special_ObjectDuplicate : Object is null" );
		RETURN_AS( Entity * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_ObjectDelete )
{
	VERBOSE_STARTFUNC( "Special_ObjectDelete" );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity != nullptr )
	{
		SceneNode * pnode = l_entity->getParentSceneNode();

		if ( pnode != nullptr )
		{
			pnode->detachObject( l_entity );

			if ( pnode->numAttachedObjects() == 0 && pnode->numChildren() == 0 )
			{
				ScriptEngine::GetContext()->sceneManager->destroySceneNode( pnode->getName() );
			}
		}

		if ( ! l_entity->getUserObjectBindings().getUserAny().has_value() )
		{
			ScriptEngine::GetContext()->physicsSimulation->DestroyObject( any_cast<PhysicsObject *>( l_entity->getUserObjectBindings().getUserAny() ) );
		}

		ScriptEngine::GetContext()->sceneManager->destroyEntity( l_entity );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Special_ObjectDelete : Entity is null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_ImportMesh )
{
#if 0
	//TODOAFX
	VERBOSE_STARTFUNC( "Special_ImportMesh" );
	String l_meshName;
	String l_path;
	String l_entName;
	String l_dir;
	SceneNode * l_node;
	Entity * l_entity;
	int l_objNum;

	if ( ScriptEngine::GetContext()->lastDataRep.empty() )
	{
		l_dir = ScriptEngine::GetContext()->sceneFileParser->m_context->baseDirectory;
	}
	else
	{
		l_dir = ScriptEngine::GetContext()->lastDataRep;
	}

#	if ELYPSE_WINDOWS
	CFileDialog FileOpenDialog( TRUE, nullptr, nullptr,
								OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
								"Mesh Files (*.mesh)|*.mesh|", nullptr );
	FileOpenDialog.m_ofn.lpstrInitialDir = l_dir.c_str();//"C:\\Program Files\\DragonJoker\\Elypse\\rsc\\";

	if ( FileOpenDialog.DoModal() == IDOK )
	{
		POSITION pos = FileOpenDialog.GetStartPosition();

		while ( pos )
		{
			l_path = FileOpenDialog.GetNextPathName( pos );
			l_meshName = l_path.substr( l_path.find_last_of( "\\" ) + 1, l_path.length() );
			ScriptEngine::GetContext()->lastDataRep = l_path.substr( 0, l_path.find_last_of( "\\" ) );
			l_entName = l_meshName.substr( 0, l_meshName.length() - 5 );
			l_objNum = 1;

			if ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_entName ) )
			{
				while ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_entName + String( "_" ) + StringConverter::toString( l_objNum ) ) )
				{
					l_objNum ++;
				}

				l_entName = l_entName + String( "_" ) + StringConverter::toString( l_objNum );
			}

			l_entity = ScriptEngine::GetContext()->sceneManager->createEntity( l_entName, l_meshName );
			l_node = ScriptEngine::GetContext()->sceneManager->getRootSceneNode()->createChildSceneNode( l_entName );
			l_node->attachObject( l_entity );
			RETURN_AS( Entity * ) l_entity;
		}
	}
	else
	{
		RETURN_AS( Entity * ) nullptr;
	}

#	elif ELYPSE_LINUX
	static bool sl_gtk_initialised = false;

	if ( ! sl_gtk_initialised )
	{
		sl_gtk_initialised = true;
		gtk_init( nullptr, nullptr );
	}

	GtkFileFilter * l_filter = gtk_file_filter_new();
	GtkFileChooser * l_dialog;
	l_dialog = reinterpret_cast <GtkFileChooser *>(
				   gtk_file_chooser_dialog_new( "Open Mesh File(s)", nullptr,
						   GTK_FILE_CHOOSER_ACTION_OPEN,
						   GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						   GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
						   nullptr ) );
	gtk_file_chooser_set_current_folder( l_dialog, l_dir.c_str() );
	gtk_file_chooser_set_select_multiple( l_dialog, TRUE );
	gtk_file_filter_add_pattern( l_filter, "*.mesh" );
	gtk_file_filter_set_name( l_filter, "Mesh Files (*.mesh)" );
	gtk_file_chooser_set_filter( l_dialog, l_filter );

	if ( gtk_dialog_run( GTK_DIALOG( l_dialog ) ) == GTK_RESPONSE_ACCEPT )
	{
		char * l_fileName;
		GSList * l_fileList;
		GSList * l_element;
		l_fileList = gtk_file_chooser_get_filenames( l_dialog );
		l_element = l_fileList;

		while ( l_element != nullptr )
		{
			l_fileName = reinterpret_cast <char *>( l_element->data );

			if ( FileExists( l_fileName ) )
			{
				l_path = String( l_fileName );
				l_meshName = l_path.substr( l_path.find_last_of( "/" ) + 1, l_path.length() );
				ScriptEngine::GetContext()->lastDataRep = l_path.substr( 0, l_path.find_last_of( "/" ) );
				l_entName = l_meshName.substr( 0, l_meshName.length() - 5 );
				l_objNum = 1;

				if ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_entName ) )
				{
					while ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_entName + String( "_" ) + StringConverter::toString( l_objNum ) ) )
					{
						l_objNum ++;
					}

					l_entName = l_entName + String( "_" ) + StringConverter::toString( l_objNum );
				}

				l_entity = ScriptEngine::GetContext()->sceneManager->createEntity( l_entName, l_meshName );
				l_node = ScriptEngine::GetContext()->sceneManager->getRootSceneNode()->createChildSceneNode( l_entName );
				l_node->attachObject( l_entity );
				RETURN_AS( Entity * ) l_entity;
			}

			l_element = g_slist_next( l_element );
		}

		g_slist_free( l_fileList );
	}
	else
	{
		RETURN_AS( Entity * ) nullptr;
	}

	gtk_widget_destroy( reinterpret_cast <GtkWidget *>( l_dialog ) );
#	elif ELYPSE_MACOS
//TODO : ?
#	endif
#endif
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_GetGroundCoords )
{
	VERBOSE_STARTFUNC( "Special_GetGroundCoords" );
	Vector3 v;
	Ray ray;
	GET_AND_EXEC_TWO_PARAM( Real, x, Real, y );
	Plane ground( Vector3::UNIT_Y, 0 );
	ray = ScriptEngine::GetContext()->mainCamera->getCameraToViewportRay( x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	RayResult p;
	p = ray.intersects( ground );

	if ( p.first )
	{
		v = ray.getPoint( p.second );
		RETURN_AS( Vector3 ) v;
	}
	else
	{
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}
/*
bool Local_ScriptNode_Find( ScriptNode2 * p_code, ScriptNode2 * p_find)
{
	bool ret = false;
	size_t i;
	for(i = 0;i<p_code->m_numChilds;i++)
	{
		if( p_code->m_childs[i] == p_find )
		{
			return true;
		}
		else
		{
			if( p_code->m_childs[i]->m_numChilds > 0 )
			{
				if(Local_ScriptNode_Find(p_code->m_childs[i],p_find))
				{
					ret = true;
				}
			}
		}
	}
	return ret;
}
*/

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_ClosestObject )
{
	VERBOSE_STARTFUNC( "Special_ClosestObject" );
	GET_AND_EXEC_PARAM( Vector3, l_center, 0 );
	SceneManager * l_manager = ScriptEngine::GetContext()->mainCamera->getSceneManager();
	SceneManager::MovableObjectIterator l_iterator = l_manager->getMovableObjectIterator( "Entity" );
	Real l_minDist = Math::POS_INFINITY;
	Entity * l_minObject = nullptr;
	MovableObject * l_tempObject;

	while ( l_iterator.hasMoreElements() )
	{
		l_tempObject = l_iterator.getNext();

		if ( l_tempObject->isVisible() )
		{
			Real l_squaredDistance = l_tempObject->getParentSceneNode()->_getDerivedPosition().squaredDistance( l_center );

			if ( l_squaredDistance < l_minDist )
			{
				l_minDist = l_squaredDistance;
				l_minObject = static_cast <Entity *>( l_tempObject );
			}
		}
	}

	RETURN_AS( Entity * ) l_minObject;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_MousePickupObject )
{
	VERBOSE_STARTFUNC( "Special_MousePickupObject" );
	bool found = false;
	GET_AND_EXEC_THREE_PARAM( Real, x, Real, y, bool, l_useBoundingMeshes );
	RaySceneQuery * l_query;
	SceneManager * l_manager = ScriptEngine::GetContext()->mainCamera->getSceneManager();
	Ray mouseRay = ScriptEngine::GetContext()->mainCamera->getCameraToViewportRay( x / ScriptEngine::GetContext()->mainViewport->getActualWidth() , y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
	l_query = l_manager->createRayQuery( mouseRay );
	l_query->setSortByDistance( true );
	RaySceneQueryResult & result = l_query->execute();
	RaySceneQueryResult::iterator itr = result.begin();
	PhysicsObject * l_ray = ScriptEngine::GetContext()->physicsSimulation->GetRay( mouseRay );

	if ( itr == result.end() )
	{
		found = true;
	}

	while ( ! found )
	{
		if ( itr == result.end() )
		{
			break;
		}

		if ( itr->movable == nullptr
				|| ( ! itr->movable->isVisible() )
				|| ( ! itr->movable->isAttached() )
				|| ( itr->movable->getParentSceneNode() == l_manager->getRootSceneNode() ) )
		{
			++ itr;
			continue;
		}

		if ( ! l_useBoundingMeshes )
		{
			break;
		}

		Entity * l_entity = static_cast<Entity *>( itr->movable );
		PhysicsObject * l_pobj = any_cast<PhysicsObject *>( l_entity->getUserObjectBindings().getUserAny() );

		if ( l_pobj == nullptr )
		{
			l_pobj = ScriptEngine::GetContext()->physicsSimulation->GetObjectByName( l_entity->getName() );

			if ( l_pobj == nullptr )
			{
				l_pobj = ScriptEngine::GetContext()->physicsSimulation->CreateObject( l_entity, true, true );
				l_pobj->AddBound( new BoundingMesh( l_entity->getMesh().get() ) );
				l_pobj->ReassertSpatialState();
				l_pobj->SetSpace( ScriptEngine::GetContext()->physicsSimulation->GetPhantomSpace() );
				l_pobj->SetFloating( true );
				l_pobj->SetStatic( true );
				l_pobj->SetPhantom( true );
			}
			else
			{
				l_entity->getUserObjectBindings().setUserAny( Ogre::Any( l_pobj ) );
			}
		}

		l_pobj->ReassertSpatialState();

		if ( l_pobj->CollisionTest_Complete( l_ray ) )
		{
			break;
		}

		++ itr;
	}

	if ( itr != result.end() )
	{
		RETURN_AS( Entity * ) static_cast<Entity *>( itr->movable );
	}
	else
	{
		RETURN_AS( Entity * ) nullptr;
	}

	l_manager->destroyQuery( l_query );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_SaveScript )
{
	VERBOSE_STARTFUNC( "Special_SaveScript" );
	SceneFileSaver * SFS;
	String l_file;

	if ( ScriptEngine::GetContext()->lastSceneFile.empty() )
	{
		GET_AND_EXEC_PARAM( String, l_filename, 0 );
		l_file = l_filename;
	}
	else
	{
		l_file = ScriptEngine::GetContext()->lastSceneFile;
	}

	SFS = new SceneFileSaver( ScriptEngine::GetContext()->sceneManager , ScriptEngine::GetContext()->scriptEngine , ScriptEngine::GetContext()->gui );
	SFS->SaveAll( l_file );
	delete SFS;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_StringFromUrl )
{
	VERBOSE_STARTFUNC( "Special_StringFromUrl" );
	GET_AND_EXEC_TWO_PARAM( String, l_url, String, l_postParams );

	try
	{
		RETURN_AS( String ) ScriptEngine::GetContext()->plugin->GetStringFromUrl( l_url, l_postParams );
	}
	catch ( ... )
	{
		SCRIPT_ERROR( "Error @ Special_StringFromUrl : Exception occured when trying to reach url : " + l_url );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_ThreadedStringFromUrl )
{
	VERBOSE_STARTFUNC( "Special_ThreadedStringFromUrl" );
	GET_AND_EXEC_TWO_PARAM( String, l_url, String , l_postParams );

	try
	{
		ScriptNode * l_stringResult = caller->m_childs[2];
		ScriptNode * l_endCode = caller->m_childs[3];
		ScriptEngine::GetContext()->plugin->ThreadedStringFromUrl( l_url, l_postParams, l_stringResult, l_endCode );
	}
	catch ( ... )
	{
		SCRIPT_ERROR( "Error @ Special_StringFromUrl : Exception occured when trying to reach url : " + ( l_url ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_SetSessionCookie )
{
	VERBOSE_STARTFUNC( "Special_ThreadedStringFromUrl" );
	GET_AND_EXEC_PARAM( String, p_sessionCookieString, 0 );
	ScriptEngine::GetContext()->plugin->SetSessionCookie( p_sessionCookieString );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_GetWebFileType )
{
	VERBOSE_STARTFUNC( "Special_GetWebFileType" );
	GET_AND_EXEC_PARAM( String, p_url, 0 );
	CURLManager l_manager;
	l_manager.Initialise();
	String const & l_type = l_manager.GetContentType( p_url );

	if ( l_type.empty() )
	{
		RETURN_AS( String ) "none";
		return;
	}

	if ( l_type.find( "image" ) != String::npos )
	{
		RETURN_AS( String ) "image";
	}
	else if ( l_type.find( "video" ) != String::npos )
	{
		RETURN_AS( String ) "video";
	}
	else if ( l_type.find( "text" ) != String::npos )
	{
		RETURN_AS( String ) "text";
	}
	else if ( l_type.find( "application" ) != String::npos )
	{
		RETURN_AS( String ) "application";
	}
	else
	{
		RETURN_AS( String ) "unknown";
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_OpenUrl )
{
	VERBOSE_STARTFUNC( "Special_OpenUrl" );
	GET_AND_EXEC_TWO_PARAM( String, l_url , bool, l_method );

	try
	{
		ScriptEngine::GetContext()->plugin->OpenURL( l_url, false, l_method );
		RETURN_AS( bool ) true;
	}
	catch ( ... )
	{
		SCRIPT_ERROR( "Error @ Special_OpenUrl : Exception occured when trying to reach url : " + l_url );
		RETURN_AS( bool ) false;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_ExecuteJavaScript )
{
	VERBOSE_STARTFUNC( "Special_OpenUrl" );
	GET_AND_EXEC_PARAM( String, p_functionName , 0 );

	try
	{
		RETURN_AS( bool ) ScriptEngine::GetContext()->plugin->ExecuteJavascript( p_functionName );
	}
	catch ( ... )
	{
		SCRIPT_ERROR( "Error @ Spc_ExecuteJavaScript : Exception occured when trying to execute code : " + p_functionName );
		RETURN_AS( bool ) false;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_Quit )
{
	VERBOSE_STARTFUNC( "Special_Quit" );
	ScriptEngine::GetContext()->plugin->Quit();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_GetCharWidth )
{
	GET_AND_EXEC_THREE_PARAM( char, p_char, String, p_fontName, Real, p_fontSize );
	FontPtr l_ptr = FontManager::getSingletonPtr()->getByName( p_fontName );
	RETURN_AS( Real ) ScriptEngine::GetContext()->gui->GetWidth( p_char, l_ptr, p_fontSize );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_GetLineWidth )
{
	GET_AND_EXEC_THREE_PARAM( String, p_line, String, p_fontName, Real, p_fontSize );
	FontPtr l_ptr = FontManager::getSingletonPtr()->getByName( p_fontName );
	RETURN_AS( Real ) ScriptEngine::GetContext()->gui->GetWidth( p_line, l_ptr, p_fontSize );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_GetCurrentDirectory )
{
	RETURN_AS( String ) ScriptEngine::GetContext()->emuseInstance->GetCurrentDirectory();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_TestAllEntities )
{
	VERBOSE_STARTFUNC( "Special_TestAllEntities" );
	StringVector groupNamesList = ResourceGroupManager::getSingletonPtr()->getResourceGroups();
	StringVector::const_iterator itGroup = groupNamesList.begin();

	while ( itGroup != groupNamesList.end() )
	{
		ResourceGroupManager::getSingletonPtr()->loadResourceGroup( *itGroup );
		StringVectorPtr resourceNames = ResourceGroupManager::getSingletonPtr()->findResourceNames( *itGroup, "*.mesh" );
		StringVector::const_iterator itResourceName = resourceNames->begin();

		while ( itResourceName != resourceNames->end() )
		{
			Entity * l_entity = ScriptEngine::GetContext()->sceneManager->createEntity( "mesh [ " + ( *itResourceName ) + " ]", ( *itResourceName ) );
			auto & l_subEntities = l_entity->getSubEntities();

			for ( auto & l_sub : l_subEntities )
			{
				if ( l_sub->getMaterialName().find( "_-_Default" ) != String::npos )
				{
					EMUSE_LOG_MESSAGE_RELEASE( "ERREUR_DATA : Nom de material foireux : " + l_sub->getMaterialName() + " dans " + ( *itResourceName ) );
				}
			}

			ScriptEngine::GetContext()->sceneManager->destroyEntity( l_entity );
			MeshManager::getSingletonPtr()->unload( ( *itResourceName ) );
			++itResourceName;
		}

		ResourceGroupManager::getSingletonPtr()->unloadUnreferencedResourcesInGroup( *itGroup );
//		ResourceGroupManager::getSingletonPtr()->unloadResourceGroup(*itGroup);
		++itGroup;
	}

	ResourceManager::ResourceMapIterator l_iter2 = MaterialManager::getSingletonPtr()->getResourceIterator();

	while ( l_iter2.hasMoreElements() )
	{
		MaterialPtr l_ptr = l_iter2.getNext().staticCast< Material >();
		l_ptr->load();
		l_ptr->unload();
//		Entity * l_entity = ScriptEngine::GetContext()->sceneManager->createEntity( "testEntity",l_ptr->getName());
//		ScriptEngine::GetContext()->sceneManager->destroyEntity( l_entity);
	}

	itGroup = groupNamesList.begin();

	while ( itGroup != groupNamesList.end() )
	{
		ResourceGroupManager::getSingletonPtr()->unloadUnreferencedResourcesInGroup( *itGroup );
		++itGroup;
	}

	ResourceManager::ResourceMapIterator l_iter3 = TextureManager::getSingletonPtr()->getResourceIterator();

	while ( l_iter3.hasMoreElements() )
	{
		Ogre::TexturePtr l_ptr = l_iter3.getNext().staticCast< Ogre::Texture >();

		if ( l_ptr->getHeight() != NextPowOfTwo( static_cast<uint32_t>( l_ptr->getHeight() ) ) || l_ptr->getWidth() != NextPowOfTwo( static_cast<uint32_t>( l_ptr->getWidth() ) ) )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "ERREUR_DATA : Texture non puissance de 2 : " + l_ptr->getName() + " : " + StringConverter::toString( l_ptr->getHeight() ) + " x " + StringConverter::toString( l_ptr->getWidth() ) );
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_GetMD5Hash )
{
	VERBOSE_STARTFUNC( "GetMD5Hash" );
	GET_AND_EXEC_PARAM( String, p_string, 0 );
	RETURN_AS( String ) MD5::Hash( p_string );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Spc_SetCursor )
{
	VERBOSE_STARTFUNC( "SetCursor" );
	GET_AND_EXEC_PARAM( int, p_cursor, 0 );
	ScriptEngine::GetContext()->plugin->ChangeCursorTo( static_cast <CursorType>( p_cursor ) );

	if ( p_cursor >= TextCursor && p_cursor <= ArrowCursor )
	{
		ScriptEngine::GetContext()->plugin->SetBaseCursor( static_cast <CursorType>( p_cursor ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Rand )
{
	VERBOSE_STARTFUNC( "Math_Rand" );
	GET_AND_EXEC_TWO_PARAM( Real, p_min, Real, p_max );
	RETURN_AS( Real ) Math::RangeRandom( p_min, p_max );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Abs )
{
	VERBOSE_STARTFUNC( "Math_Abs" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Abs( p_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Floor )
{
	VERBOSE_STARTFUNC( "Math_Floor" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Floor( p_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Ceil )
{
	VERBOSE_STARTFUNC( "Math_Ceil" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Ceil( p_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Round )
{
	VERBOSE_STARTFUNC( "Math_Round" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Ceil( p_value + 0.5f );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Sin )
{
	VERBOSE_STARTFUNC( "Mth_Sin" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Sin( Degree( p_value ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Cos )
{
	VERBOSE_STARTFUNC( "Mth_Cos" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Cos( Degree( p_value ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Tan )
{
	VERBOSE_STARTFUNC( "Mth_Tan" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Tan( Degree( p_value ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_ASin )
{
	VERBOSE_STARTFUNC( "Mth_ASin" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::ASin( p_value ).valueDegrees();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_ACos )
{
	VERBOSE_STARTFUNC( "Mth_ACos" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::ACos( p_value ).valueDegrees();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_ATan )
{
	VERBOSE_STARTFUNC( "Mth_ATan" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::ATan( p_value ).valueDegrees();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Sqr )
{
	VERBOSE_STARTFUNC( "Mth_Sqr" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Sqr( p_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Sqrt )
{
	VERBOSE_STARTFUNC( "Mth_Sqrt" );
	GET_AND_EXEC_PARAM( Real, p_value, 0 );
	RETURN_AS( Real ) Math::Sqrt( p_value );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Min )
{
	VERBOSE_STARTFUNC( "Mth_Min" );
	GET_AND_EXEC_TWO_PARAM( Real, p_a, Real, p_b );
	RETURN_AS( Real ) min( p_a, p_b );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Max )
{
	VERBOSE_STARTFUNC( "Mth_Max" );
	GET_AND_EXEC_TWO_PARAM( Real, p_a, Real, p_b );
	RETURN_AS( Real ) max( p_a, p_b );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mth_Minmax )
{
	VERBOSE_STARTFUNC( "Mth_Minmax" );
	GET_AND_EXEC_THREE_PARAM( Real, p_min, Real, p_value, Real, p_max );
	RETURN_AS( Real ) minmax( p_min, p_value, p_max );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tim_GetCurrentSecond )
{
	VERBOSE_STARTFUNC( "Time_GetCurrentSecond" );
	tm * ti;
	time_t abstime;
	time( & abstime );
	ti = localtime( & abstime );
	RETURN_AS( int ) ti->tm_sec;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tim_GetCurrentMinute )
{
	VERBOSE_STARTFUNC( "Time_GetCurrentMinute" );
	tm * ti;
	time_t abstime;
	time( & abstime );
	ti = localtime( & abstime );
	RETURN_AS( int ) ti->tm_min;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Tim_GetCurrentHour )
{
	VERBOSE_STARTFUNC( "Time_GetCurrentHour" );
	tm * ti;
	time_t abstime;
	time( & abstime );
	ti = localtime( & abstime );
	RETURN_AS( int ) ti->tm_hour;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mir_SetAlpha )
{
	VERBOSE_STARTFUNC( "Mirror_SetAlpha" );
	GET_AND_EXEC_THREE_PARAM( String, l_sMirrorName, String, l_sMaterialName, Real, l_sNewAlpha );
	auto l_mirror = ScriptEngine::GetContext()->mirrorManager->GetElementByName( l_sMirrorName );

	if ( l_mirror )
	{
		l_mirror->ChangeMaterialAlpha( l_sMaterialName, l_sNewAlpha );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Mirror_SetAlpha : Mirror " + l_sMirrorName + " does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mir_Update )
{
	VERBOSE_STARTFUNC( "Mirror_Update" );
	GET_AND_EXEC_PARAM( String, l_sMirrorName, 0 );
	auto l_mirror = ScriptEngine::GetContext()->mirrorManager->GetElementByName( l_sMirrorName );

	if ( l_mirror )
	{
		l_mirror->RequireUpdate();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Mirror_Update : Mirror " + l_sMirrorName + " does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PEf_Enable )
{
	VERBOSE_STARTFUNC( "PEf_Enable" );
	GET_AND_EXEC_TWO_PARAM( String, l_sPostEffectName, bool, l_sEnable );
	auto l_postEffect = ScriptEngine::GetContext()->postEffectManager->GetElementByName( l_sPostEffectName );

	if ( l_postEffect )
	{
		l_postEffect->SetEnabled( l_sEnable );
	}
	else
	{
		SCRIPT_ERROR( "Error @ PEf_Enable : Effect " + l_sPostEffectName + " does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sce_Load )
{
	VERBOSE_STARTFUNC( "Scene_Load" );
	GET_AND_EXEC_PARAM( String, l_sSceneName, 0 );
	auto l_context = ScriptEngine::GetContext();
	Zone * l_zone = l_context->universe->GetZone( l_sSceneName );

	if ( l_zone != nullptr )
	{
		l_context->scriptEngine->LoadZone( l_zone );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Scene_Load : Scene [" + l_sSceneName + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sce_Unload )
{
	VERBOSE_STARTFUNC( "Scene_Unload" );
	GET_AND_EXEC_PARAM( String, l_sSceneName, 0 );
	Zone * l_zone = ScriptEngine::GetContext()->universe->GetZone( l_sSceneName );

	if ( l_zone != nullptr )
	{
		ScriptEngine::GetContext()->scriptEngine->UnloadZone( l_zone );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Scene_Load : Scene [" + l_sSceneName + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Sce_Shadows )
{
	VERBOSE_STARTFUNC( "Scene_Unload" );
	GET_AND_EXEC_PARAM( String, l_shadowsType, 0 );

	if ( l_shadowsType == "texture" )
	{
		ScriptEngine::GetContext()->sceneManager->setShadowTechnique( SHADOWTYPE_TEXTURE_ADDITIVE );
	}
	else if ( l_shadowsType == "stencil" )
	{
		ScriptEngine::GetContext()->sceneManager->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );
	}
	else if ( l_shadowsType == "none" )
	{
		ScriptEngine::GetContext()->sceneManager->setShadowTechnique( SHADOWTYPE_NONE );
	}
	else
	{
		ScriptEngine::GetContext()->sceneManager->setShadowTechnique( SHADOWTYPE_NONE );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Substr )
{
	VERBOSE_STARTFUNC( "String_Substr" );
	GET_AND_EXEC_THREE_PARAM( String, l_string, int, l_start, int, l_len );

	if ( l_start >= 0 && l_start < static_cast< int >( l_string.length() ) )
	{
		RETURN_AS( String ) l_string.substr( static_cast <size_t>( l_start ), static_cast<size_t>( l_len ) );
	}
	else
	{
		SCRIPT_ERROR( "Error @ String_Substr : index out of bounts : line " + StringConverter::toString( l_start ) + " , on string : [" + l_string + "]" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Strlen )
{
	VERBOSE_STARTFUNC( "String_Strlen" );
	GET_AND_EXEC_PARAM( String, l_string, 0 );
	RETURN_AS( int ) static_cast<int>( l_string.length() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Contains )
{
	VERBOSE_STARTFUNC( "String_Contains" );
	GET_AND_EXEC_TWO_PARAM( String, l_string, String, l_search );
	RETURN_AS( bool )( l_string.find( l_search ) != String::npos );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Find )
{
	VERBOSE_STARTFUNC( "String_Find" );
	GET_AND_EXEC_TWO_PARAM( String, l_string, String, l_search );
	size_t l_fpos = l_string.find( l_search );

	if ( l_fpos == String::npos )
	{
		RETURN_AS( int ) - 1;
	}
	else
	{
		RETURN_AS( int ) static_cast<int>( l_fpos );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Split )
{
	VERBOSE_STARTFUNC( "String_Split" );
	GET_AND_EXEC_THREE_PARAM( String, l_string, String, l_splitString, NodeValueBaseArray, p_arr );
	General::Utils::vector::deleteAll( p_arr );
	const StringVector & l_stringArray = StringUtil::split( l_string , l_splitString , 0 );

	for ( auto & l_str : l_stringArray )
	{
		NodeValue<String> * l_string = new NodeValue<String>();
		l_string->m_value = l_str;
		p_arr.push_back( l_string );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_SplitOne )
{
	VERBOSE_STARTFUNC( "String_SplitOne" );
	GET_AND_EXEC_TWO_PARAM( String, l_string , String, l_splitString );
	const StringVector & l_array = StringUtil::split( l_string, l_splitString, 1 );

	if ( l_array.size() == 2 )
	{
		l_string = l_array[1];
		RETURN_AS( String ) l_array[0];
	}
	else if ( l_array.size() == 1 )
	{
		l_string = EMPTY_STRING;
		RETURN_AS( String ) l_array[0];
	}
	else
	{
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Trim )
{
	VERBOSE_STARTFUNC( "String_Trim" );
	GET_AND_EXEC_PARAM( String, l_string, 0 );
	StringUtil::trim( l_string, true, true );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_GetChar )
{
	VERBOSE_STARTFUNC( "String_GetChar" );
	GET_AND_EXEC_TWO_PARAM( String, l_string, int, l_sIndex );
	uint32_t l_index = static_cast <uint32_t>( l_sIndex );

	if ( l_string.length() > l_index )
	{
		RETURN_AS( char ) l_string[l_index];
	}
	else
	{
		SCRIPT_ERROR( "Error @ String_GetChar : index out of bounds in string(" + l_string + "), index " + StringConverter::toString( l_index ) );
		RETURN_AS( char ) '\0';
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_SetChar )
{
	VERBOSE_STARTFUNC( "Str_SetChar" );
	GET_AND_EXEC_THREE_PARAM( String, l_string, int, l_sIndex, char, l_sChar );
	uint32_t l_index = static_cast <uint32_t>( l_sIndex );

	if ( l_string.length() > l_index )
	{
		l_string[l_index] = l_sChar;
	}
	else
	{
		SCRIPT_ERROR( "Error @ Str_SetChar : index out of bounds in string(" + l_string + "), index " + StringConverter::toString( l_index ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Empty )
{
	VERBOSE_STARTFUNC( "Str_Empty" );
	GET_AND_EXEC_PARAM( String, l_string , 0 );
	RETURN_AS( bool ) l_string.empty();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Clear )
{
	VERBOSE_STARTFUNC( "Str_Clear" );
	GET_AND_EXEC_PARAM( String, l_string , 0 );
	l_string.clear();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Replace )
{
	VERBOSE_STARTFUNC( "Str_Replace" );
	GET_AND_EXEC_THREE_PARAM( String, p_target, String, p_find, String, p_replaced );
	RETURN_AS( String ) string::replace( p_target, p_find, p_replaced );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Erase )
{
	VERBOSE_STARTFUNC( "Str_Erase" );
	GET_AND_EXEC_THREE_PARAM( String, p_target, int, p_start, int, p_end );

	if ( static_cast <uint32_t>( p_start ) < p_target.length() )
	{
		p_target.erase( static_cast <uint32_t>( p_start ), static_cast <uint32_t>( p_end ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Lower )
{
	VERBOSE_STARTFUNC( "Str_Lower" );
	GET_AND_EXEC_PARAM( String, p_target, 0 );
	StringUtil::toLowerCase( p_target );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Str_Upper )
{
	VERBOSE_STARTFUNC( "Str_Upper" );
	GET_AND_EXEC_PARAM( String, p_target, 0 );
	StringUtil::toUpperCase( p_target );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Chr_IsLower )
{
	VERBOSE_STARTFUNC( "Char_IsLower" );
	GET_AND_EXEC_PARAM( char, l_charNode , 0 );
	RETURN_AS( bool )( l_charNode >= 'a' && l_charNode <= 'z' );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Chr_IsUpper )
{
	VERBOSE_STARTFUNC( "Char_IsUpper" );
	GET_AND_EXEC_PARAM( char, l_charNode , 0 );
	RETURN_AS( bool )( l_charNode >= 'A' && l_charNode <= 'Z' );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Chr_IsAlpha )
{
	VERBOSE_STARTFUNC( "Char_IsAlpha" );
	GET_AND_EXEC_PARAM( char, l_charNode , 0 );
	RETURN_AS( bool )( l_charNode >= 'a' && l_charNode <= 'z' ) || ( l_charNode >= 'A' && l_charNode <= 'Z' );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Chr_IsNumeral )
{
	VERBOSE_STARTFUNC( "Char_IsNumeral" );
	GET_AND_EXEC_PARAM( char, l_charNode , 0 );
	RETURN_AS( bool )( l_charNode >= '0' && l_charNode <= '9' );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_Create )
{
	VERBOSE_STARTFUNC( "AnimationGroup_Create" );
	GET_AND_EXEC_PARAM( String, l_groupName, 0 );

	if ( l_groupName.empty() )
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_Create : empty name not allowed" );
	}
	else
	{
		RETURN_AS( AnimatedObjectGroup * ) ScriptEngine::GetContext()->animationManager->CreateElement( l_groupName, *ScriptEngine::GetContext()->animationManager ).get();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_GetByName )
{
	VERBOSE_STARTFUNC( "AnimationGroup_GetByName" );
	GET_AND_EXEC_PARAM( String, l_groupName, 0 );
	auto l_aog = ScriptEngine::GetContext()->animationManager->GetElementByName( l_groupName );

	if ( !l_aog )
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_GetByName : Animation group " + l_groupName + " does not exist" );
	}

	RETURN_AS( AnimatedObjectGroup * ) l_aog.get();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_Exists )
{
	VERBOSE_STARTFUNC( "AnimationGroup_Exists" );
	GET_AND_EXEC_PARAM( String, l_groupName, 0 );
	auto l_aog = ScriptEngine::GetContext()->animationManager->GetElementByName( l_groupName );
	RETURN_AS( bool )( l_aog != nullptr );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_Destroy )
{
	VERBOSE_STARTFUNC( "AnimationGroup_Destroy" );
	GET_AND_EXEC_PARAM( String, l_groupName, 0 );
	auto l_group = ScriptEngine::GetContext()->animationManager->RemoveElement( l_groupName );

	if ( !l_group )
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_Destroy : Animation group " + l_groupName + " doesn't exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_DestroyAll )
{
	VERBOSE_STARTFUNC( "AnimationGroup_DestroyAll" );
	ScriptEngine::GetContext()->animationManager->Clear();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_GetObject )
{
	VERBOSE_STARTFUNC( "AnimationGroup_GetObject" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, String, l_name );

	if ( l_group != nullptr )
	{
		AnimatedObject * l_object = l_group->GetObjectByName( l_name );

		if ( l_object == nullptr )
		{
			SCRIPT_ERROR( "Error @ AnimationGroup_GetObject : group object [" + l_name + "] does not exist" );
		}

		RETURN_AS( AnimatedObject * ) l_object;
	}
	else
	{
		RETURN_AS( AnimatedObject * ) nullptr;
		SCRIPT_ERROR( "Error @ AnimationGroup_GetObject : animation group nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_AddObject )
{
	VERBOSE_STARTFUNC( "AnimationGroup_GetObject" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, Entity *, l_object );

	if ( l_group != nullptr && l_object != nullptr )
	{
		AnimatedObject * l_aObject = l_group->AddEntity( l_object );

		if ( l_aObject == nullptr )
		{
			SCRIPT_ERROR( "Error @ AnimationGroup_AddObject : can't add " + l_object->getName() + " to animation group" );
			RETURN_AS( bool ) false;
		}
		else
		{
			RETURN_AS( bool ) true;
		}
	}
	else
	{
		RETURN_AS( bool ) false;
		SCRIPT_ERROR( "Error @ AnimationGroup_AddObject : animation group or entity nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_RemObject )
{
	VERBOSE_STARTFUNC( "AnimationGroup_RemoveObject" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, String, l_name );

	if ( l_group != nullptr )
	{
		if ( l_group->GetObjectByName( l_name ) == nullptr )
		{
			SCRIPT_ERROR( "Error @ AnimationGroup_RemoveObject : group object [" + l_name + "] does not exist" );
		}
		else
		{
			l_group->RemoveObject( l_name );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_RemoveObject : animation group nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_ChainAll )
{
	VERBOSE_STARTFUNC( "AnimationGroup_ChainAll" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, bool, l_loopedLast );

	if ( l_group != nullptr )
	{
		l_group->ChainAllAnimations( l_loopedLast );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_RemoveObject : animation group nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AOb_GetAnimation )
{
	VERBOSE_STARTFUNC( "AnimatedObject_GetAnimation" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObject *, l_object, String, l_name );

	if ( l_object != nullptr )
	{
		EMAnimation * l_animation = l_object->GetAnimation( l_name );

		if ( l_animation == nullptr )
		{
			SCRIPT_ERROR( "Error @ AnimatedObject_GetAnimation : anination [" + l_name + "] does not exist" );
		}

		RETURN_AS( EMAnimation * )l_animation;
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimatedObject_GetAnimation : animated object nullptr" );
		RETURN_AS( EMAnimation * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AOb_GetNumPlayingAnimations )
{
	VERBOSE_STARTFUNC( "AnimatedObject_GetNumPlayingAnimations" );
	GET_AND_EXEC_PARAM( AnimatedObject *, l_object, 0 );

	if ( l_object != nullptr )
	{
		int l_nbAnims = static_cast< int >( l_object->GetNumPlayingAnimations() );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimatedObject_GetAnimation : animated object nullptr" );
		RETURN_AS( int ) 0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AOb_GetPlayingAnimationName )
{
	VERBOSE_STARTFUNC( "AOb_GetPlayingAnimationName" );
	GET_AND_EXEC_PARAM( AnimatedObject *, l_object, 0 );
	GET_AND_EXEC_PARAM( int, l_index, 1 );

	if ( l_object != nullptr )
	{
		String l_name = l_object->GetPlayingAnimationName( static_cast <uint32_t>( l_index ) );
		RETURN_AS( String ) l_name;
	}
	else
	{
		SCRIPT_ERROR( "Error @ AOb_GetPlayingAnimationName : animated object nullptr" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_Start )
{
	VERBOSE_STARTFUNC( "Animation_Start" );
	GET_AND_EXEC_PARAM( EMAnimation *, l_anim, 0 );

	if ( l_anim != nullptr )
	{
		l_anim->Play();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_Start : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_Stop )
{
	VERBOSE_STARTFUNC( "Animation_Stop" );
	GET_AND_EXEC_PARAM( EMAnimation *, l_anim, 0 );

	if ( l_anim != nullptr )
	{
		l_anim->Stop();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_Stop : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_Pause )
{
	VERBOSE_STARTFUNC( "Animation_Pause" );
	GET_AND_EXEC_PARAM( EMAnimation *, l_anim, 0 );

	if ( l_anim != nullptr )
	{
		l_anim->Pause();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_Pause : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_SetTime )
{
	VERBOSE_STARTFUNC( "Animation_SetTime" );
	GET_AND_EXEC_TWO_PARAM( EMAnimation *, l_anim, Real, l_time );

	if ( l_anim != nullptr )
	{
		l_anim->SetTime( l_time );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_SetTime : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_GetLength )
{
	VERBOSE_STARTFUNC( "Animation_GetLength" );
	GET_AND_EXEC_PARAM( EMAnimation *, l_anim, 0 );

	if ( l_anim != nullptr )
	{
		RETURN_AS( Real ) l_anim->GetLength();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_GetLength : no animation" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_SetNext )
{
	VERBOSE_STARTFUNC( "Animation_SetNextAnimation" );
	GET_AND_EXEC_TWO_PARAM( EMAnimation *, l_anim, EMAnimation *, p_next );

	if ( l_anim != nullptr && p_next != nullptr )
	{
		l_anim->SetNextAnimation( p_next );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_SetReversed )
{
	VERBOSE_STARTFUNC( "Animation_SetReversed" );
	GET_AND_EXEC_TWO_PARAM( EMAnimation *, p_anim, bool, p_reversed );

	if ( p_anim != nullptr )
	{
		p_anim->SetReversed( p_reversed );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_SetReversed : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_SetLooped )
{
	VERBOSE_STARTFUNC( "Animation_SetLooped" );
	GET_AND_EXEC_TWO_PARAM( EMAnimation *, p_anim, bool, p_looped );

	if ( p_anim != nullptr )
	{
		p_anim->SetLooped( p_looped );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_SetLooped : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ani_Restart )
{
	VERBOSE_STARTFUNC( "Animation_Restart" );
	GET_AND_EXEC_PARAM( EMAnimation *, l_anim, 0 );

	if ( l_anim != nullptr )
	{
		l_anim->Stop();
		l_anim->Play();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Animation_Restart : no animation" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_StartAnimation )
{
	VERBOSE_STARTFUNC( "AnimationGroup_StartAnimation" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, String, l_name );

	if ( l_group != nullptr )
	{
		l_group->StartAnimation( l_name );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_StartAnimation : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_StopAnimation )
{
	VERBOSE_STARTFUNC( "AnimationGroup_StopAnimation" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, String, l_name );

	if ( l_group != nullptr )
	{
		l_group->StopAnimation( l_name );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_StopAnimation : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_ChainAnimation )
{
	VERBOSE_STARTFUNC( "AnimationGroup_StopAnimation" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, String, l_nextName );

	if ( l_group != nullptr )
	{
		l_group->ChainAnimation( l_name, l_nextName );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_StopAnimation : no animation group" );
	}
}


EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_PauseAnimation )
{
	VERBOSE_STARTFUNC( "AnimationGroup_PauseAnimation" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, String, l_name );

	if ( l_group != nullptr )
	{
		l_group->PauseAnimation( l_name );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_PauseAnimation : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetTimeAnimation )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetTimeAnimation" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, Real, l_time );

	if ( l_group != nullptr )
	{
		if ( l_time >= 0 )
		{
			l_group->SetTimeAnimation( l_name, l_time );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetTimeAnimation : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetAnimationTimeScale )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetAnimationTimeScale" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, Real, l_time );

	if ( l_group != nullptr )
	{
		if ( l_time >= 0 )
		{
			l_group->SetTimeScaleAnimation( l_name, l_time );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetTimeScaleAnimation : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetAnimationWeight )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetAnimationWeight" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, Real, l_weight );

	if ( l_group != nullptr )
	{
		if ( l_weight >= 0 )
		{
			l_group->SetWeightAnimation( l_name, l_weight );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetAnimationWeight : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetAnimationTimeOut )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetAnimationTimeOut" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, Real, l_weight );

	if ( l_group != nullptr )
	{
		if ( l_weight >= 0 )
		{
			l_group->SetAnimationTimeOut( l_name, l_weight );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetAnimationWeight : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetAnimationTimeIn )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetAnimationTimeIn" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, Real, l_weight );

	if ( l_group != nullptr )
	{
		if ( l_weight >= 0 )
		{
			l_group->SetAnimationTimeIn( l_name, l_weight );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetAnimationWeight : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetAnimationReversed )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetAnimationReversed" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, bool, l_reversed );

	if ( l_group != nullptr )
	{
		l_group->SetAnimationReversed( l_name, l_reversed );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetAnimationReversed : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_SetAnimationLooped )
{
	VERBOSE_STARTFUNC( "AnimationGroup_SetAnimationLooped" );
	GET_AND_EXEC_THREE_PARAM( AnimatedObjectGroup *, l_group, String, l_name, bool, l_looped );

	if ( l_group != nullptr )
	{
		l_group->SetAnimationReversed( l_name, l_looped );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_SetAnimationLooped : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_StopAllAnimations )
{
	VERBOSE_STARTFUNC( "AnimationGroup_StopAllAnimations" );
	GET_AND_EXEC_PARAM( AnimatedObjectGroup *, l_group, 0 );

	if ( l_group != nullptr )
	{
		l_group->StopAllAnimations();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_PauseAllAnimations )
{
	VERBOSE_STARTFUNC( "AnimationGroup_StopAllAnimations" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, bool, p_pause );

	if ( l_group != nullptr )
	{
		l_group->SetPauseAll( p_pause );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( AoG_RestartAnimation )
{
	VERBOSE_STARTFUNC( "AnimationGroup_RestartAnimation" );
	GET_AND_EXEC_TWO_PARAM( AnimatedObjectGroup *, l_group, String, l_name );

	if ( l_group != nullptr )
	{
		l_group->StopAnimation( l_name );
		l_group->StartAnimation( l_name );
	}
	else
	{
		SCRIPT_ERROR( "Error @ AnimationGroup_RestartAnimation : no animation group" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Qua_FromAxisAngle )
{
	VERBOSE_STARTFUNC( "Quat_FromAxisAngle" );
	GET_AND_EXEC_TWO_PARAM( Vector3, l_axis , Real, l_angle );
	Quaternion q;
	q.FromAngleAxis( Degree( l_angle ) , l_axis );
	RETURN_AS( Quaternion ) q;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Qua_GetAngle )
{
	VERBOSE_STARTFUNC( "Qua_GetAngle" );
	GET_AND_EXEC_PARAM( Quaternion, l_qua, 0 );
	Vector3 l_axis;
	Degree l_angle;
	l_qua.ToAngleAxis( l_angle, l_axis );
	RETURN_AS( Real ) l_angle.valueDegrees();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Qua_GetAxis )
{
	VERBOSE_STARTFUNC( "Qua_GetAngle" );
	GET_AND_EXEC_PARAM( Quaternion, l_qua, 0 );
	Vector3 l_axis;
	Degree l_angle;
	l_qua.ToAngleAxis( l_angle, l_axis );
	RETURN_AS( Vector3 ) l_axis;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Qua_Slerp )
{
	VERBOSE_STARTFUNC( "Quat_Slerp" );
	GET_AND_EXEC_THREE_PARAM( Quaternion, l_quatA, Quaternion, l_quatB , Real, l_time );
	Real time = l_time;

	if ( time > 1.0 )
	{
		time = 1.0;
	}

	if ( time < 0.0 )
	{
		time = 0.0;
	}

	Radian r;
	Vector3 v;
	Quaternion q = Quaternion::Slerp( time, l_quatA, l_quatB , true );
	q.normalise();
	RETURN_AS( Quaternion ) q;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Uni_GetCameraByName )
{
	VERBOSE_STARTFUNC( "Universe_GetCameraByName" );
	GET_AND_EXEC_TWO_PARAM( String, l_uniName, String, l_camName );
	auto l_uni = ScriptEngine::GetContext()->universeManager->GetElementByName( l_uniName );

	if ( l_uni )
	{
		SceneManager * l_manager = l_uni->GetManager();

		if ( l_manager->hasCamera( l_camName ) )
		{
			RETURN_AS( Camera * ) l_manager->getCamera( l_camName );
		}
		else
		{
			SCRIPT_ERROR( "Error @ Universe_GetCameraByName : Camera [" + l_camName + "] does not exist in universe [" + l_uniName + "]" );
			RETURN_AS( Camera * ) nullptr;
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Universe_GetCameraByName : Universe [" + l_uniName + "] does not exist" );
		RETURN_AS( Camera * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Uni_GetEntityByName )
{
	VERBOSE_STARTFUNC( "Universe_GetEntityByName" );
	GET_AND_EXEC_TWO_PARAM( String, l_uniName, String, l_entName );
	auto l_uni = ScriptEngine::GetContext()->universeManager->GetElementByName( l_uniName );

	if ( l_uni )
	{
		SceneManager * l_manager = l_uni->GetManager();

		if ( l_manager->hasEntity( l_entName ) )
		{
			RETURN_AS( Entity * ) l_manager->getEntity( l_entName );
		}
		else
		{
			SCRIPT_ERROR( "Error @ Universe_GetEntityByName : Entity [" + l_entName + "] does not exist in universe [" + l_uniName + "]" );
			RETURN_AS( Entity * ) nullptr;
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Universe_GetEntityByName : Universe [" + l_uniName + "] does not exist" );
		RETURN_AS( Entity * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Uni_GetLightByName )
{
	VERBOSE_STARTFUNC( "Universe_GetLightByName" );
	GET_AND_EXEC_TWO_PARAM( String, l_uniName , String, l_lightName );
	auto l_uni = ScriptEngine::GetContext()->universeManager->GetElementByName( l_uniName );

	if ( l_uni )
	{
		SceneManager * l_manager = l_uni->GetManager();

		if ( l_manager->hasLight( l_lightName ) )
		{
			RETURN_AS( Light * ) l_manager->getLight( l_lightName );
		}
		else
		{
			SCRIPT_ERROR( "Error @ Universe_GetLightByName : Light [" + ( l_lightName ) + "] does not exist in universe [" + ( l_uniName ) + "]" );
			RETURN_AS( Light * ) nullptr;
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Universe_GetLightByName : Universe [" + ( l_uniName ) + "] does not exist" );
		RETURN_AS( Light * ) nullptr;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Uni_GetSceneNodeByName )
{
	VERBOSE_STARTFUNC( "Universe_GetSceneNodeByName" );
	GET_AND_EXEC_TWO_PARAM( String, l_uniName , String, l_nodeName );
	auto l_uni = ScriptEngine::GetContext()->universeManager->GetElementByName( l_uniName );

	if ( l_uni )
	{
		SceneManager * l_manager = l_uni->GetManager();

		if ( l_manager->hasSceneNode( l_nodeName ) )
		{
			RETURN_AS( SceneNode * ) l_manager->getSceneNode( l_nodeName );
		}
		else
		{
			RETURN_AS( SceneNode * ) nullptr;
			SCRIPT_ERROR( "Error @ Universe_GetSceneNodeByName : SceneNode [" + l_nodeName + "] does not exist in universe [" + l_uniName + "]" );
		}
	}
	else
	{
		RETURN_AS( SceneNode * ) nullptr;
		SCRIPT_ERROR( "Error @ Universe_GetCameraByName : Universe [" + l_uniName + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Uni_GetUniverseList )
{
	VERBOSE_STARTFUNC( "Universe_GetUniverseList" );
	UniverseManager::TypeMap::iterator i;
	Arr_Clear( caller );
	NodeValueBaseArray & l_array = caller->get<NodeValueBaseArray>();

	for ( auto i : *ScriptEngine::GetContext()->universeManager )
	{
		NodeValue< String > * l_node = new NodeValue<String>();
		l_node->m_value = i.second->GetName();
		l_array.push_back( l_node );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Uni_GetSceneList )
{
	VERBOSE_STARTFUNC( "Universe_GetSceneList" );
	GET_AND_EXEC_PARAM( String, l_uniName , 0 );
	Arr_Clear( caller );
	auto l_uni = ScriptEngine::GetContext()->universeManager->GetElementByName( l_uniName );

	if ( l_uni )
	{
		NodeValueBaseArray & l_array = caller->get<NodeValueBaseArray>();

		for ( ZoneMap::iterator i = l_uni->m_zones.begin() ; i != l_uni->m_zones.end() ; i++ )
		{
			NodeValue<String> * l_node = new NodeValue<String>();
			l_node->m_value = i->second->GetName();
			l_array.push_back( l_node );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Universe_GetUniverseList : Universe [" + l_uniName + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetByName )
{
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	RETURN_AS( PhysicsObject * ) ScriptEngine::GetContext()->physicsSimulation->GetObjectByName( l_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_Create )
{
	GET_AND_EXEC_THREE_PARAM( Entity *, p_entity, bool, p_static, bool, p_phantom );
	RETURN_AS( PhysicsObject * ) ScriptEngine::GetContext()->physicsSimulation->CreateObject( p_entity, p_static, p_phantom );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_Destroy )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	ScriptEngine::GetContext()->physicsSimulation->DestroyObject( p_name );
}


EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetSpeed )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		RETURN_AS( Vector3 ) l_pobj->GetSpeed();
	}
	else
	{
		SCRIPT_ERROR( "POb_GetSpeed : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_SetSpeed )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, Vector3, l_speed );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetSpeed( l_speed );
	}
	else
	{
		SCRIPT_ERROR( "POb_SetSpeed : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetAngularSpeed )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		RETURN_AS( Vector3 ) l_pobj->GetAngularSpeed();
	}
	else
	{
		SCRIPT_ERROR( "POb_GetAngularSpeed : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_SetAngularSpeed )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, Vector3, l_speed );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetAngularSpeed( l_speed );
	}
	else
	{
		SCRIPT_ERROR( "POb_SetAngularSpeed : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_AddForce )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, Vector3, l_force );

	if ( l_pobj != nullptr )
	{
		l_pobj->AddForce( l_force );
	}
	else
	{
		SCRIPT_ERROR( "POb_AddForce : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_SetForce )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, Vector3, l_force );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetForce( l_force );
	}
	else
	{
		SCRIPT_ERROR( "POb_SetForce : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_AddTorque )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, Vector3, l_force );

	if ( l_pobj != nullptr )
	{
		l_pobj->AddTorque( l_force );
	}
	else
	{
		SCRIPT_ERROR( "POb_AddTorque : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_SetTorque )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, Vector3, l_force );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetTorque( l_force );
	}
	else
	{
		SCRIPT_ERROR( "POb_SetTorque : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_SetIgnoreGravity )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, bool, l_gravity );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetFloating( l_gravity );
	}
	else
	{
		SCRIPT_ERROR( "POb_SetIgnoreGravity : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetIgnoreGravity )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		RETURN_AS( bool ) l_pobj->GetFloating();
	}
	else
	{
		SCRIPT_ERROR( "POb_GetIgnoreGravity : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_SetEnabled )
{
	GET_AND_EXEC_TWO_PARAM( PhysicsObject *, l_pobj, bool, l_enabled );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetEnabled( l_enabled );
	}
	else
	{
		SCRIPT_ERROR( "POb_SetEnabled : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_IsEnabled )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		RETURN_AS( bool ) l_pobj->IsEnabled();
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_CollisionCheck )
{
	GET_AND_EXEC_THREE_PARAM( PhysicsObject *, l_pobjA, PhysicsObject *, l_pobjB, bool, l_lookupOnly );

	if ( l_pobjA != nullptr && l_pobjB != nullptr )
	{
		if ( l_lookupOnly )
		{
			RETURN_AS( bool ) l_pobjA->CollisionTest_Fast( l_pobjB );
		}
		else
		{
			RETURN_AS( bool ) l_pobjA->CollisionTest_Complete( l_pobjB );
		}
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetNumCollisionsWith )
{
	GET_AND_EXEC_THREE_PARAM( PhysicsObject *, l_pobjA, PhysicsObject *, l_pobjB, bool, l_lookupOnly );

	if ( l_pobjA != nullptr && l_pobjB != nullptr )
	{
		if ( l_lookupOnly )
		{
			RETURN_AS( int ) l_pobjA->NumCollisionTest_Fast( l_pobjB );
		}
		else
		{
			RETURN_AS( int ) l_pobjA->NumCollisionTest_Complete( l_pobjB );
		}
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_CheckAllCollisions )
{
	GET_AND_EXEC_FOUR_PARAM( PhysicsObject *, l_pobj, bool, l_dynamic, bool, l_static, bool, l_phantom );

	if ( l_pobj != nullptr )
	{
		if ( l_dynamic )
		{
			ScriptEngine::GetContext()->physicsSimulation->GetDynamicSpace()->CollideObject( l_pobj );
		}

		if ( l_static )
		{
			ScriptEngine::GetContext()->physicsSimulation->GetStaticSpace()->CollideObject( l_pobj );
		}

		if ( l_phantom )
		{
			ScriptEngine::GetContext()->physicsSimulation->GetPhantomSpace()->CollideObject( l_pobj );
		}

		RETURN_AS( int ) static_cast< int >( l_pobj->GetNumContacts() );
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
		RETURN_AS( int ) 0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_ReassertPosition )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		l_pobj->ReassertSpatialState();
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetNumCollisions )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		RETURN_AS( int ) static_cast< int >( l_pobj->GetNumContacts() );
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_GetName )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		RETURN_AS( String ) l_pobj->GetEntity()->getName();
	}
	else
	{
		SCRIPT_ERROR( "POb_IsEnabled : PhysicsObject nullptr" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_Reset )
{
	GET_AND_EXEC_PARAM( PhysicsObject *, l_pobj, 0 );

	if ( l_pobj != nullptr )
	{
		l_pobj->SetAngularSpeed( Vector3::ZERO );
		l_pobj->SetSpeed( Vector3::ZERO );
		l_pobj->SetForce( Vector3::ZERO );
		l_pobj->SetTorque( Vector3::ZERO );
	}
	else
	{
		SCRIPT_ERROR( "POb_Reset : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_AddBoundingBox )
{
	GET_AND_EXEC_THREE_PARAM( PhysicsObject *, l_pobj, Vector3, p_size, Vector3, p_offset );

	if ( l_pobj != nullptr )
	{
		BoundingBox * l_box = new BoundingBox( p_size );
		l_pobj->AddBound( l_box );
		l_box->SetPosition( p_offset );
	}
	else
	{
		SCRIPT_ERROR( "POb_Reset : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_AddBoundingSphere )
{
	GET_AND_EXEC_THREE_PARAM( PhysicsObject *, l_pobj, Real, p_radius, Vector3, p_offset );

	if ( l_pobj != nullptr )
	{
		BoundingSphere * l_sphere = new BoundingSphere( p_radius );
		l_pobj->AddBound( l_sphere );
		l_sphere->SetPosition( p_offset );
	}
	else
	{
		SCRIPT_ERROR( "POb_Reset : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( POb_AddBoundingMesh )
{
	GET_AND_EXEC_THREE_PARAM( PhysicsObject *, l_pobj, String, p_meshName, Vector3, p_offset );

	if ( l_pobj != nullptr )
	{
		try
		{
			MeshPtr l_mesh = MeshManager::getSingletonPtr()->getByName( p_meshName );
			BoundingMesh * l_bmesh = new BoundingMesh( l_mesh.get() );
			l_pobj->AddBound( l_bmesh );
			l_bmesh->SetPosition( p_offset );
		}
		catch ( ... )
		{
			SCRIPT_ERROR( "POb_Reset : PhysicsObject nullptr" );
		}
	}
	else
	{
		SCRIPT_ERROR( "POb_Reset : PhysicsObject nullptr" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetCurrentSimulation )
{
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	PhysicsSimulation * l_simu = ScriptEngine::GetContext()->physicsSimulation;
	ScriptEngine::GetContext()->physicsSimulation = ScriptEngine::GetContext()->physicsEngine->GetSimulation( l_name );

	if ( ScriptEngine::GetContext()->physicsSimulation == nullptr )
	{
		ScriptEngine::GetContext()->physicsSimulation = l_simu;
		SCRIPT_ERROR( "PhS_SetCurrentSimulation : Simulation [" + l_name + "] does not exist" );
	}
	else
	{
		ScriptEngine::GetContext()->physicsSpace = ScriptEngine::GetContext()->physicsSimulation->GetDynamicSpace();
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetGravity )
{
	GET_AND_EXEC_PARAM( Vector3, l_gravity, 0 );
	ScriptEngine::GetContext()->physicsSimulation->SetGravity( l_gravity );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_GetGravity )
{
	RETURN_AS( Vector3 ) ScriptEngine::GetContext()->physicsSimulation->GetGravity();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetTimescale )
{
	GET_AND_EXEC_PARAM( Real, l_timescale, 0 );
	ScriptEngine::GetContext()->physicsSimulation->SetTimescale( l_timescale );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_GetTimescale )
{
	RETURN_AS( Real ) ScriptEngine::GetContext()->physicsSimulation->GetTimescale();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetSteptime )
{
	GET_AND_EXEC_TWO_PARAM( Real, l_steptime, Real, l_steptimeLimit );
	ScriptEngine::GetContext()->physicsSimulation->SetSteptime( l_steptime );
	ScriptEngine::GetContext()->physicsSimulation->SetSteptimeLimit( l_steptimeLimit );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_GetSteptime )
{
	RETURN_AS( Real ) ScriptEngine::GetContext()->physicsSimulation->GetSteptime();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_GetSteptimeLimit )
{
	RETURN_AS( Real ) ScriptEngine::GetContext()->physicsSimulation->GetSteptimeLimit();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetErp )
{
	GET_AND_EXEC_PARAM( Real, l_erp, 0 );
	ScriptEngine::GetContext()->physicsSimulation->SetERP( l_erp );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetCfm )
{
	GET_AND_EXEC_PARAM( Real, l_cfm, 0 );
	ScriptEngine::GetContext()->physicsSimulation->SetCFM( l_cfm );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetAutoUpdate )
{
	GET_AND_EXEC_PARAM( bool, l_autoUpdate, 0 );
	ScriptEngine::GetContext()->physicsSimulation->SetAutoUpdated( l_autoUpdate );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_Update )
{
	ScriptEngine::GetContext()->physicsSimulation->Update();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_SetUpdateScript )
{
	ScriptEngine::GetContext()->physicsSimulation->SetUpdateScript( caller->m_childs[0] );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_Raytrace )
{
	GET_AND_EXEC_TWO_PARAM( Vector3, p_origin, Vector3, p_direction );
	const DistanceMap & l_map = ScriptEngine::GetContext()->physicsSimulation->Raytrace( Ray( p_origin, p_direction ) );

	if ( l_map.empty() )
	{
		RETURN_AS( PhysicsObject * ) nullptr;
	}
	else
	{
		RETURN_AS( PhysicsObject * ) l_map.begin()->second;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_ClearObjects )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	PhysicsSimulation * l_physicsSimulation = ScriptEngine::GetContext()->physicsEngine->GetSimulation( p_name );

	if ( l_physicsSimulation != nullptr )
	{
		l_physicsSimulation->ClearObjects();

//		l_physicsSimulation->ClearSpaces();
		if ( ScriptEngine::GetContext()->physicsSimulation == l_physicsSimulation )
		{
			ScriptEngine::GetContext()->physicsSpace = l_physicsSimulation->GetDynamicSpace();
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( PhS_Destroy )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( ScriptEngine::GetContext()->physicsSimulation->GetName() == p_name )
	{
		ScriptEngine::GetContext()->physicsSimulation = ScriptEngine::GetContext()->physicsEngine->GetSimulation( "Main" );
		ScriptEngine::GetContext()->physicsSpace = ScriptEngine::GetContext()->physicsSimulation->GetStaticSpace();
	}

	ScriptEngine::GetContext()->physicsEngine->DestroySimulation( p_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Wim_Reload )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	WebImage * l_image = WebImageManager::GetSingletonPtr()->GetWebImage( p_name );

	if ( l_image == nullptr )
	{
		return;
	}

	l_image->Reload();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Wim_Create )
{
	GET_AND_EXEC_TWO_PARAM( String, p_name, String, p_url );
	WebImage * l_image = WebImageManager::GetSingletonPtr()->Create( p_name );
	l_image->SetUrl( p_url );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Pub_Reload )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	PubObject * l_object = PubManager::GetSingletonPtr()->GetPubObject( p_name );

	if ( l_object == nullptr )
	{
		return;
	}

	l_object->Reload();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Wim_Delete )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	WebImageManager::GetSingletonPtr()->DestroyImage( p_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Seq_Start )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	auto l_sequence = ScriptEngine::GetContext()->sequenceManager->GetElementByName( p_name );

	if ( !l_sequence )
	{
		return;
	}

	l_sequence->Start();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Seq_Stop )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	auto l_sequence = ScriptEngine::GetContext()->sequenceManager->GetElementByName( p_name );

	if ( !l_sequence )
	{
		return;
	}

	l_sequence->Stop();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Seq_Pause )
{
	GET_AND_EXEC_TWO_PARAM( String, p_name, bool, p_pause );
	auto l_sequence = ScriptEngine::GetContext()->sequenceManager->GetElementByName( p_name );

	if ( !l_sequence )
	{
		return;
	}

	l_sequence->Pause( p_pause );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Nwk_CreateClient )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( ! ScriptEngine::GetContext()->network->CreateClient( p_name ) )
	{
		SCRIPT_ERROR( "Error @ Nwk_CreateClient : Can't create client " + p_name );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Nwk_ConnectClient )
{
	GET_AND_EXEC_THREE_PARAM( String, p_name, String, p_host, int, p_port );

	if ( ! ScriptEngine::GetContext()->network->Connect( p_name, p_host, p_port ) )
	{
		SCRIPT_ERROR( "Error @ Nwk_ConnectClient : Can't connect client " + p_name );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Nwk_CloseClient )
{
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( ! ScriptEngine::GetContext()->network->CloseClient( p_name ) )
	{
		SCRIPT_ERROR( "Error @ Nwk_CloseClient : Can't close client " + p_name );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Nwk_ClientSendMessage )
{
	GET_AND_EXEC_TWO_PARAM( String, p_name, String, p_message );
	ScriptEngine::GetContext()->network->Write( p_name, p_message );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_GetByName )
{
	GET_AND_EXEC_PARAM( String, p_matName, 0 );

	if ( p_matName.empty() || ! MaterialManager::getSingletonPtr()->resourceExists( p_matName ) )
	{
		SCRIPT_ERROR( "Error @ Material_GetByName : Material [" + p_matName + "] doesn't exist" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	Material * l_material = static_cast <Material *>( MaterialManager::getSingletonPtr()->getByName( p_matName ).get() );
	RETURN_AS( Material * ) l_material;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_Exists )
{
	GET_AND_EXEC_PARAM( String, p_matName, 0 );
	RETURN_AS( bool )( ! p_matName.empty() && MaterialManager::getSingletonPtr()->resourceExists( p_matName ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_Create )
{
	GET_AND_EXEC_PARAM( String, p_matName, 0 );

	if ( p_matName.empty() )
	{
		SCRIPT_ERROR( "Error @ Material_Create : Material name can't be void" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	if ( MaterialManager::getSingletonPtr()->resourceExists( p_matName ) )
	{
		SCRIPT_ERROR( "Error @ Material_Create : Material " + p_matName + " already exists" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	Material * l_material = static_cast <Material *>( MaterialManager::getSingletonPtr()->create( p_matName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ).get() );
	RETURN_AS( Material * ) l_material;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_Destroy )
{
	GET_AND_EXEC_PARAM( String, p_matName, 0 );

	if ( p_matName.empty() )
	{
		SCRIPT_ERROR( "Error @ Material_Destroy : Void name" );
		return;
	}

	if ( ! MaterialManager::getSingletonPtr()->resourceExists( p_matName ) )
	{
		SCRIPT_ERROR( "Error @ Material_Destroy : Material " + p_matName + " doesn't exist" );
		return;
	}

	MaterialManager::getSingletonPtr()->remove( p_matName );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_CreateCopy )
{
	GET_AND_EXEC_TWO_PARAM( String, p_matName, String, p_newMatName );

	if ( p_matName.empty() || p_newMatName.empty() )
	{
		SCRIPT_ERROR( "Error @ Material_CreateCopy : Void name" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	if ( ! MaterialManager::getSingletonPtr()->resourceExists( p_matName ) )
	{
		SCRIPT_ERROR( "Error @ Material_CreateCopy : Material " + p_matName + " doesn't exist" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	Material * l_material = static_cast <Material *>( MaterialManager::getSingletonPtr()->getByName( p_matName ).get() );

	if ( MaterialManager::getSingletonPtr()->resourceExists( p_newMatName ) )
	{
		SCRIPT_ERROR( "Error @ Material_CreateCopy : Material " + p_newMatName + " already exist" );
		l_material = static_cast <Material *>( MaterialManager::getSingletonPtr()->getByName( p_newMatName ).get() );
	}
	else
	{
		l_material = l_material->clone( p_newMatName ).get();
	}

	RETURN_AS( Material * ) l_material;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_CreateMatCopy )
{
	GET_AND_EXEC_TWO_PARAM( Material *, p_material, String, p_newMatName );
	Material * l_material = nullptr;

	if ( p_newMatName.empty() )
	{
		SCRIPT_ERROR( "Error @ Material_CreateCopy : Void name" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_CreateCopy : Void Material" );
		RETURN_AS( Material * ) nullptr;
		return;
	}

	if ( MaterialManager::getSingletonPtr()->resourceExists( p_newMatName ) )
	{
		SCRIPT_ERROR( "Error @ Material_CreateCopy : Material " + p_newMatName + " already exist" );
		l_material = static_cast <Material *>( MaterialManager::getSingletonPtr()->getByName( p_newMatName ).get() );
	}
	else
	{
		l_material = p_material->clone( p_newMatName ).get();
	}

	RETURN_AS( Material * ) l_material;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_ScrollUV )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_ScrollUV : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_ScrollUV : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( Real, p_u, 2 );
	GET_AND_EXEC_PARAM( Real, p_v, 3 );
	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit );
	Real l_oldU = l_unit->getTextureUScroll();
	Real l_oldV = l_unit->getTextureVScroll();
	l_unit->setTextureUScroll( l_oldU + p_u );
	l_unit->setTextureVScroll( l_oldV + p_v );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_SetScrollUV )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_SetScrollUV : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_SetScrollUV : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( Real, p_u, 2 );
	GET_AND_EXEC_PARAM( Real, p_v, 3 );
	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit );
	l_unit->setTextureUScroll( p_u );
	l_unit->setTextureVScroll( p_v );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_RotateUV )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_RotateUV : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_RotateUV : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( Real, p_rotate, 2 );
	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit );
	Real l_oldRotate = l_unit->getTextureRotate().valueDegrees();
	l_unit->setTextureRotate( Degree( l_oldRotate + p_rotate ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_SetRotateUV )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_SetRotateUV : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_SetRotateUV : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( Real, p_rotate, 2 );
	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit );
	l_unit->setTextureRotate( Degree( p_rotate ) );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_ScaleUV )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_ScaleUV : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_ScaleUV : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( Real, p_u, 2 );
	GET_AND_EXEC_PARAM( Real, p_v, 3 );
	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit );
	Real l_oldU = l_unit->getTextureUScale();
	Real l_oldV = l_unit->getTextureVScale();
	l_unit->setTextureUScale( l_oldU + p_u );
	l_unit->setTextureVScale( l_oldV + p_v );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_SetScaleUV )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_SetScaleUV : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_SetScaleUV : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( Real, p_u, 2 );
	GET_AND_EXEC_PARAM( Real, p_v, 3 );
	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit );
	l_unit->setTextureUScale( p_u );
	l_unit->setTextureVScale( p_v );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_CreateTextureUnit )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_CreateTextureUnit : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( String, p_texture, 1 );

	if ( p_texture.empty() || ! TextureManager::getSingletonPtr()->resourceExists( p_texture ) )
	{
		SCRIPT_ERROR( "Error @ Material_ChangeImage : Wanted texture [" + p_texture + "] doesn't exist" );
		return;
	}

	TextureUnitState * l_unit = p_material->getBestTechnique()->getPass( 0 )->createTextureUnitState( p_texture );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_ChangeImage )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_ChangeImage : Void material" );
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_ChangeImage : Wanted texture unit doesn't exist" );
		return;
	}

	GET_AND_EXEC_PARAM( String, p_texture, 2 );

	if ( p_texture.empty() )
	{
		SCRIPT_ERROR( "Error @ Material_ChangeImage : void texture name" );
		return;
	}

	if ( ! TextureManager::getSingletonPtr()->resourceExists( p_texture ) )
	{
		SCRIPT_ERROR( "Error @ Material_ChangeImage : Wanted texture [" + p_texture + "] doesn't exist" );
		return;
	}

	p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit )->setTextureName( p_texture );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Mat_GetImageName )
{
	GET_AND_EXEC_PARAM( Material *, p_material, 0 );

	if ( p_material == nullptr )
	{
		SCRIPT_ERROR( "Error @ Material_GetImageName : Void material" );
		RETURN_AS( String ) EMPTY_STRING;
		return;
	}

	GET_AND_EXEC_PARAM( int, p_unit, 1 );

	if ( p_unit >= p_material->getBestTechnique()->getPass( 0 )->getNumTextureUnitStates() )
	{
		SCRIPT_ERROR( "Error @ Material_GetImageName : Wanted texture unit doesn't exist" );
		RETURN_AS( String ) EMPTY_STRING;
		return;
	}

	RETURN_AS( String ) p_material->getBestTechnique()->getPass( 0 )->getTextureUnitState( p_unit )->getTextureName();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_OpenForRead )
{
	Path l_filePath = ScriptEngine::GetContext()->emuseInstance->GetFL()->GetInstallDir() / "rsc" / ScriptEngine::GetContext()->emuseInstance->GetFL()->GetFileName() / "config.cfg";

	if ( FileExists( l_filePath.c_str() ) )
	{
		if ( ! ScriptEngine::GetContext()->configFile.is_open() )
		{
			ScriptEngine::GetContext()->configFile.open( l_filePath.c_str() );
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_CloseForRead )
{
	Path l_filePath = ScriptEngine::GetContext()->emuseInstance->GetFL()->GetInstallDir() / "rsc" / ScriptEngine::GetContext()->emuseInstance->GetFL()->GetFileName() / "config.cfg";

	if ( FileExists( l_filePath.c_str() ) )
	{
		if ( ScriptEngine::GetContext()->configFile.is_open() && ! ScriptEngine::GetContext()->configFile.bad() )
		{
			ScriptEngine::GetContext()->configFile.close();
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_Destroy )
{
	Path l_filePath = ScriptEngine::GetContext()->emuseInstance->GetFL()->GetInstallDir() / "rsc" / ScriptEngine::GetContext()->emuseInstance->GetFL()->GetFileName() / "config.cfg";

	if ( FileExists( l_filePath.c_str() ) )
	{
		FileDelete( l_filePath.c_str() );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_WriteLine )
{
	GET_AND_EXEC_PARAM( String, p_line, 0 );
	Path l_filePath = ScriptEngine::GetContext()->emuseInstance->GetFL()->GetInstallDir() / "rsc" / ScriptEngine::GetContext()->emuseInstance->GetFL()->GetFileName() / "config.cfg";
	std::ofstream l_file( l_filePath.c_str(), std::ios_base::app | std::ios_base::out );
	l_file << p_line << std::endl;
	l_file.close();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_WriteBuffer )
{
	GET_AND_EXEC_PARAM( String, p_line, 0 );
	GET_AND_EXEC_PARAM( int, p_size, 1 );
	Path l_filePath = ScriptEngine::GetContext()->emuseInstance->GetFL()->GetInstallDir() / "rsc" / ScriptEngine::GetContext()->emuseInstance->GetFL()->GetFileName() / "config.cfg";
	std::ofstream l_file( l_filePath.c_str(), std::ios_base::app | std::ios_base::binary );
	l_file.write( ( char * )( p_line.c_str() ), p_size );
	l_file.close();
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_ReadLine )
{
	String l_line;

	if ( ScriptEngine::GetContext()->configFile.is_open() && ! ScriptEngine::GetContext()->configFile.bad() )
	{
		std::getline( ScriptEngine::GetContext()->configFile, l_line );
	}

	RETURN_AS( String ) l_line;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Con_ReadBuffer )
{
	GET_AND_EXEC_PARAM( int, p_size, 0 );
	Path l_filePath = ScriptEngine::GetContext()->emuseInstance->GetFL()->GetInstallDir() / "rsc" / ScriptEngine::GetContext()->emuseInstance->GetFL()->GetFileName() / "config.cfg";
	String l_line;

	if ( ScriptEngine::GetContext()->configFile.is_open() && ! ScriptEngine::GetContext()->configFile.bad() )
	{
		char * l_cLine = new char[p_size];
		ScriptEngine::GetContext()->configFile.read( l_cLine, p_size );
		l_line.append( l_cLine, p_size );
		delete [] l_cLine;
	}

	RETURN_AS( String ) l_line;
}
