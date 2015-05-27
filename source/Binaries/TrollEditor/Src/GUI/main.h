
#ifndef ___Troll_Application___
#define ___Troll_Application___

#include <wx/app.h>
#include "Module_GUI.h"

namespace Troll
{
	namespace GUI
	{
		class TEApplication : public wxApp
		{
		public:
			MainFrame * m_mainFrame;

		public:
			TEApplication();
			~TEApplication();

			virtual bool OnInit();
		};
	}
}

#endif

