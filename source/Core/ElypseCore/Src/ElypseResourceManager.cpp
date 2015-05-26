#include "PrecompiledHeader.h"

#include "ElypseResourceGroupManager.h"

using namespace EMuse;
using namespace Ogre;

String ElypseResourceGroupManager::sm_strPrefix;

void ElypseResourceGroupManager :: createResourceGroup( const String & name, const bool inGlobalPool )
{
	ResourceGroupManager::getSingleton().createResourceGroup( sm_strPrefix + name, inGlobalPool );
}

void ElypseResourceGroupManager :: initialiseResourceGroup( const String & name )
{
	ResourceGroupManager::getSingleton().initialiseResourceGroup( sm_strPrefix + name );
}

void ElypseResourceGroupManager :: loadResourceGroup( const String & name, bool loadMainResources, bool loadWorldGeom )
{
	ResourceGroupManager::getSingleton().loadResourceGroup( sm_strPrefix + name, loadMainResources, loadWorldGeom );
}

void ElypseResourceGroupManager :: unloadResourceGroup( const String & name, bool reloadableOnly )
{
	ResourceGroupManager::getSingleton().unloadResourceGroup( sm_strPrefix + name, reloadableOnly );
}

void ElypseResourceGroupManager :: clearResourceGroup( const String & name )
{
	ResourceGroupManager::getSingleton().clearResourceGroup( sm_strPrefix + name );
}

void ElypseResourceGroupManager :: destroyResourceGroup( const String & name )
{
	ResourceGroupManager::getSingleton().destroyResourceGroup( sm_strPrefix + name );
}

void ElypseResourceGroupManager :: addResourceLocation( const String & name, const String & locType, const String & resGroup, bool recursive )
{
	ResourceGroupManager::getSingleton().addResourceLocation( name, locType, sm_strPrefix + resGroup, recursive );
}

void ElypseResourceGroupManager :: removeResourceLocation( const String & name, const String & resGroup )
{
	ResourceGroupManager::getSingleton().removeResourceLocation( name, sm_strPrefix + resGroup );
}

bool ElypseResourceGroupManager :: resourceLocationExists( const String & name, const String & resGroup )
{
	return ResourceGroupManager::getSingleton().resourceLocationExists( name, sm_strPrefix + resGroup );
}