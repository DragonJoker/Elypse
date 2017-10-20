/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_FRAME_H___
#define ___ELYPSE_FRAME_H___

#include "ElypseBinaryPrerequisites.h"

namespace ElypseBinary
{
	class ElypseFrame
		: public wxFrame
	{
	public:
		ElypseFrame( wxString const & p_title, wxSize const & p_size )
			: wxFrame( NULL, -1, p_title, wxPoint( 0, 0 ), p_size, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX )
		{
			Centre();
			Show();
			Update();
		}

		virtual ~ElypseFrame()
		{
		}
	};
}

#endif
