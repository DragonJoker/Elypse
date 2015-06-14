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
#ifndef ___MODULE_GUI_PROPERTIES_H___
#define ___MODULE_GUI_PROPERTIES_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Module_Project.h"
#include "Project/2D/Module_2D.h"
#include "Project/3D/Module_3D.h"
#include "Project/Media/Module_Media.h"
#include "Project/Temporal/Module_Temporal.h"

#define TROLL_GUI_PROPERTIES_NAMESPACE TROLL_GUI_NAMESPACE::Properties
#define BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE BEGIN_TROLL_GUI_NAMESPACE { namespace Properties
#define END_TROLL_GUI_PROPERTIES_NAMESPACE END_TROLL_GUI_NAMESPACE }

BEGIN_TROLL_GUI_NAMESPACE
{
	namespace Properties
	{
		class CameraProperties;
		class ContinuousEventProperties;
		class EntityProperties;
		class KeyFrameProperties;
		class LightProperties;
		class ObjectProperties;
		class OverlayProperties;
		class PonctualEventProperties;
		class ProjectProperties;
		class SceneNodeProperties;
		class SequenceProperties;
		class SoundObjectProperties;
		class SoundProperties;
		class VideoObjectProperties;
		class VideoProperties;
	}
}
END_TROLL_GUI_NAMESPACE

#endif
