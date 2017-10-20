/*
See LICENSE file in root folder
*/
#ifndef ___EMGUI__h___
#define ___EMGUI__h___

#include "Module_Gui.h"

#include "Module_Script.h"

namespace Elypse
{
	namespace Gui
	{
		class d_dll_export EMGui
			: d_noncopyable
		{
		protected:
			EMOverlayMap m_overlays;
			OverlayGroupMap m_groups;

			EMOverlay * m_currentClickedOverlay;
			EMOverlay * m_currentMouseOverOverlay;
			EMOverlay * m_topmostOverlay;

			String m_instanceKey;

			Real m_aspectRatio;

		public:
			EMGui( String const & p_instanceKey, Real p_aspect );
			~EMGui();

		public:
			void AddElement( EMOverlay * p_element );

			EMOverlay * GetTopmostAt( Real p_x, Real p_y )const;
			void MouseOver( Real p_mouseX, Real p_mouseY );
			ScriptNode * Click( Real p_mouseX, Real p_mouseY );

			void SetAllOverlaysVisibility( bool p_visible )const;

			void ClearAll() noexcept;

			EMOverlayGroup * CreateOverlayGroup( String const & p_name );

			void RemoveOverlay( String const & p_overlayName );
			void DestroyOverlayGroup( String const & p_name );
			void DestroyEMOverlay( String const & p_name );

		public:
			inline const OverlayGroupMap &	GetAllOverlays()const
			{
				return m_groups;
			}
			inline EMOverlay 		*		GetTopmost()const
			{
				return m_topmostOverlay;
			}
			inline String const & GetInstanceKey()const
			{
				return m_instanceKey;
			}

			inline EMOverlayGroup * GetOverlayGroup( String const & p_name )const
			{
				return General::Utils::map::findOrNull( m_groups, p_name );
			}
			inline EMOverlay * GetElementByName( String const & p_elementName )const
			{
				return General::Utils::map::findOrNull( m_overlays, p_elementName );
			}

		public:
			Real GetWidth( char p_char, const FontPtr & p_font, Real p_fontSize )const;
			Real GetWidth( String const & p_string, const FontPtr & p_font, Real p_fontSize )const;
		};
	}
}

#endif
