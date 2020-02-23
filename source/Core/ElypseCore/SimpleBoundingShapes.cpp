/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SimpleBoundingShapes.h"

#include <ode/collision.h>

BoundingRay::BoundingRay()
	: BoundingShape( COC_Ray )
{
	m_geom = dCreateRay( nullptr, 1.0 );
	_register();
}

BoundingRay::~BoundingRay()
{
}

void BoundingRay::SetRay( const Ray & p_ray )
{
	m_ray = p_ray;
	const Vector3 & l_direction = m_ray.getDirection();
	const Vector3 & l_origin = m_ray.getOrigin();
	dGeomRaySet( m_geom, l_origin.x, l_origin.y, l_origin.z, l_direction.x, l_direction.y, l_direction.z );
	dGeomRaySetLength( m_geom, Math::POS_INFINITY );
}

void BoundingRay::SetOrigin( const Vector3 & p_origin )
{
	m_ray.setOrigin( p_origin );
	const Vector3 l_direction = m_ray.getOrigin();
	dGeomRaySet( m_geom, p_origin.x, p_origin.y, p_origin.z, l_direction.x, l_direction.y, l_direction.z );
}

void BoundingRay::SetDirection( const Vector3 & p_direction )
{
	m_ray.setDirection( p_direction );
	const Vector3 l_origin = m_ray.getDirection();
	dGeomRaySet( m_geom, l_origin.x, l_origin.y, l_origin.z, p_direction.x, p_direction.y, p_direction.z );
	dGeomRaySetLength( m_geom, p_direction.length() );
}

BoundingShape * BoundingRay::Clone()
{
	BoundingRay * l_ray = new BoundingRay();
	l_ray->SetRay( m_ray );
	return l_ray;
}
