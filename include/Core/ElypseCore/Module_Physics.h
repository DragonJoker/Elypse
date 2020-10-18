/*
See LICENSE file in root folder
*/
#ifndef ____MODULE_PHYSICS___H___
#define ____MODULE_PHYSICS___H___

#include "Elypse.h"

#include <OgreVector.h>

#include <OgrePlane.h>
#include <ode/common.h>

namespace Elypse
{
	namespace Physics
	{
		class Space;
		class PhysicsSimulation;
		class PhysicsObject;
		class PhysicsMaterial;

		class BoundingShape;

		class ObjectState;

		class Mass;

		class BoundingPlane;
		class BoundingBox;
		class BoundingRay;
		class BoundingSphere;
		class BoundingCapsule;
		class BoundingCylinder;
		class BoundingMesh;

		class PhysicsEngine;

		struct Face;
		struct Edge;
		struct Vertex;

		struct Face
		{
			Vertex	*	vertices[3];
			Vector3		normal;
			Plane		plane;
			Edge	*	edges[3];
			bool		used;
		};

		struct Edge
		{
			Vertex	*	vertices[2];
			Face	*	faces[2];
			bool		degenerate;
		};

		struct Vertex
		{
			Vector3					composedNormal;
			Vector3					position;
		};


		enum BoundingShapeClass
		{
			COC_Sphere,
			COC_Box,
			COC_Capsule,
			COC_Cylinder,
			COC_Plane,
			COC_Ray,
			COC_TriangleMesh,
			COC_Terrain,
			COC_Convex,
			COC_Transform
		};

		enum StepMode
		{
			Step_Basic,
			Step_Quick,
//		Step_Fast1
		};

		typedef std::map <String, Space *>				SpaceMap;
		typedef std::map <String, PhysicsSimulation *>	PhysicsSimulationMap;
		typedef std::map <String, PhysicsObject *>		PhysicsObjectMap;
		typedef std::map <String, BoundingShape *>		BoundingShapeMap;
		typedef std::map <String, PhysicsMaterial *>	PhysicsMaterialMap;
		typedef std::map <dGeomID, BoundingShape *>		BoundingShapeGeomMap;
		typedef std::map <PhysicsObject *, Vector3Array>	ContactMap;
		typedef std::map <Real, PhysicsObject *>			DistanceMap;

		typedef std::vector<BoundingShape *>				BoundingShapeArray;
		typedef std::vector<PhysicsObject *>				PhysicsObjectArray;
		typedef std::vector<ObjectState>				ObjectStateArray;

		typedef std::set<PhysicsObject *>				PhysicsObjectSet;
	}
}

using namespace Elypse::Physics;

#endif
