//***********************************************************************************************************
#ifndef ___Troll_TimePanel___
#define ___Troll_TimePanel___
//***********************************************************************************************************
#include "../Module_GUI.h"
#include <wx/panel.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class TimePanel;

		class TimeTick : public wxPanel
		{
		protected:
			TimePanel * m_panel;

		public:
			TimeTick( wxWindow * p_parent, wxWindowID p_id = wxID_ANY,
					  const wxPoint & p_position = wxDefaultPosition, const wxSize & p_size = wxDefaultSize );

		private:
			void _onLeftMouseDown( wxMouseEvent & p_event );
			void _onLeftMouseUp( wxMouseEvent & p_event );

			DECLARE_EVENT_TABLE()
		};

		class TimePanel : public wxPanel
		{
		protected:
			int m_totalWidth;
			float m_scale;
			float m_first;
			TimeTick * m_tick;
			float m_currentTime;
			bool m_leftDown;

		public:
			TimePanel( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
					   const wxSize & p_size = wxDefaultSize );
			~TimePanel();

			void IncrementTickPosition( int p_left );
			void UpdateTick( int p_left, bool p_updateSequence = false );

		public:
			inline void SetFirst( float p_first )
			{
				m_first = p_first;
				Refresh();
			}
			inline void IncrementFirst( float p_inc )
			{
				m_first += p_inc;
				Refresh();
			}
			inline void DecrementFirst( float p_dec )
			{
				m_first -= p_dec;
				Refresh();
			}
			inline void SetLeftDown( bool p_down )
			{
				m_leftDown = p_down;
			}

			inline float GetFirst()const
			{
				return m_first;
			}

		private:
			void _onPaint( wxPaintEvent & p_event );
			void _onLeftMouseDown( wxMouseEvent & p_event );
			void _onLeftMouseUp( wxMouseEvent & p_event );
			void _onMouseMove( wxMouseEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************