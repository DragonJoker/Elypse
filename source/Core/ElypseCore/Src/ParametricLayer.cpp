#include "PrecompiledHeader.h"

#include "ParametricLayer.h"
#include "Surface.h"

#include <OgreVector3.h>
#include <OgreAxisAlignedBox.h>
#include <OgreManualObject.h>
#include <OgreCamera.h>

/*
 0 1
 2 3
*/

ParametricLayer::ParametricLayer(	Vector3 * p_controlPoints, unsigned int p_resolution,
									ParametricLayer * p_parent, unsigned int p_quadrant,
									unsigned int p_depth )
	:	m_controlPoints( p_controlPoints ),
		m_parent( p_parent ),
		m_depth( p_depth ),
		m_resolution( p_resolution ),
		m_quadrant( p_quadrant )
{
	m_precalcX = new Real[8 * p_resolution + 4];
	m_precalcY = new Real[8 * p_resolution + 4];

	if ( m_parent != NULL )
	{
		m_sizeX = Real( m_parent->m_sizeX / 2.0 );
		m_sizeY = Real( m_parent->m_sizeY / 2.0 );
		m_positionX = m_parent->m_positionX + ( m_quadrant & 1 ) * m_sizeX;
		m_positionY = m_parent->m_positionY + ( m_quadrant >> 1 ) * m_sizeY;
	}
	else
	{
		m_positionX = 0.0;
		m_positionY = 0.0;
		m_sizeX = 1.0;
		m_sizeY = 1.0;
	}

	PrecalcCoefficients();
	m_subLayers[0] = NULL;
	m_subLayers[1] = NULL;
	m_subLayers[2] = NULL;
	m_subLayers[3] = NULL;
	m_points[0] = NULL;
	m_points[1] = NULL;
	m_points[2] = NULL;
	m_points[3] = NULL;
	m_boxes[0] = new AxisAlignedBox;
	m_boxes[1] = new AxisAlignedBox;
	m_boxes[2] = new AxisAlignedBox;
	m_boxes[3] = new AxisAlignedBox;
}

ParametricLayer::~ParametricLayer()
{
	if ( m_subLayers[0] )
	{
		delete m_subLayers[0];
	}

	if ( m_subLayers[1] )
	{
		delete m_subLayers[1];
	}

	if ( m_subLayers[2] )
	{
		delete m_subLayers[2];
	}

	if ( m_subLayers[3] )
	{
		delete m_subLayers[3];
	}

	if ( m_points[0] )
	{
		delete [] m_points[0];
	}

	if ( m_points[1] )
	{
		delete [] m_points[1];
	}

	if ( m_points[2] )
	{
		delete [] m_points[2];
	}

	if ( m_points[3] )
	{
		delete [] m_points[3];
	}

	delete m_boxes[0];
	delete m_boxes[1];
	delete m_boxes[2];
	delete m_boxes[3];
	delete [] m_precalcX;
	delete [] m_precalcY;
}

void ParametricLayer::CreateAtDeph( unsigned int p_depth )
{
	if ( m_points[0] == NULL )
	{
		CreateSurface( 0 );
	}

	if ( m_points[1] == NULL )
	{
		CreateSurface( 1 );
	}

	if ( m_points[2] == NULL )
	{
		CreateSurface( 2 );
	}

	if ( m_points[3] == NULL )
	{
		CreateSurface( 3 );
	}

	if ( p_depth > 0 )
	{
		if ( m_subLayers[0] == NULL )
		{
			m_subLayers[0] = new ParametricLayer( m_controlPoints, m_resolution, this, 0, m_depth + 1 );
		}

		if ( m_subLayers[1] == NULL )
		{
			m_subLayers[1] = new ParametricLayer( m_controlPoints, m_resolution, this, 1, m_depth + 1 );
		}

		if ( m_subLayers[2] == NULL )
		{
			m_subLayers[2] = new ParametricLayer( m_controlPoints, m_resolution, this, 2, m_depth + 1 );
		}

		if ( m_subLayers[3] == NULL )
		{
			m_subLayers[3] = new ParametricLayer( m_controlPoints, m_resolution, this, 3, m_depth + 1 );
		}

		m_subLayers[0]->CreateAtDeph( p_depth - 1 );
		m_subLayers[1]->CreateAtDeph( p_depth - 1 );
		m_subLayers[2]->CreateAtDeph( p_depth - 1 );
		m_subLayers[3]->CreateAtDeph( p_depth - 1 );
	}
}

void ParametricLayer::CreateSubLayer( unsigned int p_quadrant )
{
	if ( m_subLayers[p_quadrant] == NULL )
	{
		m_subLayers[p_quadrant] = new ParametricLayer( m_controlPoints, m_resolution, this, p_quadrant, m_depth + 1 );
		m_subLayers[p_quadrant]->CreateAtDeph( 0 );
	}
}

void ParametricLayer::CreateSurface( unsigned int p_quadrant )
{
	m_points[p_quadrant] = new Vector3[m_resolution * m_resolution];
	CalculateQuadrant( p_quadrant );
}

unsigned int ParametricLayer::CountFinalPoints()const
{
	unsigned int p_count = 0;

	if ( m_subLayers[0] )
	{
		p_count += m_subLayers[0]->CountFinalPoints();
	}
	else
	{
		p_count += m_resolution * m_resolution;
	}

	if ( m_subLayers[1] )
	{
		p_count += m_subLayers[1]->CountFinalPoints();
	}
	else
	{
		p_count += m_resolution * m_resolution;
	}

	if ( m_subLayers[2] )
	{
		p_count += m_subLayers[2]->CountFinalPoints();
	}
	else
	{
		p_count += m_resolution * m_resolution;
	}

	if ( m_subLayers[3] )
	{
		p_count += m_subLayers[3]->CountFinalPoints();
	}
	else
	{
		p_count += m_resolution * m_resolution;
	}

	return p_count;
}

void ParametricLayer::Draw( Surface * p_surface )
{
	if ( m_subLayers[0] )
	{
		m_subLayers[0]->Draw( p_surface );
	}
	else
	{
		DrawQuadrant( 0, p_surface );
	}

	if ( m_subLayers[1] )
	{
		m_subLayers[1]->Draw( p_surface );
	}
	else
	{
		DrawQuadrant( 1, p_surface );
	}

	if ( m_subLayers[2] )
	{
		m_subLayers[2]->Draw( p_surface );
	}
	else
	{
		DrawQuadrant( 2, p_surface );
	}

	if ( m_subLayers[3] )
	{
		m_subLayers[3]->Draw( p_surface );
	}
	else
	{
		DrawQuadrant( 3, p_surface );
	}
}

void ParametricLayer::DrawQuadrant( unsigned int p_quadrant , Surface * p_surface )
{
	Vector3 * l_allPoints = m_points[p_quadrant];

	if ( l_allPoints != NULL )
	{
		p_surface->CreateFromSpaceMap( l_allPoints, m_resolution, m_resolution, "Mesh/PureRed", false, false, false, false );
	}
}

void ParametricLayer::CalculateQuadrant( unsigned int p_quadrant )
{
	unsigned int i , j , k , l;
	Vector3 * l_allPoints = m_points[p_quadrant];
	m_boxes[p_quadrant]->setNull();
	Real * l_fpointer;
	Real * l_tempPoints = new Real[12];
	unsigned int l_shiftX = ( p_quadrant & 1 ) * 4 * ( m_resolution - 1 ) ;
	unsigned int l_shiftY = ( p_quadrant >> 1 ) * 4 * ( m_resolution - 1 );

	for ( i = 0 ; i < m_resolution ; i ++ )
	{
		k = l_shiftX + 4 * i;
		l_tempPoints[0] = m_controlPoints[0].x * m_precalcX[ k ]
						  + m_controlPoints[1].x * m_precalcX[ k + 1 ]
						  + m_controlPoints[2].x * m_precalcX[ k + 2 ]
						  + m_controlPoints[3].x * m_precalcX[ k + 3 ];
		l_tempPoints[1] = m_controlPoints[4].x * m_precalcX[ k ]
						  + m_controlPoints[5].x * m_precalcX[ k + 1 ]
						  + m_controlPoints[6].x * m_precalcX[ k + 2 ]
						  + m_controlPoints[7].x * m_precalcX[ k + 3 ];
		l_tempPoints[2] = m_controlPoints[8].x * m_precalcX[ k ]
						  + m_controlPoints[9].x * m_precalcX[ k + 1 ]
						  + m_controlPoints[10].x * m_precalcX[ k + 2 ]
						  + m_controlPoints[11].x * m_precalcX[ k + 3 ];
		l_tempPoints[3] = m_controlPoints[12].x * m_precalcX[ k ]
						  + m_controlPoints[13].x * m_precalcX[ k + 1 ]
						  + m_controlPoints[14].x * m_precalcX[ k + 2 ]
						  + m_controlPoints[15].x * m_precalcX[ k + 3 ];
		l_tempPoints[4] = m_controlPoints[0].y * m_precalcX[ k ]
						  + m_controlPoints[1].y * m_precalcX[ k + 1 ]
						  + m_controlPoints[2].y * m_precalcX[ k + 2 ]
						  + m_controlPoints[3].y * m_precalcX[ k + 3 ];
		l_tempPoints[5] = m_controlPoints[4].y * m_precalcX[ k ]
						  + m_controlPoints[5].y * m_precalcX[ k + 1 ]
						  + m_controlPoints[6].y * m_precalcX[ k + 2 ]
						  + m_controlPoints[7].y * m_precalcX[ k + 3 ];
		l_tempPoints[6] = m_controlPoints[8].y * m_precalcX[ k ]
						  + m_controlPoints[9].y * m_precalcX[ k + 1 ]
						  + m_controlPoints[10].y * m_precalcX[ k + 2 ]
						  + m_controlPoints[11].y * m_precalcX[ k + 3 ];
		l_tempPoints[7] = m_controlPoints[12].y * m_precalcX[ k ]
						  + m_controlPoints[13].y * m_precalcX[ k + 1 ]
						  + m_controlPoints[14].y * m_precalcX[ k + 2 ]
						  + m_controlPoints[15].y * m_precalcX[ k + 3 ];
		l_tempPoints[8] = m_controlPoints[0].z * m_precalcX[ k ]
						  + m_controlPoints[1].z * m_precalcX[ k + 1 ]
						  + m_controlPoints[2].z * m_precalcX[ k + 2 ]
						  + m_controlPoints[3].z * m_precalcX[ k + 3 ];
		l_tempPoints[9] = m_controlPoints[4].z * m_precalcX[ k ]
						  + m_controlPoints[5].z * m_precalcX[ k + 1 ]
						  + m_controlPoints[6].z * m_precalcX[ k + 2 ]
						  + m_controlPoints[7].z * m_precalcX[ k + 3 ];
		l_tempPoints[10] = m_controlPoints[8].z * m_precalcX[ k ]
						   + m_controlPoints[9].z * m_precalcX[ k + 1 ]
						   + m_controlPoints[10].z * m_precalcX[ k + 2 ]
						   + m_controlPoints[11].z * m_precalcX[ k + 3 ];
		l_tempPoints[11] = m_controlPoints[12].z * m_precalcX[ k ]
						   + m_controlPoints[13].z * m_precalcX[ k + 1 ]
						   + m_controlPoints[14].z * m_precalcX[ k + 2 ]
						   + m_controlPoints[15].z * m_precalcX[ k + 3 ];
		l = k = 0;

		for ( j = 0 ; j < m_resolution ; j ++ )
		{
			l = l_shiftY + 4 * j;
			l_fpointer = l_tempPoints;
			l_allPoints[ i * m_resolution + j ].x =  l_tempPoints[0] * m_precalcY[ l ]
					+ l_tempPoints[1] * m_precalcY[ l + 1 ]
					+ l_tempPoints[2] * m_precalcY[ l + 2 ]
					+ l_tempPoints[3] * m_precalcY[ l + 3 ];
			k = l;
			l_allPoints[ i * m_resolution + j ].y = l_tempPoints[4] * m_precalcY[ l ]
													+ l_tempPoints[5] * m_precalcY[ l + 1 ]
													+ l_tempPoints[6] * m_precalcY[ l + 2 ]
													+ l_tempPoints[7] * m_precalcY[ l + 3 ];
			k = l;
			l_allPoints[ i * m_resolution + j ].z = l_tempPoints[8] * m_precalcY[ l ]
													+ l_tempPoints[9] * m_precalcY[ l + 1 ]
													+ l_tempPoints[10] * m_precalcY[ l + 2 ]
													+ l_tempPoints[11] * m_precalcY[ l + 3 ];
//				 l_line++;
		}
	}

	for ( i = 0 ; i < m_resolution * m_resolution ; i++ )
	{
		m_boxes[p_quadrant]->merge( l_allPoints[i] );
	}

	delete [] l_tempPoints;
}


void ParametricLayer::PrecalcCoefficients()
{
	Real l_resolution = Real( 1.0 / ( 2.0 * m_resolution - 2 ) );
	Real l_f , l_omf;

	for ( unsigned int i = 0 ; i < ( 2 * m_resolution + 1 ) ; i++ )
	{
		l_f = m_positionX + m_sizeX * ( i * l_resolution );
		l_omf = Real( 1.0 - l_f );
		m_precalcX[ 4 * i ] = ( l_omf * l_omf * l_omf );
		m_precalcX[ 4 * i + 1 ] = 3.0f * l_omf * l_omf * l_f;
		m_precalcX[ 4 * i + 2 ] = 3.0f * l_omf * l_f * l_f;
		m_precalcX[ 4 * i + 3 ] = l_f * l_f * l_f;
		l_f = m_positionY + m_sizeY * ( i * l_resolution );
		l_omf = Real( 1.0 - l_f );
		m_precalcY[ 4 * i ] = ( l_omf * l_omf * l_omf );
		m_precalcY[ 4 * i + 1 ] = 3.0f * l_omf * l_omf * l_f;
		m_precalcY[ 4 * i + 2 ] = 3.0f * l_omf * l_f * l_f;
		m_precalcY[ 4 * i + 3 ] = l_f * l_f * l_f;
	}
}

ParametricLayer * ParametricLayer::GetNeighbour( unsigned int p_quadrant , unsigned int p_side )const
{
	ParametricLayer * l_selfSide;

	if ( p_quadrant == 0 && ( p_side == 1 || p_side == 2 ) )
	{
		return m_subLayers[p_side];
	}

	if ( p_quadrant == 1 )
	{
		if ( p_side == 3 )
		{
			return m_subLayers[0];
		}

		if ( p_side == 2 )
		{
			return m_subLayers[3];
		}
	}

	if ( p_quadrant == 2 && ( p_side < 2 ) )
	{
		return m_subLayers[3 * p_side];
	}

	if ( p_quadrant == 3 )
	{
		if ( p_side == 0 )
		{
			return m_subLayers[1];
		}

		if ( p_side == 3 )
		{
			return m_subLayers[2];
		}
	}

	if ( m_parent == NULL )
	{
		return NULL;
	}

	l_selfSide = m_parent->GetNeighbour( m_quadrant , p_side );

	if ( l_selfSide == NULL )
	{
		return NULL;
	}

	if ( p_side == 0 )
	{
		return l_selfSide->GetSubLayer( p_quadrant + 2 );
	}

	if ( p_side == 1 )
	{
		return l_selfSide->GetSubLayer( p_quadrant - 1 );
	}

	if ( p_side == 2 )
	{
		return l_selfSide->GetSubLayer( p_quadrant - 2 );
	}

	if ( p_side == 3 )
	{
		return l_selfSide->GetSubLayer( p_quadrant + 1 );
	}

	return NULL;
}

void ParametricLayer::CutToDepth( unsigned int p_depth )
{
	if ( ( m_depth ) < p_depth )
	{
		if ( m_subLayers[0] != NULL )
		{
			m_subLayers[0]->CutToDepth( p_depth );
		}

		if ( m_subLayers[1] != NULL )
		{
			m_subLayers[1]->CutToDepth( p_depth );
		}

		if ( m_subLayers[2] != NULL )
		{
			m_subLayers[2]->CutToDepth( p_depth );
		}

		if ( m_subLayers[3] != NULL )
		{
			m_subLayers[3]->CutToDepth( p_depth );
		}
	}
	else
	{
		if ( m_subLayers[0] != NULL )
		{
			delete( m_subLayers[0] );
			m_subLayers[0] = NULL;
		}

		if ( m_subLayers[1] != NULL )
		{
			delete( m_subLayers[1] );
			m_subLayers[1] = NULL;
		}

		if ( m_subLayers[2] != NULL )
		{
			delete( m_subLayers[2] );
			m_subLayers[2] = NULL;
		}

		if ( m_subLayers[3] != NULL )
		{
			delete( m_subLayers[3] );
			m_subLayers[3] = NULL;
		}
	}
}

bool ParametricLayer::UpdateResolution( Camera * p_camera )
{
	Real p_boxSize;
	Real p_dist;
	int p_rez;
	AxisAlignedBox * l_box;
	bool l_ret = false;
	unsigned int l_quadrant;

	for ( l_quadrant = 0 ; l_quadrant < 4 ; l_quadrant ++ )
	{
		if ( m_points[l_quadrant] != NULL )
		{
			l_box = m_boxes[l_quadrant];
			p_boxSize = ( l_box->getMaximum() - l_box->getMinimum() ).length();
			p_dist = ( p_camera->getDerivedPosition() - l_box->getCenter() ).length();
			p_rez = Math::IFloor( Math::Log( Real( 16.0 * p_boxSize / p_dist ) ) );

			if ( p_rez < 0 )
			{
				p_rez = 0;
			}

			if ( p_rez == 0 )
			{
				if ( m_parent != NULL )
				{
					m_parent->RemoveQuadrant( m_quadrant );
					return true ;
				}
			}

			if ( p_rez == 1 )
			{
				if ( m_subLayers[l_quadrant] )
				{
					l_ret = l_ret || m_subLayers[l_quadrant]->UpdateResolution( p_camera );
				}
			}

			if ( p_rez > 1 )
			{
				if ( !m_subLayers[l_quadrant] )
				{
					CreateSubLayer( l_quadrant );
					m_subLayers[l_quadrant]->UpdateResolution( p_camera );
					l_ret = true;
				}
				else
				{
					l_ret = l_ret || m_subLayers[l_quadrant]->UpdateResolution( p_camera );
				}
			}
		}
	}

	return l_ret;
}

void ParametricLayer::RemoveQuadrant( unsigned int p_quadrant )
{
	if ( m_subLayers[p_quadrant] != NULL )
	{
		delete( m_subLayers[p_quadrant] );
		m_subLayers[p_quadrant] = NULL;
	}
}
