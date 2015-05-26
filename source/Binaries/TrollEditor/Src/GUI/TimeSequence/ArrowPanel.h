//***********************************************************************************************************
#ifndef ___Troll_ArrowPanel___
#define ___Troll_ArrowPanel___
//***********************************************************************************************************
#include "../Module_GUI.h"
#include <wx/panel.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class ArrowPanel : public wxPanel
		{
		private:
			bool m_mouseLeftDown;
			wxPanel * m_boundPanel;

		public:
			ArrowPanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
						const wxSize & p_size = wxDefaultSize );
			~ArrowPanel();

		public:
			inline void SetBoundPanel( wxPanel * p_panel )
			{
				m_boundPanel = p_panel;
			}
			inline void SetMouseLeftDown( bool p_leftDown )
			{
				m_mouseLeftDown = p_leftDown;
			}

		private:
			void _onPaint( wxPaintEvent & p_event );
			void _onMouseLeftDown( wxMouseEvent & p_event );
			void _onMouseLeftUp( wxMouseEvent & p_event );
			void _onMouseMove( wxMouseEvent & p_event );
			void _onMouseLeave( wxMouseEvent & p_event );
			void _onKillFocus( wxFocusEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************