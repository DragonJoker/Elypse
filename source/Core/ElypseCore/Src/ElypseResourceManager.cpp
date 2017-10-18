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

#include "ElypseResourceGroupManager.h"

using namespace Elypse;
using namespace Ogre;

String ElypseResourceGroupManager::sm_strPrefix;

void ElypseResourceGroupManager::createResourceGroup( String const & name, const bool inGlobalPool )
{
	ResourceGroupManager::getSingleton().createResourceGroup( sm_strPrefix + name, inGlobalPool );
}

void ElypseResourceGroupManager::initialiseResourceGroup( String const & name )
{
	ResourceGroupManager::getSingleton().initialiseResourceGroup( sm_strPrefix + name );
}

void ElypseResourceGroupManager::loadResourceGroup( String const & name, bool loadMainResources, bool loadWorldGeom )
{
	ResourceGroupManager::getSingleton().loadResourceGroup( sm_strPrefix + name, loadMainResources, loadWorldGeom );
}

void ElypseResourceGroupManager::unloadResourceGroup( String const & name, bool reloadableOnly )
{
	ResourceGroupManager::getSingleton().unloadResourceGroup( sm_strPrefix + name, reloadableOnly );
}

void ElypseResourceGroupManager::clearResourceGroup( String const & name )
{
	ResourceGroupManager::getSingleton().clearResourceGroup( sm_strPrefix + name );
}

void ElypseResourceGroupManager::destroyResourceGroup( String const & name )
{
	ResourceGroupManager::getSingleton().destroyResourceGroup( sm_strPrefix + name );
}

void ElypseResourceGroupManager::addResourceLocation( String const & name, String const & locType, String const & resGroup, bool recursive )
{
	ResourceGroupManager::getSingleton().addResourceLocation( name, locType, sm_strPrefix + resGroup, recursive );
}

void ElypseResourceGroupManager::removeResourceLocation( String const & name, String const & resGroup )
{
	ResourceGroupManager::getSingleton().removeResourceLocation( name, sm_strPrefix + resGroup );
}

bool ElypseResourceGroupManager::resourceLocationExists( String const & name, String const & resGroup )
{
	return ResourceGroupManager::getSingleton().resourceLocationExists( name, sm_strPrefix + resGroup );
}