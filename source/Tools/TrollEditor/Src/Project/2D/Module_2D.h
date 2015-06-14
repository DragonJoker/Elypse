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
#ifndef ___Module_2D___
#define ___Module_2D___

#include "Project/Module_Project.h"

#include <wx/string.h>

#include <OgrePrerequisites.h>

#define TROLL_PROJECT_2D_NAMESPACE TROLL_PROJECT_NAMESPACE::Objects2D
#define BEGIN_TROLL_PROJECT_2D_NAMESPACE BEGIN_TROLL_PROJECT_NAMESPACE { namespace Objects2D
#define END_TROLL_PROJECT_2D_NAMESPACE END_TROLL_PROJECT_NAMESPACE }

BEGIN_TROLL_PROJECT_NAMESPACE
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
END_TROLL_PROJECT_NAMESPACE

#endif
