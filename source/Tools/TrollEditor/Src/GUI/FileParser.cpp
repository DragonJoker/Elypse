/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/FileParser.h"

#include <cstdarg>
#include <StringUtils.h>

namespace Troll
{
	namespace GUI
	{
		FileParserContext::FileParserContext( FILE * p_pFile )
			: ui64Line( 0 )
			, pFile( p_pFile )
		{
		}

		FileParserContext::~FileParserContext()
		{
			ui64Line = 0;
			pFile = NULL;
		}

		//*************************************************************************************************

		FileParser::FileParser( int p_iRootSectionNumber, int p_iSectionCount )
			: m_iRootSectionNumber( p_iRootSectionNumber )
			, m_iSectionCount( p_iSectionCount )
			, m_pParsingContext()
			, m_iIgnoreLevel( 0 )
			, m_bIgnored( false )
		{
		}

		FileParser::~FileParser()
		{
		}

		bool FileParser::ParseFile( wxString const & p_strFileName )
		{
			m_iIgnoreLevel = 0;
			m_bIgnored = false;
			FILE * l_file = fopen( make_string( p_strFileName ).c_str(), "r" );
			bool l_bReturn = false;

			if ( l_file )
			{
				l_bReturn = ParseFile( l_file );
				fclose( l_file );
			}

			return l_bReturn;
		}

		bool FileParser::ParseFile( FILE * p_file )
		{
			bool l_bReturn = false;
			char l_szLine[1024];

			if ( p_file && !feof( p_file ) )
			{
				bool l_bNextIsOpenBrace = false;
				bool l_bCommented = false;
				//Logger::LogMessage( wxT( "FileParser : Parsing file [" ) + p_file.GetFileName() + wxT( "]" ) );
				DoInitialiseParser( p_file );
				m_pParsingContext->stackSections.push( m_iRootSectionNumber );
				m_pParsingContext->ui64Line = 0;
				bool l_bReuse = false;
				wxString l_strLine;
				wxString l_strLine2;

				while ( p_file && !feof( p_file ) )
				{
					if ( !l_bReuse )
					{
						fgets( l_szLine, 1000, p_file );
						l_strLine = make_wxString( l_szLine );
						m_pParsingContext->ui64Line++;
					}
					else
					{
						l_bReuse = false;
					}

					//Logger::LogDebug( l_strLine );
					trim( l_strLine );

					if ( !l_strLine.empty() )
					{
						if ( l_strLine.size() < 2 || l_strLine.substr( 0, 2 ) != wxT( "//" ) )
						{
							if ( !l_bCommented )
							{
								if ( l_strLine.size() >= 2 && l_strLine.substr( 0, 2 ) == wxT( "/*" ) )
								{
									l_bCommented = true;
								}
								else
								{
									if ( l_strLine.find( wxT( "{" ) ) == l_strLine.size() - 1 && l_strLine.size() > 1 )
									{
										// We got a "{" at the end of the line, so we split the line in two and reuse the line
										l_strLine2 = l_strLine.substr( 0, l_strLine.size() - 1 );
										l_strLine2.Trim();
										l_bNextIsOpenBrace = DoParseScriptLine( l_strLine2 );
										l_strLine = wxT( "{" );
										l_bReuse = true;
									}
									else
									{
										if ( l_bNextIsOpenBrace )
										{
											if ( l_strLine != wxT( "{" ) )
											{
												l_strLine2 = wxT( "}" );
												l_bNextIsOpenBrace = DoParseScriptLine( l_strLine2 );
												l_bReuse = true;
											}
											else
											{
												l_bNextIsOpenBrace = false;

												if ( m_bIgnored )
												{
													m_iIgnoreLevel++;
												}
											}
										}
										else
										{
											l_bNextIsOpenBrace = DoParseScriptLine( l_strLine );
										}

										if ( m_iIgnoreLevel > 0 )
										{
											if ( l_strLine.find( wxT( "}" ) ) == l_strLine.size() - 1 )
											{
												m_iIgnoreLevel--;

												if ( m_iIgnoreLevel <= 0 )
												{
													m_bIgnored = false;
													m_iIgnoreLevel = 0;
												}
											}
										}
									}
								}
							}
							else if ( l_strLine.size() >= 2 && l_strLine.substr( 0, 2 ) == wxT( "*/" ) )
							{
								l_bCommented = false;
							}
						}
					}
				}

				if ( m_pParsingContext->stackSections.top() != m_iRootSectionNumber )
				{
					ParseError( wxT( "Unexpected end of file" ), m_pParsingContext );
				}
				else
				{
					DoValidate();
					l_bReturn = true;
				}

				//Logger::LogMessage( wxT( "FileParser : Finished parsing file [" ) + p_file.GetFileName() + wxT( "]" ) );
			}

			DoCleanupParser();
			return l_bReturn;
		}

		void FileParser::ParseError( wxString const & p_strError, FileParserContextPtr p_pContext )
		{
			wxString l_strError;
			l_strError << wxT( "Error Line #" ) << p_pContext->ui64Line << wxT( " / " ) << p_strError;
			//Logger::LogMessage( l_strError );
		}

		void FileParser::ParseWarning( wxString const & p_strWarning, FileParserContextPtr p_pContext )
		{
			wxString l_strError;
			l_strError << wxT( "Warning Line #" ) << p_pContext->ui64Line << wxT( " / " ) << p_strWarning;
			//Logger::LogMessage( l_strError );
		}

		bool FileParser::CheckParams( wxString const & p_strParams, ParameterTypeArrayConstIt p_itBegin, ParameterTypeArrayConstIt p_itEnd, FileParserContextPtr p_pContext )
		{
			bool l_bReturn = true;
			wxString l_strParams( p_strParams );
			trim( l_strParams );
			wxString l_strMissingParam;
			static wxString l_arrayStrParams[] =
			{
				wxT( "text" )
				, wxT( "boolean" )
				, wxT( "8 bits signed integer" )
				, wxT( "16 bits signed integer" )
				, wxT( "32 bits signed integer" )
				, wxT( "64 bits signed integer" )
				, wxT( "8 bits unsigned integer" )
				, wxT( "16 bits unsigned integer" )
				, wxT( "32 bits unsigned integer" )
				, wxT( "64 bits unsigned integer" )
				, wxT( "simple precision float" )
				, wxT( "double precision float" )
				, wxT( "long double precision float" )
#if OGRE_DOUBLE_PRECISION == 1
				, wxT( "double precision float" )
#else
				, wxT( "simple precision float" )
#endif
				, wxT( "pixelformat" )
				, wxT( "2 integers" )
				, wxT( "3 integers" )
				, wxT( "4 integers" )
				, wxT( "2 simple precision floats" )
				, wxT( "3 simple precision floats" )
				, wxT( "4 simple precision floats" )
				, wxT( "2 double precision floats" )
				, wxT( "3 double precision floats" )
				, wxT( "4 double precision floats" )
				, wxT( "size" )
				, wxT( "colour" )
				, wxT( "unknown parameter type" )
			};
			std::for_each( p_itBegin, p_itEnd, [&]( ePARAMETER_TYPE p_eType )
			{
				if ( l_bReturn )
				{
					switch ( p_eType )
					{
					case ePARAMETER_TYPE_TEXT:
					{
						wxArrayString l_array = split( l_strParams, wxT( " \t,;" ), 1 );
						l_strParams.clear();

						if ( l_array.size() )
						{
							l_bReturn = !l_array[0].empty();

							if ( l_array.size() > 1 )
							{
								l_strParams = l_array[1];
							}
						}
					}
					break;

					case ePARAMETER_TYPE_BOOL:
					{
						wxArrayString l_array = split( l_strParams, wxT( " \t,;" ), 1, false );
						l_strParams.clear();

						if ( l_array.size() )
						{
							l_bReturn = l_array[0] == wxT( "true" ) || l_array[0] == wxT( "false" );

							if ( l_array.size() > 1 )
							{
								l_strParams = l_array[1];
							}
						}
					}
					break;

					case ePARAMETER_TYPE_INT8:
					{
						int8_t l_i8;
						l_bReturn = ParseInteger( l_strParams, l_i8 );
					}
					break;

					case ePARAMETER_TYPE_INT16:
					{
						int16_t l_i16;
						l_bReturn = ParseInteger( l_strParams, l_i16 );
					}
					break;

					case ePARAMETER_TYPE_INT32:
					{
						int32_t l_i32;
						l_bReturn = ParseInteger( l_strParams, l_i32 );
					}
					break;

					case ePARAMETER_TYPE_INT64:
					{
						int64_t l_i64;
						l_bReturn = ParseInteger( l_strParams, l_i64 );
					}
					break;

					case ePARAMETER_TYPE_UINT8:
					{
						int8_t l_ui8;
						l_bReturn = ParseInteger( l_strParams, l_ui8 );
					}
					break;

					case ePARAMETER_TYPE_UINT16:
					{
						int16_t l_ui16;
						l_bReturn = ParseInteger( l_strParams, l_ui16 );
					}
					break;

					case ePARAMETER_TYPE_UINT32:
					{
						int32_t l_ui32;
						l_bReturn = ParseInteger( l_strParams, l_ui32 );
					}
					break;

					case ePARAMETER_TYPE_UINT64:
					{
						int64_t l_ui64;
						l_bReturn = ParseInteger( l_strParams, l_ui64 );
					}
					break;

					case ePARAMETER_TYPE_FLOAT:
					{
						float l_f;
						l_bReturn = ParseFloat( l_strParams, l_f );
					}
					break;

					case ePARAMETER_TYPE_DOUBLE:
					{
						double l_d;
						l_bReturn = ParseFloat( l_strParams, l_d );
					}
					break;

					default:
						p_eType = ePARAMETER_TYPE_COUNT;
						break;
					}

					if ( !l_bReturn )
					{
						l_strMissingParam = l_arrayStrParams[p_eType];
					}
				}
			} );

			if ( !l_bReturn )
			{
				PARSE_ERROR( wxT( "Directive <" ) + p_pContext->strFunctionName + wxT( "> needs a <" ) + l_strMissingParam + wxT( "> param that is currently missing" ) );
			}

			return l_bReturn;
		}

		void FileParser::AddParser( int p_iSection, wxString const & p_strName, PParserFunction p_pfnFunction, int p_iCount, ... )
		{
			ParameterTypeArray l_arrayParams;
			va_list l_valist;
			va_start( l_valist, p_iCount );

			for ( int i = 0; i < p_iCount; ++i )
			{
				l_arrayParams.push_back( ePARAMETER_TYPE( va_arg( l_valist, int ) ) );
			}

			va_end( l_valist );
			m_mapParsers[p_iSection][p_strName] = std::make_pair( p_pfnFunction, l_arrayParams );
		}

		bool FileParser::DoParseScriptLine( wxString & p_strLine )
		{
			if ( m_pParsingContext->stackSections.top() >= 0 && m_pParsingContext->stackSections.top() < m_iSectionCount )
			{
				return DoInvokeParser( p_strLine, m_mapParsers[m_pParsingContext->stackSections.top()] );
			}
			else
			{
				return DoDelegateParser( p_strLine );
			}
		}

		bool FileParser::DoInvokeParser( wxString & p_strLine, AttributeParserMap const & p_parsers )
		{
			bool l_bReturn = false;
			wxArrayString l_splitCmd = split( p_strLine, wxT( " \t" ), 1, false );

			if ( l_splitCmd.size() )
			{
				m_pParsingContext->strFunctionName = l_splitCmd[0];
				AttributeParserMap::const_iterator const & l_iter = p_parsers.find( l_splitCmd[0] );

				if ( l_iter == p_parsers.end() )
				{
					DoDiscardParser( p_strLine );
				}
				else
				{
					wxString l_strTmp;

					if ( l_splitCmd.size() >= 2 )
					{
						l_strTmp = l_splitCmd[1].Trim();
					}

					if ( !CheckParams( l_strTmp, l_iter->second.second.begin(), l_iter->second.second.end(), m_pParsingContext ) )
					{
						Ignore();
					}

					l_bReturn = l_iter->second.first( this, l_strTmp, m_pParsingContext );
				}
			}

			return l_bReturn;
		}
	}
}
