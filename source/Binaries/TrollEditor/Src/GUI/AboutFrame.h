//***********************************************************************************************************
#ifndef ___AboutFrame___
#define ___AboutFrame___
//***********************************************************************************************************
#include <wx/frame.h>
#include "Module_GUI.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class AboutFrame : public wxFrame
		{
		protected:
			wxFrame * m_easterEggFrame;
			wxButton * m_back;
			wxString m_word;

		public:
			AboutFrame( wxWindow * parent );

		private:
			void _onKeyUp( wxKeyEvent & p_event );
			void _onBack( wxCommandEvent & p_event );
			void _onCloseKeumar( wxCommandEvent & p_event );
			void _onShowKeumar( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
