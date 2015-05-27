
#ifndef ___TESCENE_H___
#define ___TESCENE_H___

#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/treebase.h>
#include "../GUI/Module_GUI.h"
#include <Module_Data.h>
#include "../Data/TrollMuseFile.h"
#include "Module_Project.h"
#include "3D/Module_3D.h"
#include "2D/Module_2D.h"
#include "Media/Module_Media.h"
#include "Temporal/Module_Temporal.h"

namespace Troll
{
	namespace GUI
	{
		class wxStcTextEditor;
	}

	struct TrollFile
	{
		wxString						FileName;
		wxString						Folder;
		bool							Open;
		bool							Saved;
		wxTreeItemId					ItemId;
		wxTreeItemId					FolderId;
		FileType						Filetype;
		ConfigFile 		*			CfgFile;
//		Troll::GUI::TextPanel *			EditPanel;
		Troll::GUI::wxStcTextEditor *	EditPanel;
		int								IdOnglet;
		TrollScene 		*			m_scene;
		bool							m_compiled;

		TrollFile( TrollScene * p_pScene, FileType p_eFileType, wxString const & p_strFile, EMuse::Data::MuseFile * p_pMuseFile, wxTreeItemId p_iFolderId )
			:	m_scene( p_pScene	)
			,	Filetype( p_eFileType	)
			,	CfgFile( new ConfigFile( String( ( char const * )p_strFile.char_str() ), p_pMuseFile )	)
			,	FolderId( p_iFolderId	)
			,	Saved( true	)
			,	Open( false	)
			,	ItemId( 0	)
			,	EditPanel( NULL	)
			,	IdOnglet( 0	)
			,	m_compiled( false	)
		{
			//TODOMIAOU ?
			CfgFile->DownloadFinished();
			std::size_t l_index = min( p_strFile.find_last_of( wxT( "\\" ) ), p_strFile.find_last_of( wxT( "/" ) ) );
			FileName	= p_strFile.substr( l_index + 1 );
			Folder		= p_strFile.substr( 0, l_index + 1 );
		}

		TrollFile()
			:	FileName( wxEmptyString ),
				Open( false ),
				Saved( false ),
				ItemId( 0 ),
				FolderId( 0 ),
				Filetype( sceneFile ),
				CfgFile( NULL ),
				EditPanel( NULL ),
				IdOnglet( 0 ),
				m_scene( NULL ),
				m_compiled( false )
		{
		}

		TrollFile( const TrollFile & p_file )
			:	FileName( p_file.FileName ),
				Open( p_file.Open ),
				Saved( p_file.Saved ),
				ItemId( p_file.ItemId ),
				FolderId( p_file.FolderId ),
				Filetype( p_file.Filetype ),
				CfgFile( p_file.CfgFile ),
				EditPanel( p_file.EditPanel ),
				IdOnglet( p_file.IdOnglet ),
				m_scene( p_file.m_scene ),
				m_compiled( p_file.m_compiled )
		{
		}

		~TrollFile()
		{
			if ( CfgFile != NULL )
			{
				delete CfgFile;
			}
		}
	};

	class TrollSceneFile
	{
	public:
		bool m_forceShadows;
		ShadowType m_shadowType;
		ColourValue m_ambientLight;
		wxString m_fileName;
		TrollEntityMap m_entities;
		TrollOverlayMap m_overlayGroups;
		TrollOverlayMap m_allOverlays;
		TrollSceneNodeMap m_allSceneNodes;
		TrollSceneNodeMap m_sceneNodes;
		TrollLightMap m_lights;
		TrollCameraMap m_cameras;
		TrollAnimationGroupStrMap m_animationGroups;
		TrollSoundStrMap m_sounds;
		TrollVideoStrMap m_videos;
		TrollSequenceStrMap m_sequences;

	public:
		TrollSceneFile();
		~TrollSceneFile();

		Objects3D::TrollEntity 	*		GetEntity( const wxString & p_name )
		{
			return General::Utils::map::findOrNull( m_entities, p_name );
		}
		Objects2D::TrollOverlay  	*		GetOverlay( const wxString & p_name );
		Objects3D::TrollSceneNode 	*	GetSceneNode( const wxString & p_name );
		Objects3D::TrollLight 	*		GetLight( const wxString & p_name );
		Objects3D::TrollCamera 	*		GetCamera( const wxString & p_name );
		Temporal::TrollAnimationGroup *	GetAnimationGroup( const wxString & p_name );
		Media::TrollSound 		*		GetSound( const wxString & p_name );
		Media::TrollVideo 		*		GetVideo( const wxString & p_name );
		Temporal::TrollSequence 	*		GetSequence( const wxString & p_name );
		void Write( const wxString & p_path );

	private:
		void _writeUnusedFilePieces( wxTextOutputStream * p_textStream );
	};

	class TrollScene
	{
		friend class Project;
	public:
		wxTreeItemId m_sceneFileId;
		wxTreeItemId m_loadScriptFileId;
		wxTreeItemId m_unloadScriptFileId;
		wxTreeItemId m_dataFileId;
		wxTreeItemId m_dataFolderId;
		wxTreeItemId m_sceneId;

		TrollFileArray m_sceneFiles;
		TrollFileArray m_loadScriptFiles;
		TrollFileArray m_unloadScriptFiles;
		TrollFileArray m_dataFiles;
		TrollFileArray m_dataFolders;
		TrollFileArray m_museFiles;

		TrollMuseFile * m_museFile;

	private:
		wxString m_name;
		Project * m_project;

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
		TrollSceneFile * m_currentSceneFile;
		TrollMeshStrMap m_meshes;

	public:
		TrollScene( Project * p_project, const wxString & p_name, bool p_main = false );
		~TrollScene();

		void FlushFiles();
		void FlushObjects();

		void SetCurrentSceneFile( const wxString & p_sceneFileName );

		bool				AddSceneFile( TrollFile * p_file );
		bool				AddSceneFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
										  const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool 				AddLoadScriptFile( TrollFile * p_file );
		bool 				AddLoadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
											   const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool 				AddUnloadScriptFile( TrollFile * p_file );
		bool 				AddUnloadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
				const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool 				AddDataFile( TrollFile * p_file );
		bool 				AddDataFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
										 const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool				AddDataFolder( TrollFile * p_file );
		bool				AddDataFolder( const wxString & p_pathName, const wxTreeItemId & p_folder,
										   const wxTreeItemId & p_item, bool p_saved, bool p_showError = true );
		bool				AddMuseFile( TrollFile * p_file );
		bool 				AddFile( const wxTreeItemId & p_folder, const wxTreeItemId & p_item, const wxString & p_pathName,
									 bool p_saved, bool p_showError = true );
		Objects3D::TrollEntity 	*		AddEntity( const wxString & p_name, const wxString & p_fileName );
		Objects2D::TrollOverlay 	*		AddOverlay( const wxString & p_name, const wxString & p_parentName, const wxString & p_fileName );
		Objects3D::TrollSceneNode 	*	AddSceneNode( const wxString & p_name, const wxString & p_fileName );
		Objects3D::TrollLight 	*		AddLight( const wxString & p_name, const wxString & p_fileName );
		Objects3D::TrollCamera 	*		AddCamera( const wxString & p_name, const wxString & p_fileName );
		Objects3D::TrollMesh 		*		AddMesh( const wxString & p_name );
		Temporal::TrollAnimationGroup *	AddAnimationGroup( const wxString & p_name, const wxString & p_fileName );
		Media::TrollSound 		*		AddSound( const wxString & p_name, const wxString & p_fileName );
		Media::TrollVideo 		*		AddVideo( const wxString & p_name, const wxString & p_fileName );
		Temporal::TrollSequence 	*		AddSequence( Temporal::TrollSequence * p_sequence, const wxString & p_fileName );

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

		TrollFile 			*			GetFile( const wxTreeItemId & p_itemId );
		TrollFile 			*			GetFileByName( const wxString & p_itemName );
		TrollFile 			*			GetSceneFile( unsigned int p_index );
		TrollFile 			*			GetLoadScriptFile( unsigned int p_index );
		TrollFile 			*			GetUnloadScriptFile( unsigned int p_index );
		TrollFile 			*			GetDataFile( unsigned int p_index );
		TrollFile 			*			GetDataFolder( unsigned int p_index );
		Objects3D::TrollEntity 	*		GetEntity( const wxString & p_name );
		Objects3D::TrollMesh 		*		GetMesh( const wxString & p_name );
		Objects2D::TrollOverlay  	*		GetOverlay( const wxString & p_name );
		Objects3D::TrollSceneNode 	*	GetSceneNode( const wxString & p_name );
		Objects3D::TrollLight 	*		GetLight( const wxString & p_name );
		Objects3D::TrollCamera 	*		GetCamera( const wxString & p_name );
		Temporal::TrollAnimationGroup *	GetAnimationGroup( const wxString & p_name );
		Media::TrollSound 		*		GetSound( const wxString & p_name );
		Media::TrollVideo 		*		GetVideo( const wxString & p_name );
		Temporal::TrollSequence 	*		GetSequence( const wxString & p_name );

		void GetNodesNames( wxArrayString & p_list, const wxString & p_name = wxEmptyString );
		void GetOverlaysNames( wxArrayString & p_list );
		void GetMeshesNames( wxArrayString & p_list );
		void GetSequencesNames( wxArrayString & p_list );

		void AddDependency( TrollScene * p_scene );
		void RemoveDependency( TrollScene * p_scene );

	private:
		void _removeSceneThatNeedMe( TrollScene * p_scene );
		inline void _addSceneThatNeedMe( TrollScene * p_scene )
		{
			m_scenesThatNeedMe.insert( SceneMap::value_type( p_scene->GetName(), p_scene ) );
		}

	public:
		inline const wxString 	&	GetName()const
		{
			return m_name;
		}
		inline const wxTreeItemId &	GetTreeItem()const
		{
			return m_sceneId;
		}
		inline unsigned int			GetSceneFilesNumber()const
		{
			return m_sceneFiles.size();
		}
		inline unsigned int			GetLoadScriptFilesNumber()const
		{
			return m_loadScriptFiles.size();
		}
		inline unsigned int			GetUnloadScriptFilesNumber()const
		{
			return m_unloadScriptFiles.size();
		}
		inline unsigned int			GetDataFilesNumber()const
		{
			return m_dataFiles.size();
		}
		inline unsigned int			GetDataFoldersNumber()const
		{
			return m_dataFolders.size();
		}
		inline TrollSceneFile 	*	GetCurrentSceneFile()const
		{
			return m_currentSceneFile;
		}
		inline bool					IsMainScene()const
		{
			return m_mainScene;
		}
		inline bool					IsNeeded()const
		{
			return m_needed;
		}
		inline SceneMap				GetScenesThatNeedMe()const
		{
			return m_scenesThatNeedMe;
		}
		inline wxArrayString		GetNeededScenes()const
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
		inline TrollMuseFile 	*	GetMuseFile()const
		{
			return m_museFile;
		}
	};
}

#endif

