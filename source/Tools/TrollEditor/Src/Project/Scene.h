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

namespace Troll
{
	namespace ProjectComponents
	{
		class Scene
		{
			friend class Project;

		public:
			Scene( Project & p_project, wxString const & p_name, bool p_main = false );
			~Scene();

			void FlushFiles();
			void SetCurrentSceneFile( wxString const & p_sceneFileName );
			void SaveScripts( wxString const & p_path )const;
			void UpdateFileName( wxString const & p_ancient, wxString const & p_new );
			File & AddFile( FileType p_type, wxString const & p_pathName, bool p_saved, bool p_showError );
			void RemoveFile( FileType p_type, wxString const & p_pathName );
			File & GetFile( wxString const & p_itemName );
			File & GetSceneFile( uint32_t p_index );
			File & GetLoadScriptFile( uint32_t p_index );
			File & GetUnloadScriptFile( uint32_t p_index );
			File & GetDataFile( uint32_t p_index );
			File & GetDataFolder( uint32_t p_index );

			void AddDependency( wxString const & p_strName );

			void FlushObjects();
			void AttachNode( Objects3D::TrollSceneNode * p_node, Objects3D::TrollSceneNode * p_parent );
			bool AddOverlay( Objects2D::TrollOverlay * p_overlay );
			Objects3D::TrollEntity * AddEntity( wxString const & p_name, wxString const & p_fileName );
			Objects2D::TrollOverlay * AddOverlay( wxString const & p_name, wxString const & p_parentName, wxString const & p_fileName );
			Objects3D::TrollSceneNode * AddSceneNode( wxString const & p_name, wxString const & p_fileName );
			Objects3D::TrollLight * AddLight( wxString const & p_name, wxString const & p_fileName );
			Objects3D::TrollCamera * AddCamera( wxString const & p_name, wxString const & p_fileName );
			Objects3D::TrollMesh * AddMesh( wxString const & p_name );
			Temporal::TrollAnimationGroup * AddAnimationGroup( wxString const & p_name, wxString const & p_fileName );
			Media::TrollSound * AddSound( wxString const & p_name, wxString const & p_fileName );
			Media::TrollVideo * AddVideo( wxString const & p_name, wxString const & p_fileName );
			Temporal::TrollSequence * AddSequence( Temporal::TrollSequence * p_sequence, wxString const & p_fileName );
			void RemoveEntity( wxString const & p_name );
			void RemoveSceneNode( wxString const & p_name );
			void RemoveLight( wxString const & p_name );
			void RemoveCamera( wxString const & p_name );

			Objects3D::TrollEntity * GetEntity( wxString const & p_name );
			Objects3D::TrollMesh * GetMesh( wxString const & p_name );
			Objects2D::TrollOverlay * GetOverlay( wxString const & p_name );
			Objects3D::TrollSceneNode * GetSceneNode( wxString const & p_name );
			Objects3D::TrollLight * GetLight( wxString const & p_name );
			Objects3D::TrollCamera * GetCamera( wxString const & p_name );
			Temporal::TrollAnimationGroup * GetAnimationGroup( wxString const & p_name );
			Media::TrollSound * GetSound( wxString const & p_name );
			Media::TrollVideo * GetVideo( wxString const & p_name );
			Temporal::TrollSequence * GetSequence( wxString const & p_name );

			void GetNodesNames( wxArrayString & p_list, wxString const & p_name = wxEmptyString );
			void GetOverlaysNames( wxArrayString & p_list );
			void GetMeshesNames( wxArrayString & p_list );
			void GetSequencesNames( wxArrayString & p_list );

			void AddDependency( ScenePtr p_scene );
			void RemoveDependency( ScenePtr p_scene );

			inline wxString const & GetName()const
			{
				return m_name;
			}
			inline FileArray const & GetSceneFiles()const
			{
				return m_files[sceneFile];
			}
			inline FileArray const & GetLoadScriptFiles()const
			{
				return m_files[loadScriptFile];
			}
			inline FileArray const & GetUnloadScriptFiles()const
			{
				return m_files[unloadScriptFile];
			}
			inline FileArray const & GetDataFiles()const
			{
				return m_files[dataFile];
			}
			inline FileArray const & GetDataFolders()const
			{
				return m_files[dataFolder];
			}
			inline FileArray & GetSceneFiles()
			{
				return m_files[sceneFile];
			}
			inline FileArray & GetLoadScriptFiles()
			{
				return m_files[loadScriptFile];
			}
			inline FileArray & GetUnloadScriptFiles()
			{
				return m_files[unloadScriptFile];
			}
			inline FileArray & GetDataFiles()
			{
				return m_files[dataFile];
			}
			inline FileArray & GetDataFolders()
			{
				return m_files[dataFolder];
			}
			inline SceneFile * GetCurrentSceneFile()const
			{
				return m_currentSceneFile;
			}
			inline bool IsNeeded()const
			{
				return m_needed;
			}
			inline wxArrayString GetScenesThatNeedMe()const
			{
				return m_scenesThatNeedMe;
			}
			inline wxArrayString GetNeededScenes()const
			{
				return m_neededScenes;
			}
			inline void	SetName( wxString const & p_name )
			{
				m_name = p_name;
			}
			inline void SetNeeded( bool p_needed )
			{
				m_needed = p_needed;
			}
			inline ElypseRW::TrollMuseFile const & GetMuseFile()const
			{
				return *m_museFile;
			}
			inline ElypseRW::TrollMuseFile & GetMuseFile()
			{
				return *m_museFile;
			}
			inline FileArray const & GetMuseFiles()const
			{
				return m_museFiles;
			}
			template< typename FuncT >
			File & FindFile( FuncT p_func )
			{
				auto l_it = std::find_if( m_files[sceneFile].begin(), m_files[sceneFile].end(), p_func );

				if ( l_it == m_files[sceneFile].end() )
				{
					l_it = std::find_if( m_files[loadScriptFile].begin(), m_files[loadScriptFile].end(), p_func );

					if ( l_it == m_files[loadScriptFile].end() )
					{
						l_it = std::find_if( m_files[unloadScriptFile].begin(), m_files[unloadScriptFile].end(), p_func );

						if ( l_it == m_files[unloadScriptFile].end() )
						{
							l_it = std::find_if( m_files[dataFile].begin(), m_files[dataFile].end(), p_func );

							if ( l_it == m_files[dataFile].end() )
							{
								l_it = std::find_if( m_files[dataFolder].begin(), m_files[dataFolder].end(), p_func );

								if ( l_it == m_files[dataFolder].end() )
								{
									GENLIB_EXCEPTION( "No file matching these conditions" );
								}
							}
						}
					}
				}

				return *l_it;
			}
			template< typename FuncT >
			bool HasFile( FuncT p_func )const
			{
				auto l_it = std::find_if( m_files[sceneFile].begin(), m_files[sceneFile].end(), p_func );

				if ( l_it == m_files[sceneFile].end() )
				{
					l_it = std::find_if( m_files[loadScriptFile].begin(), m_files[loadScriptFile].end(), p_func );

					if ( l_it == m_files[loadScriptFile].end() )
					{
						l_it = std::find_if( m_files[unloadScriptFile].begin(), m_files[unloadScriptFile].end(), p_func );

						if ( l_it == m_files[unloadScriptFile].end() )
						{
							l_it = std::find_if( m_files[dataFile].begin(), m_files[dataFile].end(), p_func );

							if ( l_it == m_files[dataFile].end() )
							{
								l_it = std::find_if( m_files[dataFolder].begin(), m_files[dataFolder].end(), p_func );

								if ( l_it == m_files[dataFolder].end() )
								{
									return false;
								}
							}
						}
					}
				}

				return true;
			}

			inline void DoAddSceneThatNeedMe( Scene const & p_scene )
			{
				m_scenesThatNeedMe.push_back( p_scene.GetName() );
			}

			void DoRemoveSceneThatNeedMe( Scene * p_scene );

		private:
			std::array< FileArray, FileTypeCount > m_files;
			FileArray m_museFiles;
			std::shared_ptr< ElypseRW::TrollMuseFile > m_museFile;
			wxString m_name;
			float m_minZoom;
			float m_maxZoom;
			bool m_mainScene;
			bool m_needed;
			wxArrayString m_scenesThatNeedMe;
			wxArrayString m_neededScenes;
			SceneFileStrMap m_sceneConfigFiles;
			SceneFile * m_currentSceneFile;
			Objects3D::TrollMeshStrMap m_meshes;
		};
	}
}

#endif

