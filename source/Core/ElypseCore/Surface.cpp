/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Surface.h"

#include <OgreSceneManager.h>
#include <OgreManualObject.h>

Surface::Surface()
{
}

Surface::Surface( String const & p_name, SceneManager * p_manager )
	: m_name( p_name )
{
	m_object = p_manager->createManualObject( p_name );
}

Surface::~Surface()
{
}

void Surface::CreateFromHeightMap( Real * p_heightMap , uint32_t p_width , uint32_t p_height , String const & p_materialName )
{
}

void Surface::Clear()
{
	m_object->clear();
}

void Surface::CreateFromSpaceMap( Vector3 * p_spaceMap ,
								  uint32_t p_width ,
								  uint32_t p_height ,
								  String const & p_materialName ,
								  bool p_seamNorth ,
								  bool p_seamEast ,
								  bool p_seamSouth ,
								  bool p_seamWest )
{
	uint32_t i, j, k;
//	m_object->begin(p_materialName , RenderOperation::OT_TRIANGLE_LIST);
	Vector3 l_normal, l_a, l_b;
	k = p_width + 1;
	m_object->normal( Vector3::UNIT_Y );

	for ( i = 1 ; i < ( p_height - 2 ) ; i ++ )
	{
		for ( j = 1 ; j < ( p_width - 2 ); j ++ )
		{
			/*
						l_a = p_spaceMap[ k ] - p_spaceMap[ k + p_width ];
						l_b = p_spaceMap[ k + 1 ] - p_spaceMap[ k + p_width ];
						m_object->normal( l_b.crossProduct(l_a).normalisedCopy() );
			*/
			m_object->position( p_spaceMap[ k ] );
			m_object->position( p_spaceMap[ k + p_width ] );
			m_object->position( p_spaceMap[ k + 1 ] );
			/*
						l_a = p_spaceMap[ k + p_width + 1 ] - p_spaceMap[ k + p_width ];
						m_object->normal( l_a.crossProduct(l_b).normalisedCopy() );
						// ?.? marche po ?
			*/
			m_object->position( p_spaceMap[ k + 1 ] );
			m_object->position( p_spaceMap[ k + p_width ] );
			m_object->position( p_spaceMap[ k + p_width + 1 ] );
			k++;
		}

		k += 3;
	}

	if ( p_seamNorth )
	{
		k = ( p_width - 3 ) / 2;
		m_object->position( p_spaceMap[ 0 ] );
		m_object->position( p_spaceMap[ p_width + 1 ] );
		m_object->position( p_spaceMap[ 2 ] );

		for ( i = 0 ; i < k ; i++ )
		{
			j = 2 * i + 2;
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width + 1 ] );
			m_object->position( p_spaceMap[ j + 2 ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width - 1 ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
		}
	}
	else
	{
		k = ( p_width - 3 );

		for ( i = 0 ; i < k ; i++ )
		{
			j = i + 1;
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
		}

		m_object->position( p_spaceMap[ 0 ] );
		m_object->position( p_spaceMap[ p_width + 1 ] );
		m_object->position( p_spaceMap[ 1 ] );
		m_object->position( p_spaceMap[ p_width - 2 ] );
		m_object->position( p_spaceMap[ 2 * p_width - 2 ] );
		m_object->position( p_spaceMap[ p_width - 1 ] );
	}

	if ( p_seamSouth )
	{
		k = ( p_width - 3 ) / 2;
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) + 2 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 2 ) + 1 ] );

		for ( i = 0 ; i < k ; i++ )
		{
			j = p_width * ( p_width - 1 ) + 2 + 2 * i;
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + 2 ] );
			m_object->position( p_spaceMap[ j - p_width + 1 ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j - p_width ] );
			m_object->position( p_spaceMap[ j - p_width - 1 ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j - p_width + 1] );
			m_object->position( p_spaceMap[ j - p_width ] );
		}
	}
	else
	{
		k = ( p_width - 3 );

		for ( i = 0 ; i < k ; i++ )
		{
			j = p_width * ( p_width - 2 ) + 1 + i;
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
		}

		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) + 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 2 ) + 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width ) - 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) - 2  ] );
		m_object->position( p_spaceMap[ p_width * ( p_width ) - 2 ] );
	}

	if ( p_seamEast )
	{
		k = ( p_width - 3 ) / 2;
		m_object->position( p_spaceMap[ 0 ] );
		m_object->position( p_spaceMap[ 2 * p_width ] );
		m_object->position( p_spaceMap[ p_width + 1 ] );

		for ( i = 0 ; i < k ; i++ )
		{
			j = 2 * p_width * ( i + 1 );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j - p_width + 1 ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + 2 * p_width] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
		}
	}
	else
	{
		k = ( p_width - 3 );

		for ( i = 0 ; i < k ; i++ )
		{
			j = p_width * ( i + 1 );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j + p_width] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
			m_object->position( p_spaceMap[ j + 1 ] );
		}

		m_object->position( p_spaceMap[ 0 ] );
		m_object->position( p_spaceMap[ p_width ] );
		m_object->position( p_spaceMap[ p_width + 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 2 ) + 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 2 ) ] );
	}

	if ( p_seamWest )
	{
		k = ( p_width - 3 ) / 2;
		m_object->position( p_spaceMap[ p_width - 1 ] );
		m_object->position( p_spaceMap[ 2 * p_width - 2 ] );
		m_object->position( p_spaceMap[ 3 * p_width - 1 ] );

		for ( i = 0 ; i < k ; i++ )
		{
			j = p_width + 2 * p_width * ( i + 1 ) - 1;
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width - 1 ] );
			m_object->position( p_spaceMap[ j + 2 * p_width ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j - p_width - 1] );
			m_object->position( p_spaceMap[ j - 1 ] );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j - 1 ] );
			m_object->position( p_spaceMap[ j + p_width - 1] );
		}
	}
	else
	{
		k = ( p_width - 3 );

		for ( i = 0 ; i < k ; i++ )
		{
			j = ( p_width - 2 ) + p_width * ( i + 1 );
			m_object->position( p_spaceMap[ j ] );
			m_object->position( p_spaceMap[ j + p_width ] );
			m_object->position( p_spaceMap[ j + 1 ] );
			m_object->position( p_spaceMap[ j + p_width] );
			m_object->position( p_spaceMap[ j + p_width + 1] );
			m_object->position( p_spaceMap[ j + 1 ] );
		}

		m_object->position( p_spaceMap[ p_width - 1 ] );
		m_object->position( p_spaceMap[ 2 * p_width - 2 ] );
		m_object->position( p_spaceMap[ 2 * p_width - 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width ) - 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) - 1 ] );
		m_object->position( p_spaceMap[ p_width * ( p_width - 1 ) - 2 ] );
	}

//	m_object->end();
}
