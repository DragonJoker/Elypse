/*
See LICENSE file in root folder
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
