/*
See LICENSE file in root folder
*/
#ifndef ___TE_SCENE_FILE_H___
#define ___TE_SCENE_FILE_H___

#include "Module_Project.h"

#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/treebase.h>
#include "GUI/TrollEditorGuiPrerequisites.h"
#include <Module_Data.h>
#include "Elypse/TrollMuseFile.h"
#include "3D/Module_3D.h"
#include "2D/Module_2D.h"
#include "Media/Module_Media.h"
#include "Temporal/Module_Temporal.h"
#include <ConfigFile.h>

namespace Troll
{
	namespace ProjectComponents
	{
		class SceneFile
		{
		public:
			SceneFile();
			~SceneFile();

			Objects3D::TrollEntity * GetEntity( wxString const & p_name )const;
			Objects2D::TrollOverlay * GetOverlay( wxString const & p_name )const;
			Objects3D::TrollSceneNode * GetSceneNode( wxString const & p_name )const;
			Objects3D::TrollLight * GetLight( wxString const & p_name )const;
			Objects3D::TrollCamera * GetCamera( wxString const & p_name )const;
			Temporal::TrollAnimationGroup * GetAnimationGroup( wxString const & p_name )const;
			Media::TrollSound * GetSound( wxString const & p_name )const;
			Media::TrollVideo * GetVideo( wxString const & p_name )const;
			Temporal::TrollSequence * GetSequence( wxString const & p_name )const;
			void Write( wxString const & p_path )const;

		private:
			void DoWriteUnusedFilePieces( wxTextOutputStream * p_textStream )const;

		public:
			bool m_forceShadows;
			ShadowType m_shadowType;
			ColourValue m_ambientLight;
			wxString m_fileName;
			Objects3D::TrollEntityMap m_entities;
			Objects2D::TrollOverlayMap m_overlayGroups;
			Objects2D::TrollOverlayMap m_allOverlays;
			Objects3D::TrollSceneNodeMap m_allSceneNodes;
			Objects3D::TrollSceneNodeMap m_sceneNodes;
			Objects3D::TrollLightMap m_lights;
			Objects3D::TrollCameraMap m_cameras;
			Temporal::TrollAnimationGroupStrMap m_animationGroups;
			Media::TrollSoundStrMap m_sounds;
			Media::TrollVideoStrMap m_videos;
			Temporal::TrollSequenceStrMap m_sequences;
		};
	}
}

#endif
