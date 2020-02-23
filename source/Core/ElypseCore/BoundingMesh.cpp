/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "BoundingMesh.h"

#include <OgreEdgeListBuilder.h>
#include <OgreVertexIndexData.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

using namespace Ogre;

BoundingMesh::BoundingMesh( Mesh * p_mesh )
	: BoundingShape( COC_TriangleMesh )
	, m_mesh( p_mesh )
	, m_vertices( nullptr )
	, m_odeVertices( nullptr )
	, m_odeIndices( nullptr )
	, m_indices( nullptr )
	, m_indexCount( 0 )
	, m_vertexCount( 0 )
{
	genlib_assert( m_mesh != nullptr );

	for ( uint32_t i = 0 ; i < m_mesh->getNumSubMeshes() ; i ++ )
	{
		if ( m_mesh->getSubMesh( i )->useSharedVertices )
		{
			_addVertexData( m_mesh->sharedVertexData );
			break;
		}
	}

	for ( uint32_t i = 0 ; i < m_mesh->getNumSubMeshes() ; i ++ )
	{
		SubMesh * l_subMesh = m_mesh->getSubMesh( i );

		if ( ! l_subMesh->useSharedVertices )
		{
			_addIndexData( l_subMesh->indexData, m_vertexCount );
			_addVertexData( l_subMesh->vertexData );
		}
		else
		{
			_addIndexData( l_subMesh->indexData );
		}
	}

	_createTriMesh();
}

BoundingMesh::~BoundingMesh()
{
	if ( m_indices != nullptr )
	{
		delete [] m_indices;
	}

	if ( m_vertices != nullptr )
	{
		delete [] m_vertices;
	}

	if ( m_odeIndices != nullptr )
	{
		delete [] m_odeIndices;
	}

	if ( m_odeVertices != nullptr )
	{
		delete [] m_odeVertices;
	}

	dGeomTriMeshDataDestroy( m_triID );
}

void BoundingMesh::_addVertexData( VertexData * p_data )
{
	genlib_assert( p_data != nullptr );
	uint32_t l_previousSize = m_vertexCount;
	m_vertexCount += static_cast <uint32_t>( p_data->vertexCount );
	Vector3 * l_vertices = new Vector3[m_vertexCount];

	if ( m_vertices )
	{
		memcpy( l_vertices, m_vertices, sizeof( Vector3 ) * l_previousSize );
		delete [] m_vertices;
	}

	m_vertices = l_vertices;
	// Get the positional buffer element
	{
		const VertexElement * l_positionElements = p_data->vertexDeclaration->findElementBySemantic( VES_POSITION );
		HardwareVertexBufferSharedPtr l_buffer = p_data->vertexBufferBinding->getBuffer( l_positionElements->getSource() );
		uint32_t l_size = static_cast <uint32_t>( l_buffer->getVertexSize() );
		unsigned char * l_ptr = static_cast <unsigned char *>( l_buffer->lock( HardwareBuffer::HBL_READ_ONLY ) );
		float * pReal;
		Vector3 * l_vertexPtr = &m_vertices[l_previousSize];
		uint32_t l_count = static_cast <uint32_t>( p_data->vertexCount );

		for ( uint32_t j = 0 ; j < l_count ; j ++ )
		{
			l_positionElements->baseVertexPointerToElement( l_ptr, & pReal );
			l_vertexPtr->x = ( *pReal++ );
			l_vertexPtr->y = ( *pReal++ );
			l_vertexPtr->z = ( *pReal++ );
			l_ptr += l_size;
			l_vertexPtr++;
		}

		l_buffer->unlock();
	}
}

void BoundingMesh::_addIndexData( IndexData * p_data, uint32_t p_offset )
{
	if ( ! p_data )
	{
		return;
	}

	uint32_t l_previousSize = m_indexCount;
	m_indexCount += static_cast <uint32_t>( p_data->indexCount );
	uint32_t * l_indices = new uint32_t[m_indexCount];

	if ( m_indices )
	{
		memcpy( l_indices, m_indices, sizeof( uint32_t ) * l_previousSize );
		delete [] m_indices;
	}

	m_indices = l_indices;
	uint32_t l_numTris = static_cast <uint32_t>( p_data->indexCount / 3 );
	HardwareIndexBufferSharedPtr l_buffer = p_data->indexBuffer;
	const bool use32bitindexes = ( l_buffer->getType() == HardwareIndexBuffer::IT_32BIT );
	uint32_t l_offset = l_previousSize;

	if ( use32bitindexes )
	{
		const uint32_t * pInt = static_cast <uint32_t *>( l_buffer->lock( HardwareBuffer::HBL_READ_ONLY ) );

		for ( uint32_t i = 0 ; i < l_numTris ; i ++ )
		{
			m_indices[l_offset ++] = p_offset + *pInt++;
			m_indices[l_offset ++] = p_offset + *pInt++;
			m_indices[l_offset ++] = p_offset + *pInt++;
		}
	}
	else
	{
		const uint16_t * pShort = static_cast <uint16_t *>( l_buffer->lock( HardwareBuffer::HBL_READ_ONLY ) );

		for ( uint32_t i = 0 ; i < l_numTris ; i ++ )
		{
			m_indices[l_offset ++] = p_offset + static_cast<uint32_t>( *pShort++ );
			m_indices[l_offset ++] = p_offset + static_cast<uint32_t>( *pShort++ );
			m_indices[l_offset ++] = p_offset + static_cast<uint32_t>( *pShort++ );
		}
	}

	l_buffer->unlock();
}

void BoundingMesh::_createTriMesh()
{
	m_odeVertices = new dVector3[m_vertexCount];
	m_odeIndices = new uint32_t[m_indexCount];

	for ( uint32_t i = 0 ; i < m_vertexCount ; i ++ )
	{
		m_odeVertices[i][0] = static_cast <dReal>( m_vertices[i].x );
		m_odeVertices[i][1] = static_cast <dReal>( m_vertices[i].y );
		m_odeVertices[i][2] = static_cast <dReal>( m_vertices[i].z );
	}

	memcpy( m_odeIndices, m_indices, sizeof( uint32_t ) * m_indexCount );
	m_triID = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple( m_triID, m_odeVertices[0], static_cast< int >( m_vertexCount ), reinterpret_cast <dTriIndex *>( m_odeIndices ), static_cast< int >( m_indexCount ) );
	m_geom = dCreateTriMesh( 0, m_triID, 0, 0, 0 );
	_register();
}

BoundingShape * BoundingMesh::Clone()
{
	return new BoundingMesh( m_mesh );
}
