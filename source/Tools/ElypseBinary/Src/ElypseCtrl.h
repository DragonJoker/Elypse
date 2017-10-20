/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_CTRL_H___
#define ___ELYPSE_CTRL_H___

#include "ElypseBinaryPrerequisites.h"

#include <Module_Main.h>
#include <OwnedBy.h>

namespace ElypseBinary
{
	class ElypseCtrl
		: public wxControl
		, public owned_by< ElypseApp >
	{
	public:
		ElypseCtrl( ElypseApp & p_owner, wxWindow * p_parent, GuiCommon::wxElypsePluginSPtr p_plugin, std::shared_ptr< ElypseInstance > p_elypse );
		~ElypseCtrl();

		const String GetWHandle();

	private:
		static wxSize GetClientSize( wxWindow * p_window )
		{
			int l_height = 0;
			int l_width = 0;

			p_window->GetClientSize( & l_width, & l_height );

			return wxSize( l_width, l_height );
		}
		inline 	std::shared_ptr< ElypseInstance > GetElypse()
		{
			return m_elypse.lock();
		}

		DECLARE_EVENT_TABLE()
		void OnKeyDown( wxKeyEvent & );
		void OnKeyUp( wxKeyEvent & );
		void OnChar( wxKeyEvent & );
		void OnKillFocus( wxFocusEvent & );
		void OnMouseMove( wxMouseEvent & );
		void OnMouseWheel( wxMouseEvent & );
		void OnMButtonDown( wxMouseEvent & );
		void OnMButtonUp( wxMouseEvent & );
		void OnLButtonDown( wxMouseEvent & );
		void OnLButtonUp( wxMouseEvent & );
		void OnRButtonDown( wxMouseEvent & );
		void OnRButtonUp( wxMouseEvent & );
		void OnClose( wxCloseEvent & );
		void OnPaint( wxPaintEvent & p_event );
		void OnEraseBackground( wxEraseEvent & p_event );
		void OnRenderTimer( wxTimerEvent & p_event );

	private:
		std::weak_ptr< ElypseInstance > m_elypse;
		GuiCommon::wxElypsePluginWPtr m_plugin;

		Real m_width;
		Real m_height;

		bool m_rightButton;
		bool m_leftButton;
		bool m_middleButton;
	};
}

#endif
