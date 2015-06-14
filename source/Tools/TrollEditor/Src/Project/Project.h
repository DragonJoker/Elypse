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
#ifndef _Project_H_
#define _Project_H_

#include "Module_Project.h"

#include "GUI/TrollEditorGuiPrerequisites.h"

BEGIN_TROLL_PROJECT_NAMESPACE
{
	class Project
		: public std::enable_shared_from_this< Project >
	{
	public:
		Project();
		Project( const wxString & p_projectName, const wxString & p_mainSceneName, const wxString & p_path, BackgroundType p_backgroundType, const wxString & p_background, bool p_shadows, AntiAliasing p_aa, const wxSize & p_resolution );
		~Project();

		void FlushObjects();

		Scene * GetScene( const wxString & p_sceneName );
		Scene * GetScene( const wxTreeItemId & p_item );
		void RemoveScene( const wxString & p_sceneName );
		void RemoveScene( Scene * p_scene );
		Scene * CreateScene( wxString const & p_strName );
		void AddScene( Scene * p_scene );
		void Save( wxTextOutputStream * p_stream );
		void Load( const wxString & p_path, GUI::FilesTree * p_tree );
		void Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, const wxString & p_path, GUI::FilesTree * p_tree = NULL );
		void Write();

		void SetBackgroundImage( const wxString & p_img );
		void SetBackgroundColour( const wxString & p_colour );

		bool FindFileInScenes( const wxString & p_fileName, File *& p_file, Scene *& p_scene );

		inline Scene * GetMainScene()const
		{
			return m_mainScene;
		}
		inline SceneMap GetScenes()const
		{
			return m_scenes;
		}
		inline const wxString & GetPath()const
		{
			return m_projectPath;
		}
		inline const wxString & GetBackground()const
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
		inline const wxString & GetName()const
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
		inline const wxString & GetProjectFileName()const
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
		inline const wxSize & GetResolution()const
		{
			return m_resolution;
		}
		inline const wxString & GetStartupScript()const
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
		inline void SetProjectFileName( const wxString & p_name )
		{
			m_projectFileName = p_name;
		}
		inline void SetProjectPath( const wxString & p_path )
		{
			m_projectPath = p_path;
		}
		inline void	SetName( const wxString & p_name )
		{
			m_name = p_name;
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
		inline void	SetResolution( const wxSize & p_res )
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
		inline void	SetStartupScript( const wxString & p_ss )
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

	private:
		void _fillSceneDependencies( Scene * p_scene );
		void _buildColour( const wxString & p_infos );

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

		Scene * m_mainScene;
		SceneMap m_scenes;
		bool m_saved;
		bool m_modified;
		wxString m_projectFileName;
		wxSize m_resolution;
	};
}
END_TROLL_PROJECT_NAMESPACE

#endif

