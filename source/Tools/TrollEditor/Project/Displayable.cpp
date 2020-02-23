#include "Displayable.h"

#include "GUI/Properties/Common/ObjectProperty.h"
#include "GUI/Properties/Common/PropertiesHolder.h"

namespace Troll
{
	namespace ProjectComponents
	{
		Displayable::Displayable()
		{
		}

		Displayable::~Displayable()
		{
			if ( m_properties )
			{
				delete m_properties;
			}
		}

		GUI::Properties::ObjectProperty * Displayable::CreateProperties( wxPropertyGrid * p_grid )
		{
			HideProperties();
			m_properties = DoCreateProperties();
			m_properties->CreateProperties( GUI::Properties::PropertiesHolder::GetButtonEditor(), p_grid );
			return m_properties;
		}

		void Displayable::HideProperties()
		{
			if ( m_properties )
			{
				delete m_properties;
				m_properties = nullptr;
			}
		}
	}
}
