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

#include "TemporalObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TemporalObject::TemporalObject( wxString const & p_name, wxString const & p_fileName, TemporalObjectType p_type )
				: Object( p_name, p_fileName, ProjectComponents::ttTemporalObject )
				, m_temporalObjectType( p_type )
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
				LogMessage( l_msg );
			}

			TemporalObject::~TemporalObject()
			{
			}
		}
	}
}
