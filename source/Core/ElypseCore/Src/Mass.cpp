#include "PrecompiledHeader.h"

#include "Mass.h"

#include "BoundingMesh.h"

Mass :: Mass()
{
	SetSphereMass( 1.0, 1.0 );
}

Mass :: Mass( Real p_mass, const Vector3 & p_center, const Matrix3 & p_inertia )
{
	dMassSetParameters( & m_mass, p_mass,
						p_center.x, p_center.y, p_center.z,
						p_inertia[0][0], p_inertia[1][1], p_inertia[2][2],
						p_inertia[0][1], p_inertia[0][2], p_inertia[1][2] );
}
Mass :: Mass( const Mass & p_other )
{
	memcpy( & m_mass, & p_other.m_mass, sizeof( m_mass ) );
}

Mass & Mass :: operator =( const Mass & p_other )
{
	memcpy( & m_mass, & p_other.m_mass, sizeof( m_mass ) );
	return *this;
}

Mass & Mass :: operator =( dMass * p_other )
{
	memcpy( & m_mass, p_other, sizeof( m_mass ) );
	return *this;
}

void Mass :: Adjust( Real p_mass )
{
	dMassAdjust( & m_mass, p_mass );
}

void Mass :: Translate( const Vector3 & p_offset )
{
	dMassTranslate( & m_mass, p_offset.x, p_offset.y, p_offset.z );
}

void Mass :: Rotate( const Quaternion & p_orientation )
{
	Matrix3 m;
	p_orientation.ToRotationMatrix( m );
	dMatrix3 r;
	r[0] = m[0][0];
	r[1] = m[0][1];
	r[2] = m[0][2];
	r[3] = m[1][0];
	r[4] = m[1][1];
	r[5] = m[1][2];
	r[6] = m[2][0];
	r[7] = m[2][1];
	r[8] = m[2][2];
	r[9] = 0.0;
	r[10] = 0.0;
	r[11] = 0.0;
	dMassRotate( & m_mass, r );
}

Matrix3 Mass :: GetLocalInertiaTensor() const
{
	return Matrix3( m_mass.I[0], m_mass.I[1], m_mass.I[2],
					m_mass.I[4], m_mass.I[5], m_mass.I[6],
					m_mass.I[8], m_mass.I[9], m_mass.I[10] );
}
void Mass :: Add( const Mass & p_other )
{
	dMassAdd( & m_mass, &( p_other.m_mass ) );
}

void Mass :: SetSphereMass( Real p_mass, Real p_radius )
{
	dMassSetSphereTotal( & m_mass, p_mass, p_radius );
}

void Mass :: SetSphereDensity( Real p_density, Real p_radius )
{
	dMassSetSphere( & m_mass, p_density, p_radius );
}

void Mass :: SetTrimesh( Real p_density, BoundingMesh * p_mesh )
{
	dMassSetTrimesh( & m_mass, p_density, p_mesh->GetGeomID() );
}

void Mass :: SetBox( Real p_density, const Vector3 & p_extends )
{
	dMassSetBox( & m_mass, p_density, p_extends.x, p_extends.y, p_extends.z );
}

