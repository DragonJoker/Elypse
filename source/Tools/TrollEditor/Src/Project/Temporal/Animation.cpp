/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "TrollEditorPch.h"

#include "Animation.h"

using namespace Elypse::Animation;

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	TrollAnimation::TrollAnimation( const wxString & p_name, const wxString & p_fileName )
		: TemporalObject( p_name, p_fileName, ttAnimation )
		, m_looped( false )
		, m_timein( 0.0 )
		, m_timeout( 0.0 )
	{
	}

	TrollAnimation::~TrollAnimation()
	{
	}

	int TrollAnimation::BuildPanel( wxWindow * p_parent, int p_width )
	{
		return 0;
	}

	void TrollAnimation::SetMuseAnimation( EMAnimation * p_animation )
	{
		m_museAnimation = p_animation;
		m_looped = p_animation->IsLooped();
		m_timein = p_animation->GetTimeIn();
		m_timeout = p_animation->GetTimeOut();
	}

	void TrollAnimation::Write( wxTextOutputStream * p_stream )
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
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE
