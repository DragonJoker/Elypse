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

#include "Mesh.h"

BEGIN_TROLL_PROJECT_3D_NAMESPACE
{
	TrollMesh::TrollMesh( const wxString & p_meshName )
		: m_meshName( p_meshName )
	{
	}

	TrollMesh::~TrollMesh()
	{
		m_subMaterials.clear();
	}

	wxString TrollMesh::GetMaterial( unsigned int p_index )
	{
		if ( p_index >= m_subMaterials.size() )
		{
			return wxString();
		}

		return m_subMaterials[p_index];
	}

	void TrollMesh::AddSubMaterial( const wxString & p_name )
	{
		m_subMaterials[m_subMaterials.size()] = p_name;
	}

	void TrollMesh::SetSubMaterial( const wxString & p_name, unsigned int p_index )
	{
		if ( p_index >= m_subMaterials.size() )
		{
			return;
		}

		m_subMaterials[p_index] = p_name;
	}
}
END_TROLL_PROJECT_3D_NAMESPACE
