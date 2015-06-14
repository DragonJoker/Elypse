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

#include "AnimationGroup.h"
#include "AnimatedObject.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	TrollAnimationGroup::TrollAnimationGroup( const wxString & p_name, const wxString & p_fileName )
		: TemporalObject( p_name, p_fileName, ttAnimationGroup )
	{
	}

	TrollAnimationGroup::~TrollAnimationGroup()
	{
		General::Utils::map::deleteAll( m_objects );
	}

	TrollAnimatedObject * TrollAnimationGroup::CreateAnimatedObject( Object * p_object, const wxString & p_fileName )
	{
		TrollAnimatedObjectStrMap::iterator l_it = m_objects.find( p_object->GetObjectName() );

		if ( l_it != m_objects.end() )
		{
			return l_it->second;
		}

		TrollAnimatedObject * l_object = new TrollAnimatedObject( p_object, this, m_fileName );
		m_objects.insert( std::make_pair( p_object->GetObjectName(), l_object ) );
		return l_object;
	}

	int TrollAnimationGroup::BuildPanel( wxWindow * p_parent, int p_width )
	{
		return 0;
	}

	void TrollAnimationGroup::AddChainedAnimations( const wxString & p_anim1, const wxString & p_anim2 )
	{
		m_chainedAnimations.insert( std::make_pair( p_anim1, p_anim2 ) );
	}

	void TrollAnimationGroup::Write( wxTextOutputStream * p_stream )
	{
		p_stream->WriteString( wxT( "animation_group " ) + m_name + wxT( "\n{\n" ) );
		General::Utils::map::cycle( m_objects, & TrollAnimatedObject::Write, p_stream );

		if ( m_chainedAnimations.size() > 0 )
		{
			p_stream->WriteString( wxT( "\n" ) );
			StrStrMap::iterator l_it = m_chainedAnimations.begin();

			for ( ; l_it != m_chainedAnimations.end() ; ++l_it )
			{
				p_stream->WriteString( wxT( "\tchain_animations " ) + l_it->first + wxT( " " ) + l_it->second + wxT( "\n" ) );
			}
		}

		p_stream->WriteString( wxT( "}\n\n" ) );
	}
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE
