/*
See LICENSE file in root folder
*/
#ifndef ___Module_2D___
#define ___Module_2D___

#include "Project/Module_Project.h"

#include <wx/string.h>

#include <OgrePrerequisites.h>

namespace Troll
{
	namespace ProjectComponents
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

			struct TextInfos
			{
				wxString m_caption;
				Ogre::ColourValue m_textColour;
				wxString m_fontName;
				float m_fontSize;
				TEHorizAlign m_horizontalAlign;
				TEVerticAlign m_verticalAlign;
			};

			struct BorderInfos
			{
				wxString m_material;
				float m_top;
				float m_bottom;
				float m_left;
				float m_right;
			};

			class TrollOverlay;

			typedef std::map< wxString, TrollOverlay * > TrollOverlayMap;
			typedef std::vector< TrollOverlay * > TrollOverlayArray;
		}
	}
}

#endif
