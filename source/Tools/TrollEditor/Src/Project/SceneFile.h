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

BEGIN_TROLL_PROJECT_NAMESPACE
{
	class SceneFile
	{
	public:
		SceneFile();
		~SceneFile();
		
		Objects3D::TrollEntity * GetEntity( const wxString & p_name )const;
		Objects2D::TrollOverlay * GetOverlay( const wxString & p_name )const;
		Objects3D::TrollSceneNode * GetSceneNode( const wxString & p_name )const;
		Objects3D::TrollLight * GetLight( const wxString & p_name )const;
		Objects3D::TrollCamera * GetCamera( const wxString & p_name )const;
		Temporal::TrollAnimationGroup * GetAnimationGroup( const wxString & p_name )const;
		Media::TrollSound * GetSound( const wxString & p_name )const;
		Media::TrollVideo * GetVideo( const wxString & p_name )const;
		Temporal::TrollSequence * GetSequence( const wxString & p_name )const;
		void Write( const wxString & p_path );

	private:
		void _writeUnusedFilePieces( wxTextOutputStream * p_textStream );

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
END_TROLL_PROJECT_NAMESPACE

#endif
