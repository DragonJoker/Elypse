/*
See LICENSE file in root folder
*/
#ifndef _Project_H_
#define _Project_H_

#include "Module_Project.h"

#include "GUI/TrollEditorGuiPrerequisites.h"

namespace Troll
{
	namespace ProjectComponents
	{
		class Project
			: public std::enable_shared_from_this< Project >
		{
		public:
			Project();
			Project( wxString const & p_projectName, wxString const & p_mainSceneName, wxString const & p_path, BackgroundType p_backgroundType, wxString const & p_background, bool p_shadows, AntiAliasing p_aa, wxSize const & p_resolution );
			~Project();

			void FlushObjects();

			ScenePtr GetScene( wxString const & p_sceneName );
			void RemoveScene( wxString const & p_sceneName );
			void RemoveScene( ScenePtr p_scene );
			ScenePtr CreateScene( wxString const & p_strName );
			bool AddScene( ScenePtr p_scene );
			void Load( wxString const & p_path );
			void SaveScripts();

			void SetBackgroundImage( wxString const & p_img );
			void SetBackgroundColour( wxString const & p_colour );

			bool FindFileInScenes( wxString const & p_fileName, File *& p_file, ScenePtr & p_scene );

			inline ScenePtr GetMainScene()const
			{
				return m_mainScene;
			}
			inline bool HasMainScene()const
			{
				return m_mainScene != nullptr;
			}
			inline ScenePtr GetSplashScene()const
			{
				return m_splashScene;
			}
			inline bool HasSplashScene()const
			{
				return m_splashScene != nullptr;
			}
			inline SceneArray const & GetScenes()const
			{
				return m_scenes;
			}
			inline SceneArray & GetScenes()
			{
				return m_scenes;
			}
			inline wxString const & GetPath()const
			{
				return m_projectPath;
			}
			inline wxString const & GetBackground()const
			{
				return m_bgString;
			}
			inline BackgroundType GetBackgroundType()const
			{
				return m_bgType;
			}
			inline wxColour const & GetBackgroundColour()const
			{
				return m_bgColour;
			}
			inline wxImage const & GetBackgroundImage()const
			{
				return m_bgImage;
			}
			inline wxString const & GetName()const
			{
				return m_name;
			}
			inline bool IsSaved()const
			{
				return m_saved;
			}
			inline bool IsModified()const
			{
				return m_modified;
			}
			inline wxString const & GetProjectFileName()const
			{
				return m_projectFileName;
			}
			inline AntiAliasing GetFSAA()const
			{
				return m_antiAliasing;
			}
			inline bool GetShadows()const
			{
				return m_shadows;
			}
			inline wxSize const & GetResolution()const
			{
				return m_resolution;
			}
			inline wxString const & GetStartupScript()const
			{
				return m_startupScript;
			}
			inline bool GetShowDebug()const
			{
				return m_showDebug;
			}
			inline bool GetShowFPS()const
			{
				return m_showFPS;
			}
			inline void SetSaved( bool p_saved )
			{
				m_saved = p_saved;
			}
			inline void SetModified( bool p_modified )
			{
				m_modified = p_modified;
			}
			inline void SetProjectFileName( wxString const & p_name )
			{
				m_projectFileName = p_name;
			}
			inline void SetProjectPath( wxString const & p_path )
			{
				m_projectPath = p_path;
			}
			inline void	SetName( wxString const & p_name )
			{
				m_name = p_name;
				m_modified = true;
			}
			inline void	SetSplashScene( wxString const & p_name )
			{
				m_splashScene = GetScene( p_name );
				m_modified = true;
			}
			inline void	SetMainScene( wxString const & p_name )
			{
				m_mainScene = GetScene( p_name );
				m_modified = true;
			}
			inline void SetShadows( bool p_shadows )
			{
				m_shadows = p_shadows;
				m_modified = true;
			}
			inline void SetBackgroundType( BackgroundType p_type )
			{
				m_bgType = p_type;
				m_modified = true;
			}
			inline void SetFSAA( AntiAliasing p_fsaa )
			{
				m_antiAliasing = p_fsaa;
				m_modified = true;
			}
			inline void	SetResolution( wxSize const & p_res )
			{
				m_resolution = p_res;
			}
			inline void	SetBackgroundColour( wxColour const & p_colour )
			{
				m_bgColour = p_colour;
				m_bgType = bgColour;
			}
			inline void SetBackgroundImage( wxImage const & p_image )
			{
				m_bgImage = p_image;
				m_bgType = bgImage;
			}
			inline void	SetStartupScript( wxString const & p_ss )
			{
				m_startupScript = p_ss;
			}
			inline void	SetShowDebug( bool p_sd )
			{
				m_showDebug = p_sd;
			}
			inline void SetShowFPS( bool p_sf )
			{
				m_showFPS = p_sf;
			}
			inline auto begin()
			{
				return m_scenes.begin();
			}
			inline auto begin()const
			{
				return m_scenes.begin();
			}
			inline auto end()
			{
				return m_scenes.end();
			}
			inline auto end()const
			{
				return m_scenes.end();
			}

		private:
			void DoFillSceneDependencies( Scene const & p_scene );
			void DoBuildColour( wxString const & p_infos );

		private:
			wxString m_name;
			wxString m_projectPath;
			BackgroundType m_bgType;
			wxString m_bgString;
			wxColour m_bgColour;
			wxImage m_bgImage;
			bool m_shadows;
			AntiAliasing m_antiAliasing;
			bool m_showDebug;
			bool m_showFPS;
			wxString m_startupScript;

			ScenePtr m_mainScene;
			ScenePtr m_splashScene;
			SceneArray m_scenes;
			bool m_saved;
			bool m_modified;
			wxString m_projectFileName;
			wxSize m_resolution;
		};
	}
}

#endif

