/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_SPECIAL_EFFECTS_H___
#define ___MODULE_SPECIAL_EFFECTS_H___

#include "Elypse.h"

namespace Elypse
{
	namespace SpecialEffects
	{
		class CamTex;
		class Mirror;
		class MirrorManager;
		class MirrorMaterial;
		class PostEffect;

		class ObjectMaterial;

		class WebImage;
		class WebImageManager;

		class MaterialTarget;

		typedef Manager <CamTex>		CamTexManager;
		typedef Manager <PostEffect>	PostEffectManager;
		typedef Manager <ObjectMaterial> ObjectMaterialManager;

		typedef	std::map <String, MirrorMaterial *>			MirrorMaterialMap;
		typedef	std::map <String, WebImage *>				WebImageMap;

		typedef std::vector <MaterialTarget *>				MaterialTargetArray;
	}
}

using namespace Elypse::SpecialEffects;

#endif
