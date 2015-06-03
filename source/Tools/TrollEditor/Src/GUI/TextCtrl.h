#ifndef ___MYTEXTCTRL_H___
#define ___MYTEXTCTRL_H___

#include <wx/textctrl.h>

#include "Module_GUI.h"
#include "Project/Module_Project.h"

namespace Troll
{
	namespace GUI
	{
		class TextCtrl
			: public wxTextCtrl
		{
		public:
			long m_posDeb;
			long m_posFin;
			wxPoint m_point;
			TrollFile * m_file;
			int m_linesNumber;
			bool m_scrollByWheel;
			float m_difference;
			int m_precPosition;
			TextLinesCtrl * m_lines;
			TextPanel * m_panel;
			int m_deltaLines;
			int m_deltaPages;
			bool m_modified;

			TextCtrl( wxWindow * parent, wxWindowID id,
					  const wxString & value, const wxPoint & pos,
					  const wxSize & size, long style, TrollFile * p_file );
			~TextCtrl();

			void AutoWord();
			virtual void SaveFile( const wxString & p_path, int p_fileType = 0 );
			void GoToLine( int p_line );
			void CompleteCode( int p_keyCode );

		private:
			void _onKeyDown( wxKeyEvent & p_event );
			void _onKeyUp( wxKeyEvent & p_event );
			void _onMouseWheel( wxMouseEvent & p_event );
			void _onScrollToTop( wxScrollWinEvent & p_event );
			void _onScrollToBottom( wxScrollWinEvent & p_event );
			void _onScrollLineUp( wxScrollWinEvent & p_event );
			void _onScrollLineDown( wxScrollWinEvent & p_event );
			void _onScrollPageUp( wxScrollWinEvent & p_event );
			void _onScrollPageDown( wxScrollWinEvent & p_event );
			void _onScrollThumbTrack( wxScrollWinEvent & p_event );
			void _onScrollThumbRelease( wxScrollWinEvent & p_event );


		public:
			inline TrollFile *	GetFile()const
			{
				return m_file;
			}
			inline bool		IsModified()const
			{
				return m_modified;
			}

			inline void SetLinesNumber( unsigned int p_number )
			{
				m_linesNumber = p_number;
			}
			inline void SetLines( TextLinesCtrl * p_lines )
			{
				m_lines = p_lines;
			}

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
#endif
