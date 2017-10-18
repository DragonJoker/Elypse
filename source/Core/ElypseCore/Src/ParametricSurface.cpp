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

#include "ParametricSurface.h"
#include "ParametricLayer.h"

#include <OgreVector3.h>
#include <OgreManualObject.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

ParametricSurface::ParametricSurface( SceneManager * p_manager, String const & p_name,
									  uint32_t p_resolution, uint32_t p_numControlPoints )
	: Surface( p_name, p_manager )
	, m_resolution( p_resolution )
	, m_numControlPoints( p_numControlPoints )
	, m_currentControlPointIndex( 0 )
	, m_currentDepth( 0 )
{
	m_controlPoints = new Vector3[p_numControlPoints];
	m_box = new AxisAlignedBox;
	m_firstLayer = new ParametricLayer( m_controlPoints, m_resolution + 1, NULL, 0, 0 );
	m_manager->getRootSceneNode()->createChildSceneNode()->attachObject( m_object );
}

ParametricSurface::~ParametricSurface()
{
	m_manager->destroySceneNode( m_object->getParentSceneNode()->getName() );
	m_manager->destroyManualObject( m_object );
	delete m_firstLayer;
	delete [] m_controlPoints;
	delete m_box;
}

void ParametricSurface::CreateAtDepth( uint32_t p_depth )
{
	m_firstLayer->CreateAtDeph( p_depth );
	m_currentDepth = p_depth;
}

void ParametricSurface::AddControlPoint( const Vector3 & p_controlPoint )
{
	if ( m_currentControlPointIndex > m_numControlPoints )
	{
		return;
	}

	m_box->merge( p_controlPoint );
	m_controlPoints[m_currentControlPointIndex] = p_controlPoint;
	m_currentControlPointIndex ++;
}

void ParametricSurface::Draw()
{
	m_object->begin( "Mesh/PureRed", RenderOperation::OT_TRIANGLE_LIST );
//	m_object->begin( "Base/PureRed", RenderOperation::OT_LINE_LIST);
	m_firstLayer->Draw( this );
	m_object->end();
}

void ParametricSurface::Update( Camera * p_camera )
{
	if ( m_firstLayer->UpdateResolution( p_camera ) )
	{
		m_object->clear();
		Draw();
	}

	/*
	Real p_boxSize = (m_box->getMaximum() - m_box->getMinimum()).length();
	Real p_dist = (p_camera->getDerivedPosition() - m_box->getCenter()).length();
	int p_rez = Math::IFloor(Math::Log( 16.0 * p_boxSize / p_dist ));
	if( p_rez<0 )p_rez = 0;

	if( m_currentDepth < p_rez )
	{
		CreateAtDepth(p_rez);
		m_object->clear();
		Draw();
	}
	else if( m_currentDepth > p_rez )
	{
		m_firstLayer->CutToDepth( p_rez );
		m_currentDepth = p_rez;
		m_object->clear();
		Draw();
	}
	*/
}
