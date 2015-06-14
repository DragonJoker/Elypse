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
#include "TrollEditorPch.h"

#include <wx/msgdlg.h>

#include <STLMacros.h>

#include "Project/Scene.h"

#include "Project/Project.h"
#include "Project/3D/Entity.h"
#include "Project/3D/Mesh.h"
#include "Project/3D/SceneNode.h"
#include "Project/3D/Light.h"
#include "Project/3D/Camera.h"
#include "Project/2D/Overlay.h"
#include "Project/Media/Sound.h"
#include "Project/Media/SoundObject.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"
#include "Project/Temporal/AnimationGroup.h"
#include "Project/Temporal/Sequence.h"

#include "GUI/FilesTree.h"

using namespace General::Utils;

BEGIN_TROLL_PROJECT_NAMESPACE
{
	namespace
	{
		bool AddFileToArray( Scene * p_scene, FileType p_type, wxString p_pathName, wxTreeItemId p_folder, wxTreeItemId p_item, bool p_saved, bool p_showError, FileArray & p_array )
		{
			bool l_return = true;
			auto && l_it = p_array.begin();

			while ( l_it != p_array.end() && l_return )
			{
				if ( p_pathName == ( *l_it )->FileName )
				{
					if ( p_showError )
					{
						wxMessageBox( _( "Can't add this file, it is already present in this Scene" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, NULL );
					}

					l_return = false;
				}

				++l_it;
			}

			if ( l_return )
			{
				File * l_file = new File;
				l_file->m_scene = p_scene;
				l_file->Filetype = p_type;
				int l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
				l_file->FileName = p_pathName.substr( l_index + 1 );
				std::cout << "Scene::AddFile - " << p_scene->GetName() << " - " << l_file->FileName << "\n";
				l_file->FolderId = p_folder;
				l_file->ItemId = p_item;
				l_file->Saved = p_saved;
				l_file->Open = false;

				if ( p_type != dataFolder && p_type != dataFile )
				{
					l_file->CfgFile = std::make_unique< ConfigFile >( make_string( p_pathName ), p_scene->GetMuseFile() );
				}

				p_array.push_back( l_file );
			}

			return l_return;
		}

		template< typename T, typename Function >
		T * GetObjectByName( SceneFileStrMap const & p_map, Function p_function )
		{
			T * l_result = NULL;

			auto && l_it = std::find_if( p_map.begin(), p_map.end(), [&l_result, &p_function]( std::pair< wxString, SceneFile * > const & p_file )
			{
				return ( l_result = p_function( p_file.second ) ) != NULL;
			} );

			return l_result;
		}

		void RemoveElement( wxString const & p_pathName, FileArray & p_array )
		{
			auto && l_it = std::find_if( p_array.begin(), p_array.end(), [&p_pathName]( File * p_file )
			{
				return p_pathName == p_file->FileName;
			} );

			if ( l_it != p_array.end() )
			{
				delete( * l_it );
				p_array.erase( l_it );
			}
		}
	
		template< typename Element, typename Function >
		Element * AddElement( wxString const & p_name, std::map< wxString, Element * > & p_map, Function p_function )
		{
			Element * l_return = NULL;
			auto && l_it = p_map.find( p_name );

			if ( l_it != p_map.end() )
			{
				l_return = l_it->second;
			}
			else
			{
				l_return = p_function();
				p_map.insert( std::make_pair( p_name, l_return ) );
			}

			return l_return;
		}

		template< typename ArrayType, typename Function >
		void SaveFiles( ArrayType const & p_array, wxString const & p_type, wxTextOutputStream * p_stream, Function p_function )
		{
			if ( p_array.size() > 0 )
			{
				p_stream->WriteString( wxT( "\t\t" ) + p_type + wxT( "\n\t\t{\n" ) );

				for ( auto && l_it : p_array )
				{
					LogDebug( wxT( "Scene::Save - " ) + p_function( l_it ) );
					p_stream->WriteString( wxT( "\t\t\t" ) + p_function( l_it ) + wxT( "\n" ) );
				}

				p_stream->WriteString( wxT( "\t\t}\n" ) );
			}
		}

		void AddFileToTree( FileType p_type, wxTreeItemId p_id, GUI::TETreeItemType p_treeType, Scene * p_scene, wxString const & p_strFile, GUI::FilesTree * p_tree, FileArray & p_array )
		{
			File * l_file = new File( p_scene, p_type, p_strFile, p_scene->GetMuseFile(), p_id );
			std::clog << "AddFileToTree - " << l_file->m_scene->GetName() << " - " << l_file->FileName << "\n";

			if ( p_tree )
			{
				l_file->ItemId = p_tree->AddFileToFolder( p_id, p_strFile, p_treeType, true );
			}

			p_array.push_back( l_file );
		}
	}

	Scene::Scene( ProjectSPtr p_project, const wxString & p_name, bool p_main )
		: m_minVerticCamAngle( 0 )
		, m_maxVerticCamAngle( 90 )
		, m_minHorizCamAngle( 0 )
		, m_maxHorizCamAngle( 0 )
		, m_minZoom( 0 )
		, m_maxZoom( 250 )
		, m_name( p_name )
		, m_currentSceneFile( NULL )
		, m_mainScene( p_main )
		, m_needed( false )
		, m_museFile( new TROLL_ELYPSE_NAMESPACE::TrollMuseFile( make_string( p_name ) ) )
	{
		m_museFile->SetCompletePath( make_string( p_project->GetPath() + p_name ) );
	}

	Scene::~Scene()
	{
		FlushFiles();
		FlushObjects();
	}

	void Scene::FlushFiles()
	{
		General::Utils::vector::deleteAll( m_sceneFiles );
		General::Utils::vector::deleteAll( m_loadScriptFiles );
		General::Utils::vector::deleteAll( m_unloadScriptFiles );
		General::Utils::vector::deleteAll( m_dataFiles );
		General::Utils::vector::deleteAll( m_museFiles );
		General::Utils::vector::deleteAll( m_dataFolders );
	}

	void Scene::FlushObjects()
	{
		m_currentSceneFile = NULL;
		General::Utils::map::deleteAll( m_sceneConfigFiles );
		General::Utils::map::deleteAll( m_meshes );
		delete m_museFile;
	}

	void Scene::SetCurrentSceneFile( const wxString & p_sceneFileName )
	{
		SceneFileStrMap::iterator l_it = m_sceneConfigFiles.find( p_sceneFileName );

		if ( l_it != m_sceneConfigFiles.end() )
		{
			LogMessage( wxT( "Scene::SetCurrentSceneFile - File already added" ) );
			m_currentSceneFile = l_it->second;
			return;
		}

		m_currentSceneFile = new SceneFile;
		m_currentSceneFile->m_fileName = p_sceneFileName;
		m_sceneConfigFiles.insert( std::make_pair( p_sceneFileName, m_currentSceneFile ) );
	}

	bool Scene::AddSceneFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError )
	{
		return AddFileToArray( this, sceneFile, p_pathName, p_folder, p_item, p_saved, p_showError, m_sceneFiles );
	}

	bool Scene::AddLoadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError )
	{
		return AddFileToArray( this, loadScriptFile, p_pathName, p_folder, p_item, p_saved, p_showError, m_loadScriptFiles );
	}

	bool Scene::AddUnloadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError )
	{
		return AddFileToArray( this, unloadScriptFile, p_pathName, p_folder, p_item, p_saved, p_showError, m_unloadScriptFiles );
	}

	bool Scene::AddDataFile( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError )
	{
		return AddFileToArray( this, dataFile, p_pathName, p_folder, p_item, p_saved, p_showError, m_dataFiles );
	}

	bool Scene::AddDataFolder( const wxString & p_pathName, const wxTreeItemId & p_folder, const wxTreeItemId & p_item, bool p_saved, bool p_showError )
	{
		return AddFileToArray( this, dataFolder, p_pathName, p_folder, p_item, p_saved, p_showError, m_dataFolders );
	}

	bool Scene::AddFile( const wxTreeItemId & p_folder, const wxTreeItemId & p_item, const wxString & p_pathName, bool p_saved, bool p_showError )
	{
		bool l_return = false;

		if ( m_sceneFileId == p_folder )
		{
			std::clog << "Scene::AddFile - Scene File"<< std::endl;
			l_return = AddSceneFile( p_pathName, p_folder, p_item, p_saved, p_showError );
		}
		else if ( m_loadScriptFileId == p_folder )
		{
			std::clog << "Scene::AddFile - Load Script File"<< std::endl;
			l_return = AddLoadScriptFile( p_pathName, p_folder, p_item, p_saved, p_showError );
		}
		else if ( m_unloadScriptFileId == p_folder )
		{
			std::clog << "Scene::AddFile - Unload Script File"<< std::endl;
			l_return = AddUnloadScriptFile( p_pathName, p_folder, p_item, p_saved, p_showError );
		}
		else if ( m_dataFileId == p_folder )
		{
			std::clog << "Scene::AddFile - Data File"<< std::endl;
			l_return = AddDataFile( p_pathName, p_folder, p_item, p_saved, p_showError );
		}
		else if ( m_dataFolderId == p_folder )
		{
			std::clog << "Scene::AddFile - Data Folder" << std::endl;
			l_return = AddDataFolder( p_pathName, p_folder, p_item, p_saved, p_showError );
		}

		return l_return;
	}

	Objects3D::TrollEntity * Scene::AddEntity( const wxString & p_name, const wxString & p_fileName )
	{
		Objects3D::TrollEntity * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_entities, [&p_name, &p_fileName]() { return new Objects3D::TrollEntity( p_name, p_fileName ); } );
		}

		return l_return;
	}

	Objects2D::TrollOverlay * Scene::AddOverlay( const wxString & p_name, const wxString & p_parentName, const wxString & p_fileName )
	{
		Objects2D::TrollOverlay * l_object = NULL;

		if ( m_currentSceneFile )
		{
			auto && l_it = m_currentSceneFile->m_allOverlays.find( p_name );

			if ( l_it != m_currentSceneFile->m_allOverlays.end() )
			{
				l_object = l_it->second;
			}
			else
			{
				Objects2D::TrollOverlay * l_parent = NULL;

				if ( !p_parentName.empty() )
				{
					auto && l_it2 = m_currentSceneFile->m_allOverlays.find( p_parentName );

					if ( l_it2 != m_currentSceneFile->m_allOverlays.end() )
					{
						l_parent = l_it2->second;
					}
				}

				l_object = new Objects2D::TrollOverlay( p_name, l_parent, p_fileName );

				if ( !l_parent )
				{
					m_currentSceneFile->m_overlayGroups.insert( std::make_pair( p_name, l_object ) );
				}

				m_currentSceneFile->m_allOverlays.insert( std::make_pair( p_name, l_object ) );
			}
		}

		return l_object;
	}

	bool Scene::AddOverlay( Objects2D::TrollOverlay * p_overlay )
	{
		bool l_return = false;

		if ( m_currentSceneFile )
		{
			wxString l_name = p_overlay->GetObjectName();

			if ( m_currentSceneFile->m_allOverlays.find( l_name ) == m_currentSceneFile->m_allOverlays.end() )
			{
				Objects2D::TrollOverlay * l_parent = p_overlay->GetParent();

				if ( !l_parent )
				{
					m_currentSceneFile->m_overlayGroups.insert( std::make_pair( l_name, p_overlay ) );
				}

				m_currentSceneFile->m_allOverlays.insert( std::make_pair( l_name, p_overlay ) );
				l_return = true;
			}
		}

		return l_return;
	}

	Objects3D::TrollSceneNode * Scene::AddSceneNode( const wxString & p_name, const wxString & p_fileName )
	{
		Objects3D::TrollSceneNode * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_allSceneNodes, [&p_name, &p_fileName]() { return new Objects3D::TrollSceneNode( p_name, p_fileName ); } );
		}

		return l_return;
	}

	void Scene::AttachNode( Objects3D::TrollSceneNode * p_node, Objects3D::TrollSceneNode * p_parent )
	{
		if ( p_parent != NULL )
		{
			p_node->AttachTo( p_parent );
		}
		else
		{
			m_currentSceneFile->m_sceneNodes.insert( std::make_pair( p_node->GetObjectName(), p_node ) );
		}
	}

	Objects3D::TrollLight * Scene::AddLight( const wxString & p_name, const wxString & p_fileName )
	{
		Objects3D::TrollLight * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_lights, [&p_name, &p_fileName]() { return new Objects3D::TrollLight( p_name, p_fileName ); } );
		}

		return l_return;
	}

	Objects3D::TrollCamera * Scene::AddCamera( const wxString & p_name, const wxString & p_fileName )
	{
		Objects3D::TrollCamera * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_cameras, [&p_name, &p_fileName]() { return new Objects3D::TrollCamera( p_name, p_fileName ); } );
		}

		return l_return;
	}

	Objects3D::TrollMesh * Scene::AddMesh( const wxString & p_name )
	{
		Objects3D::TrollMesh * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_meshes, [&p_name]() { return new Objects3D::TrollMesh( p_name ); } );
		}

		return l_return;
	}

	Temporal::TrollAnimationGroup * Scene::AddAnimationGroup( const wxString & p_name, const wxString & p_fileName )
	{
		Temporal::TrollAnimationGroup * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_animationGroups, [&p_name, &p_fileName]() { return new Temporal::TrollAnimationGroup( p_name, p_fileName ); } );
		}

		return l_return;
	}

	Media::TrollSound * Scene::AddSound( const wxString & p_name, const wxString & p_fileName )
	{
		TROLL_PROJECT_MEDIA_NAMESPACE::TrollSound * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_sounds, [&p_name, &p_fileName]() { return new Media::TrollSound( p_name, p_fileName ); } );
		}

		return l_return;
	}

	Media::TrollVideo * Scene::AddVideo( const wxString & p_name, const wxString & p_fileName )
	{
		Media::TrollVideo * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_name, m_currentSceneFile->m_videos, [&p_name, &p_fileName]() { return new Media::TrollVideo( p_name, p_fileName ); } );
		}

		return l_return;
	}

	Temporal::TrollSequence * Scene::AddSequence( Temporal::TrollSequence * p_sequence, const wxString & p_fileName )
	{
		Temporal::TrollSequence * l_return = NULL;

		if ( m_currentSceneFile )
		{
			l_return = AddElement( p_sequence->GetObjectName(), m_currentSceneFile->m_sequences, [&p_sequence]() { return p_sequence; } );
		}

		return l_return;
	}

	void Scene::RemoveSceneFile( const wxString & p_pathName )
	{
		RemoveElement( p_pathName, m_sceneFiles );
	}

	void Scene::RemoveLoadScriptFile( const wxString & p_pathName )
	{
		RemoveElement( p_pathName, m_loadScriptFiles );
	}

	void Scene::RemoveUnloadScriptFile( const wxString & p_pathName )
	{
		RemoveElement( p_pathName, m_unloadScriptFiles );
	}

	void Scene::RemoveDataFile( const wxString & p_pathName )
	{
		RemoveElement( p_pathName, m_dataFiles );
	}

	void Scene::RemoveDataFolder( const wxString & p_pathName )
	{
		RemoveElement( p_pathName, m_dataFolders );
	}

	void Scene::RemoveFile( const wxTreeItemId & p_item )
	{
		File * l_file = GetFile( p_item );
		std::cout << m_sceneFileId << " & " << l_file->FolderId << std::endl;

		if ( m_sceneFileId == l_file->FolderId )
		{
			LogDebug( wxT( "Scene::RemoveFile - Scene File" ) );
			RemoveSceneFile( l_file->FileName );
		}
		else if ( m_loadScriptFileId == l_file->FolderId )
		{
			LogDebug( wxT( "Scene::RemoveFile - Load Script File" ) );
			RemoveLoadScriptFile( l_file->FileName );
		}
		else if ( m_unloadScriptFileId == l_file->FolderId )
		{
			LogDebug( wxT( "Scene::RemoveFile - Unload Script File" ) );
			RemoveUnloadScriptFile( l_file->FileName );
		}
		else if ( m_dataFileId == l_file->FolderId )
		{
			LogDebug( wxT( "Scene::RemoveFile - Data File" ) );
			RemoveDataFile( l_file->FileName );
		}
		else if ( m_dataFolderId == l_file->FolderId )
		{
			LogDebug( wxT( "Scene::RemoveFile - Data Folder" ) );
			RemoveDataFolder( l_file->FileName );
		}
	}

	bool Scene::HasItem( const wxTreeItemId & p_item )
	{
		auto && l_pred = [&p_item]( File * p_file )
		{
			return p_file->ItemId == p_item;
		};

		return m_sceneId == p_item 
			|| m_sceneFileId == p_item
			|| m_loadScriptFileId == p_item
			|| m_unloadScriptFileId == p_item
			|| m_dataFolderId == p_item
			|| m_dataFileId == p_item
			|| std::find_if( m_sceneFiles.begin(), m_sceneFiles.end(), l_pred ) != m_sceneFiles.end()
			|| std::find_if( m_loadScriptFiles.begin(), m_loadScriptFiles.end(), l_pred ) != m_loadScriptFiles.end()
			|| std::find_if( m_unloadScriptFiles.begin(), m_unloadScriptFiles.end(), l_pred ) != m_unloadScriptFiles.end()
			|| std::find_if( m_dataFiles.begin(), m_dataFiles.end(), l_pred ) != m_dataFiles.end()
			|| std::find_if( m_dataFolders.begin(), m_dataFolders.end(), l_pred ) != m_dataFolders.end();
	}

	void Scene::Save( wxTextOutputStream * p_stream )
	{
		if ( p_stream )
		{
			auto && l_fileNamer = []( File * p_file )
			{
				return p_file->FileName;
			};

			auto && l_depNamer = []( wxString const & p_name )
			{
				return p_name;
			};
			
			p_stream->WriteString( wxT( "\tscene " ) + m_name + wxT( "\n\t{\n" ) );
			SaveFiles( m_sceneFiles, wxT( "scene_files" ), p_stream, l_fileNamer );
			SaveFiles( m_loadScriptFiles, wxT( "load_script_files" ), p_stream, l_fileNamer );
			SaveFiles( m_unloadScriptFiles, wxT( "unload_script_files" ), p_stream, l_fileNamer );
			SaveFiles( m_dataFiles, wxT( "data_files_3d" ), p_stream, l_fileNamer );
			SaveFiles( m_dataFiles, wxT( "data_files_other" ), p_stream, l_fileNamer );
			SaveFiles( m_neededScenes, wxT( "dependencies" ), p_stream, l_depNamer );
			p_stream->WriteString( wxT( "\t}\n" ) );
		}
	}

	void Scene::AddToTree( GUI::FilesTree * p_tree )
	{
		wxTreeItemId l_id;

		try
		{
			if ( p_tree )
			{
				l_id = p_tree->AddSceneToProject( this );
			}

			m_sceneId = l_id;

			if ( p_tree )
			{
				l_id = p_tree->AddFolderToScene( m_name, SCENE_FILES );
				m_sceneFileId = l_id;
				m_loadScriptFileId = p_tree->AddFolderToScene( m_name, LOAD_SCRIPT_FILES );
				m_unloadScriptFileId = p_tree->AddFolderToScene( m_name, UNLOAD_SCRIPT_FILES );
				m_dataFileId = p_tree->AddFolderToScene( m_name, GRAPHIC_DATA_FOLDERS );
				m_dataFolderId = p_tree->AddFolderToScene( m_name, OTHER_DATA_FOLDERS );
			}
		}
		catch ( bool )
		{
		}
	}

	void Scene::AddSceneFile( wxString const & p_strFile, GUI::FilesTree * p_tree )
	{
		AddFileToTree( sceneFile, m_sceneFileId, GUI::TITSceneFile, this, p_strFile, p_tree, m_sceneFiles );
	}

	void Scene::AddLoadScriptFile( wxString const & p_strFile, GUI::FilesTree * p_tree )
	{
		AddFileToTree( loadScriptFile, m_loadScriptFileId, GUI::TITLoadScriptFile, this, p_strFile, p_tree, m_loadScriptFiles );
	}

	void Scene::AddUnloadScriptFile( wxString const & p_strFile, GUI::FilesTree * p_tree )
	{
		AddFileToTree( unloadScriptFile, m_unloadScriptFileId, GUI::TITUnloadScriptFile, this, p_strFile, p_tree, m_unloadScriptFiles );
	}

	void Scene::Add3DDataFile( wxString const & p_strFile, GUI::FilesTree * p_tree )
	{
		AddFileToTree( dataFile, m_dataFileId, GUI::TITDataFile, this, p_strFile, p_tree, m_dataFiles );
	}

	void Scene::AddOtherDataFile( wxString const & p_strFile, GUI::FilesTree * p_tree )
	{
		AddFileToTree( dataFolder, m_dataFolderId, GUI::TITDataFolder, this, p_strFile, p_tree, m_dataFolders );
	}

	void Scene::AddDependency( wxString const & p_strName )
	{
		m_neededScenes.push_back( p_strName );
	}

	wxString Scene::Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, GUI::FilesTree * p_tree )
	{
		AddToTree( p_tree );
		wxString l_line;

		if ( p_stream && p_input && p_input->Eof() )
		{
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return wxEmptyString;
			}

			if ( l_line == wxT( "SceneFiles" ) )
			{
				l_line = p_stream->ReadLine();

				while ( l_line != wxT( "LoadScriptFiles" ) && l_line != wxT( "UnloadScriptFiles" ) && l_line != wxT( "DataFiles" )
						&& l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" )
						&& !l_line.empty() && !p_input->Eof() )
				{
					AddSceneFile( l_line, p_tree );
					l_line = p_stream->ReadLine();
				}

				if ( l_line.empty() || p_input->Eof() )
				{
					return wxEmptyString;
				}
			}

			if ( l_line == wxT( "LoadScriptFiles" ) )
			{
				l_line = p_stream->ReadLine();

				while ( l_line != wxT( "UnloadScriptFiles" ) && l_line != wxT( "DataFiles" )
						&& l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" )
						&& !l_line.empty() && !p_input->Eof() )
				{
					AddLoadScriptFile( l_line, p_tree );
					l_line = p_stream->ReadLine();
				}

				if ( l_line.empty() || p_input->Eof() )
				{
					return wxEmptyString;
				}
			}

			if ( l_line == wxT( "UnloadScriptFiles" ) )
			{
				l_line = p_stream->ReadLine();

				while ( l_line != wxT( "DataFiles" ) && l_line != wxT( "Scene" )
						&& l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" )
						&& !l_line.empty() && !p_input->Eof() )
				{
					AddUnloadScriptFile( l_line, p_tree );
					l_line = p_stream->ReadLine();
				}

				if ( l_line.empty() || p_input->Eof() )
				{
					return wxEmptyString;
				}
			}

			if ( l_line == wxT( "DataFiles" ) )
			{
				l_line = p_stream->ReadLine();

				while ( l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" )
					&& !l_line.empty() && !p_input->Eof() )
				{
					Add3DDataFile( l_line, p_tree );
					l_line = p_stream->ReadLine();
				}
			}

			if ( l_line == wxT( "DataFolders" ) )
			{
				l_line = p_stream->ReadLine();

				while ( l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" )
					&& !l_line.empty() && !p_input->Eof() )
				{
					AddOtherDataFile( l_line, p_tree );
					l_line = p_stream->ReadLine();
				}
			}

			if ( l_line == wxT( "Dependencies" ) )
			{
				l_line = p_stream->ReadLine();

				while ( l_line != wxT( "Scene" )
					&& !l_line.empty() && !p_input->Eof() )
				{
					AddDependency( l_line );
					l_line = p_stream->ReadLine();
				}
			}
		}

		return l_line;
	}

	void Scene::Write( const wxString & p_path )
	{
		SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();

		while ( l_it != m_sceneConfigFiles.end() )
		{
			l_it->second->Write( p_path );
			++l_it;
		}
	}

	void Scene::UpdateFileName( const wxString & p_ancient, const wxString & p_new )
	{
		int l_index = min( p_new.find_last_of( wxT( "\\" ) ), p_new.find_last_of( wxT( "/" ) ) );

		auto && l_func = [&p_ancient]( File * p_file )
		{
			return p_file->FileName == p_ancient;
		};

		File * l_return = General::Utils::vector::findOrNull( m_sceneFiles, l_func );

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_loadScriptFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_unloadScriptFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_dataFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_dataFolders, l_func );
		}

		if ( l_return )
		{
			l_return->FileName = p_new.substr( l_index + 1 );
			std::clog << "Scene::UpdateFileName - " << GetName() << " - " << l_return->FileName << "\n";
			l_return->Saved = true;
		}
	}

	FileType Scene::GetType( const wxTreeItemId & p_folderId )
	{
		FileType l_return = dataFile;

		if ( m_sceneFileId == p_folderId )
		{
			l_return = sceneFile;
		}
		else if ( m_loadScriptFileId == p_folderId )
		{
			l_return = loadScriptFile;
		}
		else if ( m_unloadScriptFileId == p_folderId )
		{
			l_return = unloadScriptFile;
		}
		else if ( m_dataFolderId == p_folderId )
		{
			l_return = dataFolder;
		}

		return l_return;
	}

	File * Scene::GetFile( const wxTreeItemId & p_itemId )
	{
		auto && l_func = [&p_itemId]( File * p_file )
		{
			return p_file->ItemId == p_itemId;
		};

		File * l_return = General::Utils::vector::findOrNull( m_sceneFiles, l_func );

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_loadScriptFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_unloadScriptFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_dataFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_dataFolders, l_func );
		}

		return l_return;
	}

	File * Scene::GetFileByName( const wxString & p_itemName )
	{
		auto && l_func = [&p_itemName]( File * p_file )
		{
			return p_file->FileName == p_itemName;
		};

		File * l_return = General::Utils::vector::findOrNull( m_sceneFiles, l_func );

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_loadScriptFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_unloadScriptFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_dataFiles, l_func );
		}

		if ( !l_return )
		{
			l_return = General::Utils::vector::findOrNull( m_dataFolders, l_func );
		}

		return l_return;
	}

	File * Scene::GetSceneFile( unsigned int p_index )
	{
		return General::Utils::vector::getOrNull( m_sceneFiles, p_index );
	}

	File * Scene::GetLoadScriptFile( unsigned int p_index )
	{
		return General::Utils::vector::getOrNull( m_loadScriptFiles, p_index );
	}

	File * Scene::GetUnloadScriptFile( unsigned int p_index )
	{
		return General::Utils::vector::getOrNull( m_unloadScriptFiles, p_index );
	}

	File * Scene::GetDataFile( unsigned int p_index )
	{
		return General::Utils::vector::getOrNull( m_dataFiles, p_index );
	}

	File * Scene::GetDataFolder( unsigned int p_index )
	{
		return General::Utils::vector::getOrNull( m_dataFolders, p_index );
	}

	Objects3D::TrollMesh * Scene::GetMesh( const wxString & p_name )
	{
		return General::Utils::map::findOrNull( m_meshes, p_name );
	}

	Objects3D::TrollEntity * Scene::GetEntity( const wxString & p_name )
	{
		return GetObjectByName< Objects3D::TrollEntity >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetEntity( p_name ); } );
	}

	Objects2D::TrollOverlay * Scene::GetOverlay( const wxString & p_name )
	{
		return GetObjectByName< Objects2D::TrollOverlay >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetOverlay( p_name ); } );
	}

	Objects3D::TrollSceneNode * Scene::GetSceneNode( const wxString & p_name )
	{
		return GetObjectByName< Objects3D::TrollSceneNode >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetSceneNode( p_name ); } );
	}

	Objects3D::TrollLight * Scene::GetLight( const wxString & p_name )
	{
		return GetObjectByName< Objects3D::TrollLight >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetLight( p_name ); } );
	}

	Objects3D::TrollCamera * Scene::GetCamera( const wxString & p_name )
	{
		return GetObjectByName< Objects3D::TrollCamera >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetCamera( p_name ); } );
	}

	Temporal::TrollAnimationGroup * Scene::GetAnimationGroup( const wxString & p_name )
	{
		return GetObjectByName< Temporal::TrollAnimationGroup >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetAnimationGroup( p_name ); } );
	}

	Media::TrollSound * Scene::GetSound( const wxString & p_name )
	{
		return GetObjectByName< Media::TrollSound >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetSound( p_name ); } );
	}

	Media::TrollVideo * Scene::GetVideo( const wxString & p_name )
	{
		return GetObjectByName< Media::TrollVideo >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetVideo( p_name ); } );
	}

	Temporal::TrollSequence * Scene::GetSequence( const wxString & p_name )
	{
		return GetObjectByName< Temporal::TrollSequence >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file ) { return p_file->GetSequence( p_name ); } );
	}

	void Scene::GetNodesNames( wxArrayString & p_list, const wxString & p_name )
	{
		for ( auto && l_it : m_sceneConfigFiles )
		{
			for ( auto l_nodesIt : l_it.second->m_allSceneNodes )
			{
				if ( l_nodesIt.first != p_name )
				{
					p_list.Add( l_nodesIt.first );
				}
			}
		}
	}

	void Scene::GetOverlaysNames( wxArrayString & p_list )
	{
		for ( auto && l_it : m_sceneConfigFiles )
		{
			for ( auto l_overlaysIt : l_it.second->m_allOverlays )
			{
				//std::cout << "Scene::GetOverlaysNames - " << l_overlaysIt->first << "\n";
				if ( l_overlaysIt.first != wxT( "Core/DebugOverlay" ) && l_overlaysIt.first != wxT( "Core/LensFlare" ) && l_overlaysIt.first != wxT( "Core/LoadOverlay" ) )
				{
					p_list.Add( l_overlaysIt.first );
				}
			}
		}
	}

	void Scene::GetMeshesNames( wxArrayString & p_list )
	{
		for ( auto && l_it : m_meshes )
		{
			p_list.Add( l_it.first );
		}
	}

	void Scene::GetSequencesNames( wxArrayString & p_list )
	{
		for ( auto && l_it : m_sceneConfigFiles )
		{
			for ( auto l_sequencesIt : l_it.second->m_sequences )
			{
				p_list.Add( l_sequencesIt.first );
			}
		}
	}

	void Scene::AddDependency( Scene * p_scene )
	{
		auto && l_it = std::find_if( m_neededScenes.begin(), m_neededScenes.end(), [&p_scene]( wxString const & p_name )
		{
			return p_name == p_scene->GetName();
		} );

		if ( l_it == m_neededScenes.end() )
		{
			p_scene->SetNeeded( true );
			m_neededScenes.push_back( p_scene->GetName() );
			p_scene->_addSceneThatNeedMe( this );
		}
	}

	void Scene::RemoveDependency( Scene * p_scene )
	{
		auto && l_it = std::find_if( m_neededScenes.begin(), m_neededScenes.end(), [&p_scene]( wxString const & p_name )
		{
			return p_name == p_scene->GetName();
		} );

		if ( l_it != m_neededScenes.end() )
		{
			p_scene->SetNeeded( false );
			m_neededScenes.erase( l_it );
			p_scene->_removeSceneThatNeedMe( this );
		}
	}

	void Scene::_removeSceneThatNeedMe( Scene * p_scene )
	{
		auto && l_it = m_scenesThatNeedMe.find( p_scene->GetName() );

		if ( l_it != m_scenesThatNeedMe.end() )
		{
			m_scenesThatNeedMe.erase( l_it );
		}
	}
}
END_TROLL_PROJECT_NAMESPACE
