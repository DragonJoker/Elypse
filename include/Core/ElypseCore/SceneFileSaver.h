/*
See LICENSE file in root folder
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
