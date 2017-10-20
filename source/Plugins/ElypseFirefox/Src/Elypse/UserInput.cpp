#include "stdafx.h"

#include "main.h"

#include <ElypseFrameListener.h>
#include <ElypseInstance.h>
#include <ElypseController.h>

#define VERBOSE( X) //std::cout << X << " in instance : " << m_ogre->GetName() << std::endl

#define M_COORDS_FROM_POINT( p_point) p_point.x,p_point.y

extern ElypsePluginInstance * g_main;

void ElypsePluginInstance :: _setFocus()
{
	if ( GetFocus() != m_windowHandle )
	{
		SetFocus( m_windowHandle );
	}

	SetCapture( m_windowHandle );
	m_focus = true;
}

void ElypsePluginInstance :: OnSetFocus()
{
	VERBOSE( "OnSetFocus" );

	SetCapture( m_windowHandle );

	m_focus = true;
	g_main = this;
}

void ElypsePluginInstance :: OnKillFocus()
{
	VERBOSE( "OnKillFocus" );

	if ( m_ogre != NULL && m_ogre->IsActive() )
	{
		m_ogre->GetFL()->ResetAll();
	}

	ReleaseCapture();

	m_focus = false;
	g_main = this;
}

void ElypsePluginInstance :: OnMouseMove( const POINTS & p_point )
{
//	VERBOSE( "OnMouseMove");

	if ( m_ogre != NULL && m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseMove( M_COORDS_FROM_POINT( p_point ) );
	}

	g_main = this;
}

void ElypsePluginInstance :: OnMButtonDown( const POINTS & p_point )
{
	VERBOSE( "OnMButtonDown" );

	if ( ! _contains( p_point ) )
	{
		if ( ! m_rightButton && ! m_leftButton )
		{
			SendMessage( m_windowHandle, WM_KILLFOCUS, 0, 0 );
			m_focus = false;
		}
	}
	else
	{
		m_middleButton = true;
		_setFocus();
	}

	if ( m_ogre != NULL && m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseMDown( M_COORDS_FROM_POINT( p_point ) );
	}

	g_main = this;
}

void ElypsePluginInstance :: OnMButtonUp( const POINTS & p_point )
{
	VERBOSE( "OnMButtonUp" );

	if ( ! m_rightButton && ! m_leftButton )
	{
		ReleaseCapture();
	}

	m_middleButton = false;

	if ( m_ogre != NULL && m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseMUp( M_COORDS_FROM_POINT( p_point ) );
	}
}

void ElypsePluginInstance :: OnLButtonDown( const POINTS & p_point )
{
	VERBOSE( "OnLButtonDown" );

	if ( ! _contains( p_point ) )
	{
		if ( ! m_rightButton && ! m_middleButton )
		{
			SendMessage( m_windowHandle, WM_KILLFOCUS, 0, 0 );
			m_focus = false;
		}
	}
	else
	{
		m_leftButton = true;
		_setFocus();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseLDown( M_COORDS_FROM_POINT( p_point ) );
	}

	g_main = this;
}

void ElypsePluginInstance :: OnLButtonUp( const POINTS & p_point )
{
	VERBOSE( "OnLButtonUp" );

	if ( ! m_rightButton && ! m_middleButton )
	{
		ReleaseCapture();
	}

	m_leftButton = false;

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseLUp( M_COORDS_FROM_POINT( p_point ) );
	}
}

void ElypsePluginInstance :: OnRButtonDown( const POINTS & p_point )
{
	VERBOSE( "OnRButtonDown" );

	if ( ! _contains( p_point ) )
	{
		if ( ! m_leftButton && ! m_middleButton )
		{
			SendMessage( m_windowHandle, WM_KILLFOCUS, 0, 0 );
			m_focus = false;
		}
	}
	else
	{
		m_rightButton = true;
		_setFocus();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseRDown( M_COORDS_FROM_POINT( p_point ) );
	}

	g_main = this;
}

void ElypsePluginInstance :: OnRButtonUp( const POINTS & p_point )
{
	VERBOSE( "OnRButtonUp" );

	if ( ! m_leftButton && ! m_middleButton )
	{
		ReleaseCapture();
	}

	m_rightButton = false;

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseRUp( M_COORDS_FROM_POINT( p_point ) );
	}
}

void ElypsePluginInstance :: OnMouseWheel( short p_delta, const POINTS & p_point )
{
	VERBOSE( "OnMouseWheel" );

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseWheel( p_delta );
	}
}

void ElypsePluginInstance :: OnSysKeyUp( unsigned int p_key )
{
	VERBOSE( "OnSysKeyUp : " << p_key );

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->KeyUp( p_key );
	}
}

void ElypsePluginInstance :: OnSysKeyDown( unsigned int p_key, unsigned int p_flags, unsigned int p_repeat )
{
	VERBOSE( "OnSysKeyDown : " << p_key );

	if ( m_ogre->IsActive() )
	{
		if ( ( p_flags & KF_REPEAT ) == KF_REPEAT )
		{
			m_ogre->GetFL()->KeyRepeat( p_key, p_repeat );
		}
		else
		{
			m_ogre->GetFL()->KeyDown( p_key );
		}
	}
}

void ElypsePluginInstance :: OnKeyUp( unsigned int p_key )
{
	VERBOSE( "OnKeyUp : " << p_key );

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->KeyUp( p_key );
	}
}

void ElypsePluginInstance :: OnKeyDown( unsigned int p_key, unsigned int p_flags, unsigned int p_repeat )
{
	VERBOSE( "OnKeyDown : " << p_key );

	if ( m_ogre->IsActive() )
	{
		if ( ( p_flags & KF_REPEAT ) == KF_REPEAT )
		{
			m_ogre->GetFL()->KeyRepeat( p_key, p_repeat );
		}
		else
		{
			m_ogre->GetFL()->KeyDown( p_key );
		}
	}
}
