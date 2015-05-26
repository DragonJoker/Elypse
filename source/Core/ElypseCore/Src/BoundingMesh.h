#ifndef ___BOUNDING_MESH_H___
#define ___BOUNDING_MESH_H___

#include "BoundingShape.h"

#include <ode/collision_trimesh.h>

namespace EMuse
{
	namespace Physics
	{
		class d_dll_export BoundingMesh : public BoundingShape
		{
		private:
			Ogre::Mesh * m_mesh;
			Ogre::Vector3 * m_vertices;
			Ode::dVector3 * m_odeVertices;

			unsigned int * m_odeIndices;
			unsigned int * m_indices;

			unsigned int m_indexCount;
			unsigned int m_vertexCount;
			Ode::dTriMeshDataID m_triID;

		public:
			BoundingMesh( Ogre::Mesh * p_mesh );
			~BoundingMesh();

		private:
			void _addVertexData( Ogre::VertexData * p_data );
			void _addIndexData( Ogre::IndexData * p_data, unsigned int p_offset = 0 );
			void _createTriMesh();

			virtual BoundingShape * Clone();
		};
	}
}

#endif
