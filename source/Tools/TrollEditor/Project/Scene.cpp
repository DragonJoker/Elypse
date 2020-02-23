/*
See LICENSE file in root folder
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

namespace Troll
{
	namespace ProjectComponents
	{
		namespace
		{
			File & AddFileToArray( Scene * p_scene, FileType p_type, wxString p_pathName, bool p_saved, bool p_showError, FileArray & p_array )
			{
				auto l_it = std::find_if( p_array.begin(), p_array.end(), [&p_pathName]( File const & p_file )
				{
					return p_pathName == p_file.FileName;
				} );

				if ( l_it != p_array.end() )
				{
					if ( p_showError )
					{
						wxMessageBox( _( "Can't add this file, it is already present in this Scene" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, nullptr );
					}
				}
				else
				{
					auto const l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
					File l_file{ p_scene, p_type, p_pathName.substr( l_index + 1 ), p_scene->GetMuseFile() };
					std::cout << "Scene::AddFile - " << p_scene->GetName() << " - " << l_file.FileName << "\n";
					l_file.Saved = p_saved;
					l_file.Open = false;

					if ( p_type != dataFolder && p_type != dataFile )
					{
						l_file.CfgFile = std::make_unique< ConfigFile >( make_string( p_pathName ), p_scene->GetMuseFile() );
					}

					l_it = p_array.insert( p_array.end(), std::move( l_file ) );
				}

				return *l_it;
			}

			template< typename T, typename Function >
			T * GetObjectByName( SceneFileStrMap const & p_map, Function p_function )
			{
				T * l_result = nullptr;

				auto && l_it = std::find_if( p_map.begin(), p_map.end(), [&l_result, &p_function]( std::pair< wxString, SceneFile * > const & p_file )
				{
					return ( l_result = p_function( p_file.second ) ) != nullptr;
				} );

				return l_result;
			}

			void RemoveElement( wxString const & p_pathName, FileArray & p_array )
			{
				auto && l_it = std::find_if( p_array.begin(), p_array.end(), [&p_pathName]( File const & p_file )
				{
					return p_pathName == p_file.FileName;
				} );

				if ( l_it != p_array.end() )
				{
					p_array.erase( l_it );
				}
			}

			template< typename Element, typename Function >
			Element * AddElement( wxString const & p_name, std::map< wxString, Element * > & p_map, Function p_function )
			{
				Element * l_return = nullptr;
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

			void AddFileToTree( FileType p_type, wxTreeItemId p_id, GUI::TreeItemType p_treeType, Scene * p_scene, wxString const & p_strFile, GUI::FilesTree * p_tree, FileArray & p_array )
			{
				File l_file{ p_scene, p_type, p_strFile, p_scene->GetMuseFile(), p_id };
				std::clog << "AddFileToTree - " << l_file.m_scene->GetName() << " - " << l_file.FileName << "\n";

				if ( p_tree )
				{
					l_file.ItemId = p_tree->AddFileToFolder( p_id, p_strFile, p_treeType, true );
				}

				p_array.push_back( std::move( l_file ) );
			}
		}

		Scene::Scene( Project & p_project, wxString const & p_name, bool p_main )
			: m_minZoom( 0 )
			, m_maxZoom( 250 )
			, m_name( p_name )
			, m_currentSceneFile( nullptr )
			, m_mainScene( p_main )
			, m_needed( false )
			, m_museFile( std::make_shared< ElypseRW::TrollMuseFile >( make_string( p_name ) ) )
		{
			m_museFile->SetCompletePath( make_string( p_project.GetPath() + p_name ) );
		}

		Scene::~Scene()
		{
			FlushFiles();
			FlushObjects();
		}

		void Scene::FlushFiles()
		{
			m_files[sceneFile].clear();
			m_files[loadScriptFile].clear();
			m_files[unloadScriptFile].clear();
			m_files[dataFile].clear();
			m_files[dataFolder].clear();
			m_museFiles.clear();
		}

		void Scene::SetCurrentSceneFile( wxString const & p_sceneFileName )
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

		void Scene::UpdateFileName( wxString const & p_ancient, wxString const & p_new )
		{
			auto l_index = min( p_new.find_last_of( wxT( "\\" ) ), p_new.find_last_of( wxT( "/" ) ) );

			auto l_findFunc = [&p_ancient]( File const & p_file )
			{
				return p_file.FileName == p_ancient;
			};

			auto & l_file = FindFile( l_findFunc );

			l_file.FileName = p_new.substr( l_index + 1 );
			std::clog << "Scene::UpdateFileName - " << GetName() << " - " << l_file.FileName << "\n";
			l_file.Saved = true;
		}

		File & Scene::AddFile( FileType p_type, wxString const & p_pathName, bool p_saved, bool p_showError )
		{
			switch ( p_type )
			{
			case sceneFile:
				std::clog << "Scene::AddFile - Scene File" << std::endl;
				return AddFileToArray( this, sceneFile, p_pathName, p_saved, p_showError, m_files[sceneFile] );

			case loadScriptFile:
				std::clog << "Scene::AddFile - Load Script File" << std::endl;
				return AddFileToArray( this, loadScriptFile, p_pathName, p_saved, p_showError, m_files[loadScriptFile] );

			case unloadScriptFile:
				std::clog << "Scene::AddFile - Unload Script File" << std::endl;
				return AddFileToArray( this, unloadScriptFile, p_pathName, p_saved, p_showError, m_files[unloadScriptFile] );

			case dataFile:
				std::clog << "Scene::AddFile - Data File" << std::endl;
				return AddFileToArray( this, dataFile, p_pathName, p_saved, p_showError, m_files[dataFile] );

			case dataFolder:
				std::clog << "Scene::AddFile - Data Folder" << std::endl;
				return AddFileToArray( this, dataFolder, p_pathName, p_saved, p_showError, m_files[dataFolder] );

			default:
				genlib_assert( "Scene::AddFile - Unknown file type" );
				GENLIB_EXCEPTION( "Unknown file type" );
			}
		}

		void Scene::RemoveFile( FileType p_type, wxString const & p_pathName )
		{
			switch ( p_type )
			{
			case sceneFile:
				std::clog << "Scene::AddFile - Scene File" << std::endl;
				RemoveElement( p_pathName, m_files[sceneFile] );

			case loadScriptFile:
				std::clog << "Scene::AddFile - Load Script File" << std::endl;
				RemoveElement( p_pathName, m_files[loadScriptFile] );

			case unloadScriptFile:
				std::clog << "Scene::AddFile - Unload Script File" << std::endl;
				RemoveElement( p_pathName, m_files[unloadScriptFile] );

			case dataFile:
				std::clog << "Scene::AddFile - Data File" << std::endl;
				RemoveElement( p_pathName, m_files[dataFile] );

			case dataFolder:
				std::clog << "Scene::AddFile - Data Folder" << std::endl;
				RemoveElement( p_pathName, m_files[dataFolder] );

			default:
				genlib_assert( "Scene::AddFile - Unknown file type" );
				GENLIB_EXCEPTION( "Unknown file type" );
			}
		}

		File & Scene::GetFile( wxString const & p_itemName )
		{
			auto && l_findFunc = [&p_itemName]( File const & p_file )
			{
				return p_file.FileName == p_itemName;
			};

			return FindFile( l_findFunc );
		}

		File & Scene::GetSceneFile( uint32_t p_index )
		{
			return m_files[sceneFile][p_index];
		}

		File & Scene::GetLoadScriptFile( uint32_t p_index )
		{
			return m_files[loadScriptFile][p_index];
		}

		File & Scene::GetUnloadScriptFile( uint32_t p_index )
		{
			return m_files[unloadScriptFile][p_index];
		}

		File & Scene::GetDataFile( uint32_t p_index )
		{
			return m_files[dataFile][p_index];
		}

		File & Scene::GetDataFolder( uint32_t p_index )
		{
			return m_files[dataFolder][p_index];
		}

		void Scene::FlushObjects()
		{
			m_currentSceneFile = nullptr;
			General::Utils::map::deleteAll( m_sceneConfigFiles );
			General::Utils::map::deleteAll( m_meshes );
		}

		Objects3D::TrollEntity * Scene::AddEntity( wxString const & p_name, wxString const & p_fileName )
		{
			Objects3D::TrollEntity * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_entities, [&p_name, &p_fileName]()
				{
					return new Objects3D::TrollEntity( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		Objects2D::TrollOverlay * Scene::AddOverlay( wxString const & p_name, wxString const & p_parentName, wxString const & p_fileName )
		{
			Objects2D::TrollOverlay * l_object = nullptr;

			if ( m_currentSceneFile )
			{
				auto && l_it = m_currentSceneFile->m_allOverlays.find( p_name );

				if ( l_it != m_currentSceneFile->m_allOverlays.end() )
				{
					l_object = l_it->second;
				}
				else
				{
					Objects2D::TrollOverlay * l_parent = nullptr;

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

		Objects3D::TrollSceneNode * Scene::AddSceneNode( wxString const & p_name, wxString const & p_fileName )
		{
			Objects3D::TrollSceneNode * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_allSceneNodes, [&p_name, &p_fileName]()
				{
					return new Objects3D::TrollSceneNode( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		void Scene::AttachNode( Objects3D::TrollSceneNode * p_node, Objects3D::TrollSceneNode * p_parent )
		{
			if ( p_parent != nullptr )
			{
				p_node->AttachTo( p_parent );
			}
			else
			{
				m_currentSceneFile->m_sceneNodes.insert( std::make_pair( p_node->GetObjectName(), p_node ) );
			}
		}

		Objects3D::TrollLight * Scene::AddLight( wxString const & p_name, wxString const & p_fileName )
		{
			Objects3D::TrollLight * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_lights, [&p_name, &p_fileName]()
				{
					return new Objects3D::TrollLight( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		Objects3D::TrollCamera * Scene::AddCamera( wxString const & p_name, wxString const & p_fileName )
		{
			Objects3D::TrollCamera * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_cameras, [&p_name, &p_fileName]()
				{
					return new Objects3D::TrollCamera( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		Objects3D::TrollMesh * Scene::AddMesh( wxString const & p_name )
		{
			Objects3D::TrollMesh * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_meshes, [&p_name]()
				{
					return new Objects3D::TrollMesh( p_name );
				} );
			}

			return l_return;
		}

		Temporal::TrollAnimationGroup * Scene::AddAnimationGroup( wxString const & p_name, wxString const & p_fileName )
		{
			Temporal::TrollAnimationGroup * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_animationGroups, [&p_name, &p_fileName]()
				{
					return new Temporal::TrollAnimationGroup( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		Media::TrollSound * Scene::AddSound( wxString const & p_name, wxString const & p_fileName )
		{
			ProjectComponents::Media::TrollSound * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_sounds, [&p_name, &p_fileName]()
				{
					return new Media::TrollSound( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		Media::TrollVideo * Scene::AddVideo( wxString const & p_name, wxString const & p_fileName )
		{
			Media::TrollVideo * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_name, m_currentSceneFile->m_videos, [&p_name, &p_fileName]()
				{
					return new Media::TrollVideo( p_name, p_fileName );
				} );
			}

			return l_return;
		}

		Temporal::TrollSequence * Scene::AddSequence( Temporal::TrollSequence * p_sequence, wxString const & p_fileName )
		{
			Temporal::TrollSequence * l_return = nullptr;

			if ( m_currentSceneFile )
			{
				l_return = AddElement( p_sequence->GetObjectName(), m_currentSceneFile->m_sequences, [&p_sequence]()
				{
					return p_sequence;
				} );
			}

			return l_return;
		}

		void Scene::AddDependency( wxString const & p_strName )
		{
			m_neededScenes.push_back( p_strName );
		}

		void Scene::SaveScripts( wxString const & p_path )const
		{
			auto l_it = m_sceneConfigFiles.begin();

			while ( l_it != m_sceneConfigFiles.end() )
			{
				l_it->second->Write( p_path );
				++l_it;
			}
		}

		Objects3D::TrollMesh * Scene::GetMesh( wxString const & p_name )
		{
			return General::Utils::map::findOrNull( m_meshes, p_name );
		}

		Objects3D::TrollEntity * Scene::GetEntity( wxString const & p_name )
		{
			return GetObjectByName< Objects3D::TrollEntity >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetEntity( p_name );
			} );
		}

		Objects2D::TrollOverlay * Scene::GetOverlay( wxString const & p_name )
		{
			return GetObjectByName< Objects2D::TrollOverlay >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetOverlay( p_name );
			} );
		}

		Objects3D::TrollSceneNode * Scene::GetSceneNode( wxString const & p_name )
		{
			return GetObjectByName< Objects3D::TrollSceneNode >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetSceneNode( p_name );
			} );
		}

		Objects3D::TrollLight * Scene::GetLight( wxString const & p_name )
		{
			return GetObjectByName< Objects3D::TrollLight >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetLight( p_name );
			} );
		}

		Objects3D::TrollCamera * Scene::GetCamera( wxString const & p_name )
		{
			return GetObjectByName< Objects3D::TrollCamera >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetCamera( p_name );
			} );
		}

		Temporal::TrollAnimationGroup * Scene::GetAnimationGroup( wxString const & p_name )
		{
			return GetObjectByName< Temporal::TrollAnimationGroup >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetAnimationGroup( p_name );
			} );
		}

		Media::TrollSound * Scene::GetSound( wxString const & p_name )
		{
			return GetObjectByName< Media::TrollSound >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetSound( p_name );
			} );
		}

		Media::TrollVideo * Scene::GetVideo( wxString const & p_name )
		{
			return GetObjectByName< Media::TrollVideo >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetVideo( p_name );
			} );
		}

		Temporal::TrollSequence * Scene::GetSequence( wxString const & p_name )
		{
			return GetObjectByName< Temporal::TrollSequence >( m_sceneConfigFiles, [&p_name]( SceneFile const * p_file )
			{
				return p_file->GetSequence( p_name );
			} );
		}

		void Scene::GetNodesNames( wxArrayString & p_list, wxString const & p_name )
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

		void Scene::AddDependency( ScenePtr p_scene )
		{
			auto && l_it = std::find_if( m_neededScenes.begin(), m_neededScenes.end(), [&p_scene]( wxString const & p_name )
			{
				return p_name == p_scene->GetName();
			} );

			if ( l_it == m_neededScenes.end() )
			{
				p_scene->SetNeeded( true );
				m_neededScenes.push_back( p_scene->GetName() );
				p_scene->DoAddSceneThatNeedMe( *this );
			}
		}

		void Scene::RemoveDependency( ScenePtr p_scene )
		{
			auto && l_it = std::find_if( m_neededScenes.begin(), m_neededScenes.end(), [&p_scene]( wxString const & p_name )
			{
				return p_name == p_scene->GetName();
			} );

			if ( l_it != m_neededScenes.end() )
			{
				p_scene->SetNeeded( false );
				m_neededScenes.erase( l_it );
				p_scene->DoRemoveSceneThatNeedMe( this );
			}
		}

		void Scene::DoRemoveSceneThatNeedMe( Scene * p_scene )
		{
			auto && l_it = std::find( m_scenesThatNeedMe.begin(), m_scenesThatNeedMe.end(), p_scene->GetName() );

			if ( l_it != m_scenesThatNeedMe.end() )
			{
				m_scenesThatNeedMe.erase( l_it );
			}
		}
	}
}
