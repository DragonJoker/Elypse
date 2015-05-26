#ifndef ___ElypseCtrl___
#define ___ElypseCtrl___

#if defined( __WXGTK__)
#	include <gdk/gdkx.h>
#	include <wx/glcanvas.h>
#	include <gtk/gtk.h>
//#	include <wx/gtk/win_gtk.h>
#endif

#include <wx/control.h>
#include "Module_GUI.h"

#include <wx/cursor.h>
#include <wx/timer.h>

#include <Module_Main.h>

#include <OwnedBy.h>

#include "../Main/TrollInstance.h"
#include "../EMusePlugin_wxWidget.h"
#include "TextCtrl.h"
#include "../SceneFile/TrollSceneFileParser.h"

namespace Troll
{
	class Project;
	class TEDisplayable;
	class TrollScene;

	namespace GUI
	{
		class ElypseCtrl : public wxControl
		{
		private:
#if ! defined( __WXMSW__) && defined( __WXGTK__)
			XVisualInfo * m_vi;
#endif

			Main::TrollInstance * m_emuse;
			EMusePlugin_wxWidget * m_plugin;
			TrollSceneFileParser * m_parser;
			NewOverlayFrame * m_newOverlayFrame;

			Real m_width;
			Real m_height;

			unsigned int m_fsaa;

			bool m_rightButton;
			bool m_leftButton;
			bool m_middleButton;

			bool m_focused;
			bool m_primary;
			bool m_deleteAll;
			bool m_ogreActive;
			bool m_isVista;
			bool m_fucked;
			bool m_destroyed;
			bool m_oneTimePostSetFocus;
			bool m_editionMode;

			String m_baseDirectory;
			String m_filename;
			String m_installPath;
			String m_path;
			String m_museFile;

			Troll::TEDisplayable * m_displayedObject;
			Troll::Project * m_project;
			Troll::TrollScene * m_testedScene;
			MainFrame * m_mainFrame;

			int m_cursorType;

		public:
			ElypseCtrl(	wxWindow * p_parent, Troll::Project * p_project,
						MainFrame * p_mainFrame, bool p_edit = false );
			~ElypseCtrl();

			const String GetWHandle();

			void Init();
			void SetCtrlFocus();
			void KillCtrlFocus();

			void OnSetFocus( wxFocusEvent & p_event );
			void OnKillFocus( wxFocusEvent & p_event );
			void OnShow( wxShowEvent & p_event );
			void OnActivate( wxActivateEvent & p_event );
			void OnIconise( wxIconizeEvent & p_event );
			void OnButtonLeftDown( wxMouseEvent & p_event );
			void OnButtonLeftUp( wxMouseEvent & p_event );
			void OnButtonMiddleDown( wxMouseEvent & p_event );
			void OnButtonMiddleUp( wxMouseEvent & p_event );
			void OnButtonRightDown( wxMouseEvent & p_event );
			void OnButtonRightUp( wxMouseEvent & p_event );
			void OnMouseWheel( wxMouseEvent & p_event );
			void OnMouseMove( wxMouseEvent & p_event );
			void OnMouseEnter( wxMouseEvent & p_event );
			void OnMouseLeave( wxMouseEvent & p_event );
			void OnKeyDown( wxKeyEvent & p_event );
			void OnKeyUp( wxKeyEvent & p_event );
			void OnChar( wxKeyEvent & p_event );

		public:
			inline float					GetMuseWidth()const
			{
				return m_width;
			}
			inline float					GetMuseHeight()const
			{
				return m_height;
			}
			inline Main::TrollInstance *	GetTrollInstance()const
			{
				return m_emuse;
			}

			inline void	SetNewOverlayFrame( NewOverlayFrame * p_frame )
			{
				m_newOverlayFrame = p_frame;
			}

		private:
			void _initialise();
			void _showContextMenu( const wxPoint & p_position );
			void _setCustomFocus( bool p_hasFocus );

			inline void _onPaint( wxPaintEvent & p_event )
			{
				p_event.Skip();
			}
			inline void _onEraseBackground( wxEraseEvent & p_event )
			{
				p_event.Skip();
			}
			inline void _onRenderTimer( wxTimerEvent & p_event )
			{
				p_event.Skip();
			}

			void _onClose( wxCloseEvent & p_event );
			void _onResize( wxCommandEvent & p_event );
			void _onReplace( wxCommandEvent & p_event );
			void _onNew( wxCommandEvent & p_event );
			void _onDelete( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()

		private:
			static wxSize GetClientSize( wxWindow * p_window )
			{
				int l_height = 0;
				int l_width = 0;
				p_window->GetClientSize( & l_width, & l_height );
				return wxSize( l_width, l_height );
			}
		};
	}
}
#endif
