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
#include <Thread.h>
#include <Condition.h>
#include <Singleton.h>

namespace Elypse
{
	namespace Main
	{
		class d_dll_export ElypseController : public General::Theory::Singleton <ElypseController>, public owned_by<ElypseInstance>
		{
		private:
			InstanceMap m_instances;
			InstanceMultiMap m_links;
			Root * m_root;
			Log * m_log;
			RenderWindow * m_primaryRenderWindow;

			DownloadManager * m_downloadManager;
//		Thread * m_currentRenderingThread;
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

			unsigned int m_numInstance;

			ControllerStatus m_status;

			Path m_installDir;

			Mutex m_mutex;
			Mutex m_selfMutex;
			Condition m_threadCurrentlyRendering;
			Condition m_threadEnded;

			StringPairArray m_availableRenderers;

		public:
			ElypseController();
			~ElypseController();

		private:
			bool _getNextAvailableRenderer( String & p_pluginName, String & p_rendererName );
			bool _loadRenderer( const String & p_pluginName, const String & p_rendererName );

		public:
			void PreInit( bool p_useConsole, bool p_direct3D, bool p_download, const Path & p_installationDir );
			void AddApplication( ElypseInstance * p_instance );
			void Initialise();

			RenderWindow * CreateRenderWindow( const String & p_name, unsigned int p_width, unsigned int p_height, const String & p_handle , unsigned int p_antialiasing, bool & p_mainWindow );
			SceneManager * CreateSceneManager( const String & p_name );

			void AddThread( ElypseInstance * p_parent );
			void MainLoop();

			void DeleteOgre();
			void WaitForThreadEnded();

			void InitialiseRessources( const String & p_prefix );
			void UninitialiseRessources( const String & p_prefix );

			void CheckForUpdates();

			void LinkInstanceTo( ElypseInstance * p_instance, const String & p_linkedToName );

			bool ShowConsole();

		public:
			inline Root 		*		GetRoot()const
			{
				return m_root;
			}
			inline DownloadManager *	GetDownloadManager()const
			{
				return m_downloadManager;
			}
			inline ElypseLoadingBar *	GetLoadingBar()const
			{
				return m_loadingBar;
			}
			inline Log 		*		GetLog()const
			{
				return m_log;
			}
			inline Path					GetInstallPath()const
			{
				return m_installDir;
			}

			inline unsigned int			GetNextAppIndex()
			{
				return m_numInstance ++;
			}
		};
	}
}

#endif
