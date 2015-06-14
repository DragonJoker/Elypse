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
#include "PrecompiledHeader.h"

#include "BoundingMesh.h"

#include <OgreEdgeListBuilder.h>
#include <OgreVertexIndexData.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

using namespace Ogre;

BoundingMesh::BoundingMesh( Mesh * p_mesh )
	: BoundingShape( COC_TriangleMesh ),
		m_mesh( p_mesh ),
		m_vertices( NULL ),
		m_odeVertices( NULL ),
		m_odeIndices( NULL ),
		m_indices( NULL ),
		m_indexCount( 0 ),
		m_vertexCount( 0 )
{
	genlib_assert( m_mesh != NULL );

	for ( unsigned int i = 0 ; i < m_mesh->getNumSubMeshes() ; i ++ )
	{
		if ( m_mesh->getSubMesh( i )->useSharedVertices )
		{
			_addVertexData( m_mesh->sharedVertexData );
			break;
		}
	}

	for ( unsigned int i = 0 ; i < m_mesh->getNumSubMeshes() ; i ++ )
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
	if ( m_indices != NULL )
	{
		delete [] m_indices;
	}

	if ( m_vertices != NULL )
	{
		delete [] m_vertices;
	}

	if ( m_odeIndices != NULL )
	{
		delete [] m_odeIndices;
	}

	if ( m_odeVertices != NULL )
	{
		delete [] m_odeVertices;
	}

	dGeomTriMeshDataDestroy( m_triID );
}

void BoundingMesh::_addVertexData( VertexData * p_data )
{
	genlib_assert( p_data != NULL );
	unsigned int l_previousSize = m_vertexCount;
	m_vertexCount += static_cast <unsigned int>( p_data->vertexCount );
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
		unsigned int l_size = static_cast <unsigned int>( l_buffer->getVertexSize() );
		unsigned char * l_ptr = static_cast <unsigned char *>( l_buffer->lock( HardwareBuffer::HBL_READ_ONLY ) );
		float * pReal;
		Vector3 * l_vertexPtr = &m_vertices[l_previousSize];
		unsigned int l_count = static_cast <unsigned int>( p_data->vertexCount );

		for ( unsigned int j = 0 ; j < l_count ; j ++ )
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

void BoundingMesh::_addIndexData( IndexData * p_data, unsigned int p_offset )
{
	if ( ! p_data )
	{
		return;
	}

	unsigned int l_previousSize = m_indexCount;
	m_indexCount += static_cast <unsigned int>( p_data->indexCount );
	unsigned int * l_indices = new unsigned int[m_indexCount];

	if ( m_indices )
	{
		memcpy( l_indices, m_indices, sizeof( unsigned int ) * l_previousSize );
		delete [] m_indices;
	}

	m_indices = l_indices;
	unsigned int l_numTris = static_cast <unsigned int>( p_data->indexCount / 3 );
	HardwareIndexBufferSharedPtr l_buffer = p_data->indexBuffer;
	const bool use32bitindexes = ( l_buffer->getType() == HardwareIndexBuffer::IT_32BIT );
	unsigned int l_offset = l_previousSize;

	if ( use32bitindexes )
	{
		const unsigned int * pInt = static_cast <unsigned int *>( l_buffer->lock( HardwareBuffer::HBL_READ_ONLY ) );

		for ( unsigned int i = 0 ; i < l_numTris ; i ++ )
		{
			m_indices[l_offset ++] = p_offset + *pInt++;
			m_indices[l_offset ++] = p_offset + *pInt++;
			m_indices[l_offset ++] = p_offset + *pInt++;
		}
	}
	else
	{
		const unsigned short * pShort = static_cast <unsigned short *>( l_buffer->lock( HardwareBuffer::HBL_READ_ONLY ) );

		for ( unsigned int i = 0 ; i < l_numTris ; i ++ )
		{
			m_indices[l_offset ++] = p_offset + static_cast<unsigned int>( *pShort++ );
			m_indices[l_offset ++] = p_offset + static_cast<unsigned int>( *pShort++ );
			m_indices[l_offset ++] = p_offset + static_cast<unsigned int>( *pShort++ );
		}
	}

	l_buffer->unlock();
}

void BoundingMesh::_createTriMesh()
{
	m_odeVertices = new dVector3[m_vertexCount];
	m_odeIndices = new unsigned int[m_indexCount];

	for ( unsigned int i = 0 ; i < m_vertexCount ; i ++ )
	{
		m_odeVertices[i][0] = static_cast <dReal>( m_vertices[i].x );
		m_odeVertices[i][1] = static_cast <dReal>( m_vertices[i].y );
		m_odeVertices[i][2] = static_cast <dReal>( m_vertices[i].z );
	}

	memcpy( m_odeIndices, m_indices, sizeof( unsigned int ) * m_indexCount );
	m_triID = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple( m_triID, m_odeVertices[0], static_cast< int >( m_vertexCount ), reinterpret_cast <dTriIndex *>( m_odeIndices ), static_cast< int >( m_indexCount ) );
	m_geom = dCreateTriMesh( 0, m_triID, 0, 0, 0 );
	_register();
}

BoundingShape * BoundingMesh::Clone()
{
	return new BoundingMesh( m_mesh );
}
