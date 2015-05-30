
#include "PrecompiledHeader.h"

#include "Project/Object.h"
#include "GUI/MainFrame.h"
/*
#include <wx/log.h>
*/

using namespace Troll;
using namespace Troll::GUI;

extern MainFrame * g_mainFrame;


TrollObject::TrollObject( const wxString & p_name, const wxString & p_fileName, ObjectType p_type )
	:	TEDisplayable(),
		m_name( p_name ),
		m_state( Normal ),
		m_visible( true ),
		m_objectType( p_type ),
		m_fileName( p_fileName )
{
}


