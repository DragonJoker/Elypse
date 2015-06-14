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

#include "AnimatedObject.h"

#include "Animation.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	TrollAnimatedObject::TrollAnimatedObject( Object * p_object, TrollAnimationGroup * p_group, const wxString & p_fileName )
		: TemporalObject( p_object->GetObjectName(), p_fileName, ttAnimatedObject )
		, m_object( p_object )
		, m_group( p_group )
	{
	}

	TrollAnimatedObject::~TrollAnimatedObject()
	{
	}

	int TrollAnimatedObject::BuildPanel( wxWindow * p_parent, int p_width )
	{
		return 0;
	}

	void TrollAnimatedObject::AddAnimation( TrollAnimation * p_anim )
	{
		m_animations.insert( std::make_pair( p_anim->GetObjectName(), p_anim ) );
	}

	void TrollAnimatedObject::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "\tadd_object " ) + m_name + wxT( "\n" ) );

		if ( m_animations.size() > 0 )
		{
			p_stream->WriteString( wxT( "\t{\n" ) );
			std::for_each( m_animations.begin(), m_animations.end(), [&]( std::pair< wxString, TrollAnimation * > p_pair )
			{
				p_pair.second->Write( p_stream );
			} );
			p_stream->WriteString( wxT( "\t}\n" ) );
		}
	}
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE
