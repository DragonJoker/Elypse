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
#ifndef ___Module_Project___
#define ___Module_Project___

#include "TrollEditorPrerequisites.h"

#define TROLL_PROJECT_NAMESPACE TROLL_NAMESPACE::ProjectComponents
#define BEGIN_TROLL_PROJECT_NAMESPACE BEGIN_TROLL_NAMESPACE { namespace ProjectComponents
#define END_TROLL_PROJECT_NAMESPACE END_TROLL_NAMESPACE }

BEGIN_TROLL_NAMESPACE
{
	namespace ProjectComponents
	{
		enum State
		{
			Normal,
			Selected,
			MouseOver
		};

		enum ObjectType
		{
			tt3DObject,
			ttOverlay,
			ttTemporalObject,
			ttMediaObject
		};

		enum AntiAliasing
		{
			aa0 = 0,
			aa2x = 1,
			aa4x = 2
		};

		enum BackgroundType
		{
			bgColour,
			bgImage
		};

		enum InterpolationMethod
		{
			Linear,
			Square,
			Cubic,
			Sinus,
			Bezier
		};

		enum FileType
		{
			sceneFile,
			loadScriptFile,
			unloadScriptFile,
			dataFile,
			dataFolder
		};

		enum ShadowType
		{
			stNone,
			stTextureAdditive,
			stStencilAdditive
		};

		class Displayable;
		class Object;
		class Project;
		class Scene;
		struct File;
		class SceneFile;
		class Object;

		typedef std::vector< File * > FileArray;
		typedef std::map< unsigned int, wxString > wxStringUIntMap;
		typedef std::map< wxString, SceneFile * > SceneFileStrMap;
		typedef std::map< wxString, Scene * > SceneMap;
		typedef std::map< wxString, Object * > ObjectMap;
		
		typedef std::shared_ptr< Project > ProjectSPtr;
		typedef std::weak_ptr< Project > ProjectWPtr;
	}
}
END_TROLL_NAMESPACE

#endif
