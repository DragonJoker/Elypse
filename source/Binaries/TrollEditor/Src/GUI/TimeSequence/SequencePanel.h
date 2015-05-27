
#ifndef ___Troll_SequencePanel___
#define ___Troll_SequencePanel___

#include "../Module_GUI.h"
#include <wx/panel.h>
#include "../../Project/Temporal/Module_Temporal.h"

namespace Troll
{
	namespace GUI
	{
		typedef std::multimap <int, int> IntIMultiMap;
		typedef std::map <int, IntIMultiMap> DimensionMap;

		class SequencePanel : public wxPanel
		{
		protected:
			LinePanel * m_sequencePanel;
			std::vector <PonctualEventPanel *> m_ponctualEvents;
			std::vector <ContinuousEventPanel *> m_continuousEvents;
			int m_currentLeft;
			Temporal::TrollSequence * m_sequence;
			DimensionMap m_addedPanelsDimensions;
			int m_maxTop;

		public:
			SequencePanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
						   const wxSize & p_size = wxDefaultSize );
			~SequencePanel();

			Temporal::TrollSequence * SetSequence( Temporal::TrollSequence * p_sequence );
			void AddPonctualEvent( Temporal::TrollPonctualEvent * p_event, Real p_time );
			void AddContinuousEvent( Temporal::TrollContinuousEvent * p_event );
			void RemovePonctualEvent( Temporal::TrollPonctualEvent * p_event );
			void RemoveContinuousEvent( Temporal::TrollContinuousEvent * p_event );

		private:
			void _addPonctualEvent( Temporal::TrollPonctualEvent * p_event, Real p_time );
			void _addContinuousEvent( Temporal::TrollContinuousEvent * p_event );
			int _getTopForContinuousEvent( Temporal::TrollContinuousEvent * p_event );
			int _getTopForPonctualEvent( Temporal::TrollPonctualEvent * p_event );
			int _getTop( int p_left, int p_width );
			void _showContextMenu( const wxPoint & p_pos );

			void _onAddPonctualEvent( wxCommandEvent & p_event );
			void _onAddContinuousEvent( wxCommandEvent & p_event );
			void _onLeftMouseUp( wxMouseEvent & p_event );
			void _onRightMouseUp( wxMouseEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

