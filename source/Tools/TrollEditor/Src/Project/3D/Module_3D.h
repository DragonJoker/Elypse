/*
See LICENSE file in root folder
*/
#ifndef ___Module_3D___
#define ___Module_3D___

#include "Project/Module_Project.h"

#include <OgrePrerequisites.h>

namespace Troll
{
	namespace ProjectComponents
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
}

#endif
