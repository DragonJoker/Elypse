/*
See LICENSE file in root folder
*/
#ifndef ___SIMPLE_BOUNDING_SHAPES_H___
#define ___SIMPLE_BOUNDING_SHAPES_H___

#include "BoundingShape.h"

#include <OgreRay.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export BoundingPlane : public BoundingShape
		{
		private:
			Plane m_plane;

		public:
			BoundingPlane()
				: BoundingShape( COC_Plane )
			{
			}
			virtual ~BoundingPlane() {}

		public:
			void Define( const Plane & p_plane )
			{
				m_plane = p_plane;
				m_geom = dCreatePlane( NULL, m_plane.normal.x, m_plane.normal.y, m_plane.normal.z, -m_plane.d );
				_register();
			}
			const Plane & GetDefinition()const
			{
				return m_plane;
			}
			virtual BoundingShape * Clone()
			{
				BoundingPlane * l_plane = new BoundingPlane();
				l_plane->Define( m_plane );
				return l_plane;
			}
		};

		class d_dll_export BoundingBox : public BoundingShape
		{
		private:
			Vector3 m_extends;

		public:
			BoundingBox()
				: BoundingShape( COC_Box )
			{
			}
			BoundingBox( const Vector3 & p_extends )
				: BoundingShape( COC_Box )
			{
				SetExtends( p_extends );
			}
			virtual ~BoundingBox() {}

		public:
			void SetExtends( const Vector3 & p_extends )
			{
				m_extends = p_extends;
				m_geom = dCreateBox( NULL, p_extends.x, p_extends.y, p_extends.z );
				m_box.setExtents( p_extends / ( -2.0 ) , p_extends / 2.0 );
				_register();
			}

		public:
			inline const Vector3 & GetExtends()const
			{
				return m_extends;
			}
			virtual BoundingShape * Clone()
			{
				BoundingBox * l_box = new BoundingBox();
				l_box->SetExtends( m_extends );
				return l_box;
			}
		};

		class d_dll_export BoundingRay : public BoundingShape
		{
		private:
			Ray m_ray;

		public:
			BoundingRay();
			virtual ~BoundingRay();

		public:
			void SetOrigin( const Vector3 & p_origin );
			void SetDirection( const Vector3 & p_direction );
			void SetRay( const Ray & p_ray );

			inline const Ray & GetRay()const
			{
				return m_ray;
			}
			inline const Vector3 & GetOrigin()const
			{
				return m_ray.getOrigin();
			}
			inline const Vector3 & GetDirection()const
			{
				return m_ray.getDirection();
			}
			virtual BoundingShape * Clone();
		};

		class d_dll_export BoundingSphere : public BoundingShape
		{
		private:
			Vector3 m_offset;
			Real m_radius;

		public:
			BoundingSphere() : BoundingShape( COC_Sphere )
			{}
			BoundingSphere( Real p_radius ) : BoundingShape( COC_Sphere )
			{
				SetRadius( p_radius );
			}
			virtual ~BoundingSphere() {}

		public:
			inline void SetRadius( Real p_radius )
			{
				m_radius = p_radius;
				m_geom = dCreateSphere( NULL, p_radius );
				_register();
			}
			inline void SetOffset( Real p_x, Real p_y, Real p_z )
			{
				m_offset.x = p_x;
				m_offset.y = p_y;
				m_offset.z = p_z;
			}

			inline Real GetRadius()const
			{
				return m_radius;
			}
			virtual BoundingShape * Clone()
			{
				BoundingSphere * l_sphere = new BoundingSphere();
				l_sphere->SetRadius( m_radius );
				return l_sphere;
			}
		};
	}
}

#endif
