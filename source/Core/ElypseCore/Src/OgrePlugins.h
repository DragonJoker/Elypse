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
#ifndef ___OGRE_PLUGIN_LOADER_H___
#define ___OGRE_PLUGIN_LOADER_H___

#include "Module_Main.h"

#if ELYPSE_WINDOWS
#	include <Windows.h>
#endif

#if defined(OGRE_USE_STATIC_PLUGINS)
//#	include <OgreCgPlugin.h>
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
//#		pragma comment( lib, "Plugin_CgProgramManagerStatic.lib")
#	else
#		include <OgreGLPlugin.h>
#	endif
#endif

#include <AutoSingleton.h>

namespace Elypse
{
	namespace Main
	{
		class PluginLoader
			: public AutoSingleton<PluginLoader>
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
#endif
		public:
			PluginLoader();

			bool load( String const & p_installPath, Root * p_root );
			void unload();

		private:
			void loadStatic( String const & p_installPath, Root * p_root );
			void loadDynamic( String const & p_installPath, Root * p_root );
		};
	}
}

#endif
