/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "OgrePlugins.h"

#include "ElypseLogs.h"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

namespace Elypse
{
	namespace Main
	{
		PluginLoader::PluginLoader()
#if defined( OGRE_USE_STATIC_PLUGINS )
			: m_cgPlugin( NULL )
			, m_particleFXPlugin( NULL )
			, m_graphPlugin( NULL )
#endif
		{
		}

		bool PluginLoader::load( String const & p_installPath, Root * p_root )
		{
			try
			{
#if defined(OGRE_USE_STATIC_PLUGINS)
				loadStatic( p_installPath, p_root );
#else
				loadDynamic( p_installPath, p_root );
#endif
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

		void PluginLoader::unload()
		{
#if defined(OGRE_USE_STATIC_PLUGINS)
			// don't unload plugins, since Root will have done that. Destroy here.
			delete m_particleFXPlugin;
			delete m_cgPlugin;
			delete m_graphPlugin;
#endif
		}

		void PluginLoader::loadStatic( String const & p_installPath, Root * p_root )
		{
#if defined(OGRE_USE_STATIC_PLUGINS)
#	if ELYPSE_WINDOWS
			m_graphPlugin = new D3D9Plugin();
#	else
			m_graphPlugin = new GLPlugin();
#	endif
			p_root->installPlugin( m_graphPlugin );
//			m_cgPlugin = new CgPlugin();
			p_root->installPlugin( m_cgPlugin );
			m_particleFXPlugin = new ParticleFXPlugin();
			p_root->installPlugin( m_particleFXPlugin );
#endif
		}

		void PluginLoader::loadDynamic( String const & p_installPath, Root * p_root )
		{
#if !defined(OGRE_USE_STATIC_PLUGINS)
			String l_debugSuffix;
#	if ELYPSE_WINDOWS && !defined( NDEBUG )
			l_debugSuffix = "_d";
#	endif
#	if ELYPSE_WINDOWS
			EMUSE_LOG_MESSAGE_RELEASE( "Using Vista workaround..." );
			SetCurrentDirectoryA( p_installPath.c_str() );
			String l_pluginPath = "";
#	else
			String l_pluginPath;
#	endif

#	if ELYPSE_WINDOWS
			p_root->loadPlugin( l_pluginPath + "RenderSystem_GL" + l_debugSuffix );
			//p_root->loadPlugin( l_pluginPath + "RenderSystem_Direct3D11" + l_debugSuffix );
#	else
			p_root->loadPlugin( l_pluginPath + "RenderSystem_GL" + l_debugSuffix );
#	endif
			p_root->loadPlugin( l_pluginPath + "Plugin_CgProgramManager" + l_debugSuffix );
			p_root->loadPlugin( l_pluginPath + "Plugin_ParticleFX" + l_debugSuffix );
#endif
		}
	}
}

