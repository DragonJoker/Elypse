/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_GUI_H___
#define ___MODULE_GUI_H___

#include "Elypse.h"

namespace Elypse
{
	namespace Gui
	{
		class EMOverlayGroup;
		class EMGui;
		class EMOverlay;

		class ClickableShape;
		class ClickableBox;
		class ClickableImage;
		class ClickableCircle;

		class RawImage;
		class RawImageManager;

		typedef std::map< String , EMOverlayGroup * >					OverlayGroupMap;
		typedef std::map< String , EMOverlay * >						EMOverlayMap;
		typedef std::vector< EMOverlay * >								EMOverlayArray;
		typedef std::vector< ClickableShape * >							ClickableShapeArray;
		typedef std::set< EMOverlay * >									EMOverlaySet;
	}
}

using namespace Elypse::Gui;

#endif
