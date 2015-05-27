
#include "PrecompiledHeader.h"

#include "Project/Temporal/Animation.h"
/*
#include <Animation/Animation.h>
*/

using namespace Troll::Temporal;
using namespace EMuse::Animation;


TrollAnimation :: TrollAnimation( const wxString & p_name, const wxString & p_fileName )
	:	TemporalObject( p_name, p_fileName, ttAnimation ),
		m_looped( false ),
		m_timein( 0.0 ),
		m_timeout( 0.0 )
{
}



TrollAnimation :: ~TrollAnimation()
{
}



int TrollAnimation :: BuildPanel( wxWindow * p_parent, int p_width )
{
	return 0;
}



void TrollAnimation :: SetMuseAnimation( EMAnimation * p_animation )
{
	m_museAnimation = p_animation;
	m_looped = p_animation->IsLooped();
	m_timein = p_animation->GetTimeIn();
	m_timeout = p_animation->GetTimeOut();
}



void TrollAnimation :: Write( wxTextOutputStream * p_stream )
{
	wxString l_tmp;
	p_stream->WriteString( wxT( "\t\tanimation " ) + m_name + wxT( "\n\t\t{\n" ) );

	if ( m_looped )
	{
		p_stream->WriteString( wxT( "\t\t\tlooped true\n" ) );
	}

	if ( m_timein != 0.0 )
	{
		l_tmp << m_timein;
		p_stream->WriteString( wxT( "\t\t\ttimein " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_timeout != 0.0 )
	{
		l_tmp << m_timeout;
		p_stream->WriteString( wxT( "\t\t\ttimein " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	p_stream->WriteString( wxT( "\t\t}\n" ) );
}


