#include "PrecompiledHeader.h"

#include "GUI/ElypseFrame.h"
#include "GUI/ElypseCtrl.h"
/*
#include <io.h>

#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/log.h>

#include <EMuseLogs.h>
#include <Main/EMuseController.h>
#include <Main/EMuseFrameListener.h>
#include <Physics/BoundingMesh.h>
#include <ScriptEngine.h>
#include <Main/Context.h>
#include <GUI/EMGui.h>
#include <GUI/EMOverlay.h>
#include <GUI/EMOverlayGroup.h>
*/
//*****************************************************************************************************************
using namespace Troll;
using namespace Troll::Main;
using namespace Troll::GUI;
//*****************************************************************************************************************
BEGIN_EVENT_TABLE( ElypseFrame, wxFrame )
	EVT_SET_FOCUS(	ElypseFrame::OnSetFocus )
	EVT_KILL_FOCUS(	ElypseFrame::OnKillFocus )
	EVT_SHOW(	ElypseFrame::OnShow )
	EVT_ACTIVATE(	ElypseFrame::OnActivate )

	EVT_ICONIZE(	ElypseFrame::_onIconise )
	EVT_LEFT_DOWN(	ElypseFrame::_onMouseLeftDown )
	EVT_LEFT_UP(	ElypseFrame::_onMouseLeftUp )
	EVT_MIDDLE_DOWN(	ElypseFrame::_onMouseMiddleDown )
	EVT_MIDDLE_UP(	ElypseFrame::_onMouseMiddleUp )
	EVT_RIGHT_DOWN(	ElypseFrame::_onMouseRightDown )
	EVT_RIGHT_UP(	ElypseFrame::_onMouseRightUp )
	EVT_LEFT_DCLICK(	ElypseFrame::_onMouseLeftDown )
	EVT_MIDDLE_DCLICK(	ElypseFrame::_onMouseMiddleDown )
	EVT_RIGHT_DCLICK(	ElypseFrame::_onMouseRightDown )
	EVT_MOUSEWHEEL(	ElypseFrame::_onMouseWheel )
	EVT_MOTION(	ElypseFrame::_onMouseMove )
	EVT_KEY_DOWN(	ElypseFrame::_onKeyDown )
	EVT_KEY_UP(	ElypseFrame::_onKeyUp )
	EVT_CHAR(	ElypseFrame::_onChar )
	EVT_ENTER_WINDOW(	ElypseFrame::_onMouseEnter )
	EVT_LEAVE_WINDOW(	ElypseFrame::_onMouseLeave )
END_EVENT_TABLE()
//*****************************************************************************************************************

void ElypseFrame :: OnSetFocus( wxFocusEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnSetFocus( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: OnKillFocus( wxFocusEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnKillFocus( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: OnShow( wxShowEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnShow( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: OnActivate( wxActivateEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnActivate( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onIconise( wxIconizeEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnIconise( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseLeftDown( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnButtonLeftDown( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseLeftUp( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnButtonLeftUp( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseMiddleDown( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnButtonMiddleDown( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseMiddleUp( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnButtonMiddleUp( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseRightDown( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnButtonRightDown( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseRightUp( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnButtonRightUp( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseWheel( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnMouseWheel( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseMove( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnMouseMove( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseEnter( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnMouseEnter( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onMouseLeave( wxMouseEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnMouseLeave( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onKeyDown( wxKeyEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnKeyDown( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onKeyUp( wxKeyEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnKeyUp( p_event );
	}
}

//*****************************************************************************************************************

void ElypseFrame :: _onChar( wxKeyEvent & p_event )
{
	if ( m_elypseCtrl != NULL )
	{
		m_elypseCtrl->OnChar( p_event );
	}
}

//***********************************************************************************************************
