/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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
#ifndef ___Troll_ProjectFileParser___
#define ___Troll_ProjectFileParser___

#include "GUI/FileParser.hpp"

#include <map>

namespace Troll
{
	namespace GUI
	{
		class FilesTree;
	}

	class TrollScene;
	class Project;
	/*!
	\author Sylvain DOREMUS
	\version 0.6.1.0
	\date 19/10/2011
	\~english
	\brief Project file sections enumeration
	\~french
	\brief Enumération des sections de fichier de projet
	*/
	typedef enum ePROJECT_SECTION
	{
		ePROJECT_SECTION_ROOT
		,	ePROJECT_SECTION_PROJECT
		,	ePROJECT_SECTION_SCENE
		,	ePROJECT_SECTION_SCENEFILES
		,	ePROJECT_SECTION_LOADSCRIPTFILES
		,	ePROJECT_SECTION_UNLOADSCRIPTFILES
		,	ePROJECT_SECTION_3DDATAFILES
		,	ePROJECT_SECTION_OTHERDATAFILES
		,	ePROJECT_SECTION_DEPENDENCIES
		,	ePROJECT_SECTION_COUNT
	}	ePROJECT_SECTION;
	/*!
	\author Sylvain DOREMUS
	\version 0.6.1.0
	\date 19/10/2011
	\~english
	\brief File parsing context for project files
	\~french
	\brief Contexte d'analyse pour les fichiers de projet
	*/
	class ProjectFileContext
		: public GUI::FileParserContext
	{
	public:
		Project 	*		pProject;
		TrollScene 	*	pScene;
		wxString			strName;
		wxArrayString		arrayStrings;
		GUI::FilesTree *	pFilesTree;

	public:
		ProjectFileContext( FILE * p_pFile );
	};
	//!\~english Typedef over a pointer to a ProjectFileContext	\~french Typedef d'un pointeur sur un ProjectFileContext
	typedef std::shared_ptr< ProjectFileContext > ProjectFileContextPtr;
	/*!
	\author Sylvain DOREMUS
	\version 0.6.1.0
	\date 19/10/2011
	\~english
	\brief Project file parser
	\~french
	\brief Analyseur de fichiers de projet
	*/
	class ProjectFileParser
		: public GUI::FileParser
	{
	private:
		Project 	*		m_pProject;
		GUI::FilesTree *	m_pFilesTree;

	public:
		/**@name Construction / Destruction */
		//@{
		ProjectFileParser( Project * p_pProject, GUI::FilesTree * p_tree );
		virtual ~ProjectFileParser();
		//@}

	private:
		virtual void DoInitialiseParser( FILE * p_file );
		virtual void DoCleanupParser();
		virtual void DoDiscardParser( wxString const & p_strLine );
		virtual bool DoDelegateParser( wxString const & p_strLine )
		{
			return false;
		}
		virtual void DoValidate();
	};

	DECLARE_ATTRIBUTE_PARSER( Root_Project	)
	DECLARE_ATTRIBUTE_PARSER( Project_Resolution	)
	DECLARE_ATTRIBUTE_PARSER( Project_BgColour	)
	DECLARE_ATTRIBUTE_PARSER( Project_BgImage	)
	DECLARE_ATTRIBUTE_PARSER( Project_Shadows	)
	DECLARE_ATTRIBUTE_PARSER( Project_AntiAliasing	)
	DECLARE_ATTRIBUTE_PARSER( Project_StartupScript	)
	DECLARE_ATTRIBUTE_PARSER( Project_ShowDebug	)
	DECLARE_ATTRIBUTE_PARSER( Project_ShowFPS	)
	DECLARE_ATTRIBUTE_PARSER( Project_Scene	)
	DECLARE_ATTRIBUTE_PARSER( Project_End	)
	DECLARE_ATTRIBUTE_PARSER( Scene_SceneFiles	)
	DECLARE_ATTRIBUTE_PARSER( Scene_LoadScriptFiles	)
	DECLARE_ATTRIBUTE_PARSER( Scene_UnloadScriptFiles	)
	DECLARE_ATTRIBUTE_PARSER( Scene_3DDataFiles	)
	DECLARE_ATTRIBUTE_PARSER( Scene_OtherDataFiles	)
	DECLARE_ATTRIBUTE_PARSER( Scene_Dependencies	)
	DECLARE_ATTRIBUTE_PARSER( Scene_End	)
	DECLARE_ATTRIBUTE_PARSER( SceneFiles_End	)
	DECLARE_ATTRIBUTE_PARSER( LoadScriptFiles_End	)
	DECLARE_ATTRIBUTE_PARSER( UnloadScriptFiles_End	)
	DECLARE_ATTRIBUTE_PARSER( DataFiles3D_End	)
	DECLARE_ATTRIBUTE_PARSER( OtherDataFiles_End	)
	DECLARE_ATTRIBUTE_PARSER( Dependecies_End	)
}

#endif
