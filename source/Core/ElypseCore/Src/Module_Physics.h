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
#ifndef ____MODULE_PHYSICS___H___
#define ____MODULE_PHYSICS___H___

#include "Elypse.h"

#include <OgreVector3.h>

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
