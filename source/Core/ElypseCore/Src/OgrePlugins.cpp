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
#include "PrecompiledHeader.h"

#include "OgrePlugins.h"

#include "ElypseLogs.h"

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
			
		bool PluginLoader::load( const String & p_installPath, Root * p_root )
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

		void PluginLoader::loadStatic( const String & p_installPath, Root * p_root )
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
#endif
		}

		void PluginLoader::loadDynamic( const String & p_installPath, Root * p_root )
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
			String l_pluginPath = "../lib/";
#	endif

#	if ELYPSE_WINDOWS
			p_root->loadPlugin( l_pluginPath + "RenderSystem_Direct3D9" + l_debugSuffix );
#	else
			p_root->loadPlugin( l_pluginPath + "RenderSystem_GL" + l_debugSuffix );
#	endif

			p_root->loadPlugin( l_pluginPath + "Plugin_CgProgramManager" + l_debugSuffix );
			p_root->loadPlugin( l_pluginPath + "Plugin_ParticleFX" + l_debugSuffix );
#endif
		}
	}
}

