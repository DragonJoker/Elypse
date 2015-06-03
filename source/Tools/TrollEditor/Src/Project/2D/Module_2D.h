#ifndef ___Module_2D___
#define ___Module_2D___

#include <wx/string.h>

#include <OgrePrerequisites.h>

namespace Troll
{
	namespace Objects2D
	{
		enum TEHorizAlign
		{
			hNone = -1,
			hLeft = 0,
			hCenter = 1,
			hRight = 2
		};
		enum TEVerticAlign
		{
			vNone = -1,
			vTop = 0,
			vCenter = 1,
			vBottom = 2
		};

		struct TextInfos;
		struct BorderInfos;

		class TrollOverlay;
	}
}
typedef std::map	<wxString, Troll::Objects2D::TrollOverlay *>	TrollOverlayMap;
typedef std::vector <Troll::Objects2D::TrollOverlay *>				TrollOverlayArray;

#endif
