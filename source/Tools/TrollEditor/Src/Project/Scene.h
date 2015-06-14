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
#ifndef ___TESCENE_H___
#define ___TESCENE_H___

#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/treebase.h>
#include "GUI/TrollEditorGuiPrerequisites.h"
#include <Module_Data.h>
#include "Elypse/TrollMuseFile.h"
#include "Module_Project.h"
#include "3D/Module_3D.h"
#include "2D/Module_2D.h"
#include "Media/Module_Media.h"
#include "Temporal/Module_Temporal.h"
#include "File.h"
#include "SceneFile.h"

BEGIN_TROLL_PROJECT_NAMESPACE
{
	class Scene
	{
		friend class Project;

	public:
		Scene( ProjectSPtr p_project, const wxString & p_name, bool p_main = false );
		~Scene();

		void FlushFiles();
		void FlushObjects();

		void SetCurrentSceneFile( const wxString & p_sceneFileName );

		bool AddSceneFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool AddLoadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool AddUnloadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool AddDataFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool AddDataFolder( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool AddFile( const wxTreeItemId & p_folder, const wxTreeItemId & p_item, const wxString & p_pathName, bool p_saved, bool p_showError = true );
		Objects3D::TrollEntity * AddEntity( const wxString & p_name, const wxString & p_fileName );
		Objects2D::TrollOverlay * AddOverlay( const wxString & p_name, const wxString & p_parentName, const wxString & p_fileName );
		Objects3D::TrollSceneNode * AddSceneNode( const wxString & p_name, const wxString & p_fileName );
		Objects3D::TrollLight * AddLight( const wxString & p_name, const wxString & p_fileName );
		Objects3D::TrollCamera * AddCamera( const wxString & p_name, const wxString & p_fileName );
		Objects3D::TrollMesh * AddMesh( const wxString & p_name );
		Temporal::TrollAnimationGroup * AddAnimationGroup( const wxString & p_name, const wxString & p_fileName );
		Media::TrollSound * AddSound( const wxString & p_name, const wxString & p_fileName );
		Media::TrollVideo * AddVideo( const wxString & p_name, const wxString & p_fileName );
		Temporal::TrollSequence * AddSequence( Temporal::TrollSequence * p_sequence, const wxString & p_fileName );

		bool AddOverlay( Objects2D::TrollOverlay * p_overlay );

		void AttachNode( Objects3D::TrollSceneNode * p_node, Objects3D::TrollSceneNode * p_parent );

		void RemoveSceneFile( const wxString & p_pathName );
		void RemoveLoadScriptFile( const wxString & p_pathName );
		void RemoveUnloadScriptFile( const wxString & p_pathName );
		void RemoveDataFile( const wxString & p_pathName );
		void RemoveDataFolder( const wxString & p_pathName );
		void RemoveFile( const wxTreeItemId & p_item );
		void RemoveEntity( const wxString & p_name );
		void RemoveSceneNode( const wxString & p_name );
		void RemoveLight( const wxString & p_name );
		void RemoveCamera( const wxString & p_name );

		bool HasItem( const wxTreeItemId & p_item );
		void Save( wxTextOutputStream * p_stream );
		void AddToTree( GUI::FilesTree * p_tree );
		void AddSceneFile( wxString const & p_strFile, GUI::FilesTree * p_tree );
		void AddLoadScriptFile( wxString const & p_strFile, GUI::FilesTree * p_tree );
		void AddUnloadScriptFile( wxString const & p_strFile, GUI::FilesTree * p_tree );
		void Add3DDataFile( wxString const & p_strFile, GUI::FilesTree * p_tree );
		void AddOtherDataFile( wxString const & p_strFile, GUI::FilesTree * p_tree );
		void AddDependency( wxString const & p_strName );
		wxString Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, Troll::GUI::FilesTree * p_tree = NULL );
		void Write( const wxString & p_path );
		void UpdateFileName( const wxString & p_ancient, const wxString & p_new );
		FileType GetType( const wxTreeItemId & p_folderId );

		File * GetFile( const wxTreeItemId & p_itemId );
		File * GetFileByName( const wxString & p_itemName );
		File * GetSceneFile( unsigned int p_index );
		File * GetLoadScriptFile( unsigned int p_index );
		File * GetUnloadScriptFile( unsigned int p_index );
		File * GetDataFile( unsigned int p_index );
		File * GetDataFolder( unsigned int p_index );
		Objects3D::TrollEntity * GetEntity( const wxString & p_name );
		Objects3D::TrollMesh * GetMesh( const wxString & p_name );
		Objects2D::TrollOverlay * GetOverlay( const wxString & p_name );
		Objects3D::TrollSceneNode * GetSceneNode( const wxString & p_name );
		Objects3D::TrollLight * GetLight( const wxString & p_name );
		Objects3D::TrollCamera * GetCamera( const wxString & p_name );
		Temporal::TrollAnimationGroup * GetAnimationGroup( const wxString & p_name );
		Media::TrollSound * GetSound( const wxString & p_name );
		Media::TrollVideo * GetVideo( const wxString & p_name );
		Temporal::TrollSequence * GetSequence( const wxString & p_name );

		void GetNodesNames( wxArrayString & p_list, const wxString & p_name = wxEmptyString );
		void GetOverlaysNames( wxArrayString & p_list );
		void GetMeshesNames( wxArrayString & p_list );
		void GetSequencesNames( wxArrayString & p_list );

		void AddDependency( Scene * p_scene );
		void RemoveDependency( Scene * p_scene );

		inline const wxString & GetName()const
		{
			return m_name;
		}
		inline const wxTreeItemId & GetTreeItem()const
		{
			return m_sceneId;
		}
		inline unsigned int GetSceneFilesCount()const
		{
			return m_sceneFiles.size();
		}
		inline unsigned int GetLoadScriptFilesCount()const
		{
			return m_loadScriptFiles.size();
		}
		inline unsigned int GetUnloadScriptFilesCount()const
		{
			return m_unloadScriptFiles.size();
		}
		inline unsigned int GetDataFilesCount()const
		{
			return m_dataFiles.size();
		}
		inline unsigned int GetDataFoldersCount()const
		{
			return m_dataFolders.size();
		}
		inline FileArray const & GetSceneFiles()const
		{
			return m_sceneFiles;
		}
		inline FileArray const & GetLoadScriptFiles()const
		{
			return m_loadScriptFiles;
		}
		inline FileArray const & GetUnloadScriptFiles()const
		{
			return m_unloadScriptFiles;
		}
		inline FileArray const & GetDataFiles()const
		{
			return m_dataFiles;
		}
		inline FileArray const & GetDataFolders()const
		{
			return m_dataFolders;
		}
		inline SceneFile * GetCurrentSceneFile()const
		{
			return m_currentSceneFile;
		}
		inline bool IsMainScene()const
		{
			return m_mainScene;
		}
		inline bool IsNeeded()const
		{
			return m_needed;
		}
		inline SceneMap GetScenesThatNeedMe()const
		{
			return m_scenesThatNeedMe;
		}
		inline wxArrayString GetNeededScenes()const
		{
			return m_neededScenes;
		}
		inline void	SetName( const wxString & p_name )
		{
			m_name = p_name;
		}
		inline void SetNeeded( bool p_needed )
		{
			m_needed = p_needed;
		}
		inline TROLL_ELYPSE_NAMESPACE::TrollMuseFile * GetMuseFile()const
		{
			return m_museFile;
		}
		
	private:
		inline void _addSceneThatNeedMe( Scene * p_scene )
		{
			m_scenesThatNeedMe.insert( std::make_pair( p_scene->GetName(), p_scene ) );
		}
		void _removeSceneThatNeedMe( Scene * p_scene );

	public:
		wxTreeItemId m_sceneFileId;
		wxTreeItemId m_loadScriptFileId;
		wxTreeItemId m_unloadScriptFileId;
		wxTreeItemId m_dataFileId;
		wxTreeItemId m_dataFolderId;
		wxTreeItemId m_sceneId;

		FileArray m_sceneFiles;
		FileArray m_loadScriptFiles;
		FileArray m_unloadScriptFiles;
		FileArray m_dataFiles;
		FileArray m_dataFolders;
		FileArray m_museFiles;

		TROLL_ELYPSE_NAMESPACE::TrollMuseFile * m_museFile;

	private:
		wxString m_name;
		float m_minVerticCamAngle;
		float m_maxVerticCamAngle;
		float m_minHorizCamAngle;
		float m_maxHorizCamAngle;
		float m_minZoom;
		float m_maxZoom;
		bool m_mainScene;
		bool m_needed;
		SceneMap m_scenesThatNeedMe;
		wxArrayString m_neededScenes;
		SceneFileStrMap m_sceneConfigFiles;
		SceneFile * m_currentSceneFile;
		Objects3D::TrollMeshStrMap m_meshes;
	};
}
END_TROLL_PROJECT_NAMESPACE

#endif

