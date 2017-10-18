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
#ifndef ___EMUSE_SCENE_FILE_SAVER___
#define ___EMUSE_SCENE_FILE_SAVER___

#include "Module_SceneFile.h"
#include "Module_Script.h"
#include "Module_Gui.h"

#include <fstream>

namespace Elypse
{
	namespace SceneFile
	{
		class SceneFileSaver : d_unique
		{
		private:
			std::ofstream m_buffer;
			SceneManager * m_sceneManager;
			ScriptEngine * m_scriptEngine;
			EMGui * m_gui;

		private:
			void _writeObjects();
			void _writeSceneNodes( SceneNode * p_sceneNode );
			void _writeLights();
			void _writeCameras();
			void _writeAllOverlays();
			void _writeOverlays( OverlayElement * p_overlay, int p_depth );
			void _writeCode();
			void _writeSounds();

			void _ntabs( uint32_t p_numTabs );

		public:
			SceneFileSaver( SceneManager * p_manager, ScriptEngine * p_engine, EMGui * p_gui );
			~SceneFileSaver();

		public:
			void SaveAll( String const & p_filename );
		};
	}
}

#endif
