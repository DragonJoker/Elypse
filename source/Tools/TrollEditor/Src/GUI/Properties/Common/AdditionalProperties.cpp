#include "AdditionalProperties.h"

#include "GradientButton.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			//************************************************************************************************

			ButtonData::ButtonData( ButtonEventMethod p_method, wxEvtHandler * p_handler )
				: m_method( p_method )
				, m_handler( p_handler )
			{
			}

			bool ButtonData::Call( wxPGProperty * property )
			{
				return ( *m_handler.*m_method )( property );
			}

			//************************************************************************************************

			wxPGWindowList ButtonEventEditor::CreateControls( wxPropertyGrid * p_propgrid, wxPGProperty * p_property, wxPoint const & p_pos, wxSize const & p_size )const
			{
				// create and return a single button to be used as editor
				// size and pos represent the entire value cell: use that to position the button
				return wxPGWindowList( new GradientButton( p_propgrid, wxID_ANY, _( "Edit" ), p_pos, p_size ) );
			}

			// since the editor does not need to change the primary control (the button)
			// to reflect changes, UpdateControl is just a no-op
			void ButtonEventEditor::UpdateControl( wxPGProperty * property, wxWindow * ctrl ) const
			{
			}

			bool ButtonEventEditor::OnEvent( wxPropertyGrid * p_propgrid, wxPGProperty * p_property, wxWindow * p_wnd_primary, wxEvent & p_event )const
			{
				// handle the button event
				if ( p_event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
				{
					// extract the client data from the property
					if ( ButtonData * btn = dynamic_cast< ButtonData * >( p_property->GetClientObject() ) )
					{
						// call the method
						return btn->Call( p_property );
					}
				}

				return false;
			}
			//************************************************************************************************

			wxPGProperty * CreateProperty( wxString const & p_name, wxPGChoices p_choices, wxString const & p_value )
			{
				auto l_return = new wxEnumProperty( p_name, wxPG_LABEL, p_choices );
				l_return->SetValue( p_value );
				return l_return;
			}

			wxPGProperty * CreateProperty( wxString const & p_name, wxString const & p_value, ButtonEventMethod p_method, wxEvtHandler * p_handler, wxPGEditor * p_editor )
			{
				auto l_return = new wxStringProperty( p_name, wxPG_LABEL, p_value );
				l_return->SetEditor( p_editor );
				l_return->SetClientObject( new ButtonData( p_method, p_handler ) );
				return l_return;
			}

			//************************************************************************************************
		}
	}
}
