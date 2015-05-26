//***********************************************************************************************************
#ifndef _TEScrollablePanel_H
#define _TEScrollablePanel_H
//***********************************************************************************************************
#include <wx/panel.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class TEScrollablePanel : public wxPanel
		{
		public:
			TEScrollablePanel( wxWindow * p_parent, wxWindowID p_id, wxPoint p_position, wxSize p_size );
			~TEScrollablePanel();

		private:
			void _onScroll( wxScrollEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************