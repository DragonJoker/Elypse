#ifndef ___ELYPSE_CTRL_H___
#define ___ELYPSE_CTRL_H___

#if defined( __WXGTK__)
#	include <gdk/gdkx.h>
#	include <wx/glcanvas.h>
#	include <gtk/gtk.h>
#	include <wx/gtk/win_gtk.h>
#endif

#include "Module_Binary.h"

#include "../EMuseCore/Main/Module_Main.h"

#include <General/OwnedBy.h>

namespace EMuse
{ namespace Binary
{
	class ElypseCtrl : public wxControl, public owned_by<ElypseApp>
	{
	private:
#if ! defined( __WXMSW__) && defined( __WXGTK__)
		XVisualInfo * m_vi;
#endif

		EMuseInstance * m_ogre;
		EMusePlugin_Binary * m_plugin;

		Real m_width;
		Real m_height;

		bool m_rightButton;
		bool m_leftButton;
		bool m_middleButton;

	public:
		ElypseCtrl(	ElypseApp* p_owner,
					wxWindow * p_parent,
					EMusePlugin_Binary * p_plugin,
					EMuseInstance * p_ogre);
		~ElypseCtrl();

	public:
		const String GetWHandle();

		void OnKeyDown		( wxKeyEvent &);
		void OnKeyUp		( wxKeyEvent &);
		void OnChar			( wxKeyEvent &);
		void OnKillFocus	( wxFocusEvent &);
		void OnMouseMove	( wxMouseEvent &);
		void OnMouseWheel	( wxMouseEvent &);
		void OnMButtonDown	( wxMouseEvent &);
		void OnMButtonUp	( wxMouseEvent &);
		void OnLButtonDown	( wxMouseEvent &);
		void OnLButtonUp	( wxMouseEvent &);
		void OnRButtonDown	( wxMouseEvent &);
		void OnRButtonUp	( wxMouseEvent &);
		void OnClose		( wxCloseEvent &);

	public:
		inline void OnPaint	( wxPaintEvent & p_event)			{ p_event.Skip();/*RenderOneFrame();*/ }
		inline void OnEraseBackground( wxEraseEvent & p_event)	{ p_event.Skip();/*RenderOneFrame();*/ }
		inline void OnRenderTimer( wxTimerEvent & p_event)		{ p_event.Skip();/*RenderOneFrame();*/ }

		DECLARE_EVENT_TABLE()

	private:
		static wxSize GetClientSize( wxWindow * p_window)
		{
			int l_height = 0;
			int l_width = 0;

			p_window->GetClientSize( & l_width, & l_height);

			return wxSize( l_width, l_height);
		}
	};
}
}

#endif
