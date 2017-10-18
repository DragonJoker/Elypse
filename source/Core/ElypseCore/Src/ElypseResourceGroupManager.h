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