
#include "PrecompiledHeader.h"

#include "Project/Temporal/TemporalObject.h"
#include "GUI/MainFrame.h"

using namespace Troll;
using namespace Troll::GUI;
using namespace Troll::Temporal;


TemporalObject :: TemporalObject( const wxString & p_name, const wxString & p_fileName, TemporalObjectType p_type )
	:	TrollObject( p_name, p_fileName, ttTemporalObject ),
		m_temporalObjectType( p_type )
{
	wxString l_msg = wxT( "Creating Temporal object : " );

	if ( p_type == ttAnimationGroup )
	{
		l_msg += wxT( "Animation Group - " );
	}
	else if ( p_type == ttAnimatedObject )
	{
		l_msg += wxT( "Animated Object - " );
	}
	else if ( p_type == ttAnimation )
	{
		l_msg += wxT( "Animation - " );
	}

	l_msg += wxT( "Name : " ) + p_name;
	g_mainFrame->LogDebugMessage( l_msg );
}



TemporalObject :: ~TemporalObject()
{
}


