#include "ElypseOcxPch.h"

#include "ElypseOcxCtrl.h"

#include <ElypseLogs.h>
#include <ElypseInstance.h>
#include <ElypseFrameListener.h>

#include <MinMax.h>

#define M_EXTRACT_COORDS( X ) Real( X.x ), Real( X.y )
#define M_CHECK_COORDS( X) X.x < 0 || X.y < 0 || X.x > m_width || X.y > m_height

LRESULT CALLBACK ElypseOcxCtrl::MessageProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	LPMSG lpMsg = ( LPMSG ) lParam;

	if ( ElypseOcxCtrl::GetSingletonPtr() == NULL )
	{
		return FALSE;
	}

	if ( ! ElypseOcxCtrl::GetSingletonPtr()->m_focused )
	{
		return ::CallNextHookEx( ElypseOcxCtrl::GetSingletonPtr()->GetHook(), nCode, wParam, lParam );
	}

	do
	{
		if ( ( nCode >= 0 ) && PM_REMOVE == wParam && lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST )
		{
			if ( lpMsg->message == WM_KEYDOWN  && lpMsg->wParam != 116  && lpMsg->wParam != 122 )
			{
				TranslateMessage( lpMsg );
				ElypseOcxCtrl::GetSingletonPtr()->OnKeyDown( UINT( lpMsg->wParam ), HIWORD( lpMsg->lParam ), LOWORD( lpMsg->lParam ) );
				break;
			}

			if ( lpMsg->message == WM_KEYUP  && lpMsg->wParam != 116  && lpMsg->wParam != 122 )
			{
				ElypseOcxCtrl::GetSingletonPtr()->OnKeyUp( UINT( lpMsg->wParam ), HIWORD( lpMsg->lParam ), LOWORD( lpMsg->lParam ) );
				break;

			}

			if ( lpMsg->message == WM_SYSKEYDOWN )
			{
				ElypseOcxCtrl::GetSingletonPtr()->OnSysKeyDown( UINT( lpMsg->wParam ), HIWORD( lpMsg->lParam ), LOWORD( lpMsg->lParam ) );
				break;
			}

			if ( lpMsg->message == WM_SYSKEYUP )
			{
				ElypseOcxCtrl::GetSingletonPtr()->OnSysKeyUp( UINT( lpMsg->wParam ), HIWORD( lpMsg->lParam ), LOWORD( lpMsg->lParam ) );
				break;
			}
		}

		return ::CallNextHookEx( ElypseOcxCtrl::GetSingletonPtr()->GetHook(), nCode, wParam, lParam );

	}
	while ( false );

	lpMsg->message = WM_NULL;
	lpMsg->lParam = 0L;
	lpMsg->wParam = 0;

	return ::CallNextHookEx( ElypseOcxCtrl::GetSingletonPtr()->GetHook(), nCode, wParam, lParam );
}


void ElypseOcxCtrl::OnDraw( CDC *, const CRect &, const CRect & )
{
}

void ElypseOcxCtrl::OnPaint()
{
	if ( m_plugin != NULL )
	{
		m_plugin->Paint();
	}

	CWnd::OnPaint();
}

void ElypseOcxCtrl::OnKillFocus( CWnd * p_newWnd )
{
	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::OnKillFocus" );

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->ResetAll();
	}

	if ( m_ogre != NULL )
	{
		m_ogre->SetFocus( false );
	}

	CWnd::OnKillFocus( p_newWnd );
	COleControl::ReleaseCapture();

	m_focused = false;
}

void ElypseOcxCtrl::OnSetFocus( CWnd * p_newWnd )
{
	EMUSE_MESSAGE_DEBUG( "CElypseOcxCtrl::OnSetFocus" );

	CWnd::OnSetFocus( p_newWnd );

	SetCapture();
	m_focused = true;

	if ( m_ogre != NULL )
	{
		m_ogre->SetFocus( true );
	}

	m_oneTimePostSetFocus = true;

	SetSingleton( this );
}

void ElypseOcxCtrl::OnMouseMove( UINT, CPoint p_coords )
{
	if ( ! m_ogre->IsActive() || ! m_focused )
	{
		return;
	}

	if ( m_oneTimePostSetFocus )
	{
		m_oneTimePostSetFocus = false;
		m_ogre->GetFL()->SetMouse( M_EXTRACT_COORDS( p_coords ) );
	}

	m_ogre->GetFL()->MouseMove( M_EXTRACT_COORDS( p_coords ) );
}

void ElypseOcxCtrl::OnMButtonDown( UINT, CPoint p_coords )
{
	if ( M_CHECK_COORDS( p_coords ) )
	{
		if ( ! m_rightButton && ! m_leftButton )
		{
			SendMessage( WM_KILLFOCUS );
		}
	}
	else
	{
		m_middleButton = true;

		if ( this->GetFocus() != this )
		{
			SetFocus();
		}

		SetCapture();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseMDown( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnMButtonDblClk( UINT, CPoint p_coords )
{
	if ( M_CHECK_COORDS( p_coords ) )
	{
		if ( ! m_rightButton && ! m_leftButton )
		{
			SendMessage( WM_KILLFOCUS );
		}
	}
	else
	{
		m_middleButton = true;

		if ( this->GetFocus() != this )
		{
			SetFocus();
		}

		SetCapture();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseMDown( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnMButtonUp( UINT , CPoint p_coords )
{
	if ( ! m_rightButton && ! m_leftButton )
	{
		ReleaseCapture();
	}

	m_middleButton = false;

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseMUp( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnLButtonDown( UINT , CPoint p_coords )
{
	if ( M_CHECK_COORDS( p_coords ) )
	{
		if ( ! m_rightButton && ! m_middleButton )
		{
			SendMessage( WM_KILLFOCUS );
		}
	}
	else
	{
		m_leftButton = true;

		if ( this->GetFocus() != this )
		{
			SetFocus();
		}

		SetCapture();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseLDown( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnLButtonDblClk( UINT nHitTest, CPoint p_coords )
{
	OnLButtonDown( nHitTest, p_coords );
}

void ElypseOcxCtrl::OnLButtonUp( UINT , CPoint p_coords )
{
	if ( ! m_rightButton && ! m_middleButton )
	{
		ReleaseCapture();
	}

	m_leftButton = false;

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseLUp( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnRButtonDblClk( UINT , CPoint p_coords )
{
	if ( M_CHECK_COORDS( p_coords ) )
	{
		if ( ! m_leftButton && ! m_middleButton )
		{
			SendMessage( WM_KILLFOCUS );
//			std::cout<<"killfoc";
		}
	}
	else
	{
		m_rightButton = true;

		if ( this->GetFocus() != this )
		{
			SetFocus();
		}

		SetCapture();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseRDown( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnRButtonDown( UINT , CPoint p_coords )
{
	if ( M_CHECK_COORDS( p_coords ) )
	{
		if ( ! m_leftButton && ! m_middleButton )
		{
			SendMessage( WM_KILLFOCUS );
//			std::cout<<"killfoc";
		}
	}
	else
	{
		m_rightButton = true;

		if ( this->GetFocus() != this )
		{
			SetFocus();
		}

		SetCapture();
	}

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseRDown( M_EXTRACT_COORDS( p_coords ) );
	}
}

void ElypseOcxCtrl::OnRButtonUp( UINT , CPoint p_coords )
{
	if ( ! m_leftButton && ! m_middleButton )
	{
		ReleaseCapture();
	}

	m_rightButton = false;

	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseRUp( M_EXTRACT_COORDS( p_coords ) );
	}
}


BOOL ElypseOcxCtrl::OnMouseWheel( UINT , short zdelta, CPoint )
{
	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->MouseWheel( zdelta );
	}

	return 1;
}

void ElypseOcxCtrl::OnSysKeyUp( UINT key, UINT, UINT )
{
//	std::cout<<"OnSysKeyUp : "<<key<<"\n";
	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->KeyUp( key );
	}
}

void ElypseOcxCtrl::OnSysKeyDown( UINT key, UINT flags, UINT repeat )
{
//	std::cout<<"OnSysKeyDown : "<<key<<"\n";
	if ( m_ogre->IsActive() )
	{
		if ( ( flags & KF_REPEAT ) == KF_REPEAT )
		{
			m_ogre->GetFL()->KeyRepeat( key, repeat );
		}
		else
		{
			m_ogre->GetFL()->KeyDown( key );
		}
	}
}

void ElypseOcxCtrl::OnKeyUp( UINT p_key, UINT, UINT )
{
//	std::cout<<"OnKeyUp : "<<p_key<<"\n";
	if ( m_ogre->IsActive() )
	{
		m_ogre->GetFL()->KeyUp( p_key );
	}
}

void ElypseOcxCtrl::OnKeyDown( UINT p_key, UINT p_flags, UINT p_repeat )
{
//	std::cout<<"OnKeyDown : "<<p_key<<"\n";
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

void ElypseOcxCtrl::OnChar( UINT p_key, UINT p_flags, UINT p_repeat )
{
	/*
	if (p_key == 8 || p_key == 13)
	{
		OnKeyDown( p_key, p_flags, p_repeat);
		return;
	}
	*/

	if ( m_ogre->IsActive() )
	{
		if ( ( p_flags & KF_REPEAT ) == KF_REPEAT )
		{
			m_ogre->GetFL()->OnCharRepeat( p_key, p_repeat );
		}
		else
		{
			m_ogre->GetFL()->OnChar( p_key );
		}
	}
}

void ElypseOcxCtrl::OnDeadChar( UINT p_key, UINT p_flags, UINT p_repeat )
{
	if ( m_ogre->IsActive() )
	{
		if ( ( p_flags & KF_REPEAT ) == KF_REPEAT )
		{
			m_ogre->GetFL()->OnCharRepeat( p_key, p_repeat );
		}
		else
		{
			m_ogre->GetFL()->OnChar( p_key );
		}
	}
}

