
#include "PrecompiledHeader.h"
#include "GUI/ObjectProperties/ObjectProperties.h"

using namespace Troll::GUI;


ObjectProperties :: ObjectProperties( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
	:	wxPanel( parent, wxID_ANY, pos, size, style ),
		m_updatingProperties( false )
{
}



ObjectProperties :: ~ObjectProperties()
{
}


