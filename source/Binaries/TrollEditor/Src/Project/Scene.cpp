
#include "PrecompiledHeader.h"

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
#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
/*
#include <Data/ConfigFile.h>
#include <General/STLMacros.h>
#include <General/File.h>
*/

using namespace Troll;
using namespace Troll::GUI;
using namespace Troll::Temporal;
using namespace Troll::Media;
using namespace Troll::Objects3D;
using namespace Troll::Objects2D;
using namespace General::Utils;


TrollSceneFile::TrollSceneFile()
	:	m_forceShadows( false ),
		m_shadowType( stNone ),
		m_ambientLight( ColourValue::White )
{
}



TrollSceneFile::~TrollSceneFile()
{
	General::Utils::map::deleteAll( m_entities );
	General::Utils::map::deleteAll( m_allOverlays );
	m_overlayGroups.clear();
	General::Utils::map::deleteAll( m_lights );
	General::Utils::map::deleteAll( m_cameras );
	General::Utils::map::deleteAll( m_allSceneNodes );
	m_sceneNodes.clear();
	General::Utils::map::deleteAll( m_animationGroups );
	General::Utils::map::deleteAll( m_sounds );
	General::Utils::map::deleteAll( m_videos );
	General::Utils::map::deleteAll( m_sequences );
}



TrollOverlay * TrollSceneFile::GetOverlay( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_allOverlays, p_name );
}



TrollSceneNode * TrollSceneFile::GetSceneNode( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_allSceneNodes, p_name );
}



TrollLight * TrollSceneFile::GetLight( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_lights, p_name );
}



TrollCamera * TrollSceneFile::GetCamera( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_cameras, p_name );
}



TrollAnimationGroup * TrollSceneFile::GetAnimationGroup( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_animationGroups, p_name );
}



TrollSound * TrollSceneFile::GetSound( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_sounds, p_name );
}



TrollVideo * TrollSceneFile::GetVideo( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_videos, p_name );
}



TrollSequence * TrollSceneFile::GetSequence( const wxString & p_name )
{
	return General::Utils::map::findOrNull( m_sequences, p_name );
}



void TrollSceneFile::_writeUnusedFilePieces( wxTextOutputStream * p_textStream )
{
	String l_strFilePath = ( g_mainFrame->GetAppDir() + m_fileName + wxT( "Unused.emscene" ) ).char_str().data();

	if ( General::Files::FileExists( l_strFilePath ) )
	{
		FILE * l_file = fopen( l_strFilePath.c_str(), "rb" );

		if ( l_file != NULL )
		{
			char l_char;

			while ( ! feof( l_file ) )
			{
				if ( fread( & l_char, sizeof( char ), 1, l_file ) == 1 )
				{
					if ( l_char == '\r' )
					{
						if ( fread( & l_char, sizeof( char ), 1, l_file ) < 1 )
						{
							l_char = 0;
						}
					}

					/*
										if (l_char != '{' && l_char != '}')
										{
											std::cout << "(" << (int)l_char << ")" << l_char;
										}
					*/
					char l_szTmp[2] = { l_char, 0 };
					p_textStream->WriteString( wxString( l_szTmp, wxMBConvLibc() ) );
				}
			}

			fclose( l_file );
		}
	}
}



void TrollSceneFile::Write( const wxString & p_path )
{
	g_mainFrame->LogDebugMessage( wxT( "Writing TrollSceneFile " ) + p_path + m_fileName );
	wxFileOutputStream l_output( p_path + m_fileName );
	wxTextOutputStream * l_textStream = new wxTextOutputStream( l_output );
	wxString l_tmp;

	if ( m_ambientLight != ColourValue::White )
	{
		l_tmp.Printf( wxT( "%.2f %.2f %.2f 0" ), m_ambientLight.r, m_ambientLight.g, m_ambientLight.b );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		l_textStream->WriteString( wxT( "ambient_light " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_shadowType != stNone )
	{
		l_tmp = ( m_shadowType == stStencilAdditive ? wxT( "stencil" ) : wxT( "texture" ) );
		l_textStream->WriteString( wxT( "shadows " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( m_forceShadows )
	{
		l_textStream->WriteString( wxT( "force_shadows true\n" ) );
	}

	l_textStream->WriteString( wxT( "\n" ) );

	if ( m_sounds.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// SOUNDS\n\n" ) );
		General::Utils::map::cycle( m_sounds, & TrollSound::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_videos.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// VIDEOS\n\n" ) );
		General::Utils::map::cycle( m_videos, & TrollVideo::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_overlayGroups.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// OVERLAYS\n\n" ) );
		General::Utils::map::cycle( m_overlayGroups, & TrollOverlay::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_sceneNodes.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// SCENE NODES\n\n" ) );
		General::Utils::map::cycle( m_sceneNodes, & TrollSceneNode::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_entities.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// OBJECTS\n\n" ) );
		General::Utils::map::cycle( m_entities, & TrollEntity::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_lights.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// LIGHTS\n\n" ) );
		General::Utils::map::cycle( m_lights, & TrollLight::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_cameras.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// CAMERAS\n\n" ) );
		General::Utils::map::cycle( m_cameras, & TrollCamera::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_animationGroups.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// ANIMATION GROUPS\n\n" ) );
		General::Utils::map::cycle( m_animationGroups, & TrollAnimationGroup::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	if ( m_sequences.size() > 0 )
	{
		l_textStream->WriteString( wxT( "// SEQUENCES\n\n" ) );
		General::Utils::map::cycle( m_sequences, & TrollSequence::Write, l_textStream );
		l_textStream->WriteString( wxT( "\n" ) );
	}

	l_output.Close();
}



TrollScene::TrollScene( Project * p_project, const wxString & p_name, bool p_main )
	:	m_minVerticCamAngle( 0 ),
		m_maxVerticCamAngle( 90 ),
		m_minHorizCamAngle( 0 ),
		m_maxHorizCamAngle( 0 ),
		m_minZoom( 0 ),
		m_maxZoom( 250 ),
		m_name( p_name ),
		m_currentSceneFile( NULL ),
		m_project( p_project ),
		m_mainScene( p_main ),
		m_needed( false ),
		m_museFile( new TrollMuseFile( p_name.char_str().data() ) )
{
	if ( m_mainScene )
	{
		m_museFile->SetCompletePath( ( p_project->GetPath() ).char_str().data() );
	}
	else
	{
		m_museFile->SetCompletePath( ( p_project->GetPath() + p_name ).char_str().data() );
	}
}



TrollScene::~TrollScene()
{
	FlushFiles();
	FlushObjects();
}



void TrollScene::FlushFiles()
{
	General::Utils::vector::deleteAll( m_sceneFiles );
	General::Utils::vector::deleteAll( m_loadScriptFiles );
	General::Utils::vector::deleteAll( m_unloadScriptFiles );
	General::Utils::vector::deleteAll( m_dataFiles );
	General::Utils::vector::deleteAll( m_museFiles );
	General::Utils::vector::deleteAll( m_dataFolders );
}



void TrollScene::FlushObjects()
{
	m_currentSceneFile = NULL;
	General::Utils::map::deleteAll( m_sceneConfigFiles );
	General::Utils::map::deleteAll( m_meshes );
	delete m_museFile;
}



void TrollScene::SetCurrentSceneFile( const wxString & p_sceneFileName )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.find( p_sceneFileName );

	if ( l_it != m_sceneConfigFiles.end() )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::SetCurrentSceneFile - File already added" ) );
		m_currentSceneFile = l_it->second;
		return;
	}

	m_currentSceneFile = new TrollSceneFile;
	m_currentSceneFile->m_fileName = p_sceneFileName;
	m_sceneConfigFiles.insert( SceneFileStrMap::value_type( p_sceneFileName, m_currentSceneFile ) );
}



bool TrollScene::AddSceneFile( TrollFile * p_file )
{
	for ( TrollFileArray::iterator l_it = m_sceneFiles.begin() ; l_it != m_sceneFiles.end() ; )
	{
		if ( p_file->FileName == ( * l_it )->FileName )
		{
			wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
						  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile( * p_file );
	l_file->m_scene = this;
	m_sceneFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddSceneFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
							   const wxTreeItemId & p_item, bool p_saved, bool p_showError )
{
	for ( TrollFileArray::iterator l_it = m_sceneFiles.begin() ; l_it != m_sceneFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			if ( p_showError )
			{
				wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
							  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			}

			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile;
	l_file->m_scene = this;
	//TODOMIAOU ?
	l_file->CfgFile = new ConfigFile( ( char * )( p_pathName.char_str().data() ), m_museFile );
	l_file->CfgFile->DownloadFinished();
	l_file->Filetype = sceneFile;
	int l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
	l_file->FileName = p_pathName.substr( l_index + 1 );
	l_file->Folder = p_pathName.substr( 0, l_index );
	std::cout << "TrollScene::AddSceneFile - " << l_file->Folder << " - " << l_file->FileName << "\n";
	l_file->FolderId = p_folder;
	l_file->ItemId = p_item;
	l_file->Saved = p_saved;
	l_file->Open = false;
	m_sceneFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddLoadScriptFile( TrollFile * p_file )
{
	for ( TrollFileArray::iterator l_it = m_loadScriptFiles.begin() ; l_it != m_loadScriptFiles.end() ; )
	{
		if ( p_file->FileName == ( * l_it )->FileName )
		{
			wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
						  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile( * p_file );
	l_file->m_scene = this;
	m_loadScriptFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddLoadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
									const wxTreeItemId & p_item, bool p_saved, bool p_showError )
{
	for ( TrollFileArray::iterator l_it = m_loadScriptFiles.begin() ; l_it != m_loadScriptFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			if ( p_showError )
			{
				wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
							  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			}

			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile;
	l_file->m_scene = this;
	l_file->CfgFile = new ConfigFile( p_pathName.char_str().data(), m_museFile );
	//TODOMIAOU ?
	l_file->Filetype = loadScriptFile;
	int l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
	l_file->FileName = p_pathName.substr( l_index + 1 );
	l_file->Folder = p_pathName.substr( 0, l_index );
	std::cout << "TrollScene::AddSceneFile - " << l_file->Folder << " - " << l_file->FileName << "\n";
	l_file->FolderId = p_folder;
	l_file->ItemId = p_item;
	l_file->Saved = p_saved;
	l_file->Open = false;
	m_loadScriptFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddUnloadScriptFile( TrollFile * p_file )
{
	for ( TrollFileArray::iterator l_it = m_unloadScriptFiles.begin() ; l_it != m_unloadScriptFiles.end() ; )
	{
		if ( p_file->FileName == ( * l_it )->FileName )
		{
			wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
						  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile( * p_file );
	l_file->m_scene = this;
	m_unloadScriptFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddUnloadScriptFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
									  const wxTreeItemId & p_item, bool p_saved, bool p_showError )
{
	for ( TrollFileArray::iterator l_it = m_unloadScriptFiles.begin() ; l_it != m_unloadScriptFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			if ( p_showError )
			{
				wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
							  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			}

			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile;
	l_file->m_scene = this;
	l_file->CfgFile = new ConfigFile( p_pathName.char_str().data(), m_museFile );
	l_file->Filetype = unloadScriptFile;
	int l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
	l_file->FileName = p_pathName.substr( l_index + 1 );
	l_file->Folder = p_pathName.substr( 0, l_index );
	std::cout << "TrollScene::AddSceneFile - " << l_file->Folder << " - " << l_file->FileName << "\n";
	l_file->FolderId = p_folder;
	l_file->ItemId = p_item;
	l_file->Saved = p_saved;
	l_file->Open = false;
	m_unloadScriptFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddDataFile( TrollFile * p_file )
{
	for ( TrollFileArray::iterator l_it = m_dataFiles.begin() ; l_it != m_dataFiles.end() ; )
	{
		if ( p_file->FileName == ( * l_it )->FileName )
		{
			wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
						  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile( * p_file );
	l_file->m_scene = this;
	m_dataFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddDataFile( const wxString & p_pathName, const wxTreeItemId & p_folder,
							  const wxTreeItemId & p_item, bool p_saved, bool p_showError )
{
	for ( TrollFileArray::iterator l_it = m_dataFiles.begin() ; l_it != m_dataFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			if ( p_showError )
			{
				wxMessageBox( _T( "Impossible d'ajouter ce dossier, il existe déjà dans cette scene" ),
							  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			}

			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile;
	l_file->m_scene = this;
	l_file->CfgFile = NULL;
	l_file->Filetype = dataFile;
	int l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
	l_file->FileName = p_pathName.substr( l_index + 1 );
	l_file->Folder = p_pathName;
	std::cout << "TrollScene::AddSceneFile - " << l_file->Folder << " - " << l_file->FileName << "\n";
	l_file->FolderId = p_folder;
	l_file->ItemId = p_item;
	l_file->Saved = p_saved;
	l_file->Open = false;
	m_dataFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddDataFolder( TrollFile * p_file )
{
	for ( TrollFileArray::iterator l_it = m_dataFolders.begin() ; l_it != m_dataFolders.end() ; )
	{
		if ( p_file->FileName == ( * l_it )->FileName )
		{
			wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
						  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile( * p_file );
	l_file->m_scene = this;
	m_dataFolders.push_back( l_file );
	return true;
}



bool TrollScene::AddDataFolder( const wxString & p_pathName, const wxTreeItemId & p_folder,
								const wxTreeItemId & p_item, bool p_saved, bool p_showError )
{
	for ( TrollFileArray::iterator l_it = m_dataFolders.begin() ; l_it != m_dataFolders.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			if ( p_showError )
			{
				wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
							  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			}

			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile;
	l_file->m_scene = this;
	l_file->CfgFile = NULL;
	l_file->Filetype = dataFolder;
	int l_index = min( p_pathName.find_last_of( wxT( "\\" ) ), p_pathName.find_last_of( wxT( "/" ) ) );
	l_file->FileName = p_pathName.substr( l_index + 1 );
	l_file->Folder = p_pathName.substr( 0, l_index );
	std::cout << "TrollScene::AddSceneFile - " << l_file->Folder << " - " << l_file->FileName << "\n";
	l_file->FolderId = p_folder;
	l_file->ItemId = p_item;
	l_file->Saved = p_saved;
	l_file->Open = false;
	m_dataFolders.push_back( l_file );
	return true;
}



bool TrollScene::AddMuseFile( TrollFile * p_file )
{
	for ( TrollFileArray::iterator l_it = m_museFiles.begin() ; l_it != m_museFiles.end() ; )
	{
		if ( p_file->FileName == ( * l_it )->FileName )
		{
			wxMessageBox( _T( "Impossible d'ajouter ce fichier, il existe déjà dans cette scene" ),
						  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
			return false;
		}

		++l_it;
	}

	TrollFile * l_file = new TrollFile( * p_file );
	l_file->m_scene = this;
	m_museFiles.push_back( l_file );
	return true;
}



bool TrollScene::AddFile( const wxTreeItemId & p_folder, const wxTreeItemId & p_item,
						  const wxString & p_pathName, bool p_saved, bool p_showError )
{
	if ( m_sceneFileId == p_folder )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::AddFile - TrollScene File" ) );
		return AddSceneFile( p_pathName, p_folder, p_item, p_saved, p_showError );
	}

	if ( m_loadScriptFileId == p_folder )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::AddFile - Load Script File" ) );
		return AddLoadScriptFile( p_pathName, p_folder, p_item, p_saved, p_showError );
	}

	if ( m_unloadScriptFileId == p_folder )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::AddFile - Unload Script File" ) );
		return AddUnloadScriptFile( p_pathName, p_folder, p_item, p_saved, p_showError );
	}

	if ( m_dataFileId == p_folder )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::AddFile - Data File" ) );
		return AddDataFile( p_pathName, p_folder, p_item, p_saved, p_showError );
	}

	if ( m_dataFolderId == p_folder )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::AddFile - Data Folder" ) );
		return AddDataFolder( p_pathName, p_folder, p_item, p_saved, p_showError );
	}

	return false;
}



TrollEntity * TrollScene::AddEntity( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_entities.find( p_name ) != m_currentSceneFile->m_entities.end() )
	{
		return m_currentSceneFile->m_entities.find( p_name )->second;
	}

	TrollEntity * l_object = new TrollEntity( p_name, p_fileName );
	m_currentSceneFile->m_entities.insert( TrollEntityMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollOverlay * TrollScene::AddOverlay( const wxString & p_name, const wxString & p_parentName,
									   const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_allOverlays.find( p_name ) != m_currentSceneFile->m_allOverlays.end() )
	{
		return m_currentSceneFile->m_allOverlays.find( p_name )->second;
	}

	TrollOverlay * l_parent = NULL;

	if ( ! p_parentName.empty() && m_currentSceneFile->m_allOverlays.find( p_parentName ) != m_currentSceneFile->m_allOverlays.end() )
	{
		l_parent = m_currentSceneFile->m_allOverlays.find( p_parentName )->second;
	}

	TrollOverlay * l_object = new TrollOverlay( p_name, l_parent, p_fileName );

	if ( l_parent == NULL )
	{
		m_currentSceneFile->m_overlayGroups.insert( TrollOverlayMap::value_type( p_name, l_object ) );
	}

	m_currentSceneFile->m_allOverlays.insert( TrollOverlayMap::value_type( p_name, l_object ) );
	return l_object;
}



bool TrollScene::AddOverlay( TrollOverlay * p_overlay )
{
	if ( m_currentSceneFile == NULL )
	{
		return false;
	}

	wxString l_name = p_overlay->GetName();

	if ( m_currentSceneFile->m_allOverlays.find( l_name ) != m_currentSceneFile->m_allOverlays.end() )
	{
		return false;
	}

	TrollOverlay * l_parent = p_overlay->GetParent();

	if ( l_parent == NULL )
	{
		m_currentSceneFile->m_overlayGroups.insert( TrollOverlayMap::value_type( l_name, p_overlay ) );
	}

	m_currentSceneFile->m_allOverlays.insert( TrollOverlayMap::value_type( l_name, p_overlay ) );
	return true;
}



TrollSceneNode * TrollScene::AddSceneNode( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	TrollSceneNode * l_object = General::Utils::map::findOrNull( m_currentSceneFile->m_allSceneNodes, p_name );

	if ( l_object != NULL )
	{
		return l_object;
	}

	l_object = new TrollSceneNode( p_name, p_fileName );
	m_currentSceneFile->m_allSceneNodes.insert( TrollSceneNodeMap::value_type( p_name, l_object ) );
	return l_object;
}



void TrollScene::AttachNode( TrollSceneNode * p_node, TrollSceneNode * p_parent )
{
	if ( p_parent != NULL )
	{
		p_node->AttachTo( p_parent );
	}
	else
	{
		m_currentSceneFile->m_sceneNodes.insert( TrollSceneNodeMap::value_type( p_node->GetName(), p_node ) );
	}
}



TrollLight * TrollScene::AddLight( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_lights.find( p_name ) != m_currentSceneFile->m_lights.end() )
	{
		return m_currentSceneFile->m_lights.find( p_name )->second;
	}

	TrollLight * l_object = new TrollLight( p_name, p_fileName );
	m_currentSceneFile->m_lights.insert( TrollLightMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollCamera * TrollScene::AddCamera( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_cameras.find( p_name ) != m_currentSceneFile->m_cameras.end() )
	{
		return m_currentSceneFile->m_cameras.find( p_name )->second;
	}

	TrollCamera * l_object = new TrollCamera( p_name, p_fileName );
	m_currentSceneFile->m_cameras.insert( TrollCameraMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollMesh * TrollScene::AddMesh( const wxString & p_name )
{
	if ( m_meshes.find( p_name ) != m_meshes.end() )
	{
		return m_meshes.find( p_name )->second;
	}

	TrollMesh * l_object = new TrollMesh( p_name );
	m_meshes.insert( TrollMeshStrMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollAnimationGroup * TrollScene::AddAnimationGroup( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_animationGroups.find( p_name ) != m_currentSceneFile->m_animationGroups.end() )
	{
		return m_currentSceneFile->m_animationGroups.find( p_name )->second;
	}

	TrollAnimationGroup * l_object = new TrollAnimationGroup( p_name, p_fileName );
	m_currentSceneFile->m_animationGroups.insert( TrollAnimationGroupStrMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollSound * TrollScene::AddSound( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_sounds.find( p_name ) != m_currentSceneFile->m_sounds.end() )
	{
		return m_currentSceneFile->m_sounds.find( p_name )->second;
	}

	std::cout << "TrollScene::AddSound - " << p_name << "\n";
	TrollSound * l_object = new TrollSound( p_name, p_fileName );
	m_currentSceneFile->m_sounds.insert( TrollSoundStrMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollVideo * TrollScene::AddVideo( const wxString & p_name, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_videos.find( p_name ) != m_currentSceneFile->m_videos.end() )
	{
		return m_currentSceneFile->m_videos.find( p_name )->second;
	}

	TrollVideo * l_object = new TrollVideo( p_name, p_fileName );
	m_currentSceneFile->m_videos.insert( TrollVideoStrMap::value_type( p_name, l_object ) );
	return l_object;
}



TrollSequence * TrollScene::AddSequence( TrollSequence * p_sequence, const wxString & p_fileName )
{
	if ( m_currentSceneFile == NULL )
	{
		return NULL;
	}

	if ( m_currentSceneFile->m_sequences.find( p_sequence->TrollObject::GetName() ) != m_currentSceneFile->m_sequences.end() )
	{
		return m_currentSceneFile->m_sequences.find( p_sequence->TrollObject::GetName() )->second;
	}

	m_currentSceneFile->m_sequences.insert( TrollSequenceStrMap::value_type( p_sequence->TrollObject::GetName(), p_sequence ) );
	return p_sequence;
}



void TrollScene::RemoveSceneFile( const wxString & p_pathName )
{
	for ( TrollFileArray::iterator l_it = m_sceneFiles.begin() ; l_it != m_sceneFiles.end() ; )
	{
		std::cout << "gg : " << p_pathName << " et " << ( * l_it )->FileName << std::endl;

		if ( p_pathName == ( * l_it )->FileName )
		{
			delete( * l_it );
			m_sceneFiles.erase( l_it );
			return;
		}

		++l_it;
	}
}



void TrollScene::RemoveLoadScriptFile( const wxString & p_pathName )
{
	for ( TrollFileArray::iterator l_it = m_loadScriptFiles.begin() ; l_it != m_loadScriptFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			delete( * l_it );
			m_loadScriptFiles.erase( l_it );
			return;
		}

		++l_it;
	}
}



void TrollScene::RemoveUnloadScriptFile( const wxString & p_pathName )
{
	for ( TrollFileArray::iterator l_it = m_unloadScriptFiles.begin() ; l_it != m_unloadScriptFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			delete( * l_it );
			m_unloadScriptFiles.erase( l_it );
			return;
		}

		++l_it;
	}
}



void TrollScene::RemoveDataFile( const wxString & p_pathName )
{
	for ( TrollFileArray::iterator l_it = m_dataFiles.begin() ; l_it != m_dataFiles.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			delete( * l_it );
			m_dataFiles.erase( l_it );
			return;
		}

		++l_it;
	}
}



void TrollScene::RemoveDataFolder( const wxString & p_pathName )
{
	for ( TrollFileArray::iterator l_it = m_dataFolders.begin() ; l_it != m_dataFolders.end() ; )
	{
		if ( p_pathName == ( * l_it )->FileName )
		{
			delete( * l_it );
			m_dataFolders.erase( l_it );
			return;
		}

		++l_it;
	}
}



void TrollScene::RemoveFile( const wxTreeItemId & p_item )
{
	TrollFile * l_file = GetFile( p_item );
	std::cout << m_sceneFileId << " & " << l_file->FolderId << std::endl;

	if ( m_sceneFileId == l_file->FolderId )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::RemoveFile - Scene File" ) );
		RemoveSceneFile( l_file->FileName );
		return;
	}

	if ( m_loadScriptFileId == l_file->FolderId )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::RemoveFile - Load Script File" ) );
		RemoveLoadScriptFile( l_file->FileName );
		return;
	}

	if ( m_unloadScriptFileId == l_file->FolderId )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::RemoveFile - Unload Script File" ) );
		RemoveUnloadScriptFile( l_file->FileName );
		return;
	}

	if ( m_dataFileId == l_file->FolderId )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::RemoveFile - Data File" ) );
		RemoveDataFile( l_file->FileName );
		return;
	}

	if ( m_dataFolderId == l_file->FolderId )
	{
		g_mainFrame->LogDebugMessage( wxT( "TrollScene::RemoveFile - Data Folder" ) );
		RemoveDataFolder( l_file->FileName );
		return;
	}
}



bool TrollScene::HasItem( const wxTreeItemId & p_item )
{
	if ( m_sceneId == p_item )
	{
		return true;
	}

	if ( m_sceneFileId == p_item )
	{
		return true;
	}

	if ( m_loadScriptFileId == p_item )
	{
		return true;
	}

	if ( m_unloadScriptFileId == p_item )
	{
		return true;
	}

	if ( m_dataFolderId == p_item )
	{
		return true;
	}

	if ( m_dataFileId == p_item )
	{
		return true;
	}

	for ( unsigned int i = 0 ; i < m_sceneFiles.size() ; i++ )
	{
		if ( m_sceneFiles[i]->ItemId == p_item )
		{
			return true;
		}
	}

	for ( unsigned int i = 0 ; i < m_loadScriptFiles.size() ; i++ )
	{
		if ( m_loadScriptFiles[i]->ItemId == p_item )
		{
			return true;
		}
	}

	for ( unsigned int i = 0 ; i < m_unloadScriptFiles.size() ; i++ )
	{
		if ( m_unloadScriptFiles[i]->ItemId == p_item )
		{
			return true;
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFiles.size() ; i++ )
	{
		if ( m_dataFiles[i]->ItemId == p_item )
		{
			return true;
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFolders.size() ; i++ )
	{
		if ( m_dataFolders[i]->ItemId == p_item )
		{
			return true;
		}
	}

	return false;
}



void TrollScene::Save( wxTextOutputStream * p_stream )
{
	if ( ! p_stream )
	{
		return;
	}

	p_stream->WriteString( wxT( "\tscene " ) + m_name + wxT( "\n\t{\n" ) );
	TrollFileArray::iterator l_it;

	if ( m_sceneFiles.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t\tscene_files\n\t\t{\n" ) );

		for ( l_it = m_sceneFiles.begin() ; l_it != m_sceneFiles.end() ; ++l_it )
		{
			g_mainFrame->LogDebugMessage( wxT( "TrollScene::Save - " ) + ( * l_it )->FileName );
			p_stream->WriteString( wxT( "\t\t\t" ) + ( * l_it )->FileName + wxT( "\n" ) );
		}

		p_stream->WriteString( wxT( "\t\t}\n" ) );
	}

	if ( m_loadScriptFiles.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t\tload_script_files\n\t\t{\n" ) );

		for ( l_it = m_loadScriptFiles.begin() ; l_it != m_loadScriptFiles.end() ; ++l_it )
		{
			g_mainFrame->LogDebugMessage( wxT( "TrollScene::Save - " ) + ( * l_it )->FileName );
			p_stream->WriteString( wxT( "\t\t\t" ) + ( * l_it )->FileName + wxT( "\n" ) );
		}

		p_stream->WriteString( wxT( "\t\t}\n" ) );
	}

	if ( m_unloadScriptFiles.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t\tunload_script_files\n\t\t{\n" ) );

		for ( l_it = m_unloadScriptFiles.begin() ; l_it != m_unloadScriptFiles.end() ; ++l_it )
		{
			g_mainFrame->LogDebugMessage( wxT( "TrollScene::Save - " ) + ( * l_it )->FileName );
			p_stream->WriteString( wxT( "\t\t\t" ) + ( * l_it )->FileName + wxT( "\n" ) );
		}

		p_stream->WriteString( wxT( "\t\t}\n" ) );
	}

	if ( m_dataFiles.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t\tdata_files_3d\n\t\t{\n" ) );

		for ( l_it = m_dataFiles.begin() ; l_it != m_dataFiles.end() ; ++l_it )
		{
			g_mainFrame->LogDebugMessage( wxT( "TrollScene::Save - " ) + ( * l_it )->FileName );
			p_stream->WriteString( wxT( "\t\t\t" ) + ( * l_it )->FileName + wxT( "\n" ) );
		}

		p_stream->WriteString( wxT( "\t\t}\n" ) );
	}

	if ( m_dataFolders.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t\tdata_files_other\n\t\t{\n" ) );

		for ( l_it = m_dataFolders.begin() ; l_it != m_dataFolders.end() ; ++l_it )
		{
			g_mainFrame->LogDebugMessage( wxT( "TrollScene::Save - " ) + ( * l_it )->FileName );
			p_stream->WriteString( wxT( "\t\t\t" ) + ( * l_it )->FileName + wxT( "\n" ) );
		}

		p_stream->WriteString( wxT( "\t\t}\n" ) );
	}

	if ( m_neededScenes.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t\tdependencies\n\t\t{\n" ) );

		for ( size_t i = 0 ; i < m_neededScenes.size() ; i++ )
		{
			g_mainFrame->LogDebugMessage( wxT( "TrollScene::Save - Dependency : " ) + m_neededScenes[i] );
			p_stream->WriteString( wxT( "\t\t\t" ) + m_neededScenes[i] + wxT( "\n" ) );
		}

		p_stream->WriteString( wxT( "\t\t}\n" ) );
	}

	p_stream->WriteString( wxT( "\t}\n" ) );
}

void TrollScene::AddToTree( FilesTree * p_tree )
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
			l_id = p_tree->AddFolderToScene( m_name  , wxT( "Fichiers de scène" ) );
			m_sceneFileId = l_id;
			m_loadScriptFileId = p_tree->AddFolderToScene( m_name, wxT( "Fichiers de script de load" ) );
			m_unloadScriptFileId = p_tree->AddFolderToScene( m_name, wxT( "Fichiers de script d'unload" ) );
			m_dataFileId = p_tree->AddFolderToScene( m_name, wxT( "Datas Graphiques (3D et 2D)" ) );
			m_dataFolderId = p_tree->AddFolderToScene( m_name, wxT( "Datas non Graphiques (autres)" ) );
		}
	}
	catch ( bool )
	{
	}
}

void TrollScene::AddSceneFile( wxString const & p_strFile, FilesTree * p_tree )
{
	TrollFile * l_file = new TrollFile( this, sceneFile, p_strFile, m_museFile, m_sceneFileId );
	std::cout << "TrollScene::AddSceneFile - " << l_file->Folder << " - " << l_file->FileName << "\n";

	if ( p_tree != NULL )
	{
		l_file->ItemId = p_tree->AddFileToFolder( m_sceneFileId, p_strFile, TITSceneFile, true );
	}

	m_sceneFiles.push_back( l_file );
}

void TrollScene::AddLoadScriptFile( wxString const & p_strFile, FilesTree * p_tree )
{
	TrollFile * l_file = new TrollFile( this, loadScriptFile, p_strFile, m_museFile, m_loadScriptFileId );
	std::cout << "TrollScene::AddLoadScriptFile - " << l_file->Folder << " - " << l_file->FileName << "\n";

	if ( p_tree != NULL )
	{
		l_file->ItemId = p_tree->AddFileToFolder( m_loadScriptFileId, p_strFile, TITLoadScriptFile, true );
	}

	m_loadScriptFiles.push_back( l_file );
}

void TrollScene::AddUnloadScriptFile( wxString const & p_strFile, FilesTree * p_tree )
{
	TrollFile * l_file = new TrollFile( this, unloadScriptFile, p_strFile, m_museFile, m_unloadScriptFileId );
	std::cout << "TrollScene::AddUnloadScriptFile - " << l_file->Folder << " - " << l_file->FileName << "\n";

	if ( p_tree != NULL )
	{
		l_file->ItemId = p_tree->AddFileToFolder( m_unloadScriptFileId, p_strFile, TITUnloadScriptFile, true );
	}

	m_unloadScriptFiles.push_back( l_file );
}

void TrollScene::Add3DDataFile( wxString const & p_strFile, FilesTree * p_tree )
{
	TrollFile * l_file = new TrollFile( this, dataFile, p_strFile, m_museFile, m_dataFileId );
	l_file->Folder = p_strFile;
	std::cout << "TrollScene::Add3DDataFile - " << l_file->Folder << " - " << l_file->FileName << "\n";

	if ( p_tree != NULL )
	{
		l_file->ItemId = p_tree->AddFileToFolder( m_dataFileId, p_strFile, TITDataFile, true );
	}

	m_dataFiles.push_back( l_file );
}

void TrollScene::AddOtherDataFile( wxString const & p_strFile, FilesTree * p_tree )
{
	TrollFile * l_file = new TrollFile( this, dataFolder, p_strFile, m_museFile, m_dataFolderId );
	l_file->Folder = p_strFile;
	std::cout << "TrollScene::AddOtherDataFile - " << l_file->Folder << " - " << l_file->FileName << "\n";

	if ( p_tree != NULL )
	{
		l_file->ItemId = p_tree->AddFileToFolder( m_dataFolderId, p_strFile, TITDataFolder, true );
	}

	m_dataFolders.push_back( l_file );
}

void TrollScene::AddDependency( wxString const & p_strName )
{
	m_neededScenes.push_back( p_strName );
}



wxString TrollScene::Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, FilesTree * p_tree )
{
	AddToTree( p_tree );

	if ( ! p_stream || ! p_input || p_input->Eof() )
	{
		return wxT( "" );
	}

	wxString l_line = p_stream->ReadLine();

	if ( l_line.empty() || p_input->Eof() )
	{
		return wxT( "" );
	}

	if ( l_line == wxT( "SceneFiles" ) )
	{
		l_line = p_stream->ReadLine();

		while ( l_line != wxT( "LoadScriptFiles" ) && l_line != wxT( "UnloadScriptFiles" ) && l_line != wxT( "DataFiles" )
				&& l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" ) && ! l_line.empty() && ! p_input->Eof() )
		{
			AddSceneFile( l_line, p_tree );
			l_line = p_stream->ReadLine();
		}

		if ( l_line.empty() || p_input->Eof() )
		{
			return wxT( "" );
		}
	}

	if ( l_line == wxT( "LoadScriptFiles" ) )
	{
		l_line = p_stream->ReadLine();

		while ( l_line != wxT( "UnloadScriptFiles" ) && l_line != wxT( "DataFiles" )
				&& l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" )
				&& ! l_line.empty() && ! p_input->Eof() )
		{
			AddLoadScriptFile( l_line, p_tree );
			l_line = p_stream->ReadLine();
		}

		if ( l_line.empty() || p_input->Eof() )
		{
			return wxT( "" );
		}
	}

	if ( l_line == wxT( "UnloadScriptFiles" ) )
	{
		l_line = p_stream->ReadLine();

		while ( l_line != wxT( "DataFiles" ) && l_line != wxT( "Scene" )
				&& l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && ! l_line.empty() && ! p_input->Eof() )
		{
			AddUnloadScriptFile( l_line, p_tree );
			l_line = p_stream->ReadLine();
		}

		if ( l_line.empty() || p_input->Eof() )
		{
			return wxT( "" );
		}
	}

	if ( l_line == wxT( "DataFiles" ) )
	{
		l_line = p_stream->ReadLine();

		while ( l_line != wxT( "DataFolders" ) && l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" ) && ! l_line.empty() && ! p_input->Eof() )
		{
			Add3DDataFile( l_line, p_tree );
			l_line = p_stream->ReadLine();
		}
	}

	if ( l_line == wxT( "DataFolders" ) )
	{
		l_line = p_stream->ReadLine();

		while ( l_line != wxT( "Dependencies" ) && l_line != wxT( "Scene" ) && ! l_line.empty() && ! p_input->Eof() )
		{
			AddOtherDataFile( l_line, p_tree );
			l_line = p_stream->ReadLine();
		}
	}

	if ( l_line == wxT( "Dependencies" ) )
	{
		l_line = p_stream->ReadLine();

		while ( l_line != wxT( "Scene" ) && ! l_line.empty() && ! p_input->Eof() )
		{
			AddDependency( l_line );
			l_line = p_stream->ReadLine();
		}
	}

	return l_line;
}



void TrollScene::Write( const wxString & p_path )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();

	while ( l_it != m_sceneConfigFiles.end() )
	{
		l_it->second->Write( p_path );
		++l_it;
	}
}



void TrollScene::UpdateFileName( const wxString & p_ancient, const wxString & p_new )
{
	int l_index = min( p_new.find_last_of( wxT( "\\" ) ), p_new.find_last_of( wxT( "/" ) ) );

	for ( unsigned int i = 0 ; i < m_sceneFiles.size() ; i++ )
	{
		if ( m_sceneFiles[i]->FileName == p_ancient )
		{
			m_sceneFiles[i]->FileName = p_new.substr( l_index + 1 );
			m_sceneFiles[i]->Folder = p_new.substr( 0, l_index );
			std::cout << "TrollScene::UpdateFileName - " << m_sceneFiles[i]->Folder << " - " << m_sceneFiles[i]->FileName << "\n";
			m_sceneFiles[i]->Saved = true;
			return;
		}
	}

	for ( unsigned int i = 0 ; i < m_loadScriptFiles.size() ; i++ )
	{
		if ( m_loadScriptFiles[i]->FileName == p_ancient )
		{
			m_loadScriptFiles[i]->FileName = p_new.substr( l_index + 1 );
			m_loadScriptFiles[i]->Folder = p_new.substr( 0, l_index );
			std::cout << "TrollScene::UpdateFileName - " << m_loadScriptFiles[i]->Folder << " - " << m_loadScriptFiles[i]->FileName << "\n";
			m_loadScriptFiles[i]->Saved = true;
			return;
		}
	}

	for ( unsigned int i = 0 ; i < m_unloadScriptFiles.size() ; i++ )
	{
		if ( m_unloadScriptFiles[i]->FileName == p_ancient )
		{
			m_unloadScriptFiles[i]->FileName = p_new.substr( l_index + 1 );
			m_unloadScriptFiles[i]->Folder = p_new.substr( 0, l_index );
			std::cout << "TrollScene::UpdateFileName - " << m_unloadScriptFiles[i]->Folder << " - " << m_unloadScriptFiles[i]->FileName << "\n";
			m_unloadScriptFiles[i]->Saved = true;
			return;
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFiles.size() ; i++ )
	{
		if ( m_dataFiles[i]->FileName == p_ancient )
		{
			m_dataFiles[i]->FileName = p_new.substr( l_index + 1 );
			m_dataFiles[i]->Folder = p_new.substr( 0, l_index );
			std::cout << "TrollScene::UpdateFileName - " << m_dataFiles[i]->Folder << " - " << m_dataFiles[i]->FileName << "\n";
			m_dataFiles[i]->Saved = true;
			return;
		}
	}
}



FileType TrollScene::GetType( const wxTreeItemId & p_folderId )
{
	if ( m_sceneFileId == p_folderId )
	{
		return sceneFile;
	}

	if ( m_loadScriptFileId == p_folderId )
	{
		return loadScriptFile;
	}

	if ( m_unloadScriptFileId == p_folderId )
	{
		return unloadScriptFile;
	}

	if ( m_dataFolderId == p_folderId )
	{
		return dataFolder;
	}

	return dataFile;
}



TrollFile * TrollScene::GetFile( const wxTreeItemId & p_itemId )
{
	for ( unsigned int i = 0 ; i < m_sceneFiles.size() ; i++ )
	{
		if ( m_sceneFiles[i]->ItemId == p_itemId )
		{
			return m_sceneFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_loadScriptFiles.size() ; i++ )
	{
		if ( m_loadScriptFiles[i]->ItemId == p_itemId )
		{
			return m_loadScriptFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_unloadScriptFiles.size() ; i++ )
	{
		if ( m_unloadScriptFiles[i]->ItemId == p_itemId )
		{
			return m_unloadScriptFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFiles.size() ; i++ )
	{
		if ( m_dataFiles[i]->ItemId == p_itemId )
		{
			return m_dataFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFolders.size() ; i++ )
	{
		if ( m_dataFolders[i]->ItemId == p_itemId )
		{
			return m_dataFolders[i];
		}
	}

	return NULL;
}



TrollFile * TrollScene::GetFileByName( const wxString & p_itemName )
{
	for ( unsigned int i = 0 ; i < m_sceneFiles.size() ; i++ )
	{
		if ( m_sceneFiles[i]->FileName == p_itemName )
		{
			return m_sceneFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_loadScriptFiles.size() ; i++ )
	{
		if ( m_loadScriptFiles[i]->FileName == p_itemName )
		{
			return m_loadScriptFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_unloadScriptFiles.size() ; i++ )
	{
		if ( m_unloadScriptFiles[i]->FileName == p_itemName )
		{
			return m_unloadScriptFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFiles.size() ; i++ )
	{
		if ( m_dataFiles[i]->FileName == p_itemName )
		{
			return m_dataFiles[i];
		}
	}

	for ( unsigned int i = 0 ; i < m_dataFolders.size() ; i++ )
	{
		if ( m_dataFolders[i]->FileName == p_itemName )
		{
			return m_dataFolders[i];
		}
	}

	return NULL;
}



TrollFile * TrollScene::GetSceneFile( unsigned int p_index )
{
	if ( p_index < m_sceneFiles.size() )
	{
		return m_sceneFiles[p_index];
	}

	return NULL;
}



TrollFile * TrollScene::GetLoadScriptFile( unsigned int p_index )
{
	if ( p_index < m_loadScriptFiles.size() )
	{
		return m_loadScriptFiles[p_index];
	}

	return NULL;
}



TrollFile * TrollScene::GetUnloadScriptFile( unsigned int p_index )
{
	if ( p_index < m_unloadScriptFiles.size() )
	{
		return m_unloadScriptFiles[p_index];
	}

	return NULL;
}



TrollFile * TrollScene::GetDataFile( unsigned int p_index )
{
	if ( p_index < m_dataFiles.size() )
	{
		return m_dataFiles[p_index];
	}

	return NULL;
}



TrollFile * TrollScene::GetDataFolder( unsigned int p_index )
{
	if ( p_index < m_dataFolders.size() )
	{
		return m_dataFolders[p_index];
	}

	return NULL;
}



TrollEntity * TrollScene::GetEntity( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollEntity * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetEntity( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollMesh * TrollScene::GetMesh( const wxString & p_name )
{
	TrollMeshStrMap::iterator l_it = m_meshes.find( p_name );

	if ( l_it == m_meshes.end() )
	{
		return NULL;
	}

	return l_it->second;
}



TrollOverlay * TrollScene::GetOverlay( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollOverlay * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetOverlay( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollSceneNode * TrollScene::GetSceneNode( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollSceneNode * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetSceneNode( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollLight * TrollScene::GetLight( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollLight * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetLight( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollCamera * TrollScene::GetCamera( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollCamera * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetCamera( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollAnimationGroup * TrollScene::GetAnimationGroup( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollAnimationGroup * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetAnimationGroup( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollSound * TrollScene::GetSound( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollSound * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetSound( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollVideo * TrollScene::GetVideo( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollVideo * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetVideo( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



TrollSequence * TrollScene::GetSequence( const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollSequence * l_result;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		if ( ( l_result = l_it->second->GetSequence( p_name ) ) != NULL )
		{
			return l_result;
		}

		++l_it;
	}

	return NULL;
}



void TrollScene::GetNodesNames( wxArrayString & p_list, const wxString & p_name )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollSceneNodeMap::iterator l_nodesIt;

	while ( l_it != m_sceneConfigFiles.end() )
	{
		l_nodesIt = l_it->second->m_allSceneNodes.begin();

		while ( l_nodesIt != l_it->second->m_allSceneNodes.end() )
		{
			if ( l_nodesIt->first != p_name )
			{
				p_list.Add( l_nodesIt->first );
			}

			++l_nodesIt;
		}

		++l_it;
	}
}



void TrollScene::GetOverlaysNames( wxArrayString & p_list )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollOverlayMap::iterator l_overlaysIt;

	for ( ; l_it != m_sceneConfigFiles.end() ; ++l_it )
	{
		l_overlaysIt = l_it->second->m_allOverlays.begin();

		for ( ; l_overlaysIt != l_it->second->m_allOverlays.end() ; ++l_overlaysIt )
		{
//			std::cout << "TrollScene::GetOverlaysNames - " << l_overlaysIt->first << "\n";
			if ( l_overlaysIt->first != wxT( "Core/DebugOverlay" ) && l_overlaysIt->first != wxT( "Core/LensFlare" ) && l_overlaysIt->first != wxT( "Core/LoadOverlay" ) )
			{
				p_list.Add( l_overlaysIt->first );
			}
		}
	}
}



void TrollScene::GetMeshesNames( wxArrayString & p_list )
{
	TrollMeshStrMap::iterator l_it = m_meshes.begin();

	for ( ; l_it != m_meshes.end() ; ++l_it )
	{
		p_list.Add( l_it->first );
	}
}



void TrollScene::GetSequencesNames( wxArrayString & p_list )
{
	SceneFileStrMap::iterator l_it = m_sceneConfigFiles.begin();
	TrollSequenceStrMap::iterator l_sequencesIt;

	for ( ; l_it != m_sceneConfigFiles.end() ; ++l_it )
	{
		l_sequencesIt = l_it->second->m_sequences.begin();

		for ( ; l_sequencesIt != l_it->second->m_sequences.end() ; ++l_sequencesIt )
		{
			p_list.Add( l_sequencesIt->first );
		}
	}
}



void TrollScene::AddDependency( TrollScene * p_scene )
{
	bool l_found = false;
	size_t i = 0;

	while ( i < m_neededScenes.size() && ! l_found )
	{
		if ( m_neededScenes[i] == p_scene->GetName() )
		{
			l_found = true;
		}

		i++;
	}

	if ( ! l_found )
	{
		p_scene->SetNeeded( true );
		m_neededScenes.push_back( p_scene->GetName() );
		p_scene->_addSceneThatNeedMe( this );
	}
}



void TrollScene::RemoveDependency( TrollScene * p_scene )
{
	bool l_found = false;
	wxArrayString::iterator l_it = m_neededScenes.begin();

	while ( l_it != m_neededScenes.end() && ! l_found )
	{
		if ( ( * l_it ) == p_scene->GetName() )
		{
			p_scene->SetNeeded( false );
			m_neededScenes.erase( l_it );
			p_scene->_removeSceneThatNeedMe( this );
			l_found = true;
		}

		l_it++;
	}
}



void TrollScene::_removeSceneThatNeedMe( TrollScene * p_scene )
{
	SceneMap::iterator l_it = m_scenesThatNeedMe.find( p_scene->GetName() );

	if ( l_it != m_scenesThatNeedMe.end() )
	{
		m_scenesThatNeedMe.erase( l_it );
	}
}


