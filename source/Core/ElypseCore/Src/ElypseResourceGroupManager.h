#ifndef ___EM_ResourceGroupManager___
#define ___EM_ResourceGroupManager___

namespace EMuse
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

		static void createResourceGroup( const String & name, const bool inGlobalPool = true );
		static void initialiseResourceGroup( const String & name );
		static void loadResourceGroup( const String & name, bool loadMainResources = true, bool loadWorldGeom = true );
		static void unloadResourceGroup( const String & name, bool reloadableOnly = true );
		static void clearResourceGroup( const String & name );
		static void destroyResourceGroup( const String & name );

		static void addResourceLocation( const String & name, const String & locType, const String & resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, bool recursive = false );
		static void removeResourceLocation( const String & name, const String & resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		static bool resourceLocationExists( const String & name, const String & resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	};
}

#endif