/*
See LICENSE file in root folder
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
