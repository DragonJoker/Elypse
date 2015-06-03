#ifndef ___ELYPSE_FRAME_H___
#define ___ELYPSE_FRAME_H___

#include "Module_Binary.h"

#include "ElypseApp.h"

namespace EMuse
{
	namespace Binary
	{
		class ElypseFrame : public wxFrame
		{
		private:
			ElypseApp * m_app;

		public:
			ElypseFrame( ElypseApp * p_app, const wxString & p_title, const wxSize & p_size )
				:	wxFrame(	reinterpret_cast <wxFrame *>( NULL ), -1, p_title, wxPoint( 0, 0 ), p_size,
								wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX ),
				m_app( p_app )
			{
				Centre();
				Show();
				Update();
			}
			~ElypseFrame()
			{
			}

//		DECLARE_EVENT_TABLE()

			void OnClose( wxCloseEvent & p_event )
			{
//			p_event.Veto();
//			m_app->Close();
			}
		};
	}
}

#endif
