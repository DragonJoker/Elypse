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
		enum TETreeItemType
		{
			TITProjectRoot,
			TITSceneRoot,
			TITSceneFile,
			TITLoadScriptFile,
			TITUnloadScriptFile,
			TITDataFile,
			TITDataFolder,
			TITFunctionTitle,
			TITFunction,
			TITClassFunction,
			TITFunctionsRoot,
			TITObjectsRoot,
			TITNodes,
			TITNode,
			TITObjects,
			TITObject,
			TITLights,
			TITLight,
			TITCameras,
			TITCamera,
			TITOverlays,
			TITOverlay,
			TITAnimationGroups,
			TITAnimationGroup,
			TITAnimatedObject,
			TITSounds,
			TITSound,
			TITSoundObject,
			TITSoundPlaylist,
			TITVideos,
			TITVideo,
			TITVideoObject,
			TITConstant,
			TITConstantTitle
		};

		class TETreeItemData
			: public wxTreeItemData
		{
		private:
			wxString m_desc;
			TETreeItemType m_fileType;

		public:
			TETreeItemData( wxString const & desc, TETreeItemType p_type )
				: m_desc( desc )
				, m_fileType( p_type )
			{
			}

		public:

			inline TETreeItemType GetFileType()const
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
