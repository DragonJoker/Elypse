//***********************************************************************************************************
#ifndef ___ObjectProperties___
#define ___ObjectProperties___
//***********************************************************************************************************
#include <wx/panel.h>
#include "../Module_GUI.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class ObjectProperties : public wxPanel
		{
		protected:
			bool m_updatingProperties;

		public:
			ObjectProperties( wxWindow * parent, const wxPoint & pos = wxDefaultPosition,
							  const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
			~ObjectProperties();

			virtual void UpdateProperties() = 0;
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
