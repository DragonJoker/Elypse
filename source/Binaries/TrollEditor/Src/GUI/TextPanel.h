//***********************************************************************************************************
#ifndef ___Troll_TextPanel___
#define ___Troll_TextPanel___
//***********************************************************************************************************
#include <wx/scrolwin.h>
#include "Module_GUI.h"
#include  "../Project/Module_Project.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		struct SearchInfo
		{
			int m_line;
			int m_position;
			int m_index;

			SearchInfo()
				:	m_line( 0 ),
					m_position( 0 ),
					m_index( 0 )
			{}

			SearchInfo( int p_line, int p_position, int p_index )
				:	m_line( p_line ),
					m_position( p_position ),
					m_index( p_index )
			{}
		};

		class TextPanel : public wxScrolledWindow
		{
		private:
			wxPanel * m_container;
			TextCtrl * m_text;
			TextLinesCtrl * m_lines;
			bool m_showLines;
			TrollFile * m_file;

			wxString m_search;
			std::vector <SearchInfo *> m_searchPositions;
			size_t m_currentSearchPosition;

		public:
			TextPanel( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size,
					   long style, TrollFile * p_file, bool p_showLines );

			void WriteText( const wxString & p_text );
			void SaveFile( const wxString & p_path );
			void UpdateSize();
			void UpdateScrollBars();
			bool IsModified();
			void GoToLine( int p_line );
			bool Search( const wxString & p_search );
			bool SearchNext();
			bool SearchPrevious();
			void Replace( const wxString & p_replacement );

		public:
			inline TextCtrl *	GetText()const
			{
				return m_text;
			}
			inline TrollFile 	*	GetFile()const
			{
				return m_file;
			}

		private:
			void _onResize( wxSizeEvent & p_event );
			void _onSetFocus( wxFocusEvent & p_event );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
