#include "ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			ObjectProperty::ObjectProperty( DataType p_type )
				: m_type{ p_type }
			{
			}

			ObjectProperty::~ObjectProperty()
			{
			}

			void ObjectProperty::CreateProperties( wxPGEditor * p_editor, wxPropertyGrid * p_grid )
			{
				m_grid = p_grid;
				DoPreCreateProperties( p_editor );
				DoCreateProperties( p_editor );
				DoPostCreateProperties( p_editor );
				m_grid = nullptr;
			}

			void ObjectProperty::OnPropertyChange( wxPropertyGridEvent & p_event )
			{
				auto l_property{ p_event.GetProperty() };

				if ( l_property )
				{
					auto l_it = m_actions.find( l_property->GetName() );

					if ( l_it != m_actions.end() )
					{
						l_it->second( p_event.GetProperty() );
					}
				}
			}

			void ObjectProperty::UpdateProperties()
			{
				for ( auto l_it : m_updates )
				{
					l_it.second( l_it.first );
				}
			}

			void ObjectProperty::DoAddCategory( wxString const & p_name )
			{
				m_grid->Append( new wxPropertyCategory{ p_name } );
			}
		}
	}
}
