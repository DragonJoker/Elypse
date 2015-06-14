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

#include "3DObject.h"

#include "SceneNode.h"

BEGIN_TROLL_PROJECT_3D_NAMESPACE
{
	Troll3DObject::Troll3DObject( const wxString & p_name, const wxString & p_fileName, Troll3DObjectType p_type )
		: Object( p_name, p_fileName, tt3DObject )
		, m_parent( NULL )
		, m_3DObjectType( p_type )
	{
		wxString l_msg = wxT( "Creating 3D object : " );

		if ( p_type == ttNode )
		{
			l_msg += wxT( "TrollSceneNode - " );
		}
		else if ( p_type == ttEntity )
		{
			l_msg += wxT( "Entity - " );
		}
		else if ( p_type == ttLight )
		{
			l_msg += wxT( "Light - " );
		}
		else if ( p_type == ttCamera )
		{
			l_msg += wxT( "Camera - " );
		}

		l_msg += wxT( "Name : " ) + p_name;
		LogMessage( l_msg );
	}

	Troll3DObject::~Troll3DObject()
	{
		if ( m_parent != NULL )
		{
			Detach();
		}
	}

	void Troll3DObject::AttachTo( TrollSceneNode * p_node )
	{
		if ( m_parent )
		{
			Detach();
		}

		if ( ! p_node )
		{
			return;
		}

		m_parent = p_node;
	}

	void Troll3DObject::Detach()
	{
		if ( m_parent == NULL )
		{
			return;
		}

		if ( m_3DObjectType != ttNode )
		{
			m_parent->RemoveObject( m_name );
		}
		else
		{
			m_parent->RemoveChild( m_name );
		}

		m_parent = NULL;
	}
}
END_TROLL_PROJECT_3D_NAMESPACE
