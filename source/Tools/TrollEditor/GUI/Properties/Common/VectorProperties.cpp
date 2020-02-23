#include "VectorProperties.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

WX_PG_IMPLEMENT_VARIANT_DATA_EXPORTED_DUMMY_EQ( Vector2, wxEMPTY_PARAMETER_VALUE )
WX_PG_IMPLEMENT_VARIANT_DATA_EXPORTED_DUMMY_EQ( Vector3, wxEMPTY_PARAMETER_VALUE )
WX_PG_IMPLEMENT_VARIANT_DATA_EXPORTED_DUMMY_EQ( Vector4, wxEMPTY_PARAMETER_VALUE )

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxPoint2DProperty, wxPGProperty, Vector2, Vector2 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxSize2DProperty, wxPGProperty, Vector2, Vector2 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxPoint3DProperty, wxPGProperty, Vector3, Vector3 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxSize3DProperty, wxPGProperty, Vector3, Vector3 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxSpotlightProperty, wxPGProperty, Vector3, Vector3 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxPoint4DProperty, wxPGProperty, Vector4, Vector4 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxAttenuationProperty, wxPGProperty, Vector4, Vector4 const &, TextCtrl )
			TE_PG_IMPLEMENT_PROPERTY_CLASS( wxRectangleProperty, wxPGProperty, Vector4, Vector4 const &, TextCtrl )
		}
	}
}
