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
#ifndef _MyTreeItemData_H
#define _MyTreeItemData_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/treectrl.h>

namespace Troll
{
	namespace GUI
	{
		enum class TreeItemType
		{
			ProjectRoot,
			SceneRoot,
			SceneFile,
			LoadScriptFile,
			UnloadScriptFile,
			DataFile,
			DataFolder,
			FunctionTitle,
			Function,
			ClassFunction,
			FunctionsRoot,
			ObjectsRoot,
			Nodes,
			Node,
			Objects,
			Object,
			Lights,
			Light,
			Cameras,
			Camera,
			Overlays,
			Overlay,
			AnimationGroups,
			AnimationGroup,
			AnimatedObject,
			Sounds,
			Sound,
			SoundObject,
			SoundPlaylist,
			Videos,
			Video,
			VideoObject,
			Constant,
			ConstantTitle
		};

		class TreeItemData
			: public wxTreeItemData
		{
		private:
			wxString m_desc;
			TreeItemType m_fileType;

		public:
			TreeItemData( wxString const & desc, TreeItemType p_type )
				: m_desc( desc )
				, m_fileType( p_type )
			{
			}

		public:

			inline TreeItemType GetFileType()const
			{
				return m_fileType;
			}
			inline const wxChar * GetDesc()const
			{
				return m_desc.c_str();
			}
		};
	}
}

#endif
