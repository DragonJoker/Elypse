/*
See LICENSE file in root folder
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
