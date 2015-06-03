#ifndef ___EMUSE_INSTANCE_H___
#define ___EMUSE_INSTANCE_H___

#include "Module_Main.h"

#include "Module_Download.h"

#include <Thread.h>
#include <Mutex.h>
#include <Condition.h>

#undef SetCurrentDirectory

namespace EMuse
{
	namespace Main
	{
		class d_dll_export EMuseInstance : public named, d_noncopyable
		{
		protected:
			String m_handle;
			String m_appIndexStr;
			String m_startupScript;

			Path m_installationPath;
			Url m_mainUrl;
			Url m_currentDirectory;

			Root * m_root;
			RenderWindow * m_renderWindow;

			EMusePlugin * m_plugin;
			Thread * m_thread;

			DownloadManager * m_downManager;

			//SceneManager * m_sceneMgr;
			EMuseFrameListener * m_frameListener;
			EMuseLoadingBar * m_loadingBar;

			unsigned int m_antialiasing;
			unsigned int m_height;
			unsigned int m_width;

			bool m_download;
			bool m_useDirectX;
			bool m_initialised;
			bool m_toDelete;
			bool m_deleted;
			bool m_hasFocus;
			bool m_main;
			bool m_hasPrimaryWindow;
			bool m_showFPS;
			bool m_useConsole;
			bool m_deactivated;
			bool m_linked;

			unsigned long m_lastFrameTime;

			Mutex m_mutex;
			Condition m_condition;

			InstanceSet m_instancesLinkedToSelf;

		public:
			EMuseInstance( const Path & p_installDir, EMusePlugin * p_plugin );
			virtual ~EMuseInstance();

		public:
			bool Init( unsigned int p_width, unsigned int p_height, const String & p_linkedTo );

			void Initialise();

			void LinkTo( EMuseInstance * p_originalInstance );
			void StartThread();
			void SetToDelete( bool p_delete );
			void RenderOneFrame();
			bool IsActive();
			void WaitForDeletion();

		public:
			virtual void CreateScene();
			virtual void Destroy();

		protected:
			void _isOriginLinkFor( EMuseInstance * p_instance );
			virtual void _destroyFrameListener();
			virtual void _createFramelistener();

		public:
			inline void SetMain( bool p_primary )
			{
				m_main = p_primary;
			}
			inline void SetHandle( const String & p_handle )
			{
				m_handle = p_handle;
			}
			inline void SetFileUrl( const Url & p_url )
			{
				m_mainUrl = p_url;
				m_mainUrl.GetProtocol();
			}
			inline void UseDirectX( bool p_useDirectX )
			{
				m_useDirectX = p_useDirectX;
			}
			inline void ShowFPS( bool p_showFPS )
			{
				m_showFPS = p_showFPS;
			}
			inline void SetAntialiasing( unsigned int p_antialiasing )
			{
				m_antialiasing = p_antialiasing;
			}
			inline void SetDownloadFiles( bool p_download )
			{
				m_download = p_download;
			}
			inline void	SetFocus( bool p_focus )
			{
				m_hasFocus = p_focus;
			}
			inline void	SetStartupScript( const String & p_script )
			{
				m_startupScript = p_script;
			}
			inline void	Deactivate()
			{
				m_deactivated = true;
			}
			inline void SetCurrentDirectory( const Url & p_dir )
			{
				m_currentDirectory = p_dir;
			}

			inline bool					IsDeleted()const
			{
				return m_deleted;
			}
			inline bool					IsMain()const
			{
				return m_main;
			}
			inline bool					HasFocus()const
			{
				return m_hasFocus;
			}
			inline Thread 		*		GetThread()const
			{
				return m_thread;
			}
			inline DownloadManager	*	GetDownloadManager()const
			{
				return m_downManager;
			}
			inline EMuseLoadingBar	*	GetLoadingBar()const
			{
				return m_loadingBar;
			}
			inline bool					IsInitialised()const
			{
				return m_initialised;
			}
			inline bool					IsToDelete()const
			{
				return m_toDelete;
			}
			inline EMusePlugin 	*	GetPlugin()const
			{
				return m_plugin;
			}
			inline const String 	&	GetHandle()const
			{
				return m_handle;
			}
			inline EMuseFrameListener * GetFL()const
			{
				return m_frameListener;
			}

			inline unsigned int			GetWindowWidth()const
			{
				return m_width;
			}
			inline unsigned int			GetWindowHeight()const
			{
				return m_height;
			}

			inline const Url 	&		GetFileUrl()const
			{
				return m_mainUrl;
			}
			inline const Url 	&		GetCurrentDirectory()const
			{
				return m_currentDirectory;
			}
			inline RenderWindow 	*	GetWindow()const
			{
				return m_renderWindow;
			}
		};
	}
}

#endif
