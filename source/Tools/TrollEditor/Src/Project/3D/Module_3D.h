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
#ifndef ___Module_3D___
#define ___Module_3D___

#include "Project/Module_Project.h"

#include <OgrePrerequisites.h>

#define TROLL_PROJECT_3D_NAMESPACE TROLL_PROJECT_NAMESPACE::Objects3D
#define BEGIN_TROLL_PROJECT_3D_NAMESPACE BEGIN_TROLL_PROJECT_NAMESPACE { namespace Objects3D
#define END_TROLL_PROJECT_3D_NAMESPACE END_TROLL_PROJECT_NAMESPACE }

BEGIN_TROLL_PROJECT_NAMESPACE
{
	namespace Objects3D
	{
		enum TrollLightType
		{
			ltPoint,
			ltDirectional,
			ltSpotLight
		};

		enum Troll3DObjectType
		{
			ttNode,
			ttEntity,
			ttLight,
			ttCamera
		};

		enum TrollCameraPolygonMode
		{
			tcPoint = 1,
			tcWireFrame = 2,
			tcSolid = 3
		};

		class TrollMesh;
		class Troll3DObject;
		class TrollEntity;
		class TrollLight;
		class TrollSceneNode;
		class TrollCamera;

		typedef std::map< wxString, Troll3DObject * > Troll3DObjectMap;
		typedef std::map< wxString, TrollEntity * > TrollEntityMap;
		typedef std::map< wxString, TrollMesh * > TrollMeshStrMap;
		typedef std::map< wxString, TrollSceneNode * > TrollSceneNodeMap;
		typedef std::map< wxString, TrollLight * > TrollLightMap;
		typedef std::map< wxString, TrollCamera * > TrollCameraMap;
	}
}
END_TROLL_PROJECT_NAMESPACE

#endif
