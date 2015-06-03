
#ifndef _MyTreeItemData_H
#define _MyTreeItemData_H

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
			TETreeItemData( const wxString & desc, TETreeItemType p_type )
				:	m_desc( desc ),
					m_fileType( p_type )
			{}

		public:

			inline TETreeItemType	GetFileType()const
			{
				return m_fileType;
			}
			inline const wxChar *	GetDesc()const
			{
				return m_desc.c_str();
			}
		};
	}
}
#endif
