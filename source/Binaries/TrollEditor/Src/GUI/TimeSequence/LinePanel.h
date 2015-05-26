//***********************************************************************************************************
#ifndef ___Troll_LinePanel___
#define ___Troll_LinePanel___
//***********************************************************************************************************
#include "../Module_GUI.h"
#include "../../Project/Temporal/Module_Temporal.h"
#include <wx/panel.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class LinePanel : public wxPanel
		{
		private:
			Temporal::TrollSequence * m_sequence;
			int m_currentLeft;

		public:
			LinePanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
					   const wxSize & p_size = wxDefaultSize );
			~LinePanel();

			void Replace();

			void SetSequence( Temporal::TrollSequence * p_sequence );
			void AddPonctualEvent( Temporal::TrollPonctualEvent * p_event, Real p_time );
			void AddContinuousEvent( Temporal::TrollContinuousEvent * p_event );

		public:
			inline Temporal::TrollSequence * GetSequence()const
			{
				return m_sequence;
			}

		private:
			void _showContextMenu( const wxPoint & p_pos );

			void _onAddPonctualEvent( wxCommandEvent & p_event );
			void _onAddContinuousEvent( wxCommandEvent & p_event );
			void _onLeftMouseUp( wxMouseEvent & p_event );
			void _onRightMouseUp( wxMouseEvent & p_event );
			void _onMouseEnter( wxMouseEvent & p_event );
			void _onMouseLeave( wxMouseEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************