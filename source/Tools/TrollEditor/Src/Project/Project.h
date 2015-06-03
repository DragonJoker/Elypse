
#ifndef _Project_H_
#define _Project_H_

#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/treebase.h>
#include "GUI/Module_GUI.h"
#include "Module_Project.h"

class wxColour;
class wxImage;
namespace Troll
{
	class Project
	{
	private:
		wxString m_name;
		wxString m_projectPath;
		BackgroundType m_bgType;
		wxString m_bgString;
		wxColour * m_bgColour;
		wxImage * m_bgImage;
		bool m_shadows;
		AntiAliasing m_antiAliasing;
		bool m_showDebug;
		bool m_showFPS;
		wxString m_startupScript;

		TrollScene * m_mainScene;
		SceneMap m_scenes;
		bool m_saved;
		bool m_modified;
		wxString m_projectFileName;
		wxSize m_resolution;

	public:
		Project();
		Project( const wxString & p_projectName, const wxString & p_mainSceneName, const wxString & p_path,
				 BackgroundType p_backgroundType, const wxString & p_background, bool p_shadows,
				 AntiAliasing p_aa, const wxSize & p_resolution );
		~Project();

		void FlushObjects();

		TrollScene * GetScene( const wxString & p_sceneName );
		TrollScene * GetScene( const wxTreeItemId & p_item );
		void RemoveScene( const wxString & p_sceneName );
		void RemoveScene( TrollScene * p_scene );
		TrollScene * CreateScene( wxString const & p_strName );
		void AddScene( TrollScene * p_scene );
		void Save( wxTextOutputStream * p_stream );
		void Load( const wxString & p_path, GUI::FilesTree * p_tree );
		void Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, const wxString & p_path, Troll::GUI::FilesTree * p_tree = NULL );
		void Write();

		void SetBackgroundImage( const wxString & p_img );
		void SetBackgroundColour( const wxString & p_colour );

		bool FindFileInScenes( const wxString & p_fileName, TrollFile *& p_file, TrollScene *& p_scene );

	private:
		void _fillSceneDependencies( TrollScene * p_scene );
		void _buildColour( const wxString & p_infos );

	public:
		inline TrollScene 	*	GetMainScene()const
		{
			return m_mainScene;
		}
		inline SceneMap			GetScenes()const
		{
			return m_scenes;
		}
		inline const wxString &	GetPath()const
		{
			return m_projectPath;
		}
		inline const wxString &	GetBackground()const
		{
			return m_bgString;
		}
		inline BackgroundType	GetBackgroundType()const
		{
			return m_bgType;
		}
		inline wxColour 	*	GetBackgroundColour()const
		{
			return m_bgColour;
		}
		inline wxImage 	*	GetBackgroundImage()const
		{
			return m_bgImage;
		}
		inline const wxString &	GetName()const
		{
			return m_name;
		}
		inline bool				IsSaved()const
		{
			return m_saved;
		}
		inline bool				IsModified()const
		{
			return m_modified;
		}
		inline const wxString &	GetProjectFileName()const
		{
			return m_projectFileName;
		}
		inline AntiAliasing		GetFSAA()const
		{
			return m_antiAliasing;
		}
		inline bool				GetShadows()const
		{
			return m_shadows;
		}
		inline const wxSize &	GetResolution()const
		{
			return m_resolution;
		}
		inline const wxString &	GetStartupScript()const
		{
			return m_startupScript;
		}
		inline bool				GetShowDebug()const
		{
			return m_showDebug;
		}
		inline bool				GetShowFPS()const
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
		inline void	SetBackgroundColour( wxColour * p_colour )
		{
			m_bgColour = p_colour;
		}
		inline void	SetBackgroundColour( wxColour const & p_colour )
		{
			*m_bgColour = p_colour;
		}
		inline void SetBackgroundImage( wxImage * p_image )
		{
			m_bgImage = p_image;
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
	};
}

#endif

