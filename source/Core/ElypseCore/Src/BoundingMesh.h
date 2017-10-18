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
#ifndef ___BOUNDING_MESH_H___
#define ___BOUNDING_MESH_H___

#include "BoundingShape.h"

#include <ode/collision_trimesh.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export BoundingMesh : public BoundingShape
		{
		private:
			Ogre::Mesh * m_mesh;
			Ogre::Vector3 * m_vertices;
			Ode::dVector3 * m_odeVertices;

			uint32_t * m_odeIndices;
			uint32_t * m_indices;

			uint32_t m_indexCount;
			uint32_t m_vertexCount;
			Ode::dTriMeshDataID m_triID;

		public:
			BoundingMesh( Ogre::Mesh * p_mesh );
			~BoundingMesh();

		private:
			void _addVertexData( Ogre::VertexData * p_data );
			void _addIndexData( Ogre::IndexData * p_data, uint32_t p_offset = 0 );
			void _createTriMesh();

			virtual BoundingShape * Clone();
		};
	}
}

#endif
