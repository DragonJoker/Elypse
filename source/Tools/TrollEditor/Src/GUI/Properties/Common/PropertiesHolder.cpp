#include "PropertiesHolder.h"

#include "AdditionalProperties.h"

#include <wx/propgrid/advprops.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			wxPGEditor * PropertiesHolder::m_buttonEditor = nullptr;

			PropertiesHolder::PropertiesHolder( wxWindow * p_parent, wxPoint const & p_ptPos, wxSize const & p_size )
				: wxPropertyGrid{ p_parent, wxID_ANY, p_ptPos, p_size, wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE }
			{
				if ( !m_buttonEditor )
				{
					m_buttonEditor = RegisterEditorClass( new ButtonEventEditor() );
				}

				Connect( wxEVT_PG_CHANGED, wxEVENT_HANDLER_CAST( wxPropertyGridEventFunction, PropertiesHolder::OnPropertyChange ) );
				SetBackgroundColour( GuiCommon::PanelBackgroundColour );
				SetForegroundColour( GuiCommon::PanelForegroundColour );
				SetCaptionBackgroundColour( GuiCommon::PanelBackgroundColour );
				SetCaptionTextColour( GuiCommon::PanelForegroundColour );
				SetSelectionBackgroundColour( GuiCommon::ActiveTabColour );
				SetSelectionTextColour( GuiCommon::ActiveTextColour );
				SetCellBackgroundColour( GuiCommon::InactiveTabColour );
				SetCellTextColour( GuiCommon::InactiveTextColour );
				SetLineColour( GuiCommon::BorderColour );
				SetMarginColour( GuiCommon::BorderColour );
			}

			PropertiesHolder::~PropertiesHolder()
			{
			}

			void PropertiesHolder::SetPropertyData( ObjectProperty * p_data )
			{
				if ( m_data )
				{
					wxPropertyGrid::Clear();
				}

				m_data = p_data;

				if ( m_data )
				{
					m_data->CreateProperties( m_buttonEditor, this );
				}
			}

			void PropertiesHolder::OnPropertyChange( wxPropertyGridEvent & p_event )
			{
				if ( m_data )
				{
					m_data->OnPropertyChange( p_event );
				}
			}
		}
	}
}
