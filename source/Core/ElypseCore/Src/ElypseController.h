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
#ifndef ___EMUSE_CONTROLLER_H___
#define ___EMUSE_CONTROLLER_H___

#include "Module_Main.h"
#include "Module_Download.h"

#include <Mutex.h>
#include <thread>
#include <Condition.h>
#include <Singleton.h>

#include <OgreOverlaySystem.h>

namespace Elypse
{
	namespace Main
	{
		class d_dll_export ElypseController
			: public General::Theory::Singleton< ElypseController >
			, public owned_by< ElypseInstance >
		{
		private:
			InstanceMap m_instances;
			InstanceMultiMap m_links;
			Root * m_root;
			Ogre::OverlaySystem * m_overlaySystem;
			Log * m_log;
			RenderWindow * m_primaryRenderWindow;

			DownloadManager * m_downloadManager;
//		std::thread * m_currentRenderingThread;
			ElypseLoadingBar * m_loadingBar;
//		ElypseInstance * m_currentOwner;

			bool m_direct3D;
			bool m_oneTimePostWindowInit;
			bool m_useConsole;
			bool m_downloadFiles;
			bool m_initialised;
			bool m_stopThread;
			bool m_activeDebug;
			bool m_vsync;

			uint32_t m_numInstance;

			ControllerStatus m_status;

			Path m_installDir;

			std::mutex m_mutex;
			std::mutex m_selfMutex;
			std::condition_variable m_threadCurrentlyRendering;
			std::condition_variable m_threadEnded;

			StringPairArray m_availableRenderers;

		public:
			ElypseController( ElypseInstance & p_owner );
			~ElypseController();

		private:
			bool _getNextAvailableRenderer( String & p_pluginName, String & p_rendererName );
			bool _loadRenderer( String const & p_pluginName, String const & p_rendererName );

		public:
			void PreInit( bool p_useConsole, bool p_direct3D, bool p_download, const Path & p_installationDir );
			void AddApplication( ElypseInstance * p_instance );
			void Initialise();

			RenderWindow * CreateRenderWindow( String const & p_name, uint32_t p_width, uint32_t p_height, String const & p_handle , uint32_t p_antialiasing, bool & p_mainWindow );
			SceneManager * CreateSceneManager( String const & p_name );

			void AddThread( ElypseInstance & p_parent );
			void MainLoop();

			void DeleteOgre();
			void WaitForThreadEnded();

			void InitialiseRessources( String const & p_prefix );
			void UninitialiseRessources( String const & p_prefix );

			void CheckForUpdates();

			void LinkInstanceTo( ElypseInstance * p_instance, String const & p_linkedToName );

			bool ShowConsole();

			inline Root * GetRoot()const
			{
				return m_root;
			}
			inline DownloadManager * GetDownloadManager()const
			{
				return m_downloadManager;
			}
			inline ElypseLoadingBar * GetLoadingBar()const
			{
				return m_loadingBar;
			}
			inline Log * GetLog()const
			{
				return m_log;
			}
			inline Path GetInstallPath()const
			{
				return m_installDir;
			}

			inline uint32_t GetNextAppIndex()
			{
				return m_numInstance ++;
			}
		};
	}
}

#endif
