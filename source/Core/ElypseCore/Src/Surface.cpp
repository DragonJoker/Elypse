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

#include "Surface.h"

#include <OgreSceneManager.h>
#include <OgreManualObject.h>

Surface::Surface()
{
}

Surface::Surface( const String & p_name, SceneManager * p_manager )
	: m_name( p_name )
{
	m_object = p_manager->createManualObject( p_name );
}

Surface::~Surface()
{
}

void Surface::CreateFromHeightMap( Real * p_heightMap , unsigned int p_width , unsigned int p_height , const String & p_materialName )
{
}

void Surface::Clear()
{
	m_object->clear();
}

void Surface::CreateFromSpaceMap( Vector3 * p_spaceMap ,
									unsigned int p_width ,
									unsigned int p_height ,
									const String & p_materialName ,
									bool p_seamNorth ,
									bool p_seamEast ,
									bool p_seamSouth ,
									bool p_seamWest )
{
	unsigned int i, j, k;
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
