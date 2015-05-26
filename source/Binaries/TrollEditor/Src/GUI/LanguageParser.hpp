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
#ifndef ___GC_LanguageParser___
#define ___GC_LanguageParser___

#include "StcContext.hpp"
#include "FileParser.hpp"

#include <map>

namespace Troll
{
	namespace GUI
	{
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Language file sections enumeration
		\~french
		\brief Enum�ration des sections de fichier de langage
		*/
		typedef enum eSECTION
		{
			eSECTION_ROOT
			,	eSECTION_LANGUAGE
			,	eSECTION_SECTION
			,	eSECTION_STYLE
			,	eSECTION_LIST
			,	eSECTION_COUNT
		}	eSECTION;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief File parsing context for language files
		\~french
		\brief Contexte d'analyse pour les fichiers de langage
		*/
		class LanguageFileContext : public FileParserContext
		{
		public:
			LanguageInfoPtr						pCurrentLanguage;
			StyleInfoPtr						pCurrentStyle;
			std::map<wxString, int>				mapFoldFlags;
			std::map<wxString, eSTC_TYPE>		mapTypes;
			std::map<wxString, eSTC_LEX>		mapLexers;
			wxString							strName;
			eSTC_TYPE							eStyle;
			eSTC_TYPE							eType;
			wxArrayString						arrayWords;

		public:
			LanguageFileContext( FILE * p_pFile );
		};
		//!\~english Typedef over a pointer to a LanguageFileContext	\~french Typedef d'un pointeur sur un LanguageFileContext
		typedef std::shared_ptr< LanguageFileContext > LanguageFileContextPtr;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Language file parser
		\~french
		\brief Analyseur de fichiers de langage
		*/
		class LanguageFileParser : public FileParser
		{
		private:
			StcContext * m_pStcContext;

		public:
			/**@name Construction / Destruction */
			//@{
			LanguageFileParser( StcContext * p_pStcContext );
			virtual ~LanguageFileParser();
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

		DECLARE_ATTRIBUTE_PARSER( Root_Language	)
		DECLARE_ATTRIBUTE_PARSER( Language_Pattern	)
		DECLARE_ATTRIBUTE_PARSER( Language_Lexer	)
		DECLARE_ATTRIBUTE_PARSER( Language_FoldFlags	)
		DECLARE_ATTRIBUTE_PARSER( Language_Section	)
		DECLARE_ATTRIBUTE_PARSER( Language_Style	)
		DECLARE_ATTRIBUTE_PARSER( Language_End	)
		DECLARE_ATTRIBUTE_PARSER( Style_Type	)
		DECLARE_ATTRIBUTE_PARSER( Style_FgColour	)
		DECLARE_ATTRIBUTE_PARSER( Style_BgColour	)
		DECLARE_ATTRIBUTE_PARSER( Style_FontName	)
		DECLARE_ATTRIBUTE_PARSER( Style_FontStyle	)
		DECLARE_ATTRIBUTE_PARSER( Style_FontSize	)
		DECLARE_ATTRIBUTE_PARSER( Style_End	)
		DECLARE_ATTRIBUTE_PARSER( Section_Type	)
		DECLARE_ATTRIBUTE_PARSER( Section_List	)
		DECLARE_ATTRIBUTE_PARSER( Section_Style	)
		DECLARE_ATTRIBUTE_PARSER( Section_End	)
		DECLARE_ATTRIBUTE_PARSER( List_End	)
	}
}

#endif
