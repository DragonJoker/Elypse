#ifndef ___OGRE_PLUGIN_LOADER_H___
#define ___OGRE_PLUGIN_LOADER_H___

#include "Module_Main.h"

#if ELYPSE_WINDOWS
#	include <Windows.h>
#endif

#if defined(OGRE_USE_STATIC_PLUGINS)
#	include <OgreCgPlugin.h>
#	include <OgreParticleFXPlugin.h>
#	if ELYPSE_WINDOWS
#		ifndef ____GENLIB_NO_MEMORY_DEBUG____
#			undef new
#		endif
#		include <OgreD3D9Plugin.h>
#		ifndef ____GENLIB_NO_MEMORY_DEBUG____
#			define new General::Utils::MemoryManager::GetSingleton() << General::Utils::MemoryBlock( __FILE__, __FUNCTION__, __LINE__) << new
#		endif
#		pragma comment( lib, "RenderSystem_Direct3D9Static.lib")
#		pragma comment( lib, "Plugin_ParticleFXStatic.lib")
#		pragma comment( lib, "Plugin_CgProgramManagerStatic.lib")
#	else
#		include <OgreGLPlugin.h>
#	endif
#endif

#include <AutoSingleton.h>

namespace EMuse
{
	namespace Main
	{
		class PluginLoader : public AutoSingleton<PluginLoader>
		{
		protected:
#if defined(OGRE_USE_STATIC_PLUGINS)
			CgPlugin * m_cgPlugin;
			ParticleFXPlugin * m_particleFXPlugin;
#	if ELYPSE_WINDOWS
			D3D9Plugin * m_graphPlugin;
#	else
			GLPlugin * m_graphPlugin;
#	endif
		public:
			PluginLoader()
				:	m_cgPlugin( NULL ),
					m_particleFXPlugin( NULL ),
					m_graphPlugin( NULL )
			{
			}
#else
		public:
			PluginLoader()
			{
			}
#endif

			bool load( const String & p_installPath, Root * p_root )
			{
				try
				{
#if defined(OGRE_USE_STATIC_PLUGINS)
#	if ELYPSE_WINDOWS
					m_graphPlugin = new D3D9Plugin();
#	else
					m_graphPlugin = new GLPlugin();
#	endif
					p_root->installPlugin( m_graphPlugin );
					m_cgPlugin = new CgPlugin();
					p_root->installPlugin( m_cgPlugin );
					m_particleFXPlugin = new ParticleFXPlugin();
					p_root->installPlugin( m_particleFXPlugin );
#else
#	if ELYPSE_WINDOWS
					//All thanks to vista for that one -_-
					EMUSE_LOG_MESSAGE_RELEASE( "miaou 1" );
					SetCurrentDirectoryA( p_installPath.c_str() );
					String l_pluginPath = "";
#		if defined( NDEBUG )
					p_root->loadPlugin( l_pluginPath + "RenderSystem_Direct3D9" );
#		else
					p_root->loadPlugin( l_pluginPath + "RenderSystem_Direct3D9_d" );
#		endif
#	elif ELYPSE_LINUX
					String l_pluginPath = "/usr/lib/OGRE/";
					p_root->loadPlugin( l_pluginPath + "RenderSystem_GL" );
#	elif ELYPSE_MACOS
					String l_pluginPath = "/usr/lib/OGRE/";
					p_root->loadPlugin( l_pluginPath + "RenderSystem_GL" );
#	endif
#	if defined( NDEBUG )
					p_root->loadPlugin( l_pluginPath + "Plugin_CgProgramManager" );
					p_root->loadPlugin( l_pluginPath + "Plugin_ParticleFX" );
#	else
					p_root->loadPlugin( l_pluginPath + "Plugin_CgProgramManager_d" );
					p_root->loadPlugin( l_pluginPath + "Plugin_ParticleFX_d" );
#	endif
#endif //defined(OGRE_USE_STATIC_PLUGINS)
				}
				catch ( Ogre::Exception & exc )
				{
					EMUSE_LOG_MESSAGE_RELEASE( exc.what() );
					return false;
				}
				catch ( ... )
				{
					return false;
				}

				return true;
			}

			void unload()
			{
#if defined(OGRE_USE_STATIC_PLUGINS)
				// don't unload plugins, since Root will have done that. Destroy here.
				delete m_particleFXPlugin;
				delete m_cgPlugin;
				delete m_graphPlugin;
#endif
			}
		};
	}
}

#endif
