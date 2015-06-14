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
#ifndef ___Castor_FileParser___
#define ___Castor_FileParser___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <map>
#include <stack>

#define DECLARE_SHARED_PTR( class_name )	typedef std::shared_ptr<	class_name			>	class_name##Ptr;\
											typedef std::weak_ptr<		class_name			>	class_name##WPtr;

#define DECLARE_MAP( key, value, name )		typedef std::map<			key,		value	>	name##Map;\
											typedef name##Map::iterator							name##MapIt;\
											typedef name##Map::reverse_iterator					name##MapRIt;\
											typedef name##Map::const_iterator					name##MapConstIt;\
											typedef name##Map::const_reverse_iterator			name##MapConstRIt;

#define DECLARE_SET( key, name )			typedef std::set<			key					>	name##Set;\
											typedef name##Set::iterator							name##SetIt;\
											typedef name##Set::reverse_iterator					name##SetRIt;\
											typedef name##Set::const_iterator					name##SetConstIt;\
											typedef name##Set::const_reverse_iterator			name##SetConstRIt;

#define DECLARE_VECTOR( key, name )			typedef std::vector<		key					>	name##Array;\
											typedef name##Array::iterator						name##ArrayIt;\
											typedef name##Array::reverse_iterator				name##ArrayRIt;\
											typedef name##Array::const_iterator					name##ArrayConstIt;\
											typedef name##Array::const_reverse_iterator			name##ArrayConstRIt;

#define DECLARE_ARRAY( key, count, name )	typedef std::array<			key,		count	>	name##Array;\
											typedef name##Array::iterator						name##ArrayIt;\
											typedef name##Array::reverse_iterator				name##ArrayRIt;\
											typedef name##Array::const_iterator					name##ArrayConstIt;\
											typedef name##Array::const_reverse_iterator			name##ArrayConstRIt;

#define DECLARE_LIST( key, name )			typedef std::list<			key					>	name##List;\
											typedef name##List::iterator						name##ListIt;\
											typedef name##List::reverse_iterator				name##ListRIt;\
											typedef name##List::const_iterator					name##ListConstIt;\
											typedef name##List::const_reverse_iterator			name##ListConstRIt;

#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4503 )
#endif

BEGIN_TROLL_GUI_NAMESPACE
{
	static inline wxArrayString split( wxString const & p_str, wxString const & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true )
	{
		wxArrayString l_arrayReturn;
		std::size_t l_numSplits = 0;
		std::size_t l_start = 0;
		std::size_t l_pos = 0;

		if ( ! p_str.empty() && ! p_delims.empty() && p_maxSplits > 0 )
		{
			l_arrayReturn.reserve( p_maxSplits + 1 );

			do
			{
				l_pos = p_str.find_first_of( p_delims, l_start );

				if ( l_pos == l_start )
				{
					l_start = l_pos + 1;

					if ( p_bKeepVoid )
					{
						l_arrayReturn.push_back( wxT( "" ) );
					}
				}
				else if ( l_pos == wxString::npos || l_numSplits == p_maxSplits )
				{
					l_arrayReturn.push_back( p_str.substr( l_start ) );
					return l_arrayReturn;
				}
				else
				{
					l_arrayReturn.push_back( p_str.substr( l_start, l_pos - l_start ) );
					l_start = l_pos + 1;
				}

				l_start = p_str.find_first_not_of( p_delims, l_start );
				++ l_numSplits;
			}
			while ( l_pos != wxString::npos );
		}

		return l_arrayReturn;
	}

	static wxString & trim( wxString & p_str, bool p_bLeft = true, bool p_bRight = true )
	{
		if ( p_str.size() > 0 )
		{
			std::size_t l_uiIndex;

			if ( p_bLeft )
			{
				l_uiIndex = p_str.find_first_not_of( wxT( ' ' ) );

				if ( l_uiIndex > 0 )
				{
					if ( l_uiIndex != wxString::npos )
					{
						p_str = p_str.substr( l_uiIndex, wxString::npos );
					}
					else
					{
						p_str.clear();
					}
				}

				l_uiIndex = p_str.find_first_not_of( wxT( '\t' ) );

				if ( l_uiIndex > 0 )
				{
					if ( l_uiIndex != wxString::npos )
					{
						p_str = p_str.substr( l_uiIndex, wxString::npos );
					}
					else
					{
						p_str.clear();
					}
				}
			}

			if ( p_bRight && p_str.size() > 0 )
			{
				l_uiIndex = p_str.find( wxT( '\n' ) );

				if ( l_uiIndex == p_str.size() - 1 )
				{
					p_str = p_str.substr( 0, p_str.size() - 1 );
				}

				l_uiIndex = p_str.find_last_not_of( wxT( ' ' ) );

				if ( l_uiIndex < p_str.size() - 1 )
				{
					if ( l_uiIndex != wxString::npos )
					{
						p_str = p_str.substr( 0, l_uiIndex + 1 );
					}
					else
					{
						p_str.clear();
					}
				}

				if ( p_str.size() > 0 )
				{
					l_uiIndex = p_str.find_last_not_of( wxT( '\t' ) );

					if ( l_uiIndex != wxString::npos )
					{
						p_str = p_str.substr( 0, l_uiIndex + 1 );
					}
					else
					{
						p_str.clear();
					}
				}
			}
		}

		return p_str;
	}

	class FileParser;
	/*!
	\author		Sylvain DOREMUS
	\version	0.6.1.0
	\date		19/10/2011
	\~english
	\brief		The context used into file parsing functions
	\remark		While parsing a "brace file", the context holds the important data retrieved
	\~french
	\brief		Contexte utilisé dans les fonctions d'analyse
	\remark		Lorsqu'on analyse un fichier, le contexte contient les informations importantes qui ont été récupérées.
	*/
	class FileParserContext
	{
	public:
		/**
			*\~english
			*\brief		Constructor
			*\param[in]	p_pFile	The file in parse
			*\~french
			*\brief		Constructeur
			*\param[in]	p_pFile	Le fichier en cours de traitement
			*/
		FileParserContext( FILE * p_pFile );
		/**
			*\~english
			*\brief		Destructor
			*\~french
			*\brief		Destructeur
			*/
		virtual ~FileParserContext();

	public:
		//!\~english The file currently parsed	\~french Le fichier en cours d'analyse
		FILE * pFile;
		//!\~english The current line	\~french La ligne en cours d'analyse
		unsigned long long ui64Line;
		//!\~english The sections stack	\~french La pile de sections
		std::stack< int > stackSections;
		//!\~english The current function name	\~french Le nom de la fonction actuelle
		wxString strFunctionName;
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		04/02/2013
	\~english
	\brief		Perser function parameter types enumeration
	\~french
	\brief		Enumération des types de paramètres pour une fonction d'analyse
	*/
	typedef enum ePARAMETER_TYPE
	{
		ePARAMETER_TYPE_TEXT,
		ePARAMETER_TYPE_BOOL,
		ePARAMETER_TYPE_INT8,
		ePARAMETER_TYPE_INT16,
		ePARAMETER_TYPE_INT32,
		ePARAMETER_TYPE_INT64,
		ePARAMETER_TYPE_UINT8,
		ePARAMETER_TYPE_UINT16,
		ePARAMETER_TYPE_UINT32,
		ePARAMETER_TYPE_UINT64,
		ePARAMETER_TYPE_FLOAT,
		ePARAMETER_TYPE_DOUBLE,
		ePARAMETER_TYPE_LONGDOUBLE,
		ePARAMETER_TYPE_REAL,
		ePARAMETER_TYPE_PIXELFORMAT,
		ePARAMETER_TYPE_POINT2I,
		ePARAMETER_TYPE_POINT3I,
		ePARAMETER_TYPE_POINT4I,
		ePARAMETER_TYPE_POINT2F,
		ePARAMETER_TYPE_POINT3F,
		ePARAMETER_TYPE_POINT4F,
		ePARAMETER_TYPE_POINT2D,
		ePARAMETER_TYPE_POINT3D,
		ePARAMETER_TYPE_POINT4D,
		ePARAMETER_TYPE_SIZE,
		ePARAMETER_TYPE_COLOUR,
		ePARAMETER_TYPE_COUNT,
	}	ePARAMETER_TYPE;
	//!\~english Typedef over a dynamic array of ePARAMETER_TYPE	\~french Typedef sur un tableau dynamique de ePARAMETER_TYPE
	DECLARE_VECTOR( ePARAMETER_TYPE, ParameterType );
	//!\~english Typedef over a pointer to a FileParserContext	\~french Typedef d'un pointeur sur un FileParserContextPtr
	DECLARE_SHARED_PTR( FileParserContext );
	/**
		*\~english
		*\brief		Parser function definition
		*\param[in]	p_strParams		The params contained in the line
		*\param[in]	p_pContext		The parsing context
		*\return		\p true if a brace is to be opened on next line
		*\~french
		*\brief		Définition d'une fonction d'analyse
		*\param[in]	p_strParams		Les paramètres contenus dans la ligne
		*\param[in]	p_pContext		Le contexte d'analyse
		*\return		\p true si une accolade doit être ouverte à la ligne suivante
		*/
	typedef bool ( ParserFunction )( GUI::FileParser * p_pParser, wxString & p_strParams, GUI::FileParserContextPtr p_pContext );
	//!\~english Pointer over a parser function	\~french Pointeur sur une fonction d'analyse
	typedef ParserFunction * PParserFunction;
	//!\~english Define to ease the declaration of a parser	\~french Un define pour faciliter la déclaration d'un analyseur
#	define DECLARE_ATTRIBUTE_PARSER( X ) bool X( GUI::FileParser * p_pParser, wxString & p_strParams, GUI::FileParserContextPtr p_pContext );
	//!\~english Define to ease the implementation of a parser	\~french Un define pour faciliter l'implémentation d'un analyseur
#	define IMPLEMENT_ATTRIBUTE_PARSER( X )	bool X( GUI::FileParser * p_pParser, wxString & p_strParams, GUI::FileParserContextPtr p_pContext )\
												{\
													bool l_bReturn = false;\
													if( !p_pParser->IsIgnored() )
	//!\~english Define to ease the implementation of a parser	\~french Un define pour faciliter l'implémentation d'un analyseur
#	define ATTRIBUTE_END_PUSH( x ) }l_bReturn = true;p_pContext->stackSections.push( x );return l_bReturn
	//!\~english Define to ease the implementation of a parser	\~french Un define pour faciliter l'implémentation d'un analyseur
#	define ATTRIBUTE_END() }l_bReturn = false;return l_bReturn
	//!\~english Define to ease the implementation of a parser	\~french Un define pour faciliter l'implémentation d'un analyseur
#	define ATTRIBUTE_END_POP() }l_bReturn = false;p_pContext->stackSections.pop();return l_bReturn
	//!\~english Define to ease the call to FileParser::ParseError	\~french Un define pour faciliter l'appel de FileParser::ParseError
#	define PARSE_ERROR( p_strError) GUI::FileParser::ParseError( (p_strError), (p_pContext) )
	//!\~english Define to ease the call to FileParser::ParseWarning	\~french Un define pour faciliter l'appel de FileParser::ParseWarning
#	define PARSE_WARNING( p_strWarning) GUI::FileParser::ParseWarning( (p_strWarning), (p_pContext) )
	/*!
	\author		Sylvain DOREMUS
	\version	0.6.1.0
	\date		19/10/2011
	\~english
	\brief		"Brace file" parser base class
	\~french
	\brief		Classe de base pour les analyseurs de fichier à accolades
	*/
	class FileParser
	{
	private:
		typedef std::pair< PParserFunction, ParameterTypeArray > ParserFunctionAndParams;
		DECLARE_MAP( wxString, ParserFunctionAndParams, AttributeParser );

	public:
		/**
			*\~english
			*\brief		Constructor
			*\param[in]	p_iRootSectionNumber	The root section id
			*\param[in]	p_iSectionCount			The sections count
			*\~french
			*\brief		Constructeur
			*\param[in]	p_iRootSectionNumber	L'id de la section de root
			*\param[in]	p_iSectionCount			Le nombre de sections
			*/
		FileParser( int p_iRootSectionNumber, int p_iSectionCount );
		/**
			*\~english
			*\brief		Destructor
			*\~french
			*\brief		Destructeur
			*/
		virtual ~FileParser();
		/**
			*\~english
			*\brief		Parsing function
			*\param[in]	p_strFileName	The file name
			*\return		\p true if OK
			*\~french
			*\brief		Fonction de traitement
			*\param[in]	p_strFileName	Le nom du fichier
			*\return		\p true si tout s'est bien passé
			*/
		bool ParseFile( wxString const & p_strFileName );
		/**
			*\~english
			*\brief		Parsing function
			*\param[in]	p_file	The file
			*\return		\p true if OK
			*\~french
			*\brief		Fonction de traitement
			*\param[in]	p_file	Le fichier
			*\return		\p true si tout s'est bien passé
			*/
		bool ParseFile( FILE * p_file );
		/**
			*\~english
			*\brief		Logs an error in the log file
			*\param[in]	p_strError	The error text
			*\param[in]	p_pContext	The current parsing context
			*\~french
			*\brief		Log une erreur dans le fichier de log
			*\param[in]	p_strError	Le texte de l'erreur
			*\param[in]	p_pContext	Le context d'analyse
			*/
		static void ParseError( wxString const & p_strError, FileParserContextPtr p_pContext );
		/**
			*\~english
			*\brief		Logs a warning in the log file
			*\param[in]	p_strWarning	The warning text
			*\param[in]	p_pContext		The current parsing context
			*\~french
			*\brief		Log un avertissement dans le fichier de log
			*\param[in]	p_strWarning	Le texte de l'avertissement
			*\param[in]	p_pContext		Le context d'analyse
			*/
		static void ParseWarning( wxString const & p_strWarning, FileParserContextPtr p_pContext );
		/**
			*\~english
			*\brief		Parses an integer from a line
			*\param[in]	p_strParams	The line containing the value
			*\param[out]	p_tResult	Receives the result
			*\~french
			*\brief		Extrait un entier à partir d'une ligne
			*\param[in]	p_strParams	La ligne contenant la valeur
			*\param[out]	p_tResult	Reçoit le résultat
			*/
		template< typename T >
		static bool ParseInteger( wxString & p_strParams, T & p_tResult )
		{
			bool l_bReturn = false;
			wxArrayString l_array = split( p_strParams, wxT( " \t,;" ), 1, false );

			if ( l_array.size() )
			{
				l_bReturn = l_array[0].IsNumber();
				long long int l_i64Tmp = 0;

				if ( l_bReturn )
				{
					l_array[0].ToLongLong( &l_i64Tmp );
					l_bReturn = ( l_i64Tmp > std::numeric_limits< T >::min() ) && ( l_i64Tmp < std::numeric_limits< T >::max() );
				}

				if ( l_bReturn )
				{
					p_tResult = static_cast< T >( l_i64Tmp );
				}

				p_strParams.clear();

				if ( l_array.size() > 1 )
				{
					p_strParams = l_array[1];
				}
			}

			return l_bReturn;
		}
		/**
			*\~english
			*\brief		Parses an floating number from a line
			*\param[in]	p_strParams	The line containing the value
			*\param[out]	p_tResult	Receives the result
			*\~french
			*\brief		Extrait un flottant à partir d'une ligne
			*\param[in]	p_strParams	La ligne contenant la valeur
			*\param[out]	p_tResult	Reçoit le résultat
			*/
		template< typename T >
		static bool ParseFloat( wxString & p_strParams, T & p_tResult )
		{
			bool l_bReturn = false;
			wxArrayString l_array = split( p_strParams, wxT( " \t,;" ), 1, false );

			if ( l_array.size() )
			{
				l_bReturn = l_array[0].IsNumber();
				double l_dTmp = 0;

				if ( l_bReturn )
				{
					l_array[0].ToDouble( &l_dTmp );
					l_bReturn = ( l_dTmp > std::numeric_limits< T >::min() ) && ( l_dTmp < std::numeric_limits< T >::max() );
				}

				if ( l_bReturn )
				{
					p_tResult = static_cast< T >( l_dTmp );
				}

				p_strParams.clear();

				if ( l_array.size() > 1 )
				{
					p_strParams = l_array[1];
				}
			}

			return l_bReturn;
		}
		/**
			*\~english
			*\brief		Tells if the read lines are to be ignored
			*\~french
			*\brief		Dit si les lignes suivantes doivent être ignorées
			*/
		bool IsIgnored()const
		{
			return m_bIgnored;
		}
		/**
			*\~english
			*\brief		Increments ignore level
			*\~french
			*\brief		Incrémente le niveau d'ignorés
			*/
		void Ignore()
		{
			m_bIgnored = true;
		}
		/**
			*\~english
			*\brief		Tests if the params given to the function will be appropriate
			*\param[in]	p_strParams		The given parameters
			*\param[in]	p_itBegin		An iterator to the first expected parameter
			*\param[in]	p_itEnd			An iterator to the end of the expected parameters
			*\param[in]	p_pContext		The current parsing context
			*\return		\p false if \p p_strParams doesn't contain all expected parameters types
			*\~french
			*\brief		Vérifie si les paramètres donnés à la fonction correspondent à ceux qu'elle attend
			*\param[in]	p_strParams		Les paramètres donnés
			*\param[in]	p_itBegin		Un itérateur sur le premier des paramètres attendus
			*\param[in]	p_itEnd			Un itérateur sur la fin des paramètres attendus
			*\param[in]	p_pContext		Le context d'analyse
			*\return		\p false si \p p_strParams ne contient pas tous les types de paramètres attendus par la fonction
			*/
		bool CheckParams( wxString const & p_strParams, ParameterTypeArrayConstIt p_itBegin, ParameterTypeArrayConstIt p_itEnd, FileParserContextPtr p_pContext );
		/**
			*\~english
			*\brief		Adds a parser function to the parsers list
			*\param[in]	p_iSection		The parser section
			*\param[in]	p_strName		The parser name
			*\param[in]	p_pfnFunction	The parser function
			*\param[in]	p_iCount		The expected parameters list count
			*\param[in]	...				The expected parameters list
			*\~french
			*\brief		Ajoute une foncction d'analyse à la liste
			*\param[in]	p_iSection		La section
			*\param[in]	p_strName		Le nom
			*\param[in]	p_pfnFunction	La fonction d'analyse
			*\param[in]	p_iCount		Le compte des paramètres attendus
			*\param[in]	...				La liste des paramètres attendus
			*/
		void AddParser( int p_iSection, wxString const & p_strName, PParserFunction p_pfnFunction, int p_iCount = 0, ... );

	protected:
		/**
			*\~english
			*\brief		Specific initialisation function
			*\~french
			*\brief		Initialisation spécifique
			*/
		virtual void DoInitialiseParser( FILE * p_file ) = 0;
		/**
			*\~english
			*\brief		Specific cleanup
			*\~french
			*\brief		Nettoyage spécifique
			*/
		virtual void DoCleanupParser() = 0;
		/**
			*\~english
			*\brief		Function called when \p m_iSection is out of bounds
			*\param[in]	p_strLine	The current line
			*\return		\p true if a brace is opened after this line, \false if not
			*\~french
			*\brief		Fonction appelée lorsque \p m_iSection est hors limites
			*\param[in]	p_strLine	La ligne en cours d'analyse
			*\return		\p true si une accolade doit être ouverte à la ligne suivant, \p false sinon
			*/
		virtual bool DoDelegateParser( wxString const & p_strLine ) = 0;
		/**
			*\~english
			*\brief		Function called when no parser is found for the line
			*\param[in]	p_strLine	The current line
			*\~french
			*\brief		Fonction appelée si aucun analyseur n'est trouvé pour traiter la ligne
			*\param[in]	p_strLine	La ligne en cours d'analyse
			*/
		virtual void DoDiscardParser( wxString const & p_strLine ) = 0;
		/**
			*\~english
			*\brief		Function called when file parsing is completed with no error
			*\~french
			*\brief		Fonction appelée si l'analyse est complétée sans erreurs
			*/
		virtual void DoValidate() = 0;

	private:
		bool DoParseScriptLine( wxString & p_strLine );
		bool DoInvokeParser( wxString & p_line, AttributeParserMap const & p_parsers );

	private:
		int m_iRootSectionNumber;
		int m_iSectionCount;
		int m_iIgnoreLevel;

	protected:
		//!\~english	The map holding the parsers, sorted by section	\~french	La map de parseurs, triés par section
		std::map< int, AttributeParserMap > m_mapParsers;
		//!\~english	Th parser context	\~french	Le contexte du parseur
		FileParserContextPtr m_pParsingContext;
		//!\~english	Tells the lines parsed are to be ignored	\~french	Dit que les ligne slues sont ignorées
		bool m_bIgnored;
	};
}
END_TROLL_GUI_NAMESPACE

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif

#endif
