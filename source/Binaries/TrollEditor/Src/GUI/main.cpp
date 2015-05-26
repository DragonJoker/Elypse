//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/main.h"
#include "GUI/MainFrame.h"
/*
#include <General/Memory.h>
*/
//***********************************************************************************************************
using namespace Troll::GUI;
//***********************************************************************************************************
IMPLEMENT_APP( TEApplication )
//***********************************************************************************************************

TEApplication :: TEApplication()
	:	wxApp(),
		m_mainFrame( NULL )
{
}

//***********************************************************************************************************

TEApplication :: ~TEApplication()
{
}

//***********************************************************************************************************

bool TEApplication :: OnInit()
{
	m_mainFrame = new MainFrame( _T( "Troll Editor" ), 50, 50, 800, 600 );
	m_mainFrame->Show();
	m_mainFrame->Maximize();

	if ( argc > 1 )
	{
		m_mainFrame->OpenProjectFile( argv[1] );
	}

	SetTopWindow( m_mainFrame );
	SetExitOnFrameDelete( true );
	return true;
}

//***********************************************************************************************************
