/*
See LICENSE file in root folder
*/
#ifndef ___EM_ResourceGroupManager___
#define ___EM_ResourceGroupManager___

namespace Elypse
{
	class d_dll_export ElypseResourceGroupManager
	{
	private:
		static String	sm_strPrefix;

	public:
		ElypseResourceGroupManager() {}
		~ElypseResourceGroupManager() {}

		static inline String const & getPrefix()
		{
			return sm_strPrefix;
		}
		static inline void setPrefix( String const & p_strPrefix )
		{
			sm_strPrefix = p_strPrefix;
		}

		static void createResourceGroup( String const & name, const bool inGlobalPool = true );
		static void initialiseResourceGroup( String const & name );
		static void loadResourceGroup( String const & name, bool loadMainResources = true, bool loadWorldGeom = true );
		static void unloadResourceGroup( String const & name, bool reloadableOnly = true );
		static void clearResourceGroup( String const & name );
		static void destroyResourceGroup( String const & name );

		static void addResourceLocation( String const & name, String const & locType, String const & resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, bool recursive = false );
		static void removeResourceLocation( String const & name, String const & resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		static bool resourceLocationExists( String const & name, String const & resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	};
}

#endif